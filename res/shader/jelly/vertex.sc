$input a_position, a_color0
$output v_color0, v_position

#include <bgfx_shader.sh>

void main() {
  float aspect = u_viewRect.z / u_viewRect.w;

  gl_Position = mul(u_modelViewProj, vec4(a_position * vec3(u_viewRect.z, u_viewRect.w, 1.0), 1.0));
  v_color0 = a_color0;
  v_position = a_position * vec3(aspect, 1.0, 1.0);
}

