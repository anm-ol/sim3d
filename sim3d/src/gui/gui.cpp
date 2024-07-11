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
	// Menu bar for Simulation Options
	if (ImGui::CollapsingHeader("Simulation Options"))
	{

		ImGui::Checkbox("Pause Sim3D", &engine.pause); ImGui::SameLine();
		ImGui::Checkbox("Use Multi-threading", &engine.useThreading);
		ImGui::Checkbox("Use Space-partitioning", &engine.usePartition);
	}
	if (ImGui::CollapsingHeader("Engine Settings"))
	{
		ImGui::SliderInt("Number of substeps", &engine.NumSteps, 1, 30);
		ImGui::InputFloat3("Global Acceleration", &engine.globalAcc.x);
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
	}
	if (ImGui::CollapsingHeader("Renderer Settings"))
	{
		//Only show cloth related settings if renderer.cloth pointer != NULL
		if (renderer.cloth)
		{
			std::string path = std::filesystem::current_path().string() + "/textures/";
			std::vector<std::string> textures;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				textures.push_back(entry.path().filename().string());
			}

			if (!textures.empty() && selectedTexture.empty()) {
				selectedTexture = textures[0];
			}

			if (engine.ourSpringHandler.isInit) {
				if (ImGui::BeginCombo("Select a texture", selectedTexture.c_str(), ImGuiComboFlags_NoArrowButton)) {
					for (auto& texture : textures) {

						bool isSelected = (selectedTexture == texture);
						if (ImGui::Selectable(texture.c_str(), isSelected)) {
							selectedTexture = texture;
							renderer.cloth->loadTexture(("textures/" + selectedTexture).c_str());
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
				ImGui::Checkbox("Show Cloth", &renderer.showcloth); ImGui::SameLine();
				ImGui::Checkbox("Show springs", &renderer.showsprings);

			}
		}
		ImGui::Checkbox("Show particles", &renderer.showparticles); ImGui::SameLine();
		ImGui::Checkbox("Show Lights", &renderer.showLights);
		ImGui::Separator();
	}
	ImGui::Text("Number of Particles: %i", engine.particles.size());
	ImGui::Checkbox("Selection Mode", &renderer.useSelect);
	if (renderer.useSelect)
	{
		ImGui::RadioButton("Particle", &selectedObjectType, PARTICLE);
		ImGui::RadioButton("Light", &selectedObjectType, LIGHT);
		if (engine.ourSpringHandler.isInit) ImGui::RadioButton("Cloth", &selectedObjectType, CLOTH);
		if (selectedObjectType == CLOTH) {
			ImGui::SameLine();
			ImGui::Checkbox("Rotation mode", &rotationMode);
		}
		else {
			rotationMode = false;
		}

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

	

	// Menu bar for Particle Settings
	
	
	
	//guizmos
	if (renderer.useSelect)
	{
		void* objectPtr = nullptr;
		vec3 position(0);
		//get selected object type 
		if (selectedObjectType == PARTICLE)
		{
			if (renderer.selectedObject < 0)
				renderer.selectedObject = engine.particles.size() - 1;
			else if (renderer.selectedObject > (engine.particles.size() - 1))
				renderer.selectedObject = 0;
			objectPtr = &engine.particles[renderer.selectedObject];
		}
		else if (selectedObjectType == LIGHT)
		{
			if (renderer.selectedObject < 0)
				renderer.selectedObject = renderer.m_lights.size() - 1;
			else if (renderer.selectedObject > renderer.m_lights.size() - 1)
				renderer.selectedObject = 0;
			objectPtr = &renderer.m_lights[renderer.selectedObject];
		}
		else if (selectedObjectType == CLOTH) {
			// add guizmo to the middle particle of the cloth. 
			// Make it look like it's at the center of the cloth 
			objectPtr = &engine.particles[engine.ourSpringHandler.center];
		}
		if (objectPtr)
		{
			if (selectedObjectType == PARTICLE || selectedObjectType == CLOTH)
				position = static_cast<particle*>(objectPtr)->pos;
			else if (selectedObjectType == LIGHT) {
				position = static_cast<pointLight*>(objectPtr)->pos;
				showLightMenu(*static_cast<pointLight*>(objectPtr));
			}
			// Now you can use the position variable

		// Now you can use the position variable

			ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

			mat4 view = renderer.camera.GetViewMatrix();
			mat4 projection = renderer.proj;
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
			ImGuizmo::Manipulate(value_ptr(view), value_ptr(projection), rotationMode ? ImGuizmo::OPERATION::ROTATE : ImGuizmo::OPERATION::TRANSLATE,
				ImGuizmo::MODE::WORLD, value_ptr(transform));
			//ImGuizmo::DrawCubes(value_ptr(view), value_ptr(projection), value_ptr(transform), 1);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 newPosition;
				glm::vec3 rotation;
				glm::vec3 dummyScale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(newPosition), glm::value_ptr(rotation), glm::value_ptr(dummyScale));
				if (selectedObjectType == PARTICLE) {
					static_cast<particle*>(objectPtr)->pos = newPosition;
				}
				else if (selectedObjectType == LIGHT) {
					static_cast<pointLight*>(objectPtr)->pos = newPosition;
				}
				else if (selectedObjectType == CLOTH) {
					if (rotationMode) {
						engine.ourSpringHandler.rotatecloth(rotation);
					}
					else{
						vec3 trans = newPosition - position;
						engine.ourSpringHandler.translate(trans);
					}
				}
			}
		}
	}
	pointLight p = pointLight();
	if (renderer.useSelect)
	{
		//showLightMenu(p);
	}
	ImGui::End();
	// Render dear imgui into screen
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::showLightMenu(pointLight& selectedLight)
{
	// getting the 2d position of light on screen
	vec2 screenPos = vec2(renderer.view * renderer.proj * vec4(selectedLight.pos, 0.0));
	ImGui::SetNextWindowSize(ImVec2(screenPos.x +50, screenPos.y +50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(100,100), ImGuiCond_FirstUseEver);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -6);
	ImGui::Begin("Light Settings");

	ImGui::ColorEdit3("Color", &selectedLight.color[0]);
	ImGui::DragFloat3("Position", &selectedLight.pos[0]);
	ImGui::InputFloat("Attenuation", &selectedLight.attenuation);
	ImGui::End();
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