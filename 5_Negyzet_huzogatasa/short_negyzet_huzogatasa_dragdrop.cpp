static std::vector<glm::vec3>	myPoints = {
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3(-0.5f,  -0.5f, 0.0f),
	glm::vec3(0.5f,  -0.5f, 0.0f),
	glm::vec3(0.5f,  0.5f, 0.0f),

};
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO[0]);

	GLint loc = glGetUniformLocation(renderingProgram, "uColor");
	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, myPoints.size());
	glUniform4f(loc, 1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_POINTS, 0, myPoints.size());


	glBindVertexArray(0);
}
