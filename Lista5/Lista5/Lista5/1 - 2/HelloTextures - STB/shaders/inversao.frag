#version 400 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord); // * vec4(ourColor, 1.0);

	color.r = color.r ^ 255;
	color.g = color.g ^ 255;
	color.b = color.b ^ 255;
	
}