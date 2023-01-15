#pragma once
#include <vector>
#include <glad/glad.h>
#include <unordered_map>
#include <map>
#include <ImGui/imgui.h>
#include "../Core/Maths/Maths.hpp"
#include "../Resources/Texture.hpp"
#include "../Resources/Material.hpp"
#include "../Resources/Component.hpp"
#include "../Resources/Shader.hpp"
enum Buffers : GLuint
{
	POS,
	NML,
	TUV,
	TAN
};
class Mesh : public Component, public IResource
{
public:
	std::string Name;

	Material CurrentMaterial = Material();
	Texture* CurrentTexture = nullptr;
	Shader* CurrentShader = nullptr;

	unsigned int _VBO = 0;
	unsigned int _VAO = 0;

	std::vector<Int3D> Indices;
	std::vector<Vec3D> Positions;
	std::vector<Vec2D> TextureUVs;
	std::vector<Vec3D> Normals;

	Mesh();
	~Mesh();

	std::vector<float> Vertices;

	// Loop to re-organize vertices.
	void Loop(std::vector<unsigned int>& indices, std::vector<float>& vertices);
	// Initialize The Mesh.
	void Start() override;
	void Initialize() override;
	// Draw The Mesh.
	void Update(Mat4D MVP);
	void Draw(Mat4D MVP, bool outline = false);
	void Outline(Mat4D MVP);

	// Setters
	void SetShader(Shader* shader);

	// Imgui Draw.
	void ShowComponent();
	Mesh GlobalMesh(std::vector<bool>&);
	static Texture* TexturePopup(std::string WindowName);

	// Save and Load.
	void Save(std::string space, std::string& line) override;
	Mesh* Load(const char* data, uint32_t& pos) override;

	void SetLoaded(bool value) { _loaded = value; }
	bool IsLoaded() { return _loaded; }
	void Copy(Mesh* mesh)
	{
		if (mesh == nullptr)
			return;
		auto tmp = mesh;
		Indices = mesh->Indices;
		Positions = mesh->Positions;
		TextureUVs = mesh->TextureUVs;
		Normals = mesh->Normals;
		Vertices = mesh->Vertices;
		_VBO = mesh->_VBO;
		_VAO = mesh->_VAO;
		_loaded = mesh->_loaded;
		this->R_Name = mesh->R_Name;
	}
private :
	bool _loaded = false;

	int CurrShaderIndex = -1;
	int LastShaderIndex = -1;
};