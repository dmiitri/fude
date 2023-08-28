#include "fude_internal.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

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

Fude_Renderer* fude_create_renderer(Fude_Window* window)
{
    if(window == NULL) 
        return NULL;

    glfwMakeContextCurrent(window->glfw_window);
    // gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

    Fude_Renderer* ren = malloc(sizeof(Fude_Renderer));
    if(ren == NULL)
        fude_set_failure_code(FUDE_MEMORY_ALLOCATION_FAILURE);
    memset(ren, 0, sizeof(Fude_Renderer));
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
        fude_set_failure_code(FUDE_OPENGL_VERTEX_SHADER_COMPILATION_FAILURE);
        return NULL;
    }
    
    // Create and compile the fragment shader
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        fude_set_failure_code(FUDE_OPENGL_FRAGMENT_SHADER_COMPILATION_FAILURE);
        return NULL;
    }

    // Link the vertex and fragment shader into a shader program
    ren->shader_program = glCreateProgram();
    glAttachShader(ren->shader_program, vert_shader);
    glAttachShader(ren->shader_program, frag_shader);
    glLinkProgram(ren->shader_program);

    glGetProgramiv(ren->shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        fude_set_failure_code(FUDE_OPENGL_SHADER_PROGRAM_LINKING_FAILURE);
        return NULL;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glUseProgram(ren->shader_program);

    // _fude_check_opengl_errors();

    glGenBuffers(1, &ren->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
    glBufferData(GL_ARRAY_BUFFER, FUDE_MAXIMUM_VERTICES * sizeof(Fude_Vertex), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Fude_Vertex), (const void*)offsetof(Fude_Vertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Fude_Vertex), (const void*)offsetof(Fude_Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Fude_Vertex), (const void*)offsetof(Fude_Vertex, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Fude_Vertex), (const void*)offsetof(Fude_Vertex, tex_id));

	glGenBuffers(1, &ren->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, FUDE_MAXIMUM_INDICES * sizeof(uint32_t), NULL, GL_DYNAMIC_DRAW);

    // _fude_check_opengl_errors();

    return ren;
}

void fude_destroy_renderer(Fude_Renderer* ren)
{
    if(ren == NULL) return;
    free(ren);
}

void fude_present_renderer(Fude_Renderer* ren)
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
            fude_set_failure_code(FUDE_INVALID_TEXTURE_SLOT);
        }
    }
	glUniform1iv(glGetUniformLocation(ren->shader_program, "u_textures"), 
		FUDE_MAXIMUM_TEXTURES, texture_samplers);
    glUniformMatrix4fv(glGetUniformLocation(ren->shader_program, "u_projection"),
        1, GL_FALSE, ren->projection_matrix.elements);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Fude_Vertex) * ren->vertices_count, ren->vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * ren->elements_count, ren->elements);

	glDrawElements(GL_TRIANGLES, ren->elements_count, GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(ren->window->glfw_window);
    // _fude_check_opengl_errors();
}

void fude_clear_renderer(Fude_Renderer* ren)
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

void fude_set_draw_color(Fude_Renderer* ren, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if(ren == NULL) return;
    ren->draw_color[0] = (float)r / 255.0f;
    ren->draw_color[1] = (float)g / 255.0f;
    ren->draw_color[2] = (float)b / 255.0f;
    ren->draw_color[3] = (float)a / 255.0f;
}

void fude_draw_rectangle(Fude_Renderer* ren, int32_t x, int32_t y, int32_t w, int32_t h)
{
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        fude_set_failure_code(FUDE_RENDERER_INDICES_OVERFLOW);
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        fude_set_failure_code(FUDE_RENDERER_VERTICES_OVERFLOW);
        return;
    }
    ren->vertices[ren->vertices_count + 0] = (Fude_Vertex) {
        .pos = vec2(x, y),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 1] = (Fude_Vertex) {
        .pos = vec2(x + w, y),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 2] = (Fude_Vertex) {
        .pos = vec2(x + w, y + h),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 3] = (Fude_Vertex) {
        .pos = vec2(x, y + h),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };

    ren->vertices_count += 4;
    ren->elements_count += 6;
}

void fude_draw_triangle(Fude_Renderer* ren, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
{
    if(ren == NULL) return;
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        fude_set_failure_code(FUDE_RENDERER_INDICES_OVERFLOW);
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        fude_set_failure_code(FUDE_RENDERER_VERTICES_OVERFLOW);
        return;
    }
    ren->vertices[ren->vertices_count + 0] = (Fude_Vertex) {
        .pos = vec2(x1, y1),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 1] = (Fude_Vertex) {
        .pos = vec2(x2, y2),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };
    ren->vertices[ren->vertices_count + 2] = (Fude_Vertex) {
        .pos = vec2(x3, y3),
        .color = vec4(ren->draw_color[0], ren->draw_color[1], ren->draw_color[2], ren->draw_color[3]),
        .uv = vec2(0, 0),
        .tex_id = 0.0f,
    };

    ren->vertices_count += 3;
    ren->elements_count += 3;
}

struct _Fude_Texture {
    uint32_t id;
};

Fude_Texture* fude_create_texture(Fude_Renderer* ren, uint32_t w, uint32_t h, const uint8_t* data, int comp)
{
    if(ren == NULL) return NULL;
    if(data == NULL) return NULL;

    Fude_Texture* tex = malloc(sizeof(Fude_Texture));
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

void fude_destroy_texture(Fude_Texture* tex)
{
    if(tex == NULL) return;
    free(tex);
}

void fude_update_texture(Fude_Texture* tex, uint32_t w, uint32_t h, const uint8_t* data, int comp)
{
    if(tex == NULL) return;
    if(data == NULL) return;
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void fude_draw_texture(Fude_Renderer* ren, Fude_Texture* tex, 
int sx, int sy, uint32_t sw, uint32_t sh, 
int dx, int dy, uint32_t dw, uint32_t dh)
{
    if(ren == NULL) return;
    if(ren->elements_count + 5 >= FUDE_MAXIMUM_INDICES) {
        fude_set_failure_code(FUDE_RENDERER_INDICES_OVERFLOW);
        return;
    }
    ren->elements[ren->elements_count + 0] = ren->vertices_count + 0;
    ren->elements[ren->elements_count + 1] = ren->vertices_count + 1;
    ren->elements[ren->elements_count + 2] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 3] = ren->vertices_count + 2;
    ren->elements[ren->elements_count + 4] = ren->vertices_count + 3;
    ren->elements[ren->elements_count + 5] = ren->vertices_count + 0;

    if(ren->elements_count + 5 >= FUDE_MAXIMUM_VERTICES) {
        fude_set_failure_code(FUDE_RENDERER_VERTICES_OVERFLOW);
        return;
    }

    ren->vertices[ren->vertices_count + 0] = (Fude_Vertex) {
        .pos = vec2(dx, dy),
        .uv = vec2(sx, sy),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 1] = (Fude_Vertex) {
        .pos = vec2(dx + dw, dy),
        .uv = vec2(sx + sw, sy),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 2] = (Fude_Vertex) {
        .pos = vec2(dx + dw, dy + dh),
        .uv = vec2(sx + sw, sy + sh),
        .color = vec4(0.0f, 0.0f, 0.0f, 0.0f),
        .tex_id = ren->textures_count,
    };
    ren->vertices[ren->vertices_count + 3] = (Fude_Vertex) {
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
