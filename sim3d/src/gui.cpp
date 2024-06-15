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
	ImGui::SetNextWindowSize(ImVec2(400, 0));

	ImGui::Begin("Debug Menu");

	ImGui::Checkbox("Pause Sim3D", &engine.pause);
	ImGui::Checkbox("Use Multi-threading", &engine.useThreading);
	ImGui::SliderInt("Number of Threads", &engine.m_NumThreads, 1, 12);
	ImGui::SliderInt("Number of substeps", &engine.NumSteps, 1, 30);

	//ImGui::SliderInt("H_Resolution", &renderer.HRES, 2, 50);
	//ImGui::SliderInt("V_Resolution", &renderer.VRES, 2, 50);
	ImGui::SliderFloat3("Global Accelaration", &engine.globalAcc.x, -0.5, 0.5);
	ImGui::SliderFloat("Wall Elasticity", &engine.wallElasticity, 0, 1);
	ImGui::SliderFloat("Particle Elasticity", &engine.particleElasticity, 0, 1);
	ImGui::SliderFloat("Friction", &engine.friction, 0, 1);
	//ImGui::SliderFloat("Attenuation", &renderer.m_light.attenuation, 0, 0.001);
	//ImGui::SliderFloat("Intensity", &renderer.m_light.intensity, 0, 3);
	//ImGui::SliderFloat3("Light Pos", &renderer.m_light.pos.x, -100, 100);
	ImGui::Text("Frame rate: %.1f FPS", ptrio->Framerate);
	ImGui::Text("Number of Particles: %i", engine.particles.size());
	//ImGui::ColorPicker4("Light Color", &renderer.ourlight.color.x);
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