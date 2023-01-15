#include "../Source/includes/Resources/Component.hpp"
#include "../Source/includes/Core/DataStructure/Node.hpp"
#include "../Source/includes/Core/App.hpp"
Component::Component()
{
	ComponentName = "Empty";
}
Component::~Component() {}

bool Component::IsInPause() { return gameObject->IsInPause(); }

bool* Component::GetPausePtr() { return gameObject->GetPausePtr(); }

GameState Component::GetGameState() { return gameObject->GetGameState(); }

Transform* Component::GetTransform() { return &gameObject->transform; }

std::unordered_map<std::string, IResource*>* Component::GetResourcesManager() { return &app.GetResourcesManager()->Resources; }

class PyComponent : public Component {
public:
	using Component::Component;
	// Virtual Pure Function
	void Update() override {
		PYBIND11_OVERRIDE_PURE(
			void, /* Return type */
			Component,      /* Parent class */
			Update,          /* Name of function in C++ (must match Python name) */
			);
	}
	void GameUpdate() override {
		PYBIND11_OVERRIDE_PURE(
			void, /* Return type */
			Component,      /* Parent class */
			GameUpdate,          /* Name of function in C++ (must match Python name) */
		);
	}
	void EditorUpdate() override {
		PYBIND11_OVERRIDE_PURE(
			void, /* Return type */
			Component,      /* Parent class */
			EditorUpdate,          /* Name of function in C++ (must match Python name) */
			);
	}
	void Start() override {
		PYBIND11_OVERRIDE_PURE(
			void, /* Return type */
			Component,      /* Parent class */
			Start,          /* Name of function in C++ (must match Python name) */
			);
	}
	void Initialize() override {
		PYBIND11_OVERRIDE_PURE(
			void, /* Return type */
			Component,      /* Parent class */
			Initialize,          /* Name of function in C++ (must match Python name) */
			);
	}
};


namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(Components, m) {
	py::class_<Component>(m, "Component")
		.def(py::init<>())
		.def("GetTransform", &Component::GetTransform, py::return_value_policy::reference)
		.def("IsInPause", &Component::IsInPause)
		.def_readwrite("enable", &Component::enable)
		.def_readwrite("windowOpen", &Component::windowOpen);
	m.def("CreateComponent", []() { return new Component(); }, pybind11::return_value_policy::reference);
		//.def("gameObject", &Component::gameObject);
}

ComponentsData CData;

ComponentsData::ComponentsData()
{
	Initialize();
}
ComponentsData::~ComponentsData()
{

}

void ComponentsData::Initialize()
{
	NewComponent(new Mesh);
	NewComponent(new PointLight);
	NewComponent(new DirectionalLight);
	NewComponent(new SpotLight);
	NewComponent(new RotationController);
	NewComponent(new BoxCollider);
	NewComponent(new SphereCollider);
	NewComponent(new CapsuleCollider);
	NewComponent(new CameraLock);
	NewComponent(new Rigidbody);
	NewComponent(new PlayerScript);
	NewComponent(new MainMenu);
	NewComponent(new PauseMenu);
}
