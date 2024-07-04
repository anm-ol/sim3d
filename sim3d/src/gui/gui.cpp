#include "gui.h"
#include "Engine.h"
#include "renderer.h"

#include <vector>
#include <variant>
#include <functional>

GUI::GUI(Engine& ourengine, Renderer& renderer) :  engine(ourengine), renderer(renderer), window(renderer.window)
{
	width = renderer.screen_width;
	height = renderer.screen_height;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ptrio = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	randVel = false;
	size = 0;
	maxvel = vec3(0);
}

// feed inputs to dear imgui, start new frame
void GUI::InitFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

}

void GUI::render()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	// render your GUI
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	ImGui::Begin("Settings", NULL, window_flags);
	// Other ImGui elements (e.g., particle size, max velocity, etc.)ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate); ImGui::SameLine(0,30);
	ImGui::Text("Number of Particles: %i", engine.particles.size());
	ImGui::Checkbox("Selection Mode", &renderer.useSelect);
	if (renderer.useSelect)
	{
		ImGui::RadioButton("Particle", &selectedObjectType, PARTICLE);
		ImGui::RadioButton("Light", &selectedObjectType, LIGHT);
		ImGui::RadioButton("Cloth", &selectedObjectType, CLOTH);

		ImGui::InputInt("Select Object ID:", &renderer.selectedObject);

		// add pivots only for particles and check index range
		if (selectedObjectType == PARTICLE && renderer.selectedObject >= 0 && renderer.selectedObject < engine.particles.size()) {
			ImGui::Checkbox("Add pivot", &engine.particles.at(renderer.selectedObject).isPivot);
		}
	}
	ImGui::Separator();
	// section to add particle

	if (!engine.ourSpringHandler.isInit) {
		ImGui::Text("Add particle");
		ImGui::SliderFloat("Size", &size, 0.0, 20, "%.3f");
		ImGui::SliderFloat3("Maxvel", &maxvel.x, 0, 5);
		ImGui::Checkbox("Random velocity", &randVel);
		if (ImGui::Button("Add particle"))
		{
			engine.createParticle(size, size, maxvel, true);
		}
	}
	ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate);

	// Menu bar for Simulation Options
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Simulation Options"))
		{
			ImGui::Checkbox("Pause Sim3D", &engine.pause);
			ImGui::Checkbox("Use Multi-threading", &engine.useThreading);
			ImGui::Checkbox("Use Space-partitioning", &engine.usePartition);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Menu bar for Particle Settings
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Engine Settings"))
		{
			//ImGui::SliderInt("Number of Threads", &engine.m_NumThreads, 1, 12);
			ImGui::SliderInt("Number of substeps", &engine.NumSteps, 1, 30);
			ImGui::SliderFloat3("Global Acceleration", &engine.globalAcc.x, -0.05f, 0.05f);
			ImGui::SliderFloat("Wall Elasticity", &engine.wallElasticity, 0.0f, 1.0f);
			ImGui::SliderFloat("Particle Elasticity", &engine.particleElasticity, 0.0f, 1.0f);
			ImGui::SliderFloat("Friction", &engine.friction, 0.0f, 1.0f);
			if (engine.ourSpringHandler.isInit)
			{
				if (ImGui::Button("Remove Cloth"))
				{
					engine.removeSpringHandler();
					engine.createParticle(1, 1, vec3(0), false);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Renderer settings")) {

			std::string path = std::filesystem::current_path().string() + "/textures/";
			std::vector<std::string> textures;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				textures.push_back(entry.path().filename().string());
			}

			if (!textures.empty() && currentItem.empty()) {
				currentItem = textures[0];
			}

			if (engine.ourSpringHandler.isInit) {
				if (ImGui::BeginCombo("Select a texture", currentItem.c_str(), ImGuiComboFlags_NoArrowButton)) {
					for (auto& texture : textures) {

						bool isSelected = (currentItem == texture);
						if (ImGui::Selectable(texture.c_str(), isSelected)) {
							currentItem = texture;
							renderer.cloth->loadTexture(("textures/" + currentItem).c_str());
						}
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			// toggle springs and cloth option must only be visible when cloth is being rendered 
			if (engine.ourSpringHandler.isInit) {
				ImGui::Checkbox("Show Cloth", &renderer.showcloth);
				ImGui::Checkbox("Show springs", &renderer.showsprings);

			}
			ImGui::Checkbox("Show particles", &renderer.showparticles);
			ImGui::Checkbox("Show Lights", &renderer.showLights);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	ImGui::End();
	
	//guizmos
	if (renderer.useSelect)
	{
		void* objectPtr = nullptr;
		vec3 position(0);
		//get selected object type 
		if (selectedObjectType == PARTICLE)
		{
			if (renderer.selectedObject > (engine.particles.size() - 1) || renderer.selectedObject < 0)
				renderer.selectedObject = 0;
			objectPtr = &engine.particles[renderer.selectedObject];
		}
		else if (selectedObjectType == LIGHT)
		{
			if (renderer.selectedObject > (renderer.m_lights.size() - 1) || renderer.selectedObject < 0)
				renderer.selectedObject = 0;
			objectPtr = &renderer.m_lights[renderer.selectedObject];
		}
		if (objectPtr)
		{
			if (selectedObjectType == PARTICLE)
				position = static_cast<particle*>(objectPtr)->pos;
			else if (selectedObjectType == LIGHT)
				position = static_cast<pointLight*>(objectPtr)->pos;

			// Now you can use the position variable

		// Now you can use the position variable

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

			mat4 view = renderer.camera.GetViewMatrix();
			mat4 projection = renderer.proj;
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
			ImGuizmo::Manipulate(value_ptr(view), value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE,
				ImGuizmo::MODE::WORLD, value_ptr(transform));
			//ImGuizmo::DrawCubes(value_ptr(view), value_ptr(projection), value_ptr(transform), 1);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 newPosition;
				glm::vec3 dummyRotation;
				glm::vec3 dummyScale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(newPosition), glm::value_ptr(dummyRotation), glm::value_ptr(dummyScale));
				if (selectedObjectType == PARTICLE) {
					static_cast<particle*>(objectPtr)->pos = newPosition;
				}
				else if (selectedObjectType == LIGHT) {
					static_cast<pointLight*>(objectPtr)->pos = newPosition;
				}
			}
		}
	}
	// Render dear imgui into screen
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GUI::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

ImVec2 GUI::getWindowSize() {
	return ImVec2(renderer.screen_width, renderer.screen_height);
}