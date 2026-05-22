//negyzet
static GLfloat s_vertices[] =
{
	-0.25f, -0.25f, 0.00f,
	-0.25f, 0.25f, 0.00f,
	 0.25f,  0.25f, 0.00f,
	 0.25f,  -0.25f, 0.00f
};

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_COLOR_BUFFER_BIT);
		x += increment;
		if (( (x+0.25) > 1.0f) || ((x-0.25) < -1.0f)) increment = -increment;
		glProgramUniform1f(renderingProgram, XoffsetLocation, x);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}
