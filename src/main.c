#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>

typedef uint32_t u32;
typedef int32_t s32;

u32 shader_load(const char *vertex_path, const char *fragment_path)
{
    s32 success;
    char info_log[512];
    u32 shader_program;
    u32 shader_vertex;
    u32 shader_fragment;
    s32 fd;
    fd = open(vertex_path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr,
                "F: %s L: %d -> errno: %s\n",
                __FILE__, __LINE__, strerror(errno));
    }
    char *buffer = NULL;
    struct stat statbuf;

    fstat(fd, &statbuf);
    printf("size: %ld\n", statbuf.st_size);
    buffer = realloc(buffer, sizeof(*buffer) * statbuf.st_size + 1);

    read(fd, buffer, statbuf.st_size);
    buffer[statbuf.st_size] = 0;

    shader_vertex = glCreateShader(GL_VERTEX_SHADER); 
    glCompileShader(shader_vertex);

    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_vertex, 512, NULL, info_log);
        fprintf(stderr,
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED -> %s\n",
                info_log);
    }

    close(fd);

    fd = open(fragment_path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr,
                "F: %s L: %d -> errno: %s\n",
                __FILE__, __LINE__, strerror(errno));
    }

    fstat(fd, &statbuf);
    buffer = realloc(buffer, sizeof(*buffer) * statbuf.st_size + 1);

    read(fd, buffer, statbuf.st_size);
    buffer[statbuf.st_size] = 0;

    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER); 
    glCompileShader(shader_fragment);

    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_fragment, 512, NULL, info_log);
        fprintf(stderr,
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED -> %s\n",
                info_log);
    }

    close(fd);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, shader_vertex);
    glAttachShader(shader_program, shader_fragment);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr,
                "ERROR::SHADER::PROGRAM::COMPILATION_FAILED -> %s\n",
                info_log);
    }


    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    return shader_program;
}

int main(void)
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
