#include <../Source/includes/Core/App.hpp>
#include <STB_Image/stb_image.h>
#include <chrono>

App app;


#define LIGHT_SIZE 8
void SetupImGuiStyle();

#pragma region const values
using namespace Debug;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	// Resize the FrameBuffer.
	if (width * height != 0) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}
	app.WindowChangeSize = true;

}

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	LOG(LogType::WARNING, "---------------");
	LOG(LogType::WARNING, "Debug message (%d): %s", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             LOG(LogType::WARNING, "Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG(LogType::WARNING, "Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG(LogType::WARNING, "Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG(LogType::WARNING, "Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     LOG(LogType::WARNING, "Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           LOG(LogType::WARNING, "Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               LOG(LogType::WARNING, "Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG(LogType::WARNING, "Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG(LogType::WARNING, "Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         LOG(LogType::WARNING, "Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         LOG(LogType::WARNING, "Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              LOG(LogType::WARNING, "Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          LOG(LogType::WARNING, "Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           LOG(LogType::WARNING, "Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               LOG(LogType::WARNING, "Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         LOG(LogType::WARNING, "Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG(LogType::WARNING, "Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          LOG(LogType::WARNING, "Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(LogType::WARNING, "Severity: notification"); break;
	}
}
struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };


#pragma endregion

#pragma region Initialization

App::App()
{
}

App::~App()
{
}

void App::InitApp(AppInitializer data)
{
	Console.OpenFile("Logs.txt");
	GlfwInit();
	InitWindow(data.width, data.height, data.name);
	GladLoad();
	InitShader();
}

void App::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	app.camera.input.MouseScroll = (float)yoffset;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;
}
static ImVec4 Colors[5] = { ImVec4(1, 1, 1, 1) };

void App::ClearApp()
{
	// Save Theme Color.
	std::ofstream _file;
	_file.open("Config.cfg");
	std::string txt;
	txt += string_format("Theme Id : %d", editorUi.GetThemeId());
	_file.write(txt.c_str(), txt.size());
	_file.close();

	// Delete Temporary Scene.
	std::ofstream file;
	file.open("Assets/scenes/TEMPSCENE.txt");
	if (file) {
		file.close();
		remove("Assets/scenes/TEMPSCENE.txt");
	}
	file.open("Assets/scenes/TEMPSCENE.json");
	if (file) {
		file.close();
		remove("Assets/scenes/TEMPSCENE.json");
	}
	file.close();

	if (Window)
		glfwDestroyWindow(Window);
	if (!Window)
		glfwTerminate();

	delete[] SceneNode;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// glfw: initialize and configure
// ------------------------------
void App::GlfwInit()
{
	if (!glfwInit())
		_close = true;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}

void App::InitWindow(int Width, int Height, const char* Name)
{
	Window = glfwCreateWindow(Width, Height, Name, NULL, NULL);
	if (Window == nullptr)
	{
		glfwTerminate();
	}
	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
}

void App::GladLoad()
{
	GLint flags = 0;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG(LogType::L_ERROR, "Failed to initialize GLAD")
	}
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	glfwSetScrollCallback(Window, Scroll_Callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	stbi_set_flip_vertically_on_load(true);

	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	uint32_t size = 0;
	bool sucess = false;
	auto data = Loader::ReadFile("Config.cfg", size, sucess);
	if (sucess)
	{
		uint32_t pos = 0;
		int index = 0;
		while (data[pos] != '\0')
		{
			if (data[pos] == 'T')
			{
				int id = Loader::GetInt(data, pos, 11);
				editorUi.SetThemeId(id);
				break;
			}
		}
		LOG(LogType::INFO, "Sucessfuly Load Config File");
		switch (editorUi.GetThemeId())
		{
		case 0: ImGui::StyleColorsDark(); break;
		case 1: ImGui::StyleColorsClassic(); break;
		case 2: ImGui::StyleColorsLight(); break;
		case 3: SetupImGuiStyle(); break;
		}
	}
}

ImVec2 App::GetWindowSize()
{
	glfwGetWindowSize(Window, &_width, &_height);
	return ImVec2(_width, _height);
}


void App::InitShader()
{
	auto begin = std::chrono::high_resolution_clock::now();
	// Create Shaders and Link
	auto defaultshad = resourcesManager.Create<Shader*>("Source/shaders/DefaultShader");
	defaultshad->Link();
	resourcesManager.Create<Shader*>("Source/shaders/RGBShader")->Link();
	resourcesManager.Create<Shader*>("Source/shaders/NormalShader")->Link();
	resourcesManager.Create<Shader*>("Source/shaders/NoLightsShader")->Link();
	resourcesManager.Create<Shader*>("Source/shaders/NormalMappingShader")->Link();

	// Screen Shaders
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/DefaultScreenShader")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/InvertedShader")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/BlackAndWhite")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/KernelShader")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/TheDrugOne")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/TheDrugOnev2")->Link(true);
	resourcesManager.Create<Shader*>("Source/shaders/ScreenShader/TVShader")->Link(true);

	// Set Default Shader to scene.
	ShaderProgram = defaultshad->Program;
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	LOG(LogType::INFO, "Shaders Loading Time : %.5f seconds.\n", elapsed.count() * 1e-9);
}

void App::LoadAllTextures()
{
	LoadingTime = std::chrono::high_resolution_clock::now();
	std::string path = "Assets/textures";
	for (const auto& entry : fs::directory_iterator(path)) {
		resourcesManager.Create<Texture*>(entry.path().generic_string().data());
	}
	auto end = std::chrono::high_resolution_clock::now();
}

void App::LoadAllModels()
{
	std::vector<std::string> files;
	std::string path = "Assets/meshs/";
	for (const auto& entry : fs::directory_iterator(path)) {
		// Check for file extension
		if (entry.path().generic_string().substr(entry.path().generic_string().find_last_of(".") + 1) == "obj") {
			files.push_back(entry.path().generic_string());
			files.back().erase(files.back().begin(), files.back().begin() + 13);
			resourcesManager.Create<Model3D*>(files.back().data());
		}
	}
}


void App::LoadAllScripts()
{
	ScriptManager.Initialize();
	std::string path = "Source/scripts/";
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(path)) {
		// Check for file extension
		if (entry.path().generic_string().substr(entry.path().generic_string().find_last_of(".") + 1) == "py") {
			files.push_back(entry.path().generic_string());
			files.back().erase(files.back().begin(), files.back().begin() + 15);
			files.back().erase(files.back().end() - 3, files.back().end());
			ScriptManager.Load(files.back().data());
		}
	}
}

void App::LoadResources()
{
	frameBuffer.shader = resourcesManager.Get<Shader*>("Source/shaders/ScreenShader/DefaultScreenShader");
	ImVec2 Size = GetWindowSize();
	frameBuffer.Initialize(Int2D(Size.x, Size.y), resourcesManager);
	LoadAllTextures();
	LoadAllModels();
	LoadAllScripts();
	InitScene();
}

void App::InitScene()
{
	LoadScene("Test2");
}

#pragma endregion

void App::NewFrame(bool mouseCaptured)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	glfwPollEvents();
	if (mouseCaptured)
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
}

static bool sCannotFind = false;
void App::Update()
{
	// FrameBuffer Initialize.

	camera.input = Input(Window, &State);
	collisionManager = CollisionManager(&State);

	PlayerPrefs.Clear();

	LoadResources();

	editorUi.Start();

	while (!glfwWindowShouldClose(Window) && !_close)
	{
#if MULTITHREAD

		LoadMultithread();

#endif // MULTITHREAD
		NewFrame(camera.input.MouseCaptured);
		glfwGetWindowSize(Window, &_width, &_height);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.FBO);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);


		// Resize aspect Ratio
		camera.aspect_ratio = (float)_width / (float)_height;

		// If Mouse captured and Is not in Play or is in play.
		camera.Update(ImGui::GetIO().DeltaTime, State == GameState::GS_PLAY);

		glfwSwapInterval(settings.Vsync);

		glPolygonMode(GL_FRONT_AND_BACK, !camera.input.Wireframe ? GL_FILL : GL_LINE);

		// Draw All ImGui Editor Ui.
		editorUi.DrawUi();

		Mat4D VP = camera.GetProjection() * camera.GetViewMatrix();

		// Reload All Scripts.
		if (ImGui::IsKeyPressed(ImGuiKey_F6))
		{
			ScriptManager.ReloadAll();
		}

		// Draw Outline Mesh First.
		if (auto node = editorUi.inspector.GetSelected<Node>())
			if (auto mesh = node->GetComponent<Mesh>())
				mesh->Outline(VP * mesh->GetTransform()->GetModelMatrix());
		// Update all nodes
		if (ImGui::GetIO().DeltaTime < 0.16f) {
			SceneNode->UpdateComponents(VP);

			collisionManager.Update();
		}
		for (auto res : resourcesManager.Resources)
			if (auto shad = dynamic_cast<Shader*>(res.second))
				lightManager.Update(shad);

		frameBuffer.Draw(Int2D(_width, _height));

		if (!Console.AlreadyGet) {
			Commands::CheckCommand(Console.GetActualCommand());
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(Window);
		TimeSinceStart += ImGui::GetIO().DeltaTime;
		WindowChangeSize = false;
	}
}

void App::LoadMultithread()
{
	if (!editorUi.FullyLoaded) {
		for (auto i : this->resourcesManager.Resources)
		{
			if (auto m = dynamic_cast<Mesh*>(i.second))
			{
				if (m->IsLoaded())
					continue;
				m->Initialize();
				LOG(LogType::INFO, "Loaded %s", i.first.c_str());
				break;
			}
			else if (auto tex = dynamic_cast<Texture*>(i.second))
			{
				if (tex->SemiLoaded && !tex->Loaded) {
					tex->Initialize();
				}
			}
		}
	}
	for (auto i : this->WaitingList)
	{
		if (auto m = dynamic_cast<Mesh*>(this->resourcesManager.Resources[i.second]))
		{
			if (!m->IsLoaded())
				continue;
			auto t = i.first->GetComponent<Mesh>();
			t->Copy(m);
			t->gameObject = i.first;
			this->WaitingList.erase(i.first);
			break;
		}
	}
}

//TODO : Add Drag and drop of Nodes.

void App::SaveScene(std::string SceneName)
{
	std::ofstream _file;
#if JSON
	std::string filepath = ("Assets/scenes/" + (std::string)SceneName + ".json");
	_file.open(filepath);
	_file.write("{\n", 2);
	SceneNode->Save(_file, "\t");
	_file.write("}\n", 2);
#else
	std::string filepath = ("Assets/scenes/" + (std::string)SceneName + ".txt");
	_file.open(filepath);
	SceneNode->Save(_file, "\t");
#endif
	//ScriptManager.GetScript("SceneSaving")->ExecuteFunction("SaveScene", SceneNode, this, "Scene2.json");
	LOG(LogType::INFO, "Sucessfuly Save Scene");
}

void App::LoadScene(std::string SceneName)
{
	camera.focus = Vec3D(0, 0, 0);
	camera.FirstUpdate(ImGui::GetIO().DeltaTime, State == GameState::GS_PLAY);
	auto begin = std::chrono::high_resolution_clock::now();
	lightManager.DirLights.clear();
	lightManager.PointLights.clear();
	lightManager.SpotLights.clear();
#if JSON
	std::string filename = "Assets/scenes/" + (std::string)SceneName + ".json";
	uint32_t size = 0;
	std::ifstream is(filename, std::ifstream::binary);
	if (!is)
	{
		LOG(LogType::L_ERROR, "File %s cannot be found", filename);
		std::string filename2 = "Assets/scenes/" + (std::string)SceneName + ".txt";
		std::ifstream is2(filename2, std::ifstream::binary);
		if (!is2)
		{
			LOG(LogType::L_ERROR, "File %s cannot be found", filename2);
			return;
		}
		else
		{
			SceneNode = (Node::LoadScene(filename2, resourcesManager.Resources));
		}
	}
	else
	{
		SceneNode = new Node();
		ScriptManager.GetScript("SceneLoading")->ExecuteFunction("LoadScene", SceneNode, this, "Assets/scenes/" + (std::string)SceneName + ".json");
	}
#else
	std::string filename = "Assets/scenes/" + (std::string)SceneName + ".txt";
	uint32_t size = 0;
	std::ifstream is(filename, std::ifstream::binary);
	if (!is)
	{
		LOG(LogType::L_ERROR, "File %s cannot be found", filename);
		std::string filename2 = "Assets/scenes/" + (std::string)SceneName + ".json";
		std::ifstream is2(filename2, std::ifstream::binary);
		if (!is2)
		{
			LOG(LogType::L_ERROR, "File %s cannot be found", filename2);
			return;
		}
		else
		{
			SceneNode = new Node();
			ScriptManager.GetScript("SceneLoading")->ExecuteFunction("LoadScene", SceneNode, this, "Assets/scenes/" + (std::string)SceneName + ".json");
		}
	else
	{
		SceneNode = (Node::LoadScene(filename, resourcesManager.Resources));
	}
#endif
	SceneNode->Name = SceneName;

	auto list = SceneNode->GetComponentsInChildrens<Collider*>();
	collisionManager.Colliders = { std::make_move_iterator(list.begin()),std::make_move_iterator(list.end()) };

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	LOG(LogType::INFO, "Scene Loading Time : %.5f seconds.\n", elapsed.count() * 1e-9);
}

static std::string sTmpSceneName;
void App::BeginPlay()
{
	editorUi.inspector.Selected = nullptr;
	editorUi.HideOnPlay();
	State = GameState::GS_PLAY;
	sTmpSceneName = SceneNode->Name;
	SaveScene("TEMPSCENE");

	camera.input.MouseCaptured = true;
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void App::EndPlay()
{
	editorUi.ShowAll();
	State = GameState::GS_EDITOR;
	SceneNode->children.clear();
	LoadScene("TEMPSCENE");
	SceneNode->Name = sTmpSceneName;
	if (!remove("Assets/scenes/TEMPSCENE.json")) {
		LOG(LogType::INFO, "Sucessfuly delete Assets/scenes/TEMPSCENE.json");
	}
	else {
		LOG(LogType::WARNING, "Can't delete Assets/scenes/TEMPSCENE.json");
	}
	if (!remove("Assets/scenes/TEMPSCENE.txt")) {
		LOG(LogType::INFO, "Sucessfuly delete Assets/scenes/TEMPSCENE.txt");
	}
	else {
		LOG(LogType::WARNING, "Can't delete Assets/scenes/TEMPSCENE.txt");
	}
	camera.input.MouseCaptured = false;
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	camera.focus = Vec3D(0, 0, 0);
}

void App::CopyToClipboard(Node* node)
{
	std::ofstream _file;
	std::string filepath = "Clipboard.txt";
	_file.open(filepath);
	node->Save(_file, "");
}


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_App, m) {
	py::class_<App>(m, "App")
		.def("AddToWaitlist", &App::AddToWaitlist, pybind11::return_value_policy::reference)
		.def("GetResourcesManager", &App::GetResourcesManager, pybind11::return_value_policy::reference);
	m.def("MULTITHREAD", []() {if (MULTITHREAD) return true; else return false; });
}