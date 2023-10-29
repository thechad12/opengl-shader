#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int compileShader(GLenum type, const std::string &source) {
    
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        std::cout << "Failed to compile shader ";
        std::cout << (type==GL_VERTEX_SHADER ? "vertex" : "fragment");

        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << message;
        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

int createShader(const std::string& vertex, const std::string& fragment) {
    
    unsigned int program = glCreateProgram();
    /* Vertex shader */
    unsigned int VS = compileShader(GL_VERTEX_SHADER, vertex);
    /* Fragment shader */
    unsigned int FS = compileShader(GL_VERTEX_SHADER, fragment);
    glAttachShader(program, VS);
    glAttachShader(program, FS);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(VS);
    glDeleteShader(FS);
    return program;

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing GLEW";
    }

    float points[6] { 0,1,-1,-1,1,-1 };

    unsigned int buffer;

    /* Set up vertex buffer and its attributes */
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /* Below attributes are the vertices (points) */
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    glEnableVertexAttribArray(0);

    /* Create the shaders */
    std::string VertexShader = 
        "#version 140\n"
        "attribute vec4 position;\n"
        "void main()\n"
        "{gl_Position = position;}";

    /* Pure red opaque triangle */
    std::string FragmentShader = 
        "#version 140\n"
        "void main()\n"
        "{gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);}";

    unsigned int shader = createShader(VertexShader, FragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}