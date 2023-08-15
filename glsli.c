#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

float vertices[] = {
	0.5f,  0.5f,  0.0f,
	0.5f,  -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

int indices[] = {
	0, 1, 3,
	1, 2, 3
};

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

GLuint compileShader(const char *fname, GLenum type)
{
	char *text = readFile(fname);
	if (!text) {
		return 0;
	}
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&text, NULL);
	glCompileShader(shader);
	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		fprintf(stderr, "error: failed to compile a shader: %s\n", fname);
		glDeleteShader(shader);
		free(text);
		return 0;
	}
	free(text);
	return shader;
}

GLuint makeShaderProgram(const char *vShaderPath, const char *fShaderPath)
{
	GLuint vs = compileShader(vShaderPath, GL_VERTEX_SHADER);
	if (!vs) {
		return 0;
	}
	GLuint fs = compileShader(fShaderPath, GL_FRAGMENT_SHADER);
	if (!fs) {
		glDeleteShader(vs);
		return 0;
	}
	GLuint prog = glCreateProgram();
	glAttachShader(prog, fs);
	glAttachShader(prog, vs);
	glLinkProgram(prog);
	int linked;
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (!linked) {
		fprintf(stderr, "error: failed to link a shader program\n");
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(prog);
		return 0;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	return prog;
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
	if (argc < 1) {
		return 1;
	}
	glfwSetErrorCallback(onError);
	if (!glfwInit()) {
		fprintf(stderr, "error: failed to initialize GLFW\n");
		return 1;
	}
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

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	GLuint shader_prog = makeShaderProgram("triangle.vert", "triangle.frag");
	if (!shader_prog) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_prog);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
