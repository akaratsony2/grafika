std::vector<glm::vec3> myControlPoints = {
	glm::vec3((100.0f/300.0f)-1.0f, -((200.0f/300.0f)-1.0f), 0.0f),
	glm::vec3((200.0f / 300.0f) - 1.0f, -((340.0f / 300.0f) - 1.0f), 0.0f),
	glm::vec3((400.0f / 300.0f) - 1.0f, -((400.0f / 300.0f) - 1.0f), 0.0f),
	glm::vec3((550.0f / 300.0f) - 1.0f, -((200.0f / 300.0f) - 1.0f), 0.0f),

};


void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! Fontos lehet minden egyes alkalommal törölni! */
	/* Let's clear the selected buffers! Usually importand to clear it each time! */
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO[0]);
	GLint colorLoc = glGetUniformLocation(renderingProgram, "uColor");
	glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_LINE_STRIP, 0, pointToDraw.size());

	glBindVertexArray(VAO[1]);
	glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINE_STRIP, 0, myControlPoints.size());
	glDrawArrays(GL_POINTS, 0, myControlPoints.size());

	glBindVertexArray(VAO[2]);
	glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);


	glBindVertexArray(0);
}


void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	/** Az egér mutató helyét kezelő függvény. */
	/** Callback function for mouse position change. */

	if (dragged >= 0) {
		/** Ha vonszolunk egy pontot, akkor számoljuk ki a normalizált koordinátáit. */
		/** If we are dragging a point, let's calculate the normalized values. */
		GLfloat	xNorm = xPos / ((GLfloat)window_width / 2.0f) - 1.0f;
		GLfloat	yNorm = ((GLfloat)window_height - yPos) / ((GLfloat)window_height / 2.0f) - 1.0f;

		/** Tároljuk el a módosított értékeket. */
		/** Let's store the modified values. */
		myControlPoints.at(dragged).x = xNorm;
		myControlPoints[dragged].x = xNorm;
		myControlPoints.at(dragged).y = yNorm;
		pointToDraw.clear();
		drawBezierCurve(myControlPoints);
		/** Mozgassuk a módosított értékeket a GPU memóriájába. */
		/** Let's transfer the modified values to the GPU. */
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, pointToDraw.size() * sizeof(glm::vec3), pointToDraw.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	/** Az egér gombjaihoz köthető események kezelése. */
	/** Callback function for mouse button events. */

	/** Az egér bal gombjának megnyomása indíthat el vonszolást. */
	/** Pressing left mouse button might initiate dragging. */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double	x, y;

		/** Kinyerjük az egérkurzor pixelkoordinátáit. */
		/** Obtain the pixel coordinates of the mouse. */
		glfwGetCursorPos(window, &x, &y);
		/** dragged lesz az indexe a kiválasztott pontnak [0, 1, ...], -1 jelentése, hogy semmit nem fogtunk meg. */
		/** dragged is the index of the point that is selected [0, 1, ...], -1 means nothing is grabbed. */
		dragged = getActivePoint(myControlPoints, 0.1f, x, window_height - y);
	}

	/** Az egér bal gombjának felengedése mindenképp megszünteti a vonszolási üzemmódot. */
	/** Releasing left mouse button stops dragging operation, even if it was not active before. */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		dragged = -1;
}