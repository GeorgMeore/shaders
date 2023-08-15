#version 400

in vec2 uv;
out vec4 frag_color;

uniform float time;
uniform vec2 mouse;
uniform vec2 screen;

void main() {
	if (length(uv - (mouse/screen)) < 0.1) {
		frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	} else {
		frag_color = vec4(abs(sin(time)), 0.5f, 0.2f, 1.0f);
	}
}
