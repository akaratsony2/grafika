void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {

		GLfloat	xNorm = xPos / ((GLfloat)window_width / 2.0f) - 1.0f;
		GLfloat	yNorm = ((GLfloat)window_height - yPos) / ((GLfloat)window_height / 2.0f) - 1.0f;
		generateCirclePoints(glm::vec2(xNorm, yNorm), 0.25f, 6);


		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}


void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO[0]);


	glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoints.size());
	glBindVertexArray(0);
}



void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	//-----------------------------------------------
	generateCirclePoints(glm::vec2(0.0f, 0.0f), 0.25f, 6);


	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//----------------------------------
	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_DYNAMIC_DRAW);


	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(renderingProgram);
	glPointSize(10.0f);
	glLineWidth(5.0f);
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
		dragged = getActivePoint(circlePoints, 0.1f, x, window_height - y);
	}

	/** Az egér bal gombjának felengedése mindenképp megszünteti a vonszolási üzemmódot. */
	/** Releasing left mouse button stops dragging operation, even if it was not active before. */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		dragged = -1;
}
