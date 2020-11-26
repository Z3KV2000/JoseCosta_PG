#version 400 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord); // * vec4(ourColor, 1.0);

	float media = color.r * 0.3 + color.g * 0.59 + color.b * 0.11;

	color.r = media;
	color.g = media;
	color.b = media;

}