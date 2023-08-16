#version 400

in vec2 uv;
out vec4 fragColor;

uniform vec2 mouse;
uniform vec2 screen;

bool isInsideCircle(vec3 circ, vec2 point) {
	return length(circ.xy - point) <= circ.z;
}

bool isInsideRect(vec4 rect, vec2 point) {
	return (
		point.x >= rect.x && point.x <= rect.x + rect.z &&
		point.y >= rect.y && point.y <= rect.y + rect.w
	);
}

vec2 closestPoint(vec3 circ, vec4 rect) {
	return vec2(
		max(rect.x, min(rect.x+rect.z, circ.x)),
		max(rect.y, min(rect.y+rect.w, circ.y))
	);
}

vec2 rescale(vec2 coords, vec2 scale) {
	return ((coords+vec2(1.0))/2)*scale*(screen/scale);
}

void main() {
	vec2 point = rescale(uv, vec2(1920, 1080));
	vec3 circ = vec3(rescale(mouse, vec2(1920, 1080)), 100.0);
	vec4 rect = vec4(500.0, 200.0, 400.0, 300.0);

	if (isInsideCircle(circ, closestPoint(circ, rect))) {
		fragColor = vec4(0.2, 0.2, 0.2, 0.0);
	} else {
		fragColor = vec4(0.0);
	}

	if (isInsideCircle(circ, point)) {
		fragColor += vec4(1.0, 0.0, 0.0, 0.0);
	}
	if (isInsideRect(rect, point)) {
		fragColor += vec4(0.0, 1.0, 0.0, 0.0);
	}
}
