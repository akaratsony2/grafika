	// 1. eltolás a kör megfelelő pontjára
	transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));

	// 2. forgatás a saját középpontja körül (idő alapján forog)
	transformation = glm::rotate(transformation, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));

	// 3. méretezés
	transformation = glm::scale(transformation, glm::vec3(squareSize, squareSize, 1.0f));
