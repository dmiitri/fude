#version 330 core

layout(location=0) in vec3 a_position;
layout(location=1) in vec4 a_color;
layout(location=2) in vec2 a_tex_coords;
layout(location=3) in float a_tex_index;

out vec4 v_color;
out vec2 v_tex_coords;
out float v_tex_index;

uniform mat4 u_mvp;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    v_color = a_color;
    v_tex_coords = a_tex_coords;
    v_tex_index = a_tex_index;

    u_projection;
    u_view;
    u_model;
    u_mvp;
}
