#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

std::vector<glm::vec3> circlePoints;


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

void init(GLFWwindow* window) {

	renderingProgram = createShaderProgram();

	generateCirclePoints(glm::vec2(0.0f, 0.0f), 0.25f, 6);


	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(renderingProgram);
	glPointSize(30.0f);
	glLineWidth(15.0f);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoints.size());
	glBindVertexArray(0);
}