#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

/* integer refedefinition */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef GLFWwindow Window;

void framebuffer_resize_func(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void input_process(GLFWwindow *window)
{
}


Window *window_create(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr,
                "ERROR: falied to create window | FILE: %s - LINE: %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_func);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,
                "ERROR: falied to load GLAD | FILE: %s - LINE: %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return window;
}

char *shader_source_create(const char *path)
{
    u32 fd = open(path, O_RDONLY);

    struct stat statbuf;
    fstat(fd, &statbuf);

    char *shader_source = malloc((statbuf.st_size + 1) * sizeof(*shader_source));
    read(fd, shader_source, statbuf.st_size);
    shader_source[statbuf.st_size] = 0;

    return shader_source;
}

u32 shader_program_create(const char *shader_vertex_path, const char *shader_fragment_path)
{
    u32 shader_vertex;
    u32 shader_fragment;

    i32 success;
    char info_log[512];

    char *shader_source;

    /* Vertex Shader */
    shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    shader_source = shader_source_create(shader_vertex_path);
    glShaderSource(shader_vertex, 1, (const char **)&shader_source, NULL);
    glCompileShader(shader_vertex);
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_vertex, 512, NULL, info_log);
        fprintf(stdout,
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }
    free(shader_source);

    /* Fragment Shader */
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    shader_source = shader_source_create(shader_fragment_path);
    glShaderSource(shader_fragment, 1, (const char **)&shader_source, NULL);
    glCompileShader(shader_fragment);
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_fragment, 512, NULL, info_log);
        fprintf(stdout,
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    free(shader_source);

    /* Shader Program */
    u32 shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, shader_vertex);
    glAttachShader(shader_program, shader_fragment);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_fragment, 512, NULL, info_log);
        fprintf(stdout,
                "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n");
    }

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    return shader_program;
}

int main(void)
{
    Window *window = window_create();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    }; 

    u32 shader_program = shader_program_create("shader/vertex.s", "shader/fragment.s");

    u32 vao;
    u32 vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        input_process(window);

        /* rendering */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* check events and swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
