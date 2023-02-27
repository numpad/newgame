$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_texcoords;

void main() {
  gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
  v_color0 = a_color0;
  v_texcoord0 = u_texcoords.xy + a_texcoord0 * u_texcoords.zw;
}

