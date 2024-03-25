#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<math.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include<cglm/cglm.h>
#include<cglm/call.h>

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

Window *window_create(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n",
                info_log);
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
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n",
                info_log);
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
                "ERROR::SHADER::PROGRAM::COMPILATION_FAILED %s\n",
                info_log);
    }

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    return shader_program;
}

int main(void)
{
    Window *window = window_create();

    glEnable(GL_DEPTH_TEST);

    u32 shader_program = shader_program_create("shader/vertex.s", "shader/fragment.s");

    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;
    unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &nr_channels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    u32 vao;
    u32 vbo;
    u32 ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#if 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(shader_program);

    vec3 cub_pos[2] = {
        { 0.0, 0.5, -8.0 },
        { -0.6, -0.5, -5.0 },
    };


    while (!glfwWindowShouldClose(window))
    {
        /* rendering */
        glClearColor(0.6f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* transformations */
        mat4 model, view, projection;
        glm_mat4_identity(view);
        glm_mat4_identity(projection);
        glm_mat4_identity(model);

        u32 uniform_transform_location;

        glm_translate(view, (vec3){0.0f, 0.0f, -6.0f});
        glm_rotate(view, glm_rad(-20.0f), (vec3){0.0f, 1.0f, 0.0f});
        glm_perspective(glm_rad(45.0f), (float)800 / 800.0, 0.1f, 100.0f, projection);
        glm_rotate(model, glm_rad(30), (vec3){ 1.0, 0.0, 0.0 });
            


        glUseProgram(shader_program);

        uniform_transform_location = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(uniform_transform_location, 1, GL_FALSE, (float *)&model);


        uniform_transform_location = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(uniform_transform_location, 1, GL_FALSE, (float *)&view);

        uniform_transform_location = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(uniform_transform_location, 1, GL_FALSE, (float *)&projection);


        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glDrawArrays(GL_TRIANGLES, 0, 36);



        /* check events and swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
