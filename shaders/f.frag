#version 400

#define E  2.718281828
#define PI 3.1415926

precision mediump float;

uniform float time;
uniform vec2 mouse;
uniform vec2 screen;

in vec2 uv;
out vec4 fragColor;

float plot(vec2 st, float y) {
	return 0.002/length(st - vec2(st.x, y));
}

float f(float x) {
	return pow(x, 5.0);
}

void main() {
	vec2 st = (uv + vec2(1.0))*0.5;
	float y = f(st.x);
	vec3 color = vec3(y);
	float pct = plot(st, y);
	color = (1.0-pct)*color + pct*vec3(0.0, 1.0, 0.0);
	fragColor = vec4(color, 1.0);
}
