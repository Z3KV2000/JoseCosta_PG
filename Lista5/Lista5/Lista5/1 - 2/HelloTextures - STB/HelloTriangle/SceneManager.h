#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	glm::mat4 updateTransform(float x, float y, float z, float scale = 1.0f, float rotate = 0);
	void drawRoad(glm::mat4 transform);
	void drawGrassRoad(glm::mat4 transform);
	void drawTrees(glm::mat4 transform);
	void drawCar(glm::mat4 transform);
	void drawBird(glm::mat4 transform);
	void drawObstacle(glm::mat4 transform);
	void drawEndGame(glm::mat4 transform);
	void drawWin(glm::mat4 transform);
	void drawMinhaImagem(glm::mat4 transform);
	void do_movement();
	void render();
	void run();
	void finish();
	void endGame();

	void changeShader(Shader* shader);

	// scene
	void setupScene();
	void setupCamera2D();
	void setupTexture(); //apenas mostra como criar uma textura
private:
	
	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shaderGrayScale;

	Shader *shaderBinarizacao;

	Shader *shaderOneChannel;

	Shader *shaderCorModificadora;

	Shader *shaderFiltroFabi;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture[];

};

