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
#include <vector>

#include "bitmap.h"

#include "audio.hpp"

#define WINDOW_WIDTH 2000//800
#define WINDOW_HEIGHT 1600//600

#define TEXTURE_COUNT 1
#define SPECTRUM_SIZE 256
#define MAX_AUDIO_SPECTRUM 10

GLint GprogramID = -1;

GLuint gTextureID[TEXTURE_COUNT];

GLFWwindow* window;

Audio<SPECTRUM_SIZE>* audio;

//fragment shader variables

float circleRotation = 0.0f;

float incrementingVariable = 0.0f;

float degreeToRadian(float degreeAngle) {

	return degreeAngle * (3.141592654 / 180.0);

}

float roundOffZerosInFloatDecimal(float toRound) {

	while (toRound < 0.09) {

		toRound *= 10.0;

	}

	return toRound;

}

float getAudioBeatAverage() {

	float spectrumLevel = 0.0;

	for (int i = 0; i < MAX_AUDIO_SPECTRUM; ++i) {

		spectrumLevel += (audio->getSpectrumLeft(i) * audio->getSpectrumRight(i)) / 2;

	}

	return spectrumLevel; // / float(MAX_AUDIO_SPECTRUM)

}

float getBaseBeatAverage() {

	float spectrumLevel = 0.0;

	for (int i = 0; i < MAX_AUDIO_SPECTRUM / 2; ++i) {

		spectrumLevel += (audio->getSpectrumLeft(i) * audio->getSpectrumRight(i)) / 2;

	}

	return spectrumLevel; // / float(MAX_AUDIO_SPECTRUM / 2)

}

float getHighBeatAverage() {

	float spectrumLevel = 0.0;

	for (int i = MAX_AUDIO_SPECTRUM / 2; i < MAX_AUDIO_SPECTRUM; ++i) {

		spectrumLevel += (audio->getSpectrumLeft(i) * audio->getSpectrumRight(i)) / 2;

	}

	return spectrumLevel; // / float(MAX_AUDIO_SPECTRUM / 2)

}

float getMidBeatAverage() {

	float spectrumLevel = 0.0;

	for (float i = float(MAX_AUDIO_SPECTRUM) * 0.4; i < float(MAX_AUDIO_SPECTRUM) * 0.6; ++i) {

		spectrumLevel += (audio->getSpectrumLeft(i) * audio->getSpectrumRight(i)) / 2;

	}

	return spectrumLevel; // / (float(MAX_AUDIO_SPECTRUM) * (0.6 - 0.4))

}

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

	//Init fmod
	audio = new Audio<SPECTRUM_SIZE>("../media/assignment1_audio.mp3");
	audio->playAudio(AudioLoop::forever);

	//Load texture
	glGenTextures(TEXTURE_COUNT, gTextureID);
	loadTexture("../media/assignment1_texture.bmp", gTextureID[0]);

	// Load the vertex/fragment shaders
    //vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
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

	//uniform float averageAudioBeat
	GLuint averageAudioBeat_Loc = glGetUniformLocation(GprogramID, "averageAudioBeat");

	if (averageAudioBeat_Loc != -1) { glUniform1f(averageAudioBeat_Loc, (getAudioBeatAverage())); } //roundOffZerosInFloatDecimal

	//uniform float averageBaseBeat
	GLuint averageBaseBeat_Loc = glGetUniformLocation(GprogramID, "averageBaseBeat"); //roundOffZerosInFloatDecimal

	if (averageBaseBeat_Loc != -1) { glUniform1f(averageBaseBeat_Loc, (getBaseBeatAverage())); } //roundOffZerosInFloatDecimal

	//uniform float averageHighBeat
	GLuint averageHighBeat_Loc = glGetUniformLocation(GprogramID, "averageHighBeat");

	if (averageHighBeat_Loc != -1) { glUniform1f(averageHighBeat_Loc, (getHighBeatAverage())); } //roundOffZerosInFloatDecimal

	//uniform float averageMidBeat
	GLuint averageMidBeat_Loc = glGetUniformLocation(GprogramID, "averageMidBeat");

	if (averageMidBeat_Loc != -1) { glUniform1f(averageMidBeat_Loc, (getMidBeatAverage())); } //roundOffZerosInFloatDecimal
	
	//uniform float circleRotation
	GLuint circleRotation_Loc = glGetUniformLocation(GprogramID, "circleRotation");

	if (circleRotation_Loc != -1) { glUniform1f(circleRotation_Loc, (circleRotation)); } //degreeToRadian

	++circleRotation;

	if (circleRotation > 360.0f ) { circleRotation = 0.0f; }

	//uniform float incrementingVariable
	GLuint incrementingVariable_Loc = glGetUniformLocation(GprogramID, "incrementingVariable");

	if (incrementingVariable_Loc != -1) { glUniform1f(incrementingVariable_Loc, incrementingVariable); }

	++incrementingVariable;

	//Rect
	GLfloat vVertices[] = {

		-1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,

		 1.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,

	};

	//Rect colour
	GLfloat vColors[] = {

		 1.0f,  0.0f,  0.0f, 1.0f,
		 0.0f,  1.0f,  0.0f, 1.0f,
		 0.0f,  0.0f,  1.0f, 1.0f,

		 0.0f,  0.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  0.0f,  0.0f, 1.0f,

	};

	//Rect texutre
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

		audio->update();

		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	delete audio;
	audio = nullptr;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
