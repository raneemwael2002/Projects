#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/glm/glm.hpp>
#include <cmath>
#include <vector>

using namespace std;
using namespace glm;

enum DrawingMode
{
    Points,
    Lines,
    FilledTriangle,
    DynamicLines 
};

struct Vertex
{
    vec3 Position;
    vec3 Color;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, DynamicLineVBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::DynamicLines;

std::vector<GLfloat> lineVertices; 
bool isDrawing = false;



void CreateDynamicLines()
{
    glGenBuffers(1, &DynamicLineVBO);

 
    glBindBuffer(GL_ARRAY_BUFFER, DynamicLineVBO);
    glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW); 

 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void UpdateDynamicLineBuffer()
{
    if (!lineVertices.empty())
    {
        
        glBindBuffer(GL_ARRAY_BUFFER, DynamicLineVBO);
      //  glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(vec3), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(GLfloat), lineVertices.data());
    }
    
}



void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& programId)
{
    programId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
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
    CreateDynamicLines(); 
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
        glDrawArrays(GL_POINTS, 0, 3);
        break;
    case Lines:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    case FilledTriangle:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    case DynamicLines:
        glLineWidth(5.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindBuffer(GL_ARRAY_BUFFER, DynamicLineVBO);
        //glDrawArrays(GL_LINES, 0, lineVertices.size() /3);
      glDrawArrays(GL_LINE_STRIP, 0, lineVertices.size()/3);
        break;
    default:
        break;
    }
}

float theta = 0;
void Update()
{
    // Add all tick code
    theta += 0.0001f;

    GLuint Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
    glUniform1f(Theta_Location, theta);

    if (Current_DrawingMode == DynamicLines)
        UpdateDynamicLineBuffer(); 
}

int main()
{
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

    if (Init()) return 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
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
                if (event.key.code == sf::Keyboard::Num4)
                {
                    Current_DrawingMode = DrawingMode::DynamicLines;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (Current_DrawingMode == DynamicLines && event.mouseButton.button == sf::Mouse::Left)
                {
                    isDrawing = true;
                    float x = (event.mouseButton.x / (float)WIDTH) * 2 - 1;
                    float y = -((event.mouseButton.y / (float)HEIGHT) * 2 - 1);
                    lineVertices.push_back(x);
                    lineVertices.push_back(y);
                    lineVertices.push_back(0); 
                }
                break;

            case sf::Event::MouseMoved:
                if (isDrawing && Current_DrawingMode == DynamicLines)
                {
                    float x = (event.mouseMove.x / (float)WIDTH) * 2 - 1;
                    float y = -((event.mouseMove.y / (float)HEIGHT) * 2 - 1);
                    lineVertices.push_back(x);
                    lineVertices.push_back(y);
                    lineVertices.push_back(0); 
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (Current_DrawingMode == DynamicLines && event.mouseButton.button == sf::Mouse::Left)
                {
                    isDrawing = false;
                }
                break;
            }
        }

        Update();
        Render();

        window.display();
    }
    return 0;
}
