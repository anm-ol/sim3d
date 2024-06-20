#include "gui.h"
#include "Engine.h"
#include "renderer.h"

#include <vector>

GUI::GUI(Engine& ourengine, Renderer& renderer) :  engine(ourengine), renderer(renderer), window(renderer.window)
{
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
	// render your GUI
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 0));

	ImGui::Begin("Debug Menu");

	ImGui::Checkbox("Pause Sim3D", &engine.pause); ImGui::SameLine();
	ImGui::Checkbox("Use Multi-threading", &engine.useThreading); ImGui::SameLine();
	ImGui::Checkbox("Use Space-partitioning", &engine.usePartition);
	ImGui::Checkbox("Selection Mode", &renderer.select);

	ImGui::SliderInt("Number of Threads", &engine.m_NumThreads, 1, 12);
	ImGui::SliderInt("Number of substeps", &engine.NumSteps, 1, 30);

	ImGui::SliderFloat3("Global Accelaration", &engine.globalAcc.x, -0.5, 0.5);
	ImGui::SliderFloat("Wall Elasticity", &engine.wallElasticity, 0, 1);
	ImGui::SliderFloat("Particle Elasticity", &engine.particleElasticity, 0, 1);
	ImGui::SliderFloat("Friction", &engine.friction, 0, 1);
	ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate); ImGui::SameLine(0,30);
	ImGui::Text("Number of Particles: %i", engine.particles.size());
	

	ImGui::SliderFloat("Size", &size, 0.0, 20, "%.3f");
	ImGui::SliderFloat3("Maxvel", &maxvel.x,0,5);
	ImGui::Checkbox("Random velocity", &randVel);
	if (ImGui::Button("Add particle"))
	{
		engine.createParticle(size, size, maxvel, true);
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