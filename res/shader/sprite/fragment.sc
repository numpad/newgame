$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_texture, 0);
uniform vec4 u_damageflash;

void main() {
  vec4 color = texture2D(u_texture, v_texcoord0);
  color.rgb = mix(color.rgb, vec3_splat(1.0), u_damageflash.x);
  gl_FragColor = color;
}

