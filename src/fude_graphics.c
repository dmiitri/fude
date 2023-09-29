#include "fude.h"

#include "glad/glad.h"
#include <stddef.h>

void CheckOpenGLError(void)
{
    GLenum err = GL_NO_ERROR;
    f_trace_log(FUDE_LOG_INFO, "CHECKING OPENGL ERROR");
    while((err = glGetError()) != GL_NO_ERROR) {
        if(err == GL_NO_ERROR) {
            break;
        } else if(err == GL_INVALID_ENUM) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_INVALID_ENUM", err);
        } else if(err == GL_INVALID_VALUE) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_INVALID_VALUE", err);
        } else if(err == GL_INVALID_OPERATION) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_INVALID_OPERATION", err);
        } else if(err == GL_STACK_OVERFLOW) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_STACK_OVERFLOW", err);
        } else if(err == GL_STACK_UNDERFLOW) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_STACK_UNDERFLOW", err);
        } else if(err == GL_OUT_OF_MEMORY) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_OUT_OF_MEMORY", err);
        } else if(err == GL_INVALID_FRAMEBUFFER_OPERATION) {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR %d: GL_INVALID_FRAMEBUFFER_OPERATION", err);
        } else {
            f_trace_log(FUDE_LOG_ERROR, "OPENGL ERROR: %d", err);
        }
    }
}


fude_result _fude_init_renderer(fude* app, const fude_config* config)
{
    (void)config;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &app->renderer.id);
    glBindVertexArray(app->renderer.id);

    glGenBuffers(1, &app->renderer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fude_vertex)*FUDE_RENDERER_MAXIMUM_VERTICES, 
            app->renderer.vertices.data, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            sizeof(fude_vertex), (GLvoid*)offsetof(fude_vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 
            sizeof(fude_vertex), (GLvoid*)offsetof(fude_vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
            sizeof(fude_vertex), (GLvoid*)offsetof(fude_vertex, tex_coords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 
            sizeof(fude_vertex), (GLvoid*)offsetof(fude_vertex, tex_index));

    glGenBuffers(1, &app->renderer.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->renderer.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*FUDE_RENDERER_MAXIMUM_INDICIES, 
            app->renderer.indices.data, GL_DYNAMIC_DRAW);

    // TODO: Setup the default shader and the default texture
    return FUDE_OK;
}

void f_begin(fude* f, fude_draw_mode mode, fude_shader shader)
{
    f->renderer.working.count = 0;
    f->renderer.working.mode = mode;
    f->renderer.shader = shader;
}

void f_dump_vertex(const fude_vertex* vertex)
{
    f_trace_log(FUDE_LOG_INFO, "x=%f, y=%f, z=%f", vertex->position.x, vertex->position.y, vertex->position.z);
    f_trace_log(FUDE_LOG_INFO, "r=%f, g=%f, b=%f a=%f", vertex->color.r, vertex->color.g, vertex->color.b, vertex->color.a);
    f_trace_log(FUDE_LOG_INFO, "u=%f, v=%f, tex_index=%f", vertex->tex_coords.u, vertex->tex_coords.v, vertex->tex_index);
    f_trace_log(FUDE_LOG_INFO, "object_id=%f", vertex->object_id);
}

void f_end(fude* app)
{
    if(app->renderer.working.mode == FUDE_MODE_QUADS && app->renderer.working.count >= 4) {
        uint32_t nquads = app->renderer.working.count / 4;
        for(uint32_t i = 0; i < 6*nquads; i+=6) {
            app->renderer.indices.data[i + 0] = app->renderer.vertices.count + 0;
            app->renderer.indices.data[i + 1] = app->renderer.vertices.count + 1;
            app->renderer.indices.data[i + 2] = app->renderer.vertices.count + 2;
            app->renderer.indices.data[i + 3] = app->renderer.vertices.count + 2;
            app->renderer.indices.data[i + 4] = app->renderer.vertices.count + 3;
            app->renderer.indices.data[i + 5] = app->renderer.vertices.count + 0;
            app->renderer.indices.count += 6;
            app->renderer.vertices.count += 4;
            app->renderer.working.count -= 4;
        }
    }

    if(app->renderer.working.count >= 3) {
        for(uint32_t i = 0 ; i < app->renderer.working.count; i += 3) {
            app->renderer.indices.data[i + 0] = app->renderer.vertices.count + 0;
            app->renderer.indices.data[i + 1] = app->renderer.vertices.count + 1;
            app->renderer.indices.data[i + 2] = app->renderer.vertices.count + 2;
            app->renderer.vertices.count += 3;
            app->renderer.indices.count += 3;
            app->renderer.working.count -= 3;
        }
    }
}

void f_color4f(fude* app, float r, float g, float b, float a)
{
    app->renderer.working.vertex.color.r = r;
    app->renderer.working.vertex.color.g = g;
    app->renderer.working.vertex.color.b = b;
    app->renderer.working.vertex.color.a = a;
}

void f_color(fude* app, fude_color color)
{
    f_color4f(app, (float)color.r/255, (float)color.g/255, (float)color.b/255, (float)color.a/255);
}

void f_vertex3f(fude* app, float x, float y, float z)
{
    app->renderer.working.vertex.position.x = x;
    app->renderer.working.vertex.position.y = y;
    app->renderer.working.vertex.position.z = z;

    app->renderer.vertices.data[app->renderer.working.count] = app->renderer.working.vertex;
    app->renderer.working.count += 1;
    app->renderer.working.vertex.tex_index = 0;
}

void f_vertex2f(fude* app, float x, float y)
{
    f_vertex3f(app, x, y, 0.0f); // TODO: Make the Z coordinate dynamic
}

void f_texture(fude* app, fude_texture texture, float u, float v, uint32_t index)
{
    if(index > FUDE_RENDERER_MAXIMUM_TEXTURES) return;
    app->renderer.working.vertex.tex_coords.u = u;
    app->renderer.working.vertex.tex_coords.v = v;
    app->renderer.working.vertex.tex_index = index;
    app->renderer.textures.data[index] = texture;
    app->renderer.textures.samplers[index] = index;
}

void f_flush(fude* app)
{
    // sync the data
    glBindBuffer(GL_ARRAY_BUFFER, app->renderer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, app->renderer.vertices.count*sizeof(fude_vertex), 
            app->renderer.vertices.data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->renderer.ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, app->renderer.indices.count*sizeof(uint32_t), 
            app->renderer.indices.data);

    for(size_t i = 0; i < FUDE_RENDERER_MAXIMUM_TEXTURES; ++i) {
        if(app->renderer.textures.data[i].id != 0) {
            glActiveTexture(GL_TEXTURE0 + app->renderer.textures.samplers[i]);
            glBindTexture(GL_TEXTURE_2D, app->renderer.textures.data[i].id);
        }
    }

    f_set_shader_uniform(app->renderer.shader, 
            app->renderer.shader.uniform_loc[FUDE_UNIFORM_TEXTURE_SAMPLERS_LOC],
            FUDE_SHADERDT_INT, FUDE_RENDERER_MAXIMUM_TEXTURES, 
            app->renderer.textures.samplers, false);

    // make draw call
    glUseProgram(app->renderer.shader.id);
    glBindVertexArray(app->renderer.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->renderer.ibo);
    glDrawElements(GL_TRIANGLES, app->renderer.indices.count, GL_UNSIGNED_INT, NULL);

    // clear the data
    f_memzero(app->renderer.vertices.data, sizeof(fude_vertex)*app->renderer.vertices.count);
    f_memzero(app->renderer.indices.data, sizeof(uint32_t)*app->renderer.indices.count);

    app->renderer.vertices.count = 0;
    app->renderer.indices.count = 0;
    app->renderer.shader = app->renderer.default_shader;

    for(uint32_t i = 0; i < FUDE_RENDERER_MAXIMUM_TEXTURES; ++i)
        app->renderer.textures.data[i] = app->renderer.default_texture;
}

fude_result f_create_shader(fude_shader* shader, const char* vert_src, const char* frag_src)
{
    if(!shader) return FUDE_INVALID_ARGUMENTS_ERROR;
    if(!vert_src) return FUDE_INVALID_ARGUMENTS_ERROR;
    if(!frag_src) return FUDE_INVALID_ARGUMENTS_ERROR;

    uint32_t vert_module, frag_module;
    GLchar info_log[512] = {0};
    int success;
    vert_module = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_module, 1, (const GLchar* const*)&vert_src, NULL);
    glCompileShader(vert_module);
    glGetShaderiv(vert_module, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert_module, sizeof(info_log), NULL, info_log);
        f_trace_log(FUDE_LOG_ERROR, "%s", info_log);
        return FUDE_SHADER_CREATION_ERROR;
    }

    frag_module = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_module, 1, (const GLchar* const*)&frag_src, NULL);
    glCompileShader(frag_module);
    glGetShaderiv(frag_module, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag_module, sizeof(info_log), NULL, info_log);
        f_trace_log(FUDE_LOG_ERROR, "FRAGMENT SHADER: %s\n", info_log);
        return FUDE_SHADER_CREATION_ERROR;
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vert_module);
    glAttachShader(shader->id, frag_module);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader->id, sizeof(info_log), NULL, info_log);
        f_trace_log(FUDE_LOG_ERROR, "SHADER PROGRAM: %s\n", info_log);
        return FUDE_SHADER_CREATION_ERROR;
    }

    glDeleteShader(vert_module);
    glDeleteShader(frag_module);

    glUseProgram(shader->id);

    fude_result result = FUDE_OK;

    result = f_get_shader_uniform_location(*shader, &shader->uniform_loc[FUDE_UNIFORM_TEXTURE_SAMPLERS_LOC],
                FUDE_TEXTURE_SAMPLER_UNIFORM_NAME);
    if(result != FUDE_OK) {
        return result;
    }

    result = f_get_shader_uniform_location(*shader, &shader->uniform_loc[FUDE_UNIFORM_MATRIX_MVP_LOC],
                FUDE_MATRIX_MVP_UNIFORM_NAME);
    if(result != FUDE_OK) {
        return result;
    }

#if FUDE_SHADER_RETRIEVE_ALL_LOCATIONS
    result = f_get_shader_uniform_location(*shader, &shader->uniform_loc[FUDE_UNIFORM_MATRIX_PROJECTION_LOC],
                FUDE_MATRIX_PROJECTION_UNIFORM_NAME);
    if(result != FUDE_OK) {
        return result;
    }

    result = f_get_shader_uniform_location(*shader, &shader->uniform_loc[FUDE_UNIFORM_MATRIX_VIEW_LOC],
                FUDE_MATRIX_VIEW_UNIFORM_NAME);
    if(result != FUDE_OK) {
        return result;
    }

    result = f_get_shader_uniform_location(*shader, &shader->uniform_loc[FUDE_UNIFORM_MATRIX_MODEL_LOC],
                FUDE_MATRIX_MODEL_UNIFORM_NAME);
    if(result != FUDE_OK) {
        return result;
    }
#endif

    glUseProgram(0);
    return FUDE_OK;
}

fude_result f_create_texture(fude_texture* texture, const void* data, int width, int height, int channels)
{
    if(!texture) return FUDE_INVALID_ARGUMENTS_ERROR;
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLenum internal_format = GL_RGBA8;
    GLenum data_format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, 
            height, 0, data_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return FUDE_OK;
}

void f_destroy_texture_(fude_texture texture)
{
    glDeleteTextures(1, &texture.id);
}

void f_update_texture(fude_texture texture, const void* data, int width, int height, int channels)
{
    GLenum data_format = channels == 4 ? GL_RGBA : GL_RGB;
    glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, data_format, GL_UNSIGNED_BYTE, data);
}

fude_result f_create_shader_from_file(fude_shader* shader, const char* vert_path, const char* frag_path)
{
    char* vert_src = f_load_file_data(vert_path, NULL);
    char* frag_src = f_load_file_data(frag_path, NULL);
    fude_result result = f_create_shader(shader, vert_src, frag_src);

    if(result != FUDE_OK) {
        return result;
    }

    f_unload_file_data(vert_src);
    f_unload_file_data(frag_src);
    return FUDE_OK;
}

void f_destroy_shader(fude_shader shader)
{
    glDeleteProgram(shader.id);
}

fude_result f_get_shader_uniform_location(fude_shader shader, int* location, const char* name)
{
    if(!location || !name) return FUDE_INVALID_ARGUMENTS_ERROR;

    glUseProgram(shader.id);
    int _location = glGetUniformLocation(shader.id, name);
    if(_location < 0) {
        f_trace_log(FUDE_LOG_ERROR, "Uniform with name %s not found in shader program", name);
        return FUDE_UNIFORM_LOCATION_NOT_FOUND_ERROR;
    }
    return FUDE_OK;
}

fude_result f_set_shader_uniform(fude_shader shader, int location, int type, int count, const void* data, bool transponse)
{
    if(!data) return FUDE_INVALID_ARGUMENTS_ERROR;

    glUseProgram(shader.id);
    switch(type) {
        case FUDE_SHADERDT_FLOAT: glUniform1fv(location, count, (float*)data); break;
        case FUDE_SHADERDT_VEC2: glUniform2fv(location, count, (float*)data); break;
        case FUDE_SHADERDT_VEC3: glUniform3fv(location, count, (float*)data); break;
        case FUDE_SHADERDT_VEC4: glUniform4fv(location, count, (float*)data); break;
        case FUDE_SHADERDT_INT: glUniform1iv(location, count, (int*)data); break;
        case FUDE_SHADERDT_IVEC2: glUniform2iv(location, count, (int*)data); break;
        case FUDE_SHADERDT_IVEC3: glUniform3iv(location, count, (int*)data); break;
        case FUDE_SHADERDT_IVEC4: glUniform4iv(location, count, (int*)data); break;
        case FUDE_SHADERDT_MAT4: glUniformMatrix4fv(location, count, transponse, (float*)data); break;
    }
    return FUDE_OK;
}
