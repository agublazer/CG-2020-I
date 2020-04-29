#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>
using namespace std;

// colores
float color_black[3] = {0.0f, 0.0f, 0.0f};
float color_white[3] = {1.0f, 1.0f, 1.0f};
float color_red[3] = {1.0f, 0.0f, 0.0f};

// global variables
int v3 = 0;  // vector size / 3
int level = 4;
GLuint vao[2], vbo[2];
// triangulo inicial
GLfloat g_vertex_buffer_data1[] = { 
	-0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f,
	0.8f,  -0.8f, 0.0f,
};
    
// 0,1,2 point0
// 3,4,5 point1
// 6,7,8 point2

void get_points(GLfloat *points, vector<GLfloat> &result, int n){
	for(int i = 0; i<9; i++){
		result.push_back(points[i]);
	}
	if (n>0){
		GLfloat new_points1[9] = {
			points[0], points[1], 0.0f,
			//getMid(points[0], points[1]),
			(points[0]+points[3])/2, (points[1]+points[4])/2, 0.0f,
            //getMid(points[0], points[2])],
			(points[0] + points[6])/2, (points[1]+points[7])/2, 0.0f
		};
		get_points(new_points1, result, n-1);
		GLfloat new_points2[9] = {
			points[3], points[4], 0.0f,
			//getMid(points[0], points[1]),
			(points[0]+points[3])/2, (points[1]+points[4])/2, 0.0f,
            //getMid(points[1], points[2])],
			(points[3] + points[6])/2, (points[4]+points[7])/2, 0.0f
		};
		get_points(new_points2, result, n-1);
		GLfloat new_points3[9] = {
			points[6], points[7], 0.0f,
			//getMid(points[2], points[1]),
			(points[6]+points[3])/2, (points[7]+points[4])/2, 0.0f,
            //getMid(points[0], points[2])],
			(points[0] + points[6])/2, (points[1]+points[7])/2, 0.0f
		};
		get_points(new_points3, result, n-1);
	}
}

void gen_triangle(int level){
	vector<GLfloat> points_vector;

	get_points(g_vertex_buffer_data1, points_vector, level);
	const int vector_size = points_vector.size();

	// copiar todos los puntos del triangulo de serpinski
	GLfloat vector_array[points_vector.size()];  
    std::copy(points_vector.begin(), points_vector.end(), vector_array);
    v3 = ((vector_size/3)/3);  // seteamos esta variable para no tener que calcular el size/3 a cada momento

    // crear vao para almacenar en buffer
	
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vector_array), vector_array, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

unsigned int microseconds = 150000;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// menos nivel
	if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
		level -= 1;
	}

	// mas nivel
	if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
		level += 1;
	}
	gen_triangle(level);
	
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 900, 700, "Sierpinski triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	gen_triangle(level);

	do{

		// Color background
		glClear(GL_COLOR_BUFFER_BIT);
		key_callback(window, 0, 0, 0, 0);
		
		// Use our shader
		glUseProgram(programID);
		GLint color_location = glGetUniformLocation(programID, "my_color");
        // definimos variales para el color aleatorio
        float random_r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float random_g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float random_b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float color[3] = {random_r, random_g, random_b};

        glUniform3fv(color_location, 1, color);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// dibujar contorno
		glLineWidth(5.0f);
		for(int i = 3; i < v3*3; i+=3){
			glUniform3fv(color_location, 1, color_white);
			glDrawArrays(GL_LINE_LOOP, i, 3); // 3 indices starting at 0 -> 1 triangle	
			glfwSwapBuffers(window);
		}

		//dibujar triangulos
		for(int j = 1; j < 4; j++){
			for(int i = 3; i < v3*(4-j); i+=3){  // para evitar dibujar el triangulo original
				random_r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	        	random_g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	        	random_b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float color[3] = {random_r, random_g, random_b};
	        	glUniform3fv(color_location, 1, color);
				glDrawArrays(GL_TRIANGLES, i, 3); // 3 indices starting at 0 -> 1 triangle	
				// glfwSwapBuffers(window);
				// usleep(microseconds*3);
			}
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT);
			usleep(microseconds*5);
		}
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	// glDeleteBuffers(1, &vertexbuffer);
	// glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

