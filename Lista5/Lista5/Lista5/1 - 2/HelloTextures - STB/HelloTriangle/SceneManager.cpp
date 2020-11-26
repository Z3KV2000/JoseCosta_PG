

#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
float xPosition = 0;
float yPosition = 0;
float velocity = 0.005;
float roadPositions[] = {-3, -2, -1, 0, 1, 2, 3};
float grassPositions[] = { -4, -3, -2, -1, 0, 1, 2, 3 };
float treesPositions[] = {-7.5, -6.25 ,-5, -3.75, -2.5, -1.25, 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5};
float birdPosition = -5;
float obstaclePositionX = 20;
float obstaclePositionY = -0.8;
float leftSideObstacleX = obstaclePositionX;
float rightSideObstacleX = obstaclePositionX + .5;
float bottomSideObstacleY = obstaclePositionY - .5;
float topSideObstacleY = obstaclePositionY + .5;
int passedCars = 1;
bool endgame = false;
int totalCars = 0;
bool win = false;

Shader* shader;

static string binarizacao = "../shaders/binarizacao.frag";
static string grayScalePonderada = "../shaders/grayScalePonderada.frag";
static string oneChannel = "../shaders/oneChannel.frag";
static string corModificadora = "../shaders/corModificadora.frag";
static string filtroJose = "../shaders/filtroJose.frag";

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Trabalho GA", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}


	string vertexShader = "../shaders/transformations.vs";


	shaderGrayScale = new Shader(vertexShader.c_str(), grayScalePonderada.c_str());

	shaderBinarizacao = new Shader(vertexShader.c_str(), binarizacao.c_str());

	shaderOneChannel = new Shader(vertexShader.c_str(), oneChannel.c_str());

	shaderCorModificadora = new Shader(vertexShader.c_str(), corModificadora.c_str());

	shaderFiltroFabi = new Shader(vertexShader.c_str(), filtroFabi.c_str());

	shader = shaderBinarizacao;
	//setup the scene 
	// CONFIGURA��O
	setupScene();

	resized = true; 

}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void SceneManager::endGame() {

	if (xPosition <= rightSideObstacleX && xPosition >= leftSideObstacleX) {
		if (yPosition == obstaclePositionY) {
			//glfwSetWindowShouldClose(window, GL_TRUE);
			endgame = true;
		}

		else if (yPosition + 0.2 >= bottomSideObstacleY - 0.5 && yPosition - 0.2 <= topSideObstacleY) {
			//glfwSetWindowShouldClose(window, GL_TRUE);
			endgame = true;
		}

	}

}

void SceneManager::do_movement()
{
	//Se apertou esc (
	if (keys[GLFW_KEY_ESCAPE])
	glfwSetWindowShouldClose(window, GL_TRUE);

	//Tecla para mover o carro para cima
	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
		if(endgame || win) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (yPosition + 0.1 <= 2)
			yPosition += 0.1;
	}
	//Tecla para mover o carro para baixo
	if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
		if(endgame || win) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (yPosition - 0.1 >= -2)
			yPosition -= 0.1;
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	// Create transformations 
	model = glm::mat4();
	model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 transform = glm::mat4(1.0f); // initializing identity matrix

	transform = glm::translate(transform, glm::vec3(xPosition, yPosition, 0.0f));

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//Desenha todo o cen�rio do jogo
	drawGrassRoad(transform);
	drawRoad(transform);
	drawCar(transform);
	drawObstacle(transform);
	drawTrees(transform);
	drawBird(transform);
	//drawMinhaImagem(transform);

	//Em caso de colis�o, apresenta mensagem de fim de jogo
	if (endgame) {
		velocity = 0;
		drawEndGame(transform);
	}

	if (totalCars >= 6) {
		velocity = 0;
		win = true;
		drawWin(transform);
	}
}

void SceneManager::drawObstacle(glm::mat4 transform) {

	obstaclePositionX -= velocity * 10;
	rightSideObstacleX = obstaclePositionX + 2;
	leftSideObstacleX = obstaclePositionX + .5;
	
	//Calcula a posi��o dos carros obst�culo
	if (obstaclePositionX < -10) {
		obstaclePositionX = 10;
		obstaclePositionY = -0.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8 - (-0.8))));
		bottomSideObstacleY = obstaclePositionY - .5;
		topSideObstacleY = obstaclePositionY + .5;
		passedCars++;

		cout << "Bottom obstacle " << bottomSideObstacleY << "\n";
		cout << "Top obstacle " << topSideObstacleY << "\n";
		cout << "y normal car " << yPosition << "\n";
		totalCars++;
	}

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE, texture[6]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
	
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	transform = updateTransform(obstaclePositionX, obstaclePositionY, 0.0f, 0.5f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


}

void SceneManager::drawRoad(glm::mat4 transform){
	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	for (int i = 0; i < 7; i++) {
		transform = updateTransform(roadPositions[i], 0.0f, 0.0f, 1.3f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Acerta a velocidade de movimenta��o da estrada
		roadPositions[i] -= velocity;

		if (roadPositions[i] < -2) {
			roadPositions[i] = 2.5;
		}

	}

}


void SceneManager::drawEndGame(glm::mat4 transform) {
	
		GLint modelLoc;

		// bind Texture
		// Bind Textures using texture units

		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);


		transform = updateTransform(0, 0, 0.0f, 0.8f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}

void SceneManager::drawWin(glm::mat4 transform) {

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);


	transform = updateTransform(0, 0, 0.0f, 0.8f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}

void SceneManager::drawMinhaImagem(glm::mat4 transform) {

	changeShader(shaderFiltroFabi);

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	transform = updateTransform(0, 0, 0.0f, 0.8f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void SceneManager::drawGrassRoad(glm::mat4 transform)
{

	changeShader(shaderGrayScale);

	//shader->Use();

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

//	setupCamera2D();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);


	for (int i = 0; i < 8; i++) {
		transform = updateTransform(grassPositions[i], 1.0f, 0.0f, 0.7f);
		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = updateTransform(-grassPositions[i], 1.0f, 0.0f, 0.7f, glm::radians(180.0f));

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		grassPositions[i] -= velocity;

		if (grassPositions[i] < -4) {
			grassPositions[i] = 3;
		}

	}

	changeShader(shaderBinarizacao);

};

void SceneManager::changeShader(Shader* newShader) {
	shader = newShader;
	shader->Use();
	setupCamera2D();
}

void SceneManager::drawCar(glm::mat4 transform) {

	changeShader(shaderOneChannel);

	GLuint modelLoc;

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	transform = updateTransform(-2.5, yPosition, 0.0f, 0.25f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void SceneManager::drawBird(glm::mat4 transform) {

	changeShader(shaderCorModificadora);

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	transform = updateTransform(birdPosition, 3.5f, 0.0f, 0.25f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	transform = updateTransform(birdPosition, -3.5f, 0.0f, 0.25f);

	modelLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (!endgame && velocity > 0) {
		birdPosition += 0.1;

		if (birdPosition >= 10) {
			birdPosition = -5;
		}
	}

}

void SceneManager::drawTrees(glm::mat4 transform)
{
	changeShader(shaderFiltroFabi);

	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	for (int i = 0; i < 14; i++)
	{
		transform = updateTransform(treesPositions[i], 3.5f, 0.0f, 0.25f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = updateTransform(treesPositions[i], -3.5f, 0.0f, 0.25f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		treesPositions[i] -= velocity;

		if (treesPositions[i] < -8) {
			treesPositions[i] = 8;
		}

	}

};

glm::mat4 SceneManager::updateTransform(float x, float y, float z, float scale, float rotate)
{
	glm::vec3 pivot = glm::vec3(0.5f, 0.5f, 0.0f);
	glm::mat4 transUpdate = glm::mat4(1.0f);
	transUpdate = glm::rotate(transUpdate, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	transUpdate = glm::scale(transUpdate, glm::vec3(scale));
	transUpdate = glm::translate(transUpdate, glm::vec3(x, y, z));

	return transUpdate;


}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		//Render scene
		render();

		if (passedCars % 3 == 0) {
			velocity += 0.005;
			passedCars = 1;
		}
		
		// Swap the screen buffers
		glfwSwapBuffers(window);

		endGame();
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO, RBO;
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

	setupTexture();

}

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -1.0, xMax = 1.0, yMin = -1.0, yMax = 1.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{

	glGenTextures(8, texture);

	for (int i = 0; i < 7; i++) {

		// load and create a texture 
		// -------------------------
		glBindTexture(GL_TEXTURE_2D, texture[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											   // set the texture wrapping parameters
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		unsigned char *data;
		//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);

		if (i == 0) {
			data = stbi_load("../textures/tree.png", &width, &height, &nrChannels, 0);
		}

		else if (i == 1) {
			data = stbi_load("../textures/grass.png", &width, &height, &nrChannels, 0);
		}

		else if (i == 2) {
			data = stbi_load("../textures/road.png", &width, &height, &nrChannels, 0);
		}

		else if (i == 3) {
			data = stbi_load("../textures/car.png", &width, &height, &nrChannels, 0);
		}

		else if (i == 4) {
			data = stbi_load("../textures/birdflying.png", &width, &height, &nrChannels, 0);
		}
		else if (i == 5) {
			data = stbi_load("../textures/you_died.png", &width, &height, &nrChannels, 0);
		}
		else if (i == 6) {
			data = stbi_load("../textures/win.png", &width, &height, &nrChannels, 0);
		}
		else {
			data = stbi_load("../textures/car.png", &width, &height, &nrChannels, 0);
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	} 

	glEnable(GL_BLEND);   
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
