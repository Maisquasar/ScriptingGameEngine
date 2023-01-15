#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec4 OurColor;
in vec3 FragPos;

uniform sampler2D Tex;
uniform vec3 CameraPos;
uniform bool EnableTexture;
uniform float time;

void main()
{
  FragColor = vec4(Normal, 1.0);
}