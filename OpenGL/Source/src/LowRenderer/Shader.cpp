#include "../Source/includes/Resources/Shader.hpp"
const char* ReadFile(std::filesystem::path const& filename);

Shader::Shader() {}
Shader::~Shader()
{
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

bool Shader::SetShader(std::string filename)
{
	Name = filename;
	filename += "/vertex.glsl";
	const char* vertexShaderSources = ReadFile(filename);
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderSources, NULL);
	glCompileShader(VertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
		LOG(LogType::L_ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED %s", infoLog);
		return false;
	}
	else
		LOG(LogType::INFO, "Successfully loaded Shader: %s", filename.c_str());

	return true;
}
bool Shader::SetFragmentShader(std::string filename)
{
	filename += "/fragment.glsl";
	const char* fragmentShaderSource = ReadFile(filename);
	int success;
	char infoLog[512];
	// fragment shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	// check for shader compile errors
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		LOG(LogType::L_ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s", infoLog);
		return false;
	}
	else
		LOG(LogType::INFO, "Successfully loaded Shader: %s", filename.c_str());

	return true;
}
bool Shader::Link(bool screen)
{
	if (_linked)
		return false;
	ScreenShader = screen;
	// link shaders
	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);
	// check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		LOG(LogType::L_ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED %s", infoLog);
		return false;
	}
	else {
		LOG(LogType::INFO, "Successfully link Shader %s", Name.c_str());
		_linked = true;
	}

	Initialize();
	return true;
}

void Shader::Initialize()
{
	glUseProgram(this->Program);
	_location[Location::L_MVP] = glGetUniformLocation(Program, "MVP");
	_location[Location::L_M] = glGetUniformLocation(Program, "M");
	_location[Location::L_TIME] = glGetUniformLocation(Program, "time");
	_location[Location::L_ENABLE_TEXTURE] = glGetUniformLocation(Program, "EnableTexture");
	_location[Location::L_TEXTURE] = glGetUniformLocation(Program, "Tex");
	_location[Location::L_MATERIAL] = glGetUniformLocation(Program, "Mat");
	_location[Location::L_NORMAL_MAP] = glGetUniformLocation(Program, "NormalMap");
	_location[Location::L_DIRECTIONAL] = glGetUniformLocation(Program, "DirLight");
	_location[Location::L_POINT] = glGetUniformLocation(Program, "PointLight");
	_location[Location::L_SPOT] = glGetUniformLocation(Program, "SpotLight");
	_location[Location::L_COLOR] = glGetUniformLocation(Program, "Color");
}

int Shader::GetLocation(Location loc)
{
	return _location[loc];
}

const char* ReadFile(std::filesystem::path const& filename)
{
	std::ifstream is(filename, std::ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		std::streamoff length = is.tellg();
		is.seekg(0, is.beg);

		char* buffer = new char[length];

		//LOG(LogType::INFO, "Reading %d characters... ", length);
		// read data as a block:
		is.read(buffer, length);
		if (is) {
			//LOG(LogType::INFO, "all characters read successfully.");
		}
		else {
			//LOG(LogType::L_ERROR, "error: only %d could be read", (int)is.gcount());
		}
		is.close();
		buffer[length] = 0;
		return buffer;
	}
	return 0;
}

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(S_Shader, m) {
	py::class_<Shader>(m, "Shader")
		.def(py::init<>());
}