#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include<cmath>
#include<vector>

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
DrawingMode Current_DrawingMode = DrawingMode::Lines;



void CreateSierpinskiTriangle(int depth) {
	
	std::vector<GLfloat> vertices;

		auto generateSierpinskiPoints = [&](auto& self, int depth,
		float x1, float y1, float x2, float y2, float x3, float y3,
		float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3) -> void {
			if (depth == 0) {
	
				vertices.insert(vertices.end(), { x1, y1, 0.0f, r1, g1, b1 });
				vertices.insert(vertices.end(), { x2, y2, 0.0f, r2, g2, b2 });
				vertices.insert(vertices.end(), { x3, y3, 0.0f, r3, g3, b3 });
				return;
			}
			float mx1 = (x1 + x2) / 2.0f;
			float my1 = (y1 + y2) / 2.0f;

			float mx2 = (x2 + x3) / 2.0f;
			float my2 = (y2 + y3) / 2.0f;

			float mx3 = (x3 + x1) / 2.0f;
			float my3 = (y3 + y1) / 2.0f;


			float mr1 = (r1 + r2) / 2.0f, mg1 = (g1 + g2) / 2.0f, mb1 = (b1 + b2) / 2.0f;
			float mr2 = (r2 + r3) / 2.0f, mg2 = (g2 + g3) / 2.0f, mb2 = (b2 + b3) / 2.0f;
			float mr3 = (r3 + r1) / 2.0f, mg3 = (g3 + g1) / 2.0f, mb3 = (b3 + b1) / 2.0f;


			self(self, depth - 1, x1, y1, mx1, my1, mx3, my3, r1, g1, b1, mr1, mg1, mb1, mr3, mg3, mb3);
			self(self, depth - 1, mx1, my1, x2, y2, mx2, my2, mr1, mg1, mb1, r2, g2, b2, mr2, mg2, mb2);
			self(self, depth - 1, mx3, my3, mx2, my2, x3, y3, mr3, mg3, mb3, mr2, mg2, mb2, r3, g3, b3);
		};


	generateSierpinskiPoints(generateSierpinskiPoints, depth,
		-0.9f, -0.9f, 0.9f, -0.9f, 0.0f, 0.9f,
		0.2392f, 0.0117f, 0.0011f, 0.69019f, 0.1882f, 0.3215f, 0.8431f, 0.4235f, 0.5098f);


	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0); 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); 
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

	CreateSierpinskiTriangle(5);

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

	
	glDrawArrays(GL_TRIANGLES, 0, 3 * std::pow(3, 5));
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
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sierpinski Triangles", sf::Style::Close, context);

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

