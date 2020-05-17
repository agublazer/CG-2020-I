// A: SIGUIENTE OPERACION EN LA SECUENCIA DE OPERACIONES
// S: REINICIAR PUNTOS A TRIANGULO ORIGINAL PERO SEGUIR EN LA SECUENCIA DE OPERACIOEN
// D: REINICIAR PUNTOS Y COMENZAR SECUENCIA DESDE CERO
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <math.h> 

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <common/shader.hpp>

#define PI 3.14159265
using namespace std;

class Mat{
public:
	GLfloat elements[16];
	GLfloat operator()(int x, int y){
		return elements[x*4 + y];
	}
	void operator=(GLfloat *e){
		for(int i = 0; i < 16; i++){
			elements[i] = e[i];
		}
	}
	void operator=(Mat m){
		for(int i = 0; i < 16; i++){
			elements[i] = m.elements[i];
		}
	}
	Mat(GLfloat *e){
		for(int i = 0; i < 16; i++){
			elements[i] = e[i];
		}
	}
	Mat(Mat &m){
		for(int i = 0; i < 16; i++){
			elements[i] = m.elements[i];
		}
	}
	Mat(){
		GLfloat identity[16] = { 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		for(int i = 0; i < 16; i++){
			elements[i] = identity[i];
		}
	}
	void print(){
		for(int i = 0; i<4; i++){
			for(int j=0; j<4; j++){
				printf("%.3f ", elements[i*4 + j]);
			}
			cout<<endl;
		}
		cout<<endl;
	}
	// 
	/*
	void inverse(Mat *result){
		result = new Mat();
		GLfloat A = 0.0f;
		A = elements[0]*elements[5]*elements[10];  // aei
		A += elements[1]*elements[6]*elements[8];  // bfg
		A += elements[2]*elements[4]*elements[9];  // cdh
		A -= elements[2]*elements[5]*elements[8];  // ceg
		A -= elements[1]*elements[4]*elements[10];  // bdi
		A -= elements[0]*elements[6]*elements[9];  // afh
		A = 1/A;

		result->elements[0] = A*((elements[5]*elements[10])-(elements[6]*elements[9]));
		result->elements[1] = A*((elements[2]*elements[9])-(elements[1]*elements[10]));
		result->elements[2] = A*((elements[1]*elements[6])-(elements[2]*elements[5]));

		result->elements[4] = A*((elements[6]*elements[8])-(elements[4]*elements[10]));
		result->elements[5] = A*((elements[0]*elements[10])-(elements[2]*elements[8]));
		result->elements[6] = A*((elements[2]*elements[4])-(elements[0]*elements[6]));

		result->elements[8] = A*((elements[4]*elements[9])-(elements[5]*elements[8]));
		result->elements[9] = A*((elements[1]*elements[8])-(elements[0]*elements[9]));
		result->elements[10] = A*((elements[0]*elements[5])-(elements[1]*elements[4]));

		result->elements[15] = A;

	}
	void transpose(Mat *result){
		//result = new Mat;
		result->print();
		cout<<"--------------"<<endl;
		result->elements[0] = elements[0];
		result->elements[1] = elements[4];
		result->elements[2] = elements[8];
		result->elements[3] = elements[12];

		result->elements[4] = elements[1];
		result->elements[5] = elements[5];
		result->elements[6] = elements[9];
		result->elements[7] = elements[13];

		result->elements[8] = elements[2];
		result->elements[9] = elements[6];
		result->elements[10] = elements[10];
		result->elements[11] = elements[14];

		result->elements[12] = elements[3];
		result->elements[13] = elements[7];
		result->elements[14] = elements[9];
		result->elements[15] = elements[15];
		cout<<"--------------ssssss"<<endl;
		result->print();

	}*/
};

GLuint vao[2], vbo[2];
GLfloat sx = 1.3f, sy = 1.3f, sz = 1.3f;
GLfloat tx=0.17f, ty = 0.29f, tz = 0.1f;

int co = 0;  // contador de la secuencia de multiplicaciones
double angle = 10.0;
double angle_x = 68.0, angle_y = 33.0, angle_z = 57.0;
double cosine_x =  cos( angle_x * PI / 180.0 );
double sine_x =  sin( angle_x * PI / 180.0 );
double cosine_y =  cos( angle_y * PI / 180.0 );
double sine_y =  sin( angle_y * PI / 180.0 );
double cosine_z =  cos( angle_z * PI / 180.0 );
double sine_z =  sin( angle_z * PI / 180.0 );

GLfloat points_base[] = {-0.75f,  -0.5f, 0.0f, 0.25f,  0.5f, 0.0f, 0.0f,  -0.5f, 0.0f };

GLfloat points1[] = {-0.75f,  -0.5f, 0.0f, 0.25f,  0.5f, 0.0f, 0.0f,  -0.5f, 0.0f };

GLfloat identity[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_identity(identity);

GLfloat position[] = { 
	1.0f, 0.0f, 0.0f, tx,
	0.0f, 1.0f, 0.0f, ty,
	0.0f, 0.0f, 1.0f, tz,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_position(position);

GLfloat scale[] = { 
	sx, 0.0f, 0.0f, 0.0f,
	0.0f, sy, 0.0f, 0.0f,
	0.0f, 0.0f, sz, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_scale(scale);

GLfloat rotate_z[] = { 
	cosine_z, -sine_z, 0.0f, 0.0f,
	sine_z, cosine_z, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_rz(rotate_z);

GLfloat rotate_y[] = { 
	cosine_y, 0.0f, sine_y, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	-sine_y, 0.0f, cosine_y, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_ry(rotate_y);

GLfloat rotate_x[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, cosine_x, -sine_x, 0.0f,
	0.0f, sine_x, cosine_x, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_rx(rotate_x);

GLfloat inverse_rotate_y[] = { 
	cosine_y, 0.0f, -sine_y, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	sine_y, 0.0f, cosine_y, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
Mat mat_inverse_ry(inverse_rotate_y);

Mat sequence[10] = {
	mat_inverse_ry, mat_position, mat_scale, mat_rz,  // COLUMNA L
	mat_scale, mat_inverse_ry, mat_rz, mat_position, mat_rx, mat_position  // COLUMNA R
};

GLfloat* multiply(GLfloat vec[], Mat mat){
	GLfloat *result = new GLfloat[4];
	for(int i = 0; i < 4; i++){
		result[i] = 0.0f;
		for(int j = 0; j < 4; j++){
			result[i] += mat(i, j)*vec[j];
		}
	}
	return result;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLfloat *result = new GLfloat[3];
	if (key == GLFW_KEY_A and action == GLFW_PRESS){ //siguiente multiplicacion en la secuencia
		for(int i = 0; i < 3; i++){
			GLfloat point[] = {points1[i*3], points1[i*3+1], points1[i*3+2], 1.0f};
			result = multiply(point, sequence[co]);
			// imprimir puntos y matriz en cada operacion
			cout<<"####### multiplicacion #######"<<endl;
			cout<<"matriz: "<<endl;
			sequence[co].print();
			cout<<"punto:"<<endl;
			for(int j = 0; j<4; j++){				
				printf("%.3f \n", point[j]);
			}
			cout<<"resultado: "<<endl;
			for(int j = 0; j<4; j++){
				printf("%.3f ", result[j]);
			}
			cout<<endl;
			// asignar resultados a array original
			points1[i*3] = result[0];
			points1[i*3 +1] = result[1];
			points1[i*3 +2] = result[2];
		}
		co+=1;
	}
	if ((key == GLFW_KEY_D and action == GLFW_PRESS) or (co==11)){ //reiniciar puntos y secuencia
		for(int i = 0; i < 16; i++){
			points1[i] = points_base[i];
		}
		co=-1;
	}
	if (key == GLFW_KEY_S and action == GLFW_PRESS){ //reiniciar solo puntos
		for(int i = 0; i < 16; i++){
			points1[i] = points_base[i];
		}
	}
}

int main( void )
{
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// SHADERS
	const GLchar *vertexShaderSource = R"(
		#version 330 core
		// Input vertex data, different for all executions of this shader.
		layout(location = 0) in vec3 vertexPosition_modelspace;
		void main(){

	    	gl_Position.xyz = vertexPosition_modelspace;
		    gl_Position.w = 1.0;

		})";

	const GLchar *fragmentShaderSource = R"(
		#version 330 core

		// Ouput data
		out vec3 color;
		void main(){

		// Output color = red 
		color = vec3(1,0,0);
		})";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// BUFFERS
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	do{

		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram(program);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle	
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSetKeyCallback(window, key_callback);

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	// glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}

