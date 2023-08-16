#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

char *readFile(const char *fname)
{
	FILE *src = fopen(fname, "r");
	if (src == NULL) {
		fprintf(stderr, "error: failed to open a shader source file: %s\n", fname);
		return 0;
	}
	if (fseek(src, 0, SEEK_END) < 0) {
		fprintf(stderr, "error: failed to determine the size of a file: %s\n", fname);
		fclose(src);
		return 0;
	}
	long size = ftell(src);
	if (fseek(src, 0, SEEK_SET) < 0) {
		fprintf(stderr, "error: failed reset a file: %s\n", fname);
		fclose(src);
		return 0;
	}
	char *text = malloc(size + 1);
	fread(text, 1, size, src);
	if (ferror(src)) {
		fprintf(stderr, "error: failed to read the contents of a file: %s\n", fname);
		free(text);
		fclose(src);
		return 0;
	}
	text[size] = 0;
	fclose(src);
	return text;
}

GLuint compileShader(const char *text, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&text, NULL);
	glCompileShader(shader);
	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint compileShaderFile(const char *fname, GLenum type)
{
	char *text = readFile(fname);
	if (!text) {
		return 0;
	}
	GLuint shader = compileShader(text, type);
	free(text);
	return shader;
}

GLuint makeShaderProgram(const char *fShaderPath)
{
	GLuint vs = compileShader(
		"#version 400\n"
		"layout (location = 0) in vec3 pos; out vec2 uv;"
		"void main() { gl_Position = vec4(pos, 1.0); uv = pos.xy; }",
		GL_VERTEX_SHADER
	);
	if (!vs) {
		fprintf(stderr, "error: failed to compile the vertex shader\n");
		return 0;
	}
	GLuint fs = compileShaderFile(fShaderPath, GL_FRAGMENT_SHADER);
	if (!fs) {
		fprintf(stderr, "error: failed to compile the fragment shader\n");
		glDeleteShader(vs);
		return 0;
	}
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	int linked;
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (!linked) {
		fprintf(stderr, "error: failed to link the shader program\n");
		glDeleteProgram(prog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return 0;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	return prog;
}

GLuint genFullScreenQuadVAO()
{
	float quadVertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f,  -1.0f, 0.0f,
		1.0f,  1.0f,  0.0f,
		-1.0f, 1.0f, 0.0f,
	};
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	return vao;
}

void onError(int error, const char* description)
{
	fprintf(stderr, "error: glfw: %s\n", description);
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void onResize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s FILE.frag\n", argv[0]);
		return 1;
	}
	glfwSetErrorCallback(onError);
	if (!glfwInit()) {
		fprintf(stderr, "error: failed to initialize GLFW\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, argv[0], NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "error: failed to create a window\n");
		glfwTerminate();
		return 1;
	}
	glfwSetKeyCallback(window, onKey);
	glfwSetFramebufferSizeCallback(window, onResize);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();
	GLuint prog = makeShaderProgram(argv[1]);
	if (!prog) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	GLint timeLocation = glGetUniformLocation(prog, "time");
	GLint mouseLocation = glGetUniformLocation(prog, "mouse");
	GLint screenLocation = glGetUniformLocation(prog, "screen");
	GLuint quadVAO = genFullScreenQuadVAO();
	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		if (screenLocation != -1) {
			glUniform2f(screenLocation, width, height);
		}
		if (mouseLocation != -1) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glUniform2f(mouseLocation, 2*x/(double)width - 1, 1 - 2*y/(double)height);
		}
		if (timeLocation != -1) {
			glUniform1f(timeLocation, glfwGetTime());
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(prog);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
