void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! Fontos lehet minden egyes alkalommal törölni! */
	/* Let's clear the selected buffers! Usually importand to clear it each time! */
	glClear(GL_COLOR_BUFFER_BIT);
	

	// 1. példány: vízszintesen elnyújtva, bal oldal
	glm::mat4 t1 = glm::mat4(1.0f);
	t1 = glm::translate(t1, glm::vec3(-0.6f, 0.5f, 0.0f));
	t1 = glm::scale(t1, glm::vec3(0.6f, 0.2f, 1.0f)); // széles, lapos
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(t1));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);

	// 2. példány: függőlegesen elnyújtva, középen
	glm::mat4 t2 = glm::mat4(1.0f);
	t2 = glm::translate(t2, glm::vec3(0.0f, 0.0f, 0.0f));
	t2 = glm::scale(t2, glm::vec3(0.2f, 0.7f, 1.0f)); // keskeny, magas
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(t2));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);

	// 3. példány: egyenletesen kicsinyítve, jobb oldal
	glm::mat4 t3 = glm::mat4(1.0f);
	t3 = glm::translate(t3, glm::vec3(0.6f, -0.5f, 0.0f));
	t3 = glm::scale(t3, glm::vec3(0.3f, 0.3f, 1.0f)); // kicsi négyzet
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(t3));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);

	// 4. példány: átlósan torzítva, bal alul
	glm::mat4 t4 = glm::mat4(1.0f);
	t4 = glm::translate(t4, glm::vec3(-0.6f, -0.5f, 0.0f));
	t4 = glm::scale(t4, glm::vec3(0.4f, 0.6f, 1.0f));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(t4));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);


}
