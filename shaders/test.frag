#version 400

in vec2 uv;
out vec4 fragColor;

uniform float time;
uniform vec2 mouse;
uniform vec2 screen;

vec2 rescale(vec2 uvpoint) {
	return uvpoint * vec2(screen.x/screen.y, 1.0);
}

void main() {
	if (length(rescale(uv) - rescale(mouse)) < 0.1) {
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} else {
		fragColor = vec4(abs(sin(time)), 0.5f, 0.2f, 1.0f);
	}
}
