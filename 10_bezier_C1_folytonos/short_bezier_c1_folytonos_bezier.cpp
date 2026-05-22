
/* Vektor a görbe pontjainak tárolásához. */
/* Vector for storing the points of a curve. */
std::vector<glm::vec3> pointToDraw;
std::vector<glm::vec3> pointToDraw2;

/* Vektor a görbe kontrollpontjainak tárolásához. */
/* Vector for storing the control points of a curve. */
std::vector<glm::vec3> myControlPoints = {
	glm::vec3((100.0f/300.0f)-1.0f, -((200.0f/300.0f)-1.0f), 0.0f),
	glm::vec3((200.0f / 300.0f) - 1.0f, -((340.0f / 300.0f) - 1.0f), 0.0f),
	glm::vec3((400.0f / 300.0f) - 1.0f, -((400.0f / 300.0f) - 1.0f), 0.0f),
	glm::vec3((550.0f / 300.0f) - 1.0f, -((200.0f / 300.0f) - 1.0f), 0.0f),

};

std::vector<glm::vec3> myControlPoints2 = {
	glm::vec3((550.0f / 300.0f) - 1.0f, -((200.0f / 300.0f) - 1.0f), 0.0f),
	2.0f* glm::vec3((550.0f / 300.0f) - 1.0f, -((200.0f / 300.0f) - 1.0f), 0.0f)
	- glm::vec3((400.0f / 300.0f) - 1.0f, -((400.0f / 300.0f) - 1.0f), 0.0f),
	glm::vec3(0.5f,  0.3f, 0.0f),  // Q2
	glm::vec3(0.7f, -0.2f, 0.0f),  // Q3

};

void drawBezierCurve(std::vector<glm::vec3> controlPoints ,
	std::vector<glm::vec3>& output) { //OUUUUUUUUTPUT ADDED
	/*

            //OUT PUT ADDED UP


	https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Explicit_definition
	https://hu.wikipedia.org/wiki/B%C3%A9zier-g%C3%B6rbe
	*/
	glm::vec3	nextPoint;
	GLfloat		t = 0.0f;// , B;
	GLfloat		increment = 1.0f / 100.0f; /* hány darab szakaszból rakjuk össze a görbénket? */

	while (t <= 1.0f) {
		nextPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < controlPoints.size(); i++) {
			//B = blending(controlPoints.size() - 1, i, t);
			nextPoint += blending(controlPoints.size() - 1, i, t) * controlPoints[i];
			//nextPoint.x += B * controlPoints.at(i).x;
			//nextPoint.y += B * controlPoints.at(i).y;
			//nextPoint.z += B * controlPoints.at(i).z;
		}

		output.push_back(nextPoint);
		t += increment;
	}
}


void init(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram = createShaderProgram();

	/* A megadott paraméterű Bezier göbe pontjainak meghatározása. */
	/* Determining the points of the Bezier curve of the given parameters. */
	drawBezierCurve(myControlPoints,pointToDraw);
	drawBezierCurve(myControlPoints2, pointToDraw2);

	/* Létrehozzuk a szükséges vertex buffer és vertex array objektumokat. */
	/* Create the vertex buffer and vertex array objects. */
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pointToDraw.size() * sizeof(glm::vec3), pointToDraw.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, pointToDraw2.size() * sizeof(glm::vec3), pointToDraw2.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(renderingProgram);

	glPointSize(10.0f);
	glLineWidth(5.0f);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	cout << "float\t\t\t" << sizeof(float) << "\n";
	cout << "double\t\t\t" << sizeof(double) << "\n";
	cout << "long double\t\t" << sizeof(long double) << "\n";
	cout << "unsigned\t\t" << sizeof(unsigned) << "\n";
	cout << "long unsigned\t\t" << sizeof(long unsigned) << "\n";
	cout << "long long unsigned\t" << sizeof(long long unsigned) << "\n";

	// test for the NCR function
	for (int j = 0; j < 10; j++) {
		cout << j << "\n";
		for (int i = 0; i <= j; i++)
			cout << i << " " << NCR(j, i) << "  ";
		cout << "\n";
	}
}


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
	glDrawArrays(GL_LINE_STRIP, 0, pointToDraw2.size());

	glBindVertexArray(VAO[2]);
	glUniform4f(colorLoc, 0.0f, 1.0f, 1.0f, 1.0f);
	glPointSize(20.0f);
	glDrawArrays(GL_POINTS, myControlPoints.size()-1, 1);


	glBindVertexArray(0);
}
