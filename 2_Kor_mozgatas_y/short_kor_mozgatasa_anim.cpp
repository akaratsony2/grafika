
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
		float speed = 0.005f + 0.02f * (float)abs(sin(currentTime));
		y += increment > 0.0f ? speed : - speed;
		if ((y + 0.25) >= 1.0f) {
			y = 1.0f - 0.25f;
			increment = -1.0f;
		}
		if ((y - 0.25) <= -1.0f) {
			y = -1.0f + 0.25f;
			increment = 1.0f;
		}
		glProgramUniform1f(renderingProgram, YoffsetLocation, y);
	glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoints.size());
}

void generateCirclePoints(glm::vec2 center_point, GLfloat r, GLint num_segments) {
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


void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	//------------------------Modositott
	generateCirclePoints(glm::vec2(0.0f, 0.0f), 0.25f, 36);

	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	//--------------------Modositott
	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_STATIC_DRAW);

	
	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	XoffsetLocation = glGetUniformLocation(renderingProgram, "offsetX");
	YoffsetLocation = glGetUniformLocation(renderingProgram, "offsetY");

	
	glUseProgram(renderingProgram);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}