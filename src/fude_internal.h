#ifndef FUDE_INTERNAL_H
#define FUDE_INTERNAL_H

#include "fude.h"

void _fude_window_pos_callback(GLFWwindow* window, int x, int y);
void _fude_window_size_callback(GLFWwindow* window, int width, int height);
void _fude_window_close_callback(GLFWwindow* window);
void _fude_window_focus_callback(GLFWwindow* window, int focused);
void _fude_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void _fude_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void _fude_cursor_pos_callback(GLFWwindow* window, double x, double y);
void _fude_cursor_enter_callback(GLFWwindow* window, int entered);
void _fude_scroll_callback(GLFWwindow* window, double x, double y);
void _fude_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void _fude_char_callback(GLFWwindow* window, unsigned int codepoint);

fude_result _fude_init_window(fude* app, const fude_config* config);
fude_result _fude_init_renderer(fude* app, const fude_config* config);

#define FUDE_DEFAULT_VERTEX_SHADER \
    "#version 330 core\n" \
    "layout(location=0) in vec3 a_position;\n" \
    "layout(location=1) in vec4 a_color;\n" \
    "layout(location=2) in vec2 a_tex_coords;\n" \
    "layout(location=3) in float a_tex_index;\n" \
    "out vec4 v_color;\n" \
    "out vec2 v_tex_coords;\n" \
    "out float v_tex_index;\n" \
    "uniform mat4 u_mvp;\n" \
    "uniform mat4 u_projection;\n" \
    "uniform mat4 u_view;\n" \
    "uniform mat4 u_model;\n" \
    "void main()\n" \
    "{\n" \
    "    gl_Position = vec4(a_position, 1.0);\n" \
    "    v_color = a_color;\n" \
    "    v_tex_coords = a_tex_coords;\n" \
    "    v_tex_index = a_tex_index;\n" \
    "    u_projection;\n" \
    "    u_view;\n" \
    "    u_model;\n" \
    "    u_mvp;\n" \
    "}"

#define FUDE_DEFAULT_FRAGMENT_SHADER \
    "#version 330 core\n" \
    "layout(location=0) out vec4 o_color;\n" \
    "uniform sampler2D u_texture_samplers[8];\n" \
    "in vec4 v_color;\n" \
    "in vec2 v_tex_coords;\n" \
    "in float v_tex_index;\n" \
    "void main()\n" \
    "{\n" \
    "    switch(int(v_tex_index)) {\n" \
    "    case 1: o_color = texture(u_texture_samplers[1], v_tex_coords); break;\n" \
    "    case 2: o_color = texture(u_texture_samplers[2], v_tex_coords); break;\n" \
    "    case 3: o_color = texture(u_texture_samplers[3], v_tex_coords); break;\n" \
    "    case 4: o_color = texture(u_texture_samplers[4], v_tex_coords); break;\n" \
    "    case 5: o_color = texture(u_texture_samplers[5], v_tex_coords); break;\n" \
    "    case 6: o_color = texture(u_texture_samplers[6], v_tex_coords); break;\n" \
    "    case 7: o_color = texture(u_texture_samplers[7], v_tex_coords); break;\n" \
    "    default: o_color = v_color; break;\n" \
    "    }\n"\
    "}"

#endif // FUDE_INTERNAL_H
