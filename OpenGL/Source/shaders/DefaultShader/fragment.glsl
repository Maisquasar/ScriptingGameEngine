#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 OurColor;

uniform sampler2D Tex;
uniform vec3 CameraPos;
uniform bool EnableTexture;
uniform float time;

uniform float DirLight[8 * 21];
uniform float PointLight[8 * 21];
uniform float SpotLight[8 * 26];
uniform float Mat[13];

uniform sampler2D frameBuffer; 

vec4 GetMatAmbient()
{
	return vec4(Mat[0], Mat[1], Mat[2], Mat[3]);
}
vec4 GetMatDiffuse()
{
	return vec4(Mat[4], Mat[5], Mat[6], Mat[7]);
}
vec4 GetMatSpecular()
{
	return vec4(Mat[8], Mat[9], Mat[10], Mat[11]);
}
float GetMatShininess()
{
	return Mat[12];
}

bool GetDirEnable(int i)
{
	return DirLight[21 * i] == 1;
}
vec3 GetDirDirection(int i)
{
	return vec3(DirLight[21 * i + 1], DirLight[21 * i + 2], DirLight[21 * i + 3]);
}
vec3 GetDirAmbient(int i)
{
	return vec3(DirLight[21 * i + 11], DirLight[21 * i + 12], DirLight[21 * i + 13]);
}
vec3 GetDirDiffuse(int i)
{
	return vec3(DirLight[21 * i + 7], DirLight[21 * i + 8], DirLight[21 * i + 9]);
}
vec3 GetDirSpecular(int i)
{
	return vec3(DirLight[21 * i + 15], DirLight[21 * i + 16], DirLight[21 * i + 17]);
}
float GetDirAmbientStrenght(int i)
{
	return DirLight[21 * i + 19];
}
float GetDirSpecularStrenght(int i)
{
	return DirLight[21 * i + 20];
}

bool GetPointEnable(int i)
{
	return PointLight[21 * i] == 1;
}
vec3 GetPointPosition(int i)
{
	return vec3(PointLight[21 * i + 4], PointLight[21 * i + 5], PointLight[21 * i + 6]);
}
vec3 GetPointParameters(int i)
{
	return vec3(PointLight[21 * i + 1], PointLight[21 * i + 2], PointLight[21 * i + 3]);
}
vec3 GetPointAmbient(int i)
{
	return vec3(PointLight[21 * i + 11], PointLight[21 * i + 12], PointLight[21 * i + 13]);
}
vec3 GetPointDiffuse(int i)
{
	return vec3(PointLight[21 * i + 7], PointLight[21 * i + 8], PointLight[21 * i + 9]);
}
vec3 GetPointSpecular(int i)
{
	return vec3(PointLight[21 * i + 15], PointLight[21 * i + 16], PointLight[21 * i + 17]);
}
float GetPointAmbientStrenght(int i)
{
	return PointLight[21 * i + 19];
}
float GetPointSpecularStrenght(int i)
{
	return PointLight[21 * i + 20];
}

bool GetSpotEnable(int i)
{
	return SpotLight[26 * i] == 1;
}
vec3 GetSpotPosition(int i)
{
	return vec3(SpotLight[26 * i + 6], SpotLight[26 * i + 7], SpotLight[26 * i + 8]);
}
vec3 GetSpotDirection(int i)
{
	return vec3(SpotLight[26 * i + 1], SpotLight[26 * i + 2], SpotLight[26 * i + 3]);
}
vec3 GetSpotParameters(int i)
{
	return vec3(SpotLight[26 * i + 23], SpotLight[26 * i + 24], SpotLight[26 * i + 25]);
}
vec3 GetSpotAmbient(int i)
{
	return vec3(SpotLight[26 * i + 13], SpotLight[26 * i + 14], SpotLight[26 * i + 15]);
}
vec3 GetSpotDiffuse(int i)
{
	return vec3(SpotLight[26 * i + 9], SpotLight[26 * i + 10], SpotLight[26 * i + 11]);
}
vec3 GetSpotSpecular(int i)
{
	return vec3(SpotLight[26 * i + 17], SpotLight[26 * i + 18], SpotLight[26 * i + 19]);
}
float GetSpotAmbientStrenght(int i)
{
	return SpotLight[26 * i + 21];
}
float GetSpotSpecularStrenght(int i)
{
	return SpotLight[26 * i + 22];
}
vec2 GetSpotCut(int i)
{
	return vec2(SpotLight[26 * i + 4], SpotLight[26 * i + 5]);
}

vec4 CalcDirLight(int i) {
	// Ambient
	vec4 ambient = vec4(GetDirAmbientStrenght(i) * GetDirAmbient(i), 1) * GetMatAmbient();
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(GetDirDirection(i));
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = (diff * GetMatDiffuse()) * vec4(GetDirDiffuse(i), 1);

	//Specular
	vec3 viewDir = normalize(CameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat[12]);
	vec4 specular = (GetMatSpecular() * GetDirSpecularStrenght(i) * spec) * vec4(GetDirSpecular(i), 1);

	vec4 result = (ambient + diffuse + specular);
	return result;
}

vec4 CalcPointLight(int i) {
	// Ambient
	vec4 ambient = vec4(GetPointAmbientStrenght(i) * GetPointAmbient(i), 1) * GetMatAmbient();

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(GetPointPosition(i) - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = (diff * GetMatDiffuse()) * vec4(GetPointDiffuse(i), 1);

	//Specular
	vec3 viewDir = normalize(CameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat[12]);
	vec4 specular = (GetMatSpecular() * GetPointSpecularStrenght(i) * spec) * vec4(GetPointSpecular(i), 1);

	// Attenuation
	vec3 parameters = GetPointParameters(i);
	float distance = length(GetPointPosition(i) - FragPos);
	float attenuation = 1.0 / (parameters.x + parameters.y * distance + parameters.z * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	vec4 result = (ambient + diffuse + specular);
	return result;
}

vec4 CalcSpotLight(int i) {
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(GetSpotPosition(i) - FragPos);
	float theta = dot(lightDir, normalize(-GetSpotDirection(i)));
	vec2 cut = GetSpotCut(i);
	float epsilon = - cut.x + cut.y;
	float intensity = clamp((theta - cut.y) / epsilon, 0.0, 1.0);
	// Ambient
	vec4 ambient = vec4(GetSpotAmbientStrenght(i) * GetSpotAmbient(i), 1) * GetMatAmbient();

	vec4 result;
	if (theta > cut.x)
	{
		// Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = (diff * GetMatDiffuse()) * vec4(GetSpotDiffuse(i), 1);

		//Specular
		vec3 viewDir = normalize(CameraPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), Mat[12]);
		vec4 specular = (GetMatSpecular() * GetSpotSpecularStrenght(i) * spec) * vec4(GetSpotSpecular(i), 1);

		vec3 parameters = GetSpotParameters(i);
		float distance = length(GetSpotPosition(i) - FragPos);
		float attenuation = 1.0 / (parameters.x + parameters.y * distance + parameters.z * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;

		diffuse *= intensity;
		specular *= intensity;

		result = (ambient + diffuse + specular);
	}
	else
		result = ambient;
	return result;
}

vec4 uOutlineColor = vec4(1, 0, 0, 1);

void main()
{
	vec4 GlobalLight = vec4(0, 0, 0, 0);
	for (int i = 0; i < 8; i++)
	{
		if (GetDirEnable(i))
			GlobalLight += CalcDirLight(i);
	}
	for (int i = 0; i < 8; i++)
	{
		if (GetPointEnable(i))
			GlobalLight += CalcPointLight(i);
	}
	for (int i = 0; i < 8; i++)
	{
		if (GetSpotEnable(i))
			GlobalLight += CalcSpotLight(i);
	}
	if (EnableTexture)
		FragColor = texture(Tex, TexCoord) * GlobalLight;
	else
		FragColor = vec4((OurColor * GlobalLight).xyz, 1);
}

