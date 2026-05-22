const float radius = 0.6f;

void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! Fontos lehet minden egyes alkalommal törölni! */
	/* Let's clear the selected buffers! Usually importand to clear it each time! */
	glClear(GL_COLOR_BUFFER_BIT);

	float x = radius * cos(currentTime);
	float y = radius * sin(currentTime);

	glm::mat4 transformation = glm::mat4(1.0f);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0));
	transformation =  translateMatrix;

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glBindVertexArray(0);
}