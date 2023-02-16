$input v_color0, v_position

#include <bgfx_shader.sh>

SAMPLER2D(u_texture, 0);
uniform vec4 u_offset;

void main() {
  // calculate texcoord from screen position
  vec2 texcoord = (v_position.xy * 0.5 + 0.5) - u_offset.xy * 0.5;
  texcoord.x = abs(fract(texcoord.x));

  vec4 color = texture2D(u_texture, texcoord);
  gl_FragColor = color;
}

