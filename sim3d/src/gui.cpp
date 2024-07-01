#include "gui.h"
#include "Engine.h"
#include "renderer.h"

#include <vector>

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
}

void GUI::render()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	// render your GUI
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGui::Begin("Demo Window", NULL, window_flags);

	// Other ImGui elements (e.g., particle size, max velocity, etc.)ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate); ImGui::SameLine(0,30);
	ImGui::Text("Number of Particles: %i", engine.particles.size());
	ImGui::SliderFloat("Size", &size, 0.0, 20, "%.3f");
	ImGui::SliderFloat3("Maxvel", &maxvel.x, 0, 5);
	ImGui::Checkbox("Random velocity", &randVel);
	ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate);

	if (ImGui::Button("Add particle"))
	{
		engine.createParticle(size, size, maxvel, true);
	}

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
		if (ImGui::BeginMenu("Particle Settings"))
		{
			ImGui::SliderInt("Number of Threads", &engine.m_NumThreads, 1, 12);
			ImGui::SliderInt("Number of substeps", &engine.NumSteps, 1, 30);
			ImGui::SliderFloat3("Global Acceleration", &engine.globalAcc.x, -0.05f, 0.05f);
			ImGui::SliderFloat("Wall Elasticity", &engine.wallElasticity, 0.0f, 1.0f);
			ImGui::SliderFloat("Particle Elasticity", &engine.particleElasticity, 0.0f, 1.0f);
			ImGui::SliderFloat("Friction", &engine.friction, 0.0f, 1.0f);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Renderer settings")) {
			ImGui::Checkbox("Show Cloth", &renderer.showcloth);
			if (!renderer.showcloth)
				renderer.showsprings = true;
			ImGui::Checkbox("Show particles", &renderer.showparticles);
			ImGui::Checkbox("Show springs", &renderer.showsprings);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	ImGui::End();

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