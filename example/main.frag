# version 330

layout(location=0) out vec4 o_color;

uniform sampler2D u_texture_samplers[8];

in vec4 v_color;
in vec2 v_tex_coords;
in float v_tex_index;

void main()
{
    switch(int(v_tex_index)) {
    case 1: o_color = texture(u_texture_samplers[1], v_tex_coords); break;
    case 2: o_color = texture(u_texture_samplers[2], v_tex_coords); break;
    case 3: o_color = texture(u_texture_samplers[3], v_tex_coords); break;
    case 4: o_color = texture(u_texture_samplers[4], v_tex_coords); break;
    case 5: o_color = texture(u_texture_samplers[5], v_tex_coords); break;
    case 6: o_color = texture(u_texture_samplers[6], v_tex_coords); break;
    case 7: o_color = texture(u_texture_samplers[7], v_tex_coords); break;
    default: o_color = v_color; break;
    }
}
