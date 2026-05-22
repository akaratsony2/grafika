std::vector<glm::vec3> circlePoints;

void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! Fontos lehet minden egyes alkalommal törölni! */
	/* Let's clear the selected buffers! Usually importand to clear it each time! */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Csatoljuk fel a vertex array objektumot a shader programhoz. */
	/* Bind vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);
	GLint loc = glGetUniformLocation(renderingProgram, "uColor");
	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);

	/** A megadott adatok segítségével szakaszt és annak végpontjait rajzoljuk meg. */
	/** We draw a line and its endpoints with the defined array. */
	glDrawArrays(GL_LINE_LOOP, 1, circlePoints.size()-1);
	glDrawArrays(GL_POINTS, 1, circlePoints.size()-1);

	/* Leválasztjuk a VAO-t, nehogy bármilyen érték felülíródjon. */
	/* Detach VAO for safety reasons, not to modify it accidentaly. */
	glBindVertexArray(0);
}


void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	/** Az egér mutató helyét kezelő függvény. */
	/** Callback function for mouse position change. */

	if (dragged >= 0) {
		GLfloat	xNorm = xPos / ((GLfloat)window_width / 2.0f) - 1.0f;
		GLfloat	yNorm = ((GLfloat)window_height - yPos) / ((GLfloat)window_height / 2.0f) - 1.0f;

		circlePoints.at(dragged).x = xNorm;
		circlePoints.at(dragged).y = yNorm;

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double	x, y;


		glfwGetCursorPos(window, &x, &y);
		dragged = getActivePoint(circlePoints, 0.1f, x, window_height - y);
	}


	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		dragged = -1;
}



void init(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram = createShaderProgram();
	generateCirclePoints(glm::vec2(0.0f, 0.0f), 0.25f, 8);

	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_STATIC_DRAW);

	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glUseProgram(renderingProgram);

	glPointSize(10.0f);
	glLineWidth(5.0f);

	/** Állítsuk be a törlési színt az áttetszőségi értékkel együtt! [0.0, 1.0] */
	/** Set the clear color (red, green, blue, alpha), where alpha is transparency! [0.0, 1.0] */
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void generateCirclePoints(glm::vec2 center_point, GLfloat r, GLint num_segments) {
	circlePoints.clear();
	GLfloat		x, y;
	GLfloat		full_circle = 2.0f * M_PI;
	GLfloat		alpha = full_circle / (GLfloat)num_segments;

	GLfloat  t = 0.0f;
	circlePoints.push_back(glm::vec3(center_point.x, center_point.y, 0.0f));

	for (int i = 0; i <= num_segments; i++) {
		x = r * cos(t) + center_point.x;
		y = r * sin(t) + center_point.y;
		t += alpha;
		circlePoints.push_back(glm::vec3(x, y, 0.0f));
	}

}