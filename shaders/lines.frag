#version 400

in vec2 uv;
out vec4 fragColor;

uniform vec2 mouse;
uniform vec2 screen;

float dist(vec2 start, vec2 end, vec2 point) {
	vec2 coll = end - start;
	vec2 norm = normalize(vec2(-coll.y, coll.x));
	return abs(dot(norm, point - start));
}

void main() {
	vec2 topLeft = vec2(-1.0, 1.0);
	vec2 botLeft = vec2(-1.0, -1.0);
	vec2 topRight = vec2(1.0, 1.0);
	vec2 botRight = vec2(1.0, -1.0);

	float lineWidth = .005;

	fragColor = vec4(0.0);
	if (uv.x < mouse.x && uv.y > mouse.y && dist(topLeft, mouse, uv) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (uv.x > mouse.x && uv.y > mouse.y && dist(topRight, mouse, uv) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (uv.x < mouse.x && uv.y < mouse.y && dist(botLeft, mouse, uv) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (uv.x > mouse.x && uv.y < mouse.y && dist(botRight, mouse, uv) <= lineWidth) {
		fragColor += vec4(1.0);
	}
}
