void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! Fontos lehet minden egyes alkalommal törölni! */
	/* Let's clear the selected buffers! Usually importand to clear it each time! */
	glClear(GL_COLOR_BUFFER_BIT);
	int numSquares = 8;
	float squareSize = 0.25f;
	float radius = 0.6f;

	for (int i=0; i< numSquares;i++){
		float angle = 2 * 3.14f / numSquares * i;
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;

		glm::mat4 transformation(1.0f);

		// 1. eltolás a kör megfelelő pontjára
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));

		// 2. forgatás a saját középpontja körül (idő alapján forog)
		transformation = glm::rotate(transformation, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));

		// 3. méretezés
		transformation = glm::scale(transformation, glm::vec3(squareSize, squareSize, 1.0f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glBindVertexArray(0);
	}
	


}