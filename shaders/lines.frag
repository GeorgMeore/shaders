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

vec2 rescale(vec2 uvpoint) {
	return uvpoint * vec2(screen.x/screen.y, 1.0);
}

void main() {
	vec2 topLeft = rescale(vec2(-1.0, 1.0));
	vec2 botLeft = rescale(vec2(-1.0, -1.0));
	vec2 topRight = rescale(vec2(1.0, 1.0));
	vec2 botRight = rescale(vec2(1.0, -1.0));
	vec2 p = rescale(uv);
	vec2 m = rescale(mouse);

	float lineWidth = .005;

	fragColor = vec4(0.0);
	if (dist(topLeft, m, p) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (dist(topRight, m, p) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (dist(botLeft, m, p) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (dist(botRight, m, p) <= lineWidth) {
		fragColor += vec4(1.0);
	}
}
