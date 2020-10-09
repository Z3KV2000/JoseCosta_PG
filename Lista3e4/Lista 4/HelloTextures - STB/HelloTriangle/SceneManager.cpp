#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
float xPosition;
float yPosition;

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
	window = glfwCreateWindow(width, height, "Lista 4", nullptr, nullptr);
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

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
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
	if (xPosition >= 1 || xPosition <= -1) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (yPosition >= 1 || yPosition <= -1) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D])
		xPosition += 0.0005;

	if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A])
		xPosition -= 0.0005;

	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W])
		yPosition += 0.0005;

	if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S])
		yPosition -= 0.0005;
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

	drawGrassRoad(transform);
	drawRoad(transform);
	drawTrees(transform);


}

void SceneManager::drawRoad(glm::mat4 transform){
	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	for (float i = -1.0f; i < 2.0f; i++)
	{
		transform = updateTransform(float(i), 0.0f, 0.0f, 1.3f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void SceneManager::drawGrassRoad(glm::mat4 transform)
{
	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	for (float i = -4.0f; i < 4; i += 1.00f)
	{
		transform = updateTransform(float(i), 1.0f, 0.0f, 0.7f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = updateTransform(float(i), 1.0f, 0.0f, 0.7f, glm::radians(180.0f));

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

void SceneManager::drawTrees(glm::mat4 transform)
{
	GLint modelLoc;

	// bind Texture
	// Bind Textures using texture units

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	for (float i = -4.5f; i < 4.5f; i+=1.25f)
	{
		transform = updateTransform(float(i), 3.5f, 0.0f, 0.25f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		transform = updateTransform(float(i), -3.5f, 0.0f, 0.25f);

		modelLoc = glGetUniformLocation(shader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	glGenTextures(3, texture);

	for (int i = 0; i < 3; i++) {

		// load and create a texture 
		// -------------------------
		glBindTexture(GL_TEXTURE_2D, texture[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											   // set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		unsigned char *data;
		//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		if (i == 0) {
			data = stbi_load("../textures/tree.png", &width, &height, &nrChannels, 0);
		}
		else if (i == 2) {
			data = stbi_load("../textures/road.png", &width, &height, &nrChannels, 0);
		}
		else {
			data = stbi_load("../textures/grass.png", &width, &height, &nrChannels, 0);
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
