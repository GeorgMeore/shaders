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

vec2 rescale(vec2 coords, vec2 scale) {
	return ((coords+vec2(1.0))/2)*scale;
}

void main() {
	vec2 point = rescale(uv, vec2(1920, 1080));
	vec2 mpoint = rescale(mouse/screen, vec2(1920, 1080));

	vec2 topLeft = vec2(0, 1080);
	vec2 botLeft = vec2(0, 0);
	vec2 topRight = vec2(1920, 1080);
	vec2 botRight = vec2(1920, 0);

	float lineWidth = 5.0;

	fragColor = vec4(0.0);
	if (point.x < mpoint.x && dist(topLeft, mpoint, point) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (point.x > mpoint.x && dist(topRight, mpoint, point) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (point.x < mpoint.x && dist(botLeft, mpoint, point) <= lineWidth) {
		fragColor += vec4(1.0);
	}
	if (point.x > mpoint.x && dist(botRight, mpoint, point) <= lineWidth) {
		fragColor += vec4(1.0);
	}
}
