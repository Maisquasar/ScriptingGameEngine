#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Tex;

uniform float time;

float noiseQuantity = 0.3;

highp float random1d(float dt) {
    highp float c = 43758.5453;
    highp float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}


highp float noise1d(float value) {
	highp float i = floor(value);
	highp float f = fract(value);
	return mix(random1d(i), random1d(i + 1.0), smoothstep(0.0, 1.0, f));
}


highp float random2d(vec2 co) {
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy, vec2(a, b));
    highp float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}

void main()
{
float theta = time;
	// Calculate the effect relative strength
	float strength = (0.3 + 0.7 * noise1d(0.3 * theta)) * noiseQuantity; // u_resolution.x;

	// Calculate the effect jump at the current theta interval
	float jump = 500.0 * floor(0.3 * (noiseQuantity) * (theta + noise1d(theta)));

	// Shift the texture coordinates
	vec2 uv = TexCoords;
	uv.y += 0.2 * strength * (noise1d(5.0 * TexCoords.y + 2.0 * theta + jump) - 0.5);
	uv.x += 0.1 * strength * (noise1d(100.0 * strength * uv.y + 3.0 * theta + jump) - 0.5);

	// Get the texture pixel color
	vec3 pixel_color = texture2D(Tex, uv).rgb;

	// Add some white noise
	pixel_color += vec3(5.0 * strength * (random2d(TexCoords + 1.133001 * vec2(theta, 1.13)) - 0.5));

	// Fragment shader output
	FragColor = vec4(pixel_color, 1.0);

}