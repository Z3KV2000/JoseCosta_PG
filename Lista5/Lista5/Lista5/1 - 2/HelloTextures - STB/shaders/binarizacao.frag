#version 400 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord); // * vec4(ourColor, 1.0);

	float k = 0.35;

	float media = (color.r + color.g + color.b) / 3.0;

	color.r = media;
	color.g = media;
	color.b = media;

	if (color.r > k){
		color.r = 1;
	}

	else {
		color.r = 0;
	}

	if (color.g > k){
		color.g = 1;
	}

	else {
		color.g = 0;
	}

	if (color.b > k){
		color.b = 1;
	}

	else {
		color.b = 0;
	}

	
}