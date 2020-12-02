#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Personagem
{
public:
	Personagem();
	~Personagem() {}

	void initialize();

	//Para setar os atributos 
	void setTexture(int texID);
	void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
	void setPosition(glm::vec3 pos) { this->pos = pos; } 
	void setDimension(glm::vec3 scale) { this->scale = scale; }
	void setAngle(float angle) { this->angle = angle; } 

	void animate();
	void setDireita();
	void setEsquerda();

	
	void setRotation(float angle, glm::vec3 axis, bool reset = true);
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);

	//Chamados no ciclo do SceneManager
	void draw();
	void update();

	//TO DO: m�todos para anima��o

protected:
	//Atributos gerais

	GLuint VAO; //ID do buffer de geometria

	glm::mat4 transform; //matriz de transforma��o

	unsigned int texID; //identificador da textura

	Shader* shader; 

	//Atributos de controle da posi��o, orienta��o e dimens�es do sprite
	glm::vec3 pos, scale;
	float angle;

	//Atributos para controle da anima��o
	float spr_Largura;
	float spr_Altura;
	int nAnims, nFrames;
	int iAnim, iFrame;
	int frameRate;

};


