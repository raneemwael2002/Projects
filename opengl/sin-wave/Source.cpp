#include <iostream>
#include <vector>
#include <cmath>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;
const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, VAO, shaderProgram;

void CreateSineWave()
{
    const int numPoints = 500;
    const float step = 2.0f / (numPoints - 1);  
    std::vector<float> vertices; 

    
    for (int i = 0; i < numPoints; i++)
    {
        float x = -1.0f + i * step; 
        vertices.push_back(x);      
        vertices.push_back(0.0f);    
        vertices.push_back(0.0f);    
        vertices.push_back(1.0f);    
        vertices.push_back(1.0f);    
        vertices.push_back(1.0f);    
    }

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CompileShaders()
{
    const char* vertexShaderSource = R"(
        #version 330
        layout (location = 0) in vec4 vertex_position;
        layout (location = 1) in vec4 vertex_color;

        uniform float amplitude;  
        uniform float frequency;  

        out vec4 frag_color;

        void main()
        {
            float y = amplitude * sin(frequency * vertex_position.x * 3.14159f);  
            gl_Position = vec4(vertex_position.x, y, 0.0, 1.0);
            frag_color = vertex_color;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330
        in vec4 frag_color;
        out vec4 color;

        void main()
        {
            color = frag_color;
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
    std::cout << "GLEW initialized successfully" << std::endl;
    CompileShaders();
    CreateSineWave();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return 0;

}


void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniform1f(glGetUniformLocation(shaderProgram, "amplitude"), 0.1f);  
    glUniform1f(glGetUniformLocation(shaderProgram, "frequency"), 15.0f); 

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, 500); 
    glBindVertexArray(0);
}


int main()
{
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sine Wave", sf::Style::Close, context);

    if (Init())
        return 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Render();
        window.display();
    }

    return 0;
}
