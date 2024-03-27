#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdint.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>

typedef uint32_t u32;

u32 shader_load(const char *vertex_path, const char *fragment_path)
{
    u32 fd = open(vertex_path, O_RDONLY, S_IRUSR);
    if (fd == -1)
    {
        fprintf(stderr, "F: %s L: %d -> errno: %s\n",
                __FILE__, __LINE__, strerror(errno));
    }
    char *buffer;
    struct stat statbuf;

    fstat(fd, &statbuf);
    buffer = realloc(buffer, sizeof(*buffer) * statbuf.st_size);

    read(fd, buffer, statbuf.st_size);


    close(fd);
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    u32 shader_program = shader_load("shader_vertex", "shader_fragment");

    float vertices[] = {
        -0.5, -0.5,
        0.0f, 0.5,
        0.5, -0.5,
    };

    u32 vao;
    u32 vbo;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(*vertices), (void *)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5, 0.2, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
