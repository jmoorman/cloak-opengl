// Cloak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AnimatedMesh.h"
#include "Animation.h"
#include "Camera.h"
#include "EventManager.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"

#define SELECTED_GL_MAJOR_VERSION 4
#define SELECTED_GL_MINOR_VERSION 5

void exitWithSDLError() {
	std::cout << SDL_GetError() << std::endl;
	SDL_Quit();
	exit(EXIT_FAILURE);
}

Shader *vertexShader;
Shader *fragmentShader;
ShaderProgram *program;

glm::mat4 projectionMatrix;

static AnimatedMesh *mesh;

void initScene()
{
	vertexShader = new Shader(std::string("../data/shaders/shader.vert"), GL_VERTEX_SHADER); 
	fragmentShader = new Shader(std::string("../data/shaders/shader.frag"), GL_FRAGMENT_SHADER);
	program = new ShaderProgram();
	if(!vertexShader->compile()) {
		std::cout << "failed to compile shader" << std::endl;
	}
	if(!fragmentShader->compile()) {
		std::cout << "failed to compile shader" << std::endl;
	}

	if(!program->attachShader(vertexShader)) {
		std::cout << "failed to attach shader" << std::endl;	
	}
	if(!program->attachShader(fragmentShader)) {
		std::cout << "failed to attach shader" << std::endl;
	}
	program->link();
	program->use();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projectionMatrix = glm::perspective(45.f, 1.f, 0.1f, 1000.f);
}

float rotationAngle = 0.0f;
const float PIover180 = 3.1415f/180.0f;

void renderScene(Camera &camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int iModelViewLoc = glGetUniformLocation(program->getId(), "modelViewMatrix");
	int iProjectionLoc = glGetUniformLocation(program->getId(), "projectionMatrix");
	int iNormalLoc = glGetUniformLocation(program->getId(), "normalMatrix");
	int iLightPosLoc = glGetUniformLocation(program->getId(), "lightPosition");
	int iSamplerLoc = glGetUniformLocation(program->getId(), "texSampler");

	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    const glm::mat4 &viewMatrix = camera.getViewMatrix();
    //glm::mat4 modelViewMatrix;
 
	//glm::mat4 current = glm::rotate(viewMatrix, rotationAngle, glm::vec3(0.f, 1.f, 0.f));
	//current = glm::rotate(current, -90 * PIover180, glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix));

	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(iNormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(iLightPosLoc, 1, glm::value_ptr(glm::vec3(-50, 0, -50)));
	glUniform1i(iSamplerLoc, 0);

	mesh->Render();
	
}

bool g_running = true;
float g_deltaYaw = 0.f;
float g_deltaPitch = 0.f;
glm::vec3 g_position(10.f, 0.f, 0.f);

int _tmain(int argc, _TCHAR* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		exitWithSDLError();
	}

	//OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SELECTED_GL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, SELECTED_GL_MINOR_VERSION);

	//double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	//vsync
	SDL_GL_SetSwapInterval(1);
	SDL_Window *window = SDL_CreateWindow("Cloak",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			512, 512,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == NULL) {
		exitWithSDLError();
	}

	//gl context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL) {
		exitWithSDLError();
	}
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	//glew
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << glewGetErrorString(glewError) << std::endl;
		exit(EXIT_FAILURE);
	}

    Camera camera(glm::vec3(0));
    initScene();

	//initialize sample data
    TextureLoader textureLoader;
	mesh = new AnimatedMesh(&textureLoader);
	mesh->LoadModel("../data/meshes/boblampclean.md5mesh");
    mesh->LoadAnim("../data/animations/boblampclean.md5anim");

    EventManager eventManager;

	float red = 0.f, green = 0.f, blue = 0.f;
    
    Uint32 lastFrameTime = SDL_GetTicks();
	while(g_running) {
        Uint32 currentFrameTime = SDL_GetTicks();

        g_deltaYaw = 0.f;
        g_deltaPitch = 0.f;
		//handle any events
        eventManager.handleEvents();

        camera.rotate(g_deltaPitch, g_deltaYaw);
        camera.setPosition(g_position);

		//update
		rotationAngle += 0.001f;
        mesh->Update(currentFrameTime - lastFrameTime);

		//render
		renderScene(camera);

		SDL_GL_SwapWindow(window);
        lastFrameTime = currentFrameTime;
	}
	
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}