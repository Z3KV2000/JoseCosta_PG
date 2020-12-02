#include "Personagem.h"

Personagem::Personagem()
{
	initialize();
}

void Personagem::initialize()
{
	
	spr_Largura = 1.0 / 8.0;
	spr_Altura = 1.0 / 2.0;

	iFrame = 4;
	iAnim = 1;
	frameRate = 0;

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   spr_Largura + iFrame * spr_Largura, spr_Altura + iAnim * spr_Altura, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   spr_Largura + iFrame * spr_Largura, 0.0f + iAnim * spr_Altura, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f + iFrame * spr_Largura, 0.0f + iAnim * spr_Altura, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f + iFrame * spr_Largura, spr_Altura + iAnim * spr_Altura  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	transform = glm::mat4(1); 
	texID = -1;
	shader = NULL; 
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	angle = 0.0f;
}

void Personagem::setTexture(int texID)
{
	this->texID = texID;
}

void Personagem::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::rotate(transform, angle, axis);
}

void Personagem::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::translate(transform, displacements);
}

void Personagem::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::scale(transform, scaleFactors);
	scale = scaleFactors;
}

void Personagem::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shader->ID, "ourTexture1"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Personagem::update()
{
	

	setTranslation(pos);
	setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
	setScale(scale, false);
	animate();
	GLint transformLoc = glGetUniformLocation(shader->ID, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	
}

void Personagem::animate()
{	
	if(frameRate < 60) frameRate++;
	else
	{
		frameRate = 0;

		//iAnim;
		if(iFrame < 8) iFrame++;
		else iFrame = 0;
	}

	

	float vertices[]{
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   spr_Largura + iFrame * spr_Largura, spr_Altura + iAnim * spr_Altura, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   spr_Largura + iFrame * spr_Largura, 0.0f + iAnim * spr_Altura, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f + iFrame * spr_Largura, 0.0f + iAnim * spr_Altura, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f + iFrame * spr_Largura, spr_Altura + iAnim * spr_Altura  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

void Personagem::setDireita()
{
	iAnim = 1;
}

void Personagem::setEsquerda()
{
	iAnim = 0;
}
