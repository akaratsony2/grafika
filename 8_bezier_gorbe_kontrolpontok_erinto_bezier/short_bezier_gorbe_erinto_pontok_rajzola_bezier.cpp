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


void init(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram = createShaderProgram();

	/* A megadott paraméterű Bezier göbe pontjainak meghatározása. */
	/* Determining the points of the Bezier curve of the given parameters. */
	drawBezierCurve(myControlPoints);

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
	glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);
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
