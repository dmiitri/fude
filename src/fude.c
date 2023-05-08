#include "fude.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <memory.h>
#include "llama.h"

#define FUDE_INVALID_TEXTURE_SLOT 0

const char* vert_shader_source = 
	"#version 440 core\n"   
	"layout(location = 0) in vec2 a_position;\n"
	"layout(location = 1) in vec4 a_color;\n"
	"layout(location = 2) in vec2 a_texCoords;\n"
	"layout(location = 3) in float a_texId;\n"
	"uniform mat4 u_projection;\n"

	"out vec4 v_color;\n"
	"out vec2 v_texCoords;\n"
	"out float v_texId;\n"

	"void main()\n"
	"{\n"
		"gl_Position = u_projection * vec4(a_position, 0.0, 1.0);\n"
		"v_color = a_color;\n"
		"v_texCoords = a_texCoords;\n"
		"v_texId = a_texId;\n"
	"}";


const char* frag_shader_source = 
	"#version 440 core\n"
	"layout(location = 0) out vec4 o_color;\n"
	"in vec4 v_color;\n"
	"in vec2 v_texCoords;\n"
	"in float v_texId;\n"
	"uniform sampler2D u_textures[8];\n"
	"void main()\n"
	"{\n"
        "\tif(v_texId > 0.0) {\n"
            "\t\to_color = texture(u_textures[int(v_texId)], v_texCoords);\n"
            // "\t\to_color = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "\t} else {\n"
            "\t\to_color = v_color;\n"
	    "\t}"
	"}\n";

enum {
    FUDE_NO_FAILURE = 0,
    FUDE_MEMORY_ALLOCATION_FAILURE,
    FUDE_WINDOWING_SUBSYSTEM_INITIALIZATION_FAILURE,
    FUDE_NATIVE_WINDOW_CREATION_FAILURE,
    FUDE_RENDERER_INDICES_OVERFLOW,
    FUDE_RENDERER_VERTICES_OVERFLOW,
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

static struct {
    uint32_t glfw_window_count;
    int last_failure_code;
    // TODO: add a better error system
    // char failure_msg[1024];
} FUDE = {0};

static void _fude_check_opengl_errors()
{
    int check = 1;
    while (check)
    {
        const GLenum err = glGetError();
        switch (err)
        {
            case GL_NO_ERROR: check = 0; break;
            case 0x0500: FUDE.last_failure_code = FUDE_OPENGL_INVALID_ENUM; break;
            case 0x0501: FUDE.last_failure_code = FUDE_OPENGL_INVALID_VALUE; break;
            case 0x0502: FUDE.last_failure_code = FUDE_OPENGL_INVALID_OPERATION; break;
            case 0x0503: FUDE.last_failure_code = FUDE_OPENGL_STACK_OVERFLOW; break;
            case 0x0504: FUDE.last_failure_code = FUDE_OPENGL_STACK_UNDERFLOW; break;
            case 0x0505: FUDE.last_failure_code = FUDE_OPENGL_OUT_OF_MEMORY; break;
            case 0x0506: FUDE.last_failure_code = FUDE_OPENGL_INVALID_FRAMEBUFFER_OPERATION; break;
            default: FUDE.last_failure_code = FUDE_OPENGL_UNKNOWN_INVALID_CODE; break;
        }
    }
}

bool fude_init(void)
{
    if(FUDE.glfw_window_count == 0) {
        if(!glfwInit()) {
            FUDE.last_failure_code = FUDE_WINDOWING_SUBSYSTEM_INITIALIZATION_FAILURE;            
            return false;
        }
    }
}

void fude_deinit(void)
{
    if(FUDE.glfw_window_count == 0) {
        glfwTerminate();
    }
}

struct _FudeWindow {
    GLFWwindow* glfw_window;
    float width, height;
};

#include <stdio.h>

FudeWindow* fude_create_window(const char* title, uint32_t width, uint32_t height, bool resizable)
{
    if(title == NULL)
        return NULL;

    FudeWindow* window = malloc(sizeof(FudeWindow));
    if(window == NULL) {
        FUDE.last_failure_code = FUDE_MEMORY_ALLOCATION_FAILURE;
        return NULL;
    }
    memset(window, 0, sizeof(FudeWindow));
    window->width = (float)width;
    window->height = (float)height;
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window->glfw_window == NULL) {
        FUDE.last_failure_code = FUDE_NATIVE_WINDOW_CREATION_FAILURE;
        return NULL;
    }
    FUDE.glfw_window_count += 1;
    return window;
}

void fude_destroy_window(FudeWindow* window)
{
    glfwDestroyWindow(window->glfw_window);
    free(window);
    FUDE.glfw_window_count += 1;
}

void fude_poll_input_events(void)
{
    glfwPollEvents();
}

bool fude_window_should_close(FudeWindow* window)
{
    return glfwWindowShouldClose(window->glfw_window);
}

typedef struct FudeVertex {
    vec2_t pos;
    vec4_t color;
    vec2_t uv;
    float tex_id;
} FudeVertex;

struct _FudeRenderer {
    uint32_t vao, vbo, ibo, shader_program;
    float draw_color[4];
    FudeWindow* window;
    FudeVertex vertices[FUDE_MAXIMUM_VERTICES];
    uint32_t elements[FUDE_MAXIMUM_INDICES];
    uint32_t textures[FUDE_MAXIMUM_TEXTURES];
    uint32_t vertices_count, elements_count, textures_count;
    mat4_t projection_matrix;
};

FudeRenderer* fude_create_renderer(FudeWindow* window)
{
    if(window == NULL) 
        return NULL;
    glfwMakeContextCurrent(window->glfw_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    FudeRenderer* ren = malloc(sizeof(FudeRenderer));
    if(ren == NULL)
        FUDE.last_failure_code = FUDE_MEMORY_ALLOCATION_FAILURE;
    memset(ren, 0, sizeof(FudeRenderer));
    ren->window = window;

    ren->projection_matrix = mat4_ortho(0.0f, window->width, window->height, 0.0f, -1.0f, 1.0f);
    
    glGenVertexArrays(1, &ren->vao);
    glBindVertexArray(ren->vao);

    uint32_t vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_source, NULL);
    glCompileShader(vert_shader);

    int success;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        FUDE.last_failure_code = FUDE_OPENGL_VERTEX_SHADER_COMPILATION_FAILURE;
        return NULL;
    }
    
    // Create and compile the fragment shader
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        FUDE.last_failure_code = FUDE_OPENGL_FRAGMENT_SHADER_COMPILATION_FAILURE;
        return NULL;
    }

    // Link the vertex and fragment shader into a shader program
    ren->shader_program = glCreateProgram();
    glAttachShader(ren->shader_program, vert_shader);
    glAttachShader(ren->shader_program, frag_shader);
    glLinkProgram(ren->shader_program);
    
    glGetProgramiv(ren->shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        FUDE.last_failure_code = FUDE_OPENGL_SHADER_PROGRAM_LINKING_FAILURE;
        return NULL;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glUseProgram(ren->shader_program);

    _fude_check_opengl_errors();

    glGenBuffers(1, &ren->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
    glBufferData(GL_ARRAY_BUFFER, FUDE_MAXIMUM_VERTICES * sizeof(FudeVertex), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FudeVertex), (const void*)offsetof(FudeVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FudeVertex), (const void*)offsetof(FudeVertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FudeVertex), (const void*)offsetof(FudeVertex, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(FudeVertex), (const void*)offsetof(FudeVertex, tex_id));

	glGenBuffers(1, &ren->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, FUDE_MAXIMUM_INDICES * sizeof(uint32_t), NULL, GL_DYNAMIC_DRAW);

    _fude_check_opengl_errors();

    return ren;
}

void fude_destroy_renderer(FudeRenderer* ren)
{
    if(ren == NULL) return;
    free(ren);
}

void fude_present_renderer(FudeRenderer* ren)
{
    if(ren == NULL) return;

    glBindVertexArray(ren->vao);
    glUseProgram(ren->shader_program);
    glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);

	int texture_samplers[FUDE_MAXIMUM_TEXTURES] = {0};
    for(uint32_t i = 1; i < FUDE_MAXIMUM_TEXTURES; ++i) {
        if(i < ren->textures_count) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, ren->textures[i]);
            texture_samplers[i] = i;
        } else {
            texture_samplers[i] = FUDE_INVALID_TEXTURE_SLOT;
        }
    }
	glUniform1iv(glGetUniformLocation(ren->shader_program, "u_textures"), 
		FUDE_MAXIMUM_TEXTURES, texture_samplers);
    glUniformMatrix4fv(glGetUniformLocation(ren->shader_program, "u_projection"),
        1, GL_FALSE, ren->projection_matrix.elements);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FudeVertex) * ren->vertices_count, ren->vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * ren->elements_count, ren->elements);

	glDrawElements(GL_TRIANGLES, ren->elements_count, GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(ren->window->glfw_window);
    _fude_check_opengl_errors();
}

void fude_clear_renderer(FudeRenderer* ren)
{
    if(ren == NULL) return;
    ren->elements_count = 0;
    ren->vertices_count = 0;
    ren->textures_count = 1;
    glClearColor(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void fude_set_draw_color(FudeRenderer* ren, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if(ren == NULL) return;
    ren->draw_color[0] = (float)r / 255.0f;
    ren->draw_color[1] = (float)g / 255.0f;
    ren->draw_color[2] = (float)b / 255.0f;
    ren->draw_color[3] = (float)a / 255.0f;
}

void fude_draw_rectangle(FudeRenderer* ren, int32_t x, int32_t y, int32_t w, int32_t h)
{
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        FUDE.last_failure_code = FUDE_RENDERER_INDICES_OVERFLOW;
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        FUDE.last_failure_code = FUDE_RENDERER_VERTICES_OVERFLOW;
        return;
    }
    ren->vertices[ren->vertices_count + 0] = (FudeVertex) {
        .pos = vec2(x, y),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 1] = (FudeVertex) {
        .pos = vec2(x + w, y),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 2] = (FudeVertex) {
        .pos = vec2(x + w, y + h),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 3] = (FudeVertex) {
        .pos = vec2(x, y + h),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };

    ren->vertices_count += 4;
    ren->elements_count += 6;
}

void fude_draw_triangle(FudeRenderer* ren, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
{
    if(ren == NULL) return;
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        FUDE.last_failure_code = FUDE_RENDERER_INDICES_OVERFLOW;
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        FUDE.last_failure_code = FUDE_RENDERER_VERTICES_OVERFLOW;
        return;
    }
    ren->vertices[ren->vertices_count + 0] = (FudeVertex) {
        .pos = vec2(x1, y1),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 1] = (FudeVertex) {
        .pos = vec2(x2, y2),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 2] = (FudeVertex) {
        .pos = vec2(x3, y3),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };

    ren->vertices_count += 3;
    ren->elements_count += 3;
}

struct _FudeTexture {
    uint32_t id;
};

FudeTexture* fude_create_texture(FudeRenderer* ren, uint32_t w, uint32_t h, const uint8_t* data, int comp)
{
    if(ren == NULL) return NULL;
    if(data == NULL) return NULL;

    FudeTexture* tex = malloc(sizeof(FudeTexture));
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

void fude_destroy_texture(FudeTexture* tex)
{
    if(tex == NULL) return;
    free(tex);
}

void fude_update_texture(FudeTexture* tex, uint32_t w, uint32_t h, const uint8_t* data, int comp)
{
    if(tex == NULL) return;
    if(data == NULL) return;
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void fude_draw_texture(FudeRenderer* ren, FudeTexture* tex, 
int sx, int sy, uint32_t sw, uint32_t sh, 
int dx, int dy, uint32_t dw, uint32_t dh)
{
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        FUDE.last_failure_code = FUDE_RENDERER_INDICES_OVERFLOW;
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        FUDE.last_failure_code = FUDE_RENDERER_VERTICES_OVERFLOW;
        return;
    }

    ren->vertices[ren->vertices_count + 0] = (FudeVertex) {
        .pos = vec2(dx, dy),
        .uv = vec2(sx, sy),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 1] = (FudeVertex) {
        .pos = vec2(dx + dw, dy),
        .uv = vec2(sx + sw, sy),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 2] = (FudeVertex) {
        .pos = vec2(dx + dw, dy + dh),
        .uv = vec2(sx + sw, sy + sh),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 3] = (FudeVertex) {
        .pos = vec2(dx, dy + dh),
        .uv = vec2(sx, sy + sh),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };

    ren->vertices_count += 4;
    ren->elements_count += 6;
    ren->textures[ren->textures_count] = tex->id;
    ren->textures_count += 1;
}

const char* fude_failure_reason(void)
{
    switch (FUDE.last_failure_code)
    {
    case FUDE_MEMORY_ALLOCATION_FAILURE:
        return "Memory allocation failure";
    case FUDE_WINDOWING_SUBSYSTEM_INITIALIZATION_FAILURE:
        return "Windowing subsystem initialization failure";
    case FUDE_NATIVE_WINDOW_CREATION_FAILURE:
        return "Native window creation failure";
    case FUDE_RENDERER_INDICES_OVERFLOW:
        return "Renderer indices overflow";
    case FUDE_RENDERER_VERTICES_OVERFLOW:
        return "Renderer vertices overflow";
    case FUDE_OPENGL_INVALID_ENUM:
        return "OpenGL Invalid enum";
    case FUDE_OPENGL_INVALID_VALUE:
        return "OpenGL Invalid value";
    case FUDE_OPENGL_INVALID_OPERATION:
        return "OpenGL Invalid operation";
    case FUDE_OPENGL_STACK_OVERFLOW:
        return "OpenGL Stack overflow";
    case FUDE_OPENGL_STACK_UNDERFLOW:
        return "OpenGL Stack underflow";
    case FUDE_OPENGL_OUT_OF_MEMORY:
        return "OpenGL Out of memory";
    case FUDE_OPENGL_INVALID_FRAMEBUFFER_OPERATION:
        return "OpenGL Invalid framebuffer operation";
    case FUDE_OPENGL_UNKNOWN_INVALID_CODE:
        return "OpenGL Unknown invalid code";
    case FUDE_OPENGL_VERTEX_SHADER_COMPILATION_FAILURE:
        return "OpenGL vertex shader compilation failure";
    case FUDE_OPENGL_FRAGMENT_SHADER_COMPILATION_FAILURE:
        return "OpenGL fragment shader compilation failure";
    case FUDE_OPENGL_SHADER_PROGRAM_LINKING_FAILURE:
        return "OpenGL shader program linking failure";

    default:
        return NULL;
    }
}