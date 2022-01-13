#define GLFW_INCLUDE_ES2 1
#define GLFW_DLL 1
//#define GLFW_EXPOSE_NATIVE_WIN32 1
//#define GLFW_EXPOSE_NATIVE_EGL 1

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream> 

#include "bitmap.h"

#define WINDOW_WIDTH 1600//1600
#define WINDOW_HEIGHT 1200//1200

#define TEXTURE_COUNT 2

GLint GprogramID = -1;

GLuint gTextureID[TEXTURE_COUNT];

GLFWwindow* window;

void loadTexture(const char* path, GLuint textureID) {

	CBitmap bitmap(path);

	//Create linnear filtered texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Apply texture wrapping horizontal
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Apply texture wrapping along vertical (path?)

	//old school (minecrfat filtering)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //near filtering (for when texture needs to scale up when zoom in)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //far filtering (for when texture needs to scale down when zoom out)

	//bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //near filtering (for when texture needs to scale up when zoom in)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //far filtering (for when texture needs to scale down when zoom out)


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());

}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

GLuint LoadShader(GLenum type, const char* shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			// char* infoLog = malloc (sizeof(char) * infoLen );
			char infoLog[512];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);

			// free ( infoLog );
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;

}

GLuint LoadShaderFromFile(GLenum shaderType, std::string path)
{
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());

	if (sourceFile)
	{
		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)),
			std::istreambuf_iterator< char >());
		const GLchar* shaderSource = shaderString.c_str();

		return LoadShader(shaderType, shaderSource);
	}
	else
		printf("Unable to open file %s\n", path.c_str());

	return shaderID;
}

int Init(void)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	//Load texture
	glGenTextures(TEXTURE_COUNT, gTextureID);
	loadTexture("../media/colorblock.bmp", gTextureID[0]);
	loadTexture("../media/rgb.bmp", gTextureID[1]);

	// Load the vertex/fragment shaders
   // vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
	//fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

	vertexShader = LoadShaderFromFile(GL_VERTEX_SHADER, "../vertexShader1.vert");
	fragmentShader = LoadShaderFromFile(GL_FRAGMENT_SHADER, "../fragmentShader_Assignment1.frag"); //fragmentShader1


	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Bind vPosition to attribute 0   
	glBindAttribLocation(programObject, 0, "vPosition");
	glBindAttribLocation(programObject, 1, "vColor");
	glBindAttribLocation(programObject, 2, "vTexCoord");

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			//char* infoLog = malloc (sizeof(char) * infoLen );
			char infoLog[512];
			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);

			//free ( infoLog );
		}

		glDeleteProgram(programObject);
		return 0;
	}

	// Store the program object
	GprogramID = programObject;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return 1;
}

void Draw(void) {

	//Set the sampler2D varying variable to the first texture unit (index 0)
	glUniform1i(glGetUniformLocation(GprogramID, "sampler2d"), 0);

	//modify Factor 1 varying variable
	static float factor1 = 0.0f;

	factor1 += 0.01f;

	GLint factor1Loc = glGetUniformLocation(GprogramID, "Factor1");

	if (factor1Loc != -1) { glUniform1f(factor1Loc, factor1); }

	//animationTest
	static float animationTest = 0.0f;

	animationTest += 0.02f;

	GLuint animationTest_Loc = glGetUniformLocation(GprogramID, "animationTest");

	if (animationTest_Loc != -1) { glUniform1f(animationTest_Loc, animationTest); }

	//circleRadius
	static float circleRadius = 100.0f;

	circleRadius += 0.01;

	GLuint circleRadius_Loc = glGetUniformLocation(GprogramID, "circleRadius");

	if (circleRadius_Loc != -1) { glUniform1f(circleRadius_Loc, abs(sin(circleRadius)) * 100); }

	//circle center position
	static float circleCenterX = WINDOW_WIDTH / 2 / 2 / 2;
	static float circleCenterY = WINDOW_HEIGHT / 2;

	static float centerXChange = 0.0f;
	static float centerYChange = 0.0f;

	centerXChange += 0.1;
	centerYChange += 0.2;

	circleCenterX += sin(centerXChange) * 30;
	circleCenterY += cos(centerYChange) * 20;

	GLuint circleCenterX_Loc = glGetUniformLocation(GprogramID, "circleCenterX");
	GLuint circleCenterY_Loc = glGetUniformLocation(GprogramID, "circleCenterY");

	if (circleCenterX_Loc != -1) { glUniform1f(circleCenterX_Loc, circleCenterX); }
	if (circleCenterY_Loc != -1) { glUniform1f(circleCenterY_Loc, circleCenterY); }

	//Window dimension
	static float width = (float)WINDOW_WIDTH;
	static float height = (float)WINDOW_HEIGHT;

	GLuint width_Loc = glGetUniformLocation(GprogramID, "width");
	GLuint height_Loc = glGetUniformLocation(GprogramID, "height");

	if (width_Loc != -1) { glUniform1f(width_Loc, width); }
	if (height_Loc != -1) { glUniform1f(height_Loc, height); }

	//Wave
	static float wave = 0.0f;
	static float waveSpeed = 00.0f;

	wave += 0.01f;
	waveSpeed += 1.0f; //0.05f;

	GLuint wave_Loc = glGetUniformLocation(GprogramID, "wave");
	GLuint waveSpeed_Loc = glGetUniformLocation(GprogramID, "waveSpeed");

	if (wave_Loc != -1) { glUniform1f(wave_Loc, sin(wave) * 100.0f); }
	if (waveSpeed_Loc != -1) { glUniform1f(waveSpeed_Loc, waveSpeed); }
	
	GLfloat vVertices[] = {

		-1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,

		 1.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,

	};
	/*
	static GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,
							   -0.5f, -0.5f, 0.0f,
							   0.5f, -0.5f,  0.0f,
							   0.5f,  -0.5f, 0.0f,
							   0.5f, 0.5f, 0.0f,
							   -0.5f, 0.5f,  0.0f };
	*/
	/*
	GLfloat vVertices[] = {

		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,

		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,

	};
	*/
	/*
	GLfloat vVertices[] = {
		0.0f,  1.0f, 0.0f,
	   -1.0f, -1.0f, 0.0f,
		1.0f, -1.0f,  0.0f
	};
	*/

	GLfloat vColors[] = {

		 1.0f,  0.0f,  0.0f, 1.0f,
		 0.0f,  1.0f,  0.0f, 1.0f,
		 0.0f,  0.0f,  1.0f, 1.0f,

		 0.0f,  0.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  0.0f,  0.0f, 1.0f,

	};

	GLfloat vTexCoords[] = {

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f

	};

	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);

	// Set the viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(GprogramID);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

int main(void)
{
	glfwSetErrorCallback(error_callback);

	// Initialize GLFW library
	if (!glfwInit())
		return -1;

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create and open a window
	window = glfwCreateWindow(WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"Hello World",
		NULL,
		NULL);

	if (!window)
	{
		glfwTerminate();
		printf("glfwCreateWindow Error\n");
		exit(1);
	}

	glfwMakeContextCurrent(window);

	Init();

	// Repeat
	while (!glfwWindowShouldClose(window)) {


		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
