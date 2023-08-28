#ifndef FUDE_INTERNAL_H
#define FUDE_INTERNAL_H

#include "fude.h"
#include "llama.h"

enum {
    FUDE_NO_FAILURE = 0,
    FUDE_MEMORY_ALLOCATION_FAILURE,
    FUDE_WINDOWING_SUBSYSTEM_INITIALIZATION_FAILURE,
    FUDE_NATIVE_WINDOW_CREATION_FAILURE,
    FUDE_RENDERER_INDICES_OVERFLOW,
    FUDE_RENDERER_VERTICES_OVERFLOW,
    FUDE_INVALID_TEXTURE_SLOT,
    FUDE_OPENGL_INVALID_ENUM,
    FUDE_OPENGL_INVALID_VALUE,
    FUDE_OPENGL_INVALID_OPERATION,
    FUDE_OPENGL_STACK_OVERFLOW,
    FUDE_OPENGL_STACK_UNDERFLOW,
    FUDE_OPENGL_OUT_OF_MEMORY,
    FUDE_OPENGL_INVALID_FRAMEBUFFER_OPERATION,
    FUDE_OPENGL_UNKNOWN_INVALID_CODE,
    FUDE_OPENGL_VERTEX_SHADER_COMPILATION_FAILURE,
    FUDE_OPENGL_FRAGMENT_SHADER_COMPILATION_FAILURE,
    FUDE_OPENGL_SHADER_PROGRAM_LINKING_FAILURE,
};

void fude_set_failure_code(int code);

typedef struct GLFWwindow GLFWwindow;

struct _Fude_Window {
    GLFWwindow* glfw_window;
    float width, height;
};

typedef struct Fude_Vertex {
    vec2_t pos;
    vec4_t color;
    vec2_t uv;
    float tex_id;
} Fude_Vertex;

struct _Fude_Renderer {
    uint32_t vao, vbo, ibo, shader_program;
    float draw_color[4];
    Fude_Window* window;
    Fude_Vertex vertices[FUDE_MAXIMUM_VERTICES];
    uint32_t elements[FUDE_MAXIMUM_INDICES];
    uint32_t textures[FUDE_MAXIMUM_TEXTURES];
    uint32_t vertices_count, elements_count, textures_count;
    mat4_t projection_matrix;
};

#endif // FUDE_INTERNAL_H
