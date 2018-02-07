#version 400
out vec4 frag_colour;
uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

void main() {
vec2 st = gl_FragCoord.xy/resolution;
vec2 stm = mouse/resolution;
vec3 color = vec3(st.xy, abs( mod(time,2.0)-1.0) );
color *= length(st-stm);
frag_colour = vec4(color,1.0);
}