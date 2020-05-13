// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <math.h> 

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

#define PI 3.14159265
using namespace std;

GLuint vao[2], vbo[2];
GLfloat scale_x_neg = 0.7f, scale_y_neg = 0.7f, scale_z_neg = 0.7f;
GLfloat scale_x_pos = 1.3f, scale_y_pos = 1.3f, scale_z_pos = 1.3f;
GLfloat t=0.1f;
double angle = 10.0;
double cosine =  cos( angle * PI / 180.0 );
double sine =  sin( angle * PI / 180.0 );

GLfloat points1[] = { 
	0.5f,  0.0f, 0.0f,
	0.0f,  0.5f, 0.0f,
	-0.5f,  0.0f, 0.0f,
	0.3f,  0.0f, 0.0f,
	0.3f,  -0.5f, 0.0f,
	-0.2f,  0.0f, 0.0f,
	-0.2f,  0.0f, 0.0f,
	-0.2f,  -0.5f, 0.0f,
	0.3f,  -0.5f, 0.0f,
};

GLfloat identity[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_x_positive[] = { 
	1.0f, 0.0f, 0.0f, t,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_x_negative[] = { 
	1.0f, 0.0f, 0.0f, t*(-1),
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_y_positive[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, t,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_y_negative[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, t*(-1),
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_z_positive[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, t,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat position_z_negative[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, t*(-1),
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat scale_positive[] = { 
	scale_x_pos, 0.0f, 0.0f, 0.0f,
	0.0f, scale_y_pos, 0.0f, 0.0f,
	0.0f, 0.0f, scale_z_pos, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat scale_negative[] = { 
	scale_x_neg, 0.0f, 0.0f, 0.0f,
	0.0f, scale_y_neg, 0.0f, 0.0f,
	0.0f, 0.0f, scale_z_neg, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat mirror_yz[] = { 
	-1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat rotate_z[] = { 
	cosine, -sine, 0.0f, 0.0f,
	sine, cosine, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat rotate_y[] = { 
	cosine, 0.0f, sine, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	-sine, 0.0f, cosine, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

GLfloat rotate_x[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, cosine, -sine, 0.0f,
	0.0f, sine, cosine, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};


void multiply(GLfloat vec[], GLfloat matrix[], GLfloat *result){
	for(int i = 0; i < 4; i++){
		result[i] = 0.0f;
		for(int j = 0; j < 4; j++){
			result[i] += matrix[i*4 + j]*vec[j];
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLfloat result[4];
	if (key == GLFW_KEY_A and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, position_x_negative, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_D and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, position_x_positive, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_S and action == GLFW_PRESS){	
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, position_y_negative, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_W and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, position_y_positive, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_R and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, scale_negative, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_T and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, scale_positive, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_M and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, mirror_yz, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_J and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, rotate_z, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_K and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, rotate_y, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	if (key == GLFW_KEY_L and action == GLFW_PRESS){
		for(int i = 0; i < 9; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			multiply(point, rotate_x, result);
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
	}
	
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	// operaciones
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram(programID);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle		
		glDrawArrays(GL_TRIANGLES, 3, 3); // 3 indices starting at 0 -> 1 triangle		
		glDrawArrays(GL_TRIANGLES, 6, 3); // 3 indices starting at 0 -> 1 triangle		
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSetKeyCallback(window, key_callback);
		// Use our shader

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	// glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

