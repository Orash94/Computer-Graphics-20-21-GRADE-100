#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "MeshModel.h"
#include <iostream>
#include <iomanip>


/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void ChangeFrameSize(int width, int height, Renderer& renderer);
std::shared_ptr<Camera> MakeCamera();
/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}
int windowWidth = 1000, windowHeight = 1000;
int main(int argc, char **argv)
{
	
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Or & Abed Project");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	
    while (!glfwWindowShouldClose(window))
    {
		//Main Loop
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
		ChangeFrameSize(frameBufferWidth, frameBufferHeight, renderer);
		//renderer.SetViewport(frameBufferWidth, frameBufferHeight);
	}



	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}


void ChangeFrameSize(int width, int height, Renderer& renderer)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
	renderer.SetViewport(windowWidth, windowHeight);
	
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

std::shared_ptr<Camera> MakeCamera() {
	
	MeshModel mesh = MeshModel(*(Utils::LoadMeshModel("../computergraphics2021-or-and-abed/Data/camera.obj")));
	glm::vec3 nEye = glm::vec3(0, 0, 0);
	glm::vec3 nAt = glm::vec3(0, 0, -1);
	glm::vec3 nUp = glm::vec3(0, 1, 0);
	return std::make_shared<Camera>(mesh, nEye, nAt, nUp);
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	float  windowsWidth = (float)(io.DisplaySize.x) / 2;
	float  windowsHeight = (float)(io.DisplaySize.y) / 2;
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Add Camera", "CTRL+T"))
			{
				scene.AddCamera(MakeCamera());

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	//// Controls
	//ImGui::ColorEdit3("Clear ", (float*)&clear_color);
	//// TODO: Add more controls as needed
	if (scene.GetModelCount() != 0 && scene.GetCameraCount() != 0) {
		if (ImGui::Button("Clear Screen and selection")) {
			scene.cleanupScene();
		}
	}
	ImGui::Checkbox("Display Axis", &scene.showAxis);
	if (ImGui::CollapsingHeader("Camera Actions", ImGuiTreeNodeFlags_None))
	{
		static int camera_selected = -1;
		if (scene.GetCameraCount() != 0) {
			if (ImGui::Button("Clear Cameras")) {
				camera_selected = -1;
				scene.clearCameras();
			}
		}
		if (ImGui::TreeNode("Active camera selection:"))
		{

			if (scene.GetCameraCount() == 0) {
				ImGui::Text("please add camera");
			}
			else {
				for (int n = 0; n < scene.GetCameraCount(); n++)
				{

					//sprintf(buf, ((scene.GetModels())[n])->GetModelName() + "model", n);

					const std::string name = " camera #" + std::to_string(n + 1);
					// copying the contents of the
					if (ImGui::Selectable(name.c_str(), camera_selected == n)) {
						camera_selected = n;
						scene.SetActiveCameraIndex(n);
					}
					
						
				}
			}
			ImGui::TreePop();
		}

		
		if (camera_selected != -1 && scene.GetCameraCount() != 0) {
			if (ImGui::Button("delete")) {
				camera_selected = -1;
				scene.deleteActiveCamera();
			}
		}
		if (camera_selected != -1 && scene.GetCameraCount() != 0) {
		
			Camera& cam = scene.GetActiveCamera();
			if (!scene.GetCamOrWorldView()) {
				if (ImGui::Button("To Camera View"))
					scene.SetCamOrWorldView(true);
			}
			else
			{
				if (ImGui::Button("To World View"))
					scene.SetCamOrWorldView(false);
			}

			static int update = 1;
			ImGui::RadioButton("Transformation", &update, 1); ImGui::SameLine();
			ImGui::RadioButton("Lookat", &update, 0);
			scene.GetActiveCamera().setLookAtOrTransformation(update);
			//scene.GetActiveCamera().SetCameraLookAt();
			if (ImGui::TreeNode("Active camera params:"))
			{
				
				glm::vec3 glmEye = cam.getEye();
				glm::vec3 glmAt = cam.getAt();
				glm::vec3 glmUp = cam.getUp();


				static float vecEye[3] = { 0.10f, 0.20f, 0.30f };
				static float vecAt[3] = { 0.10f, 0.20f, 0.30f };
				static float vecUp[3] = { 0.10f, 0.20f, 0.30f };


				for (int i = 0; i < 3; i++) {
					vecEye[i] = glmEye[i];
					vecAt[i] = glmAt[i];
					vecUp[i] = glmUp[i];
				}

				static bool symmetriceye = false;
				float minWindow=glm::min(windowsWidth, windowsHeight);
				ImGui::Checkbox("symmetric", &symmetriceye);
				if (symmetriceye) {
					ImGui::SliderFloat("Eye", &vecEye[0], -minWindow, minWindow);
					vecEye[1]=vecEye[0];
					vecEye[2] = vecEye[0];
				}
				else {
					ImGui::SliderFloat("Eye X", &vecEye[0], -windowsWidth, windowsWidth);
					ImGui::SliderFloat("Eye Y", &vecEye[1], -windowHeight, windowHeight);
					ImGui::SliderFloat("Eye Z", &vecEye[2], -windowHeight, windowHeight);
				}
			
				
				if (ImGui::Button("Reset eye")) {
					vecEye[0] = 0.f;
					vecEye[1] = 0.f;
					vecEye[2] = 0.f;
				}
				ImGui::SliderFloat("at X", &vecAt[0], -windowsWidth, windowsWidth);
				ImGui::SliderFloat("at Y", &vecAt[1], -windowHeight, windowHeight);
				ImGui::SliderFloat("at Z", &vecAt[2], -windowHeight, windowHeight);
				if (ImGui::Button("Reset at")) {
					vecAt[0] = 0.f;
					vecAt[1] = 0.f;
					vecAt[2] = -1.f;
				}
				ImGui::SliderFloat3("Up (x,y,z)", vecUp , -1.0, 1.0);
				if (ImGui::Button("Reset up")) {
					vecUp[0] = 0.f;
					vecUp[1] = 1.f;
					vecUp[2] = 0.f;
				}

				for (int i = 0; i < 3; i++) {
					glmEye[i] = vecEye[i];
					glmAt[i] = vecAt[i];
					glmUp[i] = vecUp[i];
				}
				cam.SetCameraLookAt(glmEye, glmAt, glmUp);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Active camera Projection Type:"))
			{
				static int Projection = 1;
				ImGui::RadioButton("Orthographic", &Projection, 1); ImGui::SameLine();
				ImGui::RadioButton("Perspective", &Projection, 0);

				if (Projection) {
					float nRight, nLeft, nTop, nBottom, nNear, nFar;
					nRight = cam.GetRight();
					nLeft = cam.GetLeft();
					nTop = cam.GetTop();
					nBottom = cam.GetBottom();
					nNear = cam.GetNear();
					nFar = cam.GetFar();

					ImGui::SliderFloat(":Right ", &nRight , 0.0f, 1.0f);
					ImGui::SliderFloat(":Left ", &nLeft,  -1 , 0.0f);
					ImGui::SliderFloat(" :Top ", &nTop, 0.0f, 1.0f);
					ImGui::SliderFloat(" :Bottom", &nBottom, -1, 0.0f);
					ImGui::SliderFloat(" :Near: ", &nNear , 1.0f, 20.f);
					ImGui::SliderFloat(" :Far: ", &nFar, 5.0f, 50.f);

					cam.SetViewVolumeCoordinates(nRight, nLeft, nTop, nBottom, nNear, nFar);
					
				}
				else
				{
					float nNear, nFar, nFovy, nAspectRatio, nZoom;
					nNear = cam.GetNear();
					nFar = cam.GetFar();
					nFovy = cam.GetFovy();
					nAspectRatio = cam.GetAspectRatio();
					nZoom = cam.GetZoom();

					ImGui::SliderFloat(" :Near ", &nNear,0.1f, 200.0f);
					ImGui::SliderFloat(" :Far ", &nFar, 200.1f, 500.0f);
					ImGui::SliderFloat(":Angle of Field of View Y ", &nFovy,0.01f, 0.5f);
					ImGui::SliderFloat(" :Width", &nAspectRatio, 0.1f, 100.0f);
					ImGui::SliderFloat("Zoom: ", &nZoom, 1.0f, 16.0f);
					cam.SetZoom(nZoom);
					cam.SetPerspectiveData(nNear, nFar, nFovy, nAspectRatio);
				}

				scene.GetActiveCamera().setProjection(Projection);
				ImGui::TreePop();
			}
		}

		if (camera_selected != -1 && scene.GetCameraCount() != 0) {
			Camera& camera = scene.GetActiveCamera();
			if (ImGui::TreeNode("Camera model Transformation"))
			{
				glm::vec3 Rotate = camera.getRotate();
				glm::vec3 Translate = camera.getTranslate();

				if (ImGui::CollapsingHeader("Rotating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Rotate X", &Rotate[0], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Y", &Rotate[1], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Z", &Rotate[2], -180.0f, 180.0f);
					if (ImGui::Button("Reset Rotating")) {
						Rotate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				if (ImGui::CollapsingHeader("Translating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Translate X", &Translate[0], -windowsWidth, windowsWidth);
					ImGui::SliderFloat("Translate Y", &Translate[1], -windowsHeight, windowsHeight);
					ImGui::SliderFloat("Translate Z", &Translate[2], -windowsHeight, windowsHeight);
					if (ImGui::Button("Reset trasnalte")) {
						Translate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}


				
				glm::fvec3 scale = glm::fvec3(1.0f, 1.0f, 1.0f);
				camera.setObjectTransformationUpdates(scale, Rotate, Translate);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Camera model world Transformation:"))
			{


				glm::vec3 worldRotate = camera.getWorldRotate();
				glm::vec3 worldTranslate = camera.getWorldTranslate();

				if (ImGui::CollapsingHeader("Rotating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Rotate X", &worldRotate[0], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Y", &worldRotate[1], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Z", &worldRotate[2], -180.0f, 180.0f);
					if (ImGui::Button("Reset Rotating")) {
						worldRotate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				if (ImGui::CollapsingHeader("Translating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Translate X", &worldTranslate[0], -windowsWidth, windowsWidth);
					ImGui::SliderFloat("Translate Y", &worldTranslate[1], -windowsHeight, windowsHeight);
					ImGui::SliderFloat("Translate Z", &worldTranslate[2], -windowsHeight, windowsHeight);
					if (ImGui::Button("Reset Translating")) {
						worldTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				glm::fvec3 worldScale = glm::fvec3(1.0f, 1.0f, 1.0f);
				camera.setWorldTransformationUpdates(worldScale, worldRotate, worldTranslate);
				ImGui::TreePop();
			}
			camera.updateLookAt();


		}

	}
	if (ImGui::CollapsingHeader("Models Actions", ImGuiTreeNodeFlags_None))
	{
		static int model_selected = -1;
		if (scene.GetModelCount() != 0) {
			if (ImGui::Button("Clear Models")) {
				model_selected = -1;
				scene.clearModels();
			}
		}

		ImGui::Separator();
		
		if (ImGui::TreeNode("Active model selection:"))
		{


			for (int n = 0; n < scene.GetModelCount(); n++)
			{

				//sprintf(buf, ((scene.GetModels())[n])->GetModelName() + "model", n);
				const std::string name = " model #"+ std::to_string(n+1)+ ": " +scene.GetModels()[n]->GetModelName() ;
				// copying the contents of the
				if (ImGui::Selectable(name.c_str(), model_selected == n)) {
					model_selected = n;
					scene.SetActiveModelIndex(n);
				}

			}
			if (scene.GetModelCount() == 0) {
				ImGui::Text("please select one or more model");
			}
			ImGui::TreePop();
		}
		if (model_selected != -1 && scene.GetModelCount() != 0) {
			if (ImGui::Button("delete")) {
				model_selected = -1;
				scene.deleteActiveModel();
			}
		}
		if (model_selected != -1 && scene.GetModelCount() != 0) {
			MeshModel& model1 = scene.GetActiveModel();
			if (ImGui::TreeNode("model Transformation"))
			{

				glm::vec3 scale = model1.getScale();
				glm::vec3 Rotate = model1.getRotate();
				glm::vec3 Translate = model1.getTranslate();


				if (ImGui::CollapsingHeader("Scalling", ImGuiTreeNodeFlags_None))
				{
					static bool symmetric = false;
					ImGui::Checkbox("symmetric", &symmetric);
					if (symmetric) {
						ImGui::SliderFloat("Scale", &scale[0], -2.0f, 2.0f);
						scale = glm::vec3(scale[0], scale[0], scale[0]);
					}
					else {
						ImGui::SliderFloat("Scale X", &scale[0], -2.0f, 2.0f);
						ImGui::SliderFloat("Scale Y", &scale[1], -2.0f, 2.0f);
						ImGui::SliderFloat("Scale Z", &scale[2], -2.0f, 2.0f);
					}
					if (ImGui::Button("Reset scale")) {
						scale = glm::vec3(1.0f, 1.0f, 1.0f);
					}
				}

				if (ImGui::CollapsingHeader("Rotating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Rotate X", &Rotate[0], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Y", &Rotate[1], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Z", &Rotate[2], -180.0f, 180.0f);
					if (ImGui::Button("Reset Rotating")) {
						Rotate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				if (ImGui::CollapsingHeader("Translating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Translate X", &Translate[0], -windowsWidth, windowsWidth);
					ImGui::SliderFloat("Translate Y", &Translate[1], -windowsHeight, windowsHeight);
					ImGui::SliderFloat("Translate Z", &Translate[2], -windowsHeight, windowsHeight);
					if (ImGui::Button("Reset trasnalte")) {
						Translate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}


				//ImGui::SliderFloat3("Rotate		[x,y,z]", Rotate, 0.0f, 359.9f);
				//ImGui::SliderFloat3("Translate	[x,y,z]", Translate, 0.0f, 1.0f);
				model1.setObjectTransformationUpdates(scale, Rotate, Translate);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("model world Transformation:"))
			{

				glm::vec3 worldScale = model1.getWorldScale();
				glm::vec3 worldRotate = model1.getWorldRotate();
				glm::vec3 worldTranslate = model1.getWorldTranslate();
				if (ImGui::CollapsingHeader("Scalling", ImGuiTreeNodeFlags_None))
				{
					static bool symmetric = false;
					ImGui::Checkbox("symmetric", &symmetric);
					if (symmetric) {
						ImGui::SliderFloat("Scale", &worldScale[0], -2.0f, 2.0f);
						worldScale = glm::vec3(worldScale[0], worldScale[0], worldScale[0]);
					}
					else {
						ImGui::SliderFloat("Scale X", &worldScale[0], -2.0f, 2.0f);
						ImGui::SliderFloat("Scale Y", &worldScale[1], -2.0f, 2.0f);
						ImGui::SliderFloat("Scale Z", &worldScale[2], -2.0f, 2.0f);
					}
					if (ImGui::Button("Reset scale")) {
						worldScale = glm::vec3(1.0f, 1.0f, 1.0f);
					}
				}

				if (ImGui::CollapsingHeader("Rotating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Rotate X", &worldRotate[0], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Y", &worldRotate[1], -180.0f, 180.0f);
					ImGui::SliderFloat("Rotate Z", &worldRotate[2], -180.0f, 180.0f);
					if (ImGui::Button("Reset Rotating")) {
						worldRotate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				if (ImGui::CollapsingHeader("Translating", ImGuiTreeNodeFlags_None))
				{
					ImGui::SliderFloat("Translate X", &worldTranslate[0], -windowsWidth, windowsWidth);
					ImGui::SliderFloat("Translate Y", &worldTranslate[1], -windowsHeight, windowsHeight);
					ImGui::SliderFloat("Translate Z", &worldTranslate[2], -windowsHeight, windowsHeight);
					if (ImGui::Button("Reset Translating")) {
						worldTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
					}
				}
				//ImGui::SliderFloat3("Rotate		[x,y,z]", Rotate, 0.0f, 359.9f);
				//ImGui::SliderFloat3("Translate	[x,y,z]", Translate, 0.0f, 1.0f);
				model1.setWorldTransformationUpdates(worldScale, worldRotate, worldTranslate);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("model color selection:")) {
				glm::vec3 currMeshColor = model1.GetColor();
				float Color[3] = { currMeshColor[0], currMeshColor[1],currMeshColor[2] };
				ImGui::ColorEdit3("choose color", (float*)&Color);

				for (int i = 0; i < 3; i++) {
					currMeshColor[i] = Color[i];
				}
				model1.SetColor(currMeshColor);
			}

			ImGui::Checkbox("Display Bounding Box", &model1.displayBoundingBox);

			ImGui::Checkbox("Display Face Normals", &model1.displayFaceNormals);

			ImGui::Checkbox("Display vertices Normals", &model1.displayVerticesNormals);

		}
	}
	ImGui::End();


	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}