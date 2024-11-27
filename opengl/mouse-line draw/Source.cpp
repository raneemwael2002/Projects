#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>

using namespace std;
using namespace glm;

enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};

struct Vertex
{
	vec3 Position;
	vec3 COlor;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::FilledTriangle;


void DrawTriangleFan()
{
	const int numSegments = 16;
	const float radius = 1.0f;
	const float PI = 3.14159265f;

	// Each vertex has x, y (2 floats) + r, g, b (3 floats)
	const int floatsPerVertex = 5;
	GLfloat vertices[(numSegments + 2) * floatsPerVertex];

	// Center vertex (for the fan)
	vertices[0] = 0.0f;  // x
	vertices[1] = 0.0f;  // y
	vertices[2] = 0.9098f; // r
	vertices[3] = 0.7372f; // g
	vertices[4] = 0.7254f; // b


	for (int i = 0; i < numSegments + 1; ++i)
	{
		float angle = 2.0f * PI * i / numSegments;

		// Position
		vertices[(i + 1) * floatsPerVertex] = radius * cos(angle);     // x
		vertices[(i + 1) * floatsPerVertex + 1] = radius * sin(angle); // y

		// Color (assign based on segments)
		if (i < numSegments / 3) {
			vertices[(i + 1) * floatsPerVertex + 2] = 0.9098f; // r
			vertices[(i + 1) * floatsPerVertex + 3] = 0.7372f; // g
			vertices[(i + 1) * floatsPerVertex + 4] = 0.7254f; // b
		}
		else if (i < 2 * numSegments / 3) {
			vertices[(i + 1) * floatsPerVertex + 2] = 0.2627f; // r
			vertices[(i + 1) * floatsPerVertex + 3] = 0.1803f; // g
			vertices[(i + 1) * floatsPerVertex + 4] = 0.3294f; // b
		}
		else {
			vertices[(i + 1) * floatsPerVertex + 2] = 0.6823f; // r
			vertices[(i + 1) * floatsPerVertex + 3] = 0.2666f; // g
			vertices[(i + 1) * floatsPerVertex + 4] = 0.3529f; // b
		}
	}


	// Create and bind buffer object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up vertex attribute pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(GLfloat), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);
}



void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	DrawTriangleFan();
	
	glClearColor(0.9647f, 0.9372f, 0.7411f, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (Current_DrawingMode)
	{
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 18);
}

float theta = 0;
void Update()
{
	// add all tick code
	theta += 0.0001f;

	GLuint Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), " circle made of triangle fan", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				break;
			}
			}
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}
