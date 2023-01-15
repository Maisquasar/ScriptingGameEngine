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
  float theta = time;
  
  vec3 dir1 = vec3(cos(theta), 0, sin(theta)); 
  vec3 dir2 = vec3(sin(theta), 0, cos(theta));
  vec3 dir3 = vec3(-sin(theta), 0, cos (theta));
  
  float diffuse1 = pow(dot(Normal,dir1),2.0);
  float diffuse2 = pow(dot(Normal,dir2),2.0);
  float diffuse3 = pow(dot(Normal,dir3),2.0);
  
  vec3 col1 = diffuse1 * vec3(1,0,0);
  vec3 col2 = diffuse2 * vec3(0,0,1);
  vec3 col3 = diffuse3 * vec3(0,1,0);
  
  FragColor = vec4(col1 + col2 + col3, 1);
}