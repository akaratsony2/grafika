#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int window_width = 800;
int window_height = 800;
GLFWwindow* window = nullptr;

GLuint renderingProgram;
GLuint VAO, VBO;

int currentTask = 1;

double deltaTime = 0.0;
double lastTime = 0.0;

float offsetX = 0.0f;
float speedX = 1.0f;

float offsetY = 0.0f;
float speedY = 1.5f;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char* filePath) {
    ifstream fileStream(filePath, ios::in);
    if (!fileStream.is_open()) {
        cout << "HIBA: Nem talalhato a fajl: " << filePath << endl;
        return "";
    }
    string content;
    string line;
    while (getline(fileStream, line)) {
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram() {
    string vertShaderStr = readShaderSource("C:\\Users\\semok\\Desktop\\Compute Graphics\\asdasd\\vS.glsl");
    string fragShaderStr = readShaderSource("C:\\Users\\semok\\Desktop\\Compute Graphics\\asdasd\\fS.glsl");
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return vfProgram;
}

void resetGlobals() {
    offsetX = 0.0f;
    offsetY = 0.0f;
    speedX = 1.0f;
    speedY = 1.5f;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    glUniform1i(glGetUniformLocation(renderingProgram, "useTexture"), 0);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 1.0f, 1.0f, 1.0f);
}

void initTask1() {
    resetGlobals();
    float vertices[] = {
        -0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
         0.1f,  0.1f, 0.0f,
         0.1f, -0.1f, 0.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.5f, 0.2f, 1.0f);
}

void displayTask1(double currentTime) {
    offsetX += speedX * deltaTime;
    if (offsetX > 0.9f || offsetX < -0.9f) {
        speedX = -speedX;
    }

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), offsetX);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void initTask2() {
    resetGlobals();
    vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    int segments = 100;
    float radius = 0.1f;
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * 3.14159f / segments;
        vertices.push_back(cos(angle) * radius);
        vertices.push_back(sin(angle) * radius);
        vertices.push_back(0.0f);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.2f, 0.8f, 1.0f, 1.0f);
}

void displayTask2(double currentTime) {
    speedY = sin(currentTime * 2.0f) * 1.5f;
    offsetY += speedY * deltaTime;

    if (offsetY > 0.9f) {
        offsetY = 0.9f;
        speedY = -abs(speedY);
    }
    else if (offsetY < -0.9f) {
        offsetY = -0.9f;
        speedY = abs(speedY);
    }

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), offsetY);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
}

void initTask3() {
    resetGlobals();
    float vertices[] = {
         0.0f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
         0.1f, -0.1f, 0.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.5f, 1.0f, 0.5f, 1.0f);
}

void displayTask3(double currentTime) {
    float radius = 0.5f;
    offsetX = cos(currentTime) * radius;
    offsetY = sin(currentTime) * radius;

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), offsetX);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), offsetY);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void initTask4() {
    resetGlobals();
    vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    int sides = 6;
    float radius = 0.2f;
    for (int i = 0; i <= sides; i++) {
        float angle = i * 2.0f * 3.14159f / sides;
        vertices.push_back(cos(angle) * radius);
        vertices.push_back(sin(angle) * radius);
        vertices.push_back(0.0f);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 1.0f, 0.0f, 1.0f);
}

void displayTask4(double currentTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    offsetX = (mouseX / window_width) * 2.0f - 1.0f;
    offsetY = 1.0f - (mouseY / window_height) * 2.0f;

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), offsetX);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), offsetY);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
}

float points[4][3] = {
    {-0.5f,  0.5f, 0.0f},
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    { 0.5f,  0.5f, 0.0f}
};
int draggedPoint = -1;
float clickThreshold = 0.1f;

void initTask5() {
    resetGlobals();

    float initialPoints[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    for (int i = 0; i < 4; i++) {
        points[i][0] = initialPoints[i * 3 + 0];
        points[i][1] = initialPoints[i * 3 + 1];
        points[i][2] = initialPoints[i * 3 + 2];
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
}
void displayTask5(double currentTime) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.8f, 0.8f, 0.8f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 4);
}

float points6[8][3];
int draggedPoint6 = -1;

float points7[5][3];
int draggedPoint7 = -1;

float points8[4][3];

float points9[4][3];
int draggedPoint9 = -1;

void initTask6() {
    resetGlobals();
    float radius = 0.4f;
    for (int i = 0; i < 8; i++) {
        float angle = i * 2.0f * 3.14159f / 8.0f;
        points6[i][0] = cos(angle) * radius;
        points6[i][1] = sin(angle) * radius;
        points6[i][2] = 0.0f;
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points6), points6, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void displayTask6(double currentTime) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points6), points6);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.7f, 0.2f, 0.6f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 8);
}

void initTask7() {
    resetGlobals();
    float initialLine[5][3] = {
        {-0.6f, -0.4f, 0.0f},
        {-0.3f,  0.4f, 0.0f},
        { 0.0f, -0.2f, 0.0f},
        { 0.3f,  0.5f, 0.0f},
        { 0.6f, -0.3f, 0.0f}
    };
    for (int i = 0; i < 5; i++) {
        points7[i][0] = initialLine[i][0];
        points7[i][1] = initialLine[i][1];
        points7[i][2] = initialLine[i][2];
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points7), points7, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void displayTask7(double currentTime) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points7), points7);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);
    glLineWidth(3.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 5);
}

void initTask8() {
    resetGlobals();
    points8[0][0] = -0.75f; points8[0][1] = 0.5f;   points8[0][2] = 0.0f;
    points8[1][0] = -0.5f;  points8[1][1] = 0.15f;  points8[1][2] = 0.0f;
    points8[2][0] = 0.0f;   points8[2][1] = 0.0f;   points8[2][2] = 0.0f;
    points8[3][0] = 0.375f; points8[3][1] = 0.5f;   points8[3][2] = 0.0f;
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void displayTask8(double currentTime) {
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points8), points8, GL_DYNAMIC_DRAW);
    glLineWidth(1.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.5f, 0.5f, 0.5f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 4);
    vector<float> curvePoints;
    int segments = 100;
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float u = 1.0f - t;
        float x = u * u * u * points8[0][0] + 3.0f * u * u * t * points8[1][0] + 3.0f * u * t * t * points8[2][0] + t * t * t * points8[3][0];
        float y = u * u * u * points8[0][1] + 3.0f * u * u * t * points8[1][1] + 3.0f * u * t * t * points8[2][1] + t * t * t * points8[3][1];
        curvePoints.push_back(x);
        curvePoints.push_back(y);
        curvePoints.push_back(0.0f);
    }
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(float), curvePoints.data(), GL_DYNAMIC_DRAW);
    glLineWidth(3.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.2f, 0.8f, 0.2f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, segments + 1);
}

void initTask9() {
    resetGlobals();
    points9[0][0] = -0.75f; points9[0][1] = 0.5f;   points9[0][2] = 0.0f;
    points9[1][0] = -0.5f;  points9[1][1] = 0.15f;  points9[1][2] = 0.0f;
    points9[2][0] = 0.0f;   points9[2][1] = 0.0f;   points9[2][2] = 0.0f;
    points9[3][0] = 0.375f; points9[3][1] = 0.5f;   points9[3][2] = 0.0f;
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void displayTask9(double currentTime) {
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points9), points9, GL_DYNAMIC_DRAW);
    glLineWidth(1.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.5f, 0.5f, 0.5f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 4);
    vector<float> curvePoints;
    int segments = 100;
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float u = 1.0f - t;
        float x = u * u * u * points9[0][0] + 3.0f * u * u * t * points9[1][0] + 3.0f * u * t * t * points9[2][0] + t * t * t * points9[3][0];
        float y = u * u * u * points9[0][1] + 3.0f * u * u * t * points9[1][1] + 3.0f * u * t * t * points9[2][1] + t * t * t * points9[3][1];
        curvePoints.push_back(x);
        curvePoints.push_back(y);
        curvePoints.push_back(0.0f);
    }
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(float), curvePoints.data(), GL_DYNAMIC_DRAW);
    glLineWidth(3.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 0.6f, 1.0f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, segments + 1);
}

void initTask10() {
    resetGlobals();
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void displayTask10(double currentTime) {
    float p[4][3] = {
        {-0.8f, -0.5f, 0.0f},
        {-0.5f,  0.5f, 0.0f},
        {-0.2f,  0.5f, 0.0f},
        { 0.0f,  0.0f, 0.0f}
    };
    float q[4][3] = {
        { 0.0f,  0.0f, 0.0f},
        { 0.2f, -0.5f, 0.0f},
        { 0.5f,  0.5f, 0.0f},
        { 0.8f,  0.5f, 0.0f}
    };

    vector<float> curve;
    for (int i = 0; i <= 50; i++) {
        float t = i / 50.0f;
        float u = 1.0f - t;
        curve.push_back(u * u * u * p[0][0] + 3 * u * u * t * p[1][0] + 3 * u * t * t * p[2][0] + t * t * t * p[3][0]);
        curve.push_back(u * u * u * p[0][1] + 3 * u * u * t * p[1][1] + 3 * u * t * t * p[2][1] + t * t * t * p[3][1]);
        curve.push_back(0.0f);
    }
    for (int i = 0; i <= 50; i++) {
        float t = i / 50.0f;
        float u = 1.0f - t;
        curve.push_back(u * u * u * q[0][0] + 3 * u * u * t * q[1][0] + 3 * u * t * t * q[2][0] + t * t * t * q[3][0]);
        curve.push_back(u * u * u * q[0][1] + 3 * u * u * t * q[1][1] + 3 * u * t * t * q[2][1] + t * t * t * q[3][1]);
        curve.push_back(0.0f);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, curve.size() * sizeof(float), curve.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);
    glLineWidth(3.0f);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, 102);

    float conn[] = { 0.0f, 0.0f, 0.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof(conn), conn, GL_DYNAMIC_DRAW);
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

void initTask11() {
    resetGlobals();
    float v[] = { 0.0f, 0.1f, 0.0f, -0.1f, -0.1f, 0.0f, 0.1f, -0.1f, 0.0f };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void displayTask11(double currentTime) {
    glBindVertexArray(VAO);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    for (int i = 0; i < 40; i++) {
        float angle = i * 0.4f + currentTime;
        float rad = i * 0.02f;
        float scl = 1.0f - (i * 0.02f);
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(cos(angle) * rad, sin(angle) * rad, 0.0f));
        m = glm::rotate(m, angle * 2.0f, glm::vec3(0, 0, 1));
        m = glm::scale(m, glm::vec3(scl, scl, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m));
        glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 0.5f + (i * 0.01f), 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void initTask12() {
    resetGlobals();
    vector<float> v;
    v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f);
    for (int i = 0; i <= 6; i++) {
        v.push_back(cos(i * 3.14159f / 3.0f) * 0.08f);
        v.push_back(sin(i * 3.14159f / 3.0f) * 0.08f);
        v.push_back(0.0f);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void displayTask12(double currentTime) {
    glBindVertexArray(VAO);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    float s1 = 1.0f + sin(currentTime) * 0.5f;
    float s2 = 1.0f - sin(currentTime) * 0.5f;

    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.3f, y * 0.3f, 0.0f));
            if (abs(y) % 2 == 0) m = glm::scale(m, glm::vec3(s1, s1, 1.0f));
            else m = glm::scale(m, glm::vec3(s2, s2, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m));
            glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.8f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
        }
    }
}

void initTask13() {
    resetGlobals();
    float v[] = { 0.0f, 0.2f, 0.0f, -0.2f, 0.0f, 0.0f, -0.1f, -0.2f, 0.0f, 0.1f, -0.2f, 0.0f, 0.2f, 0.0f, 0.0f };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void displayTask13(double currentTime) {
    glBindVertexArray(VAO);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    glm::mat4 m1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
    m1 = glm::scale(m1, glm::vec3(2.0f, 0.5f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m1));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

    glm::mat4 m2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
    m2 = glm::scale(m2, glm::vec3(0.5f, 2.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m2));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

    glm::mat4 m3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.6f, 0.0f));
    m3 = glm::scale(m3, glm::vec3(1.5f, 1.5f, 1.0f));
    m3 = glm::rotate(m3, 0.5f, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m3));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
}

void initTask14() {
    resetGlobals();
    float v[] = { -0.1f, 0.1f, 0.0f, -0.1f, -0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.1f, -0.1f, 0.0f };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void displayTask14(double currentTime) {
    glBindVertexArray(VAO);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetX"), 0.0f);
    glUniform1f(glGetUniformLocation(renderingProgram, "offsetY"), 0.0f);

    for (int i = 0; i < 8; i++) {
        float angle = i * (2.0f * 3.14159f / 8.0f);
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(cos(angle) * 0.6f, sin(angle) * 0.6f, 0.0f));
        m = glm::rotate(m, (float)currentTime * 2.0f, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m));
        glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

GLuint texture15;
float t16_angle = 0.0f;
float t16_height = 0.0f;

void initTask15() {
    resetGlobals();
    float v[] = {
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,
         0.5f,-0.5f,-0.5f, 1.0f,0.0f, 0.0f,-1.0f,0.0f,
         0.5f,-0.5f, 0.5f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,
         0.5f,-0.5f, 0.5f, 1.0f,1.0f, 0.0f,-1.0f,0.0f,
        -0.5f,-0.5f, 0.5f, 0.0f,1.0f, 0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f, 0.0f,-1.0f,0.0f,
        -0.5f,-0.5f, 0.5f, 0.0f,0.0f, 0.0f,0.5f,1.0f,
         0.5f,-0.5f, 0.5f, 1.0f,0.0f, 0.0f,0.5f,1.0f,
         0.0f, 0.5f, 0.0f, 0.5f,1.0f, 0.0f,0.5f,1.0f,
         0.5f,-0.5f,-0.5f, 0.0f,0.0f, 0.0f,0.5f,-1.0f,
        -0.5f,-0.5f,-0.5f, 1.0f,0.0f, 0.0f,0.5f,-1.0f,
         0.0f, 0.5f, 0.0f, 0.5f,1.0f, 0.0f,0.5f,-1.0f,
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f, -1.0f,0.5f,0.0f,
        -0.5f,-0.5f, 0.5f, 1.0f,0.0f, -1.0f,0.5f,0.0f,
         0.0f, 0.5f, 0.0f, 0.5f,1.0f, -1.0f,0.5f,0.0f,
         0.5f,-0.5f, 0.5f, 0.0f,0.0f, 1.0f,0.5f,0.0f,
         0.5f,-0.5f,-0.5f, 1.0f,0.0f, 1.0f,0.5f,0.0f,
         0.0f, 0.5f, 0.0f, 0.5f,1.0f, 1.0f,0.5f,0.0f
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    texture15 = SOIL_load_OGL_texture("texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

void displayTask15(double currentTime) {
    glEnable(GL_DEPTH_TEST);
    projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(sin(currentTime) * 6.0f, 4.0f, cos(currentTime) * 6.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glUniform1i(glGetUniformLocation(renderingProgram, "useTexture"), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture15);

    glBindVertexArray(VAO);
    for (int z = -1; z <= 1; z++) {
        for (int x = -1; x <= 1; x++) {
            glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(x * 1.5f, 0.0f, z * 1.5f));
            glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m));
            glDrawArrays(GL_TRIANGLES, 0, 18);
        }
    }
    glDisable(GL_DEPTH_TEST);
}

void initTask16() {
    resetGlobals();
    float v[] = {
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void displayTask16(double currentTime) {
    glEnable(GL_DEPTH_TEST);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) t16_angle -= 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) t16_angle += 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) t16_height += 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) t16_height -= 2.0f * deltaTime;

    projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(cos(t16_angle) * 8.0f, t16_height, sin(t16_angle) * 8.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glUniform1i(glGetUniformLocation(renderingProgram, "useTexture"), 0);

    glBindVertexArray(VAO);

    glm::mat4 m1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m1));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 m2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m2));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 m3 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.5f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model"), 1, GL_FALSE, glm::value_ptr(m3));
    glUniform4f(glGetUniformLocation(renderingProgram, "objectColor"), 0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisable(GL_DEPTH_TEST);
}

void updateMatrices() {
    GLuint modelLoc = glGetUniformLocation(renderingProgram, "model");
    GLuint viewLoc = glGetUniformLocation(renderingProgram, "view");
    GLuint projLoc = glGetUniformLocation(renderingProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void masterInit() {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glUseProgram(renderingProgram);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    initTask1();
}

void masterDisplay(double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    updateMatrices();

    switch (currentTask) {
    case 1: displayTask1(currentTime); break;
    case 2: displayTask2(currentTime); break;
    case 3: displayTask3(currentTime); break;
    case 4: displayTask4(currentTime); break;
    case 5: displayTask5(currentTime); break;
    case 6: displayTask6(currentTime); break;
    case 7: displayTask7(currentTime); break;
    case 8: displayTask8(currentTime); break;
    case 9: displayTask9(currentTime); break;
    case 10: displayTask10(currentTime); break;
    case 11: displayTask11(currentTime); break;
    case 12: displayTask12(currentTime); break;
    case 13: displayTask13(currentTime); break;
    case 14: displayTask14(currentTime); break;
    case 15: displayTask15(currentTime); break;
    case 16: displayTask16(currentTime); break;
    }
}

void cleanUpScene() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(renderingProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_PRESS) {
        bool ctrlPressed = (mods & GLFW_MOD_CONTROL) != 0;

        if (!ctrlPressed) {
            if (key == GLFW_KEY_1) { currentTask = 1; initTask1(); }
            if (key == GLFW_KEY_2) { currentTask = 2; initTask2(); }
            if (key == GLFW_KEY_3) { currentTask = 3; initTask3(); }
            if (key == GLFW_KEY_4) { currentTask = 4; initTask4(); }
            if (key == GLFW_KEY_5) { currentTask = 5; initTask5(); }
            if (key == GLFW_KEY_6) { currentTask = 6; initTask6(); }
            if (key == GLFW_KEY_7) { currentTask = 7; initTask7(); }
            if (key == GLFW_KEY_8) { currentTask = 8; initTask8(); }
            if (key == GLFW_KEY_9) { currentTask = 9; initTask9(); }
            if (key == GLFW_KEY_0) { currentTask = 10; initTask10(); }
        }
        else {
            if (key == GLFW_KEY_1) { currentTask = 11; initTask11(); }
            if (key == GLFW_KEY_2) { currentTask = 12; initTask12(); }
            if (key == GLFW_KEY_3) { currentTask = 13; initTask13(); }
            if (key == GLFW_KEY_4) { currentTask = 14; initTask14(); }
            if (key == GLFW_KEY_5) { currentTask = 15; initTask15(); }
            if (key == GLFW_KEY_6) { currentTask = 16; initTask16(); }
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float ndcX = (mouseX / window_width) * 2.0f - 1.0f;
    float ndcY = 1.0f - (mouseY / window_height) * 2.0f;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (currentTask == 5) {
                for (int i = 0; i < 4; i++) {
                    float dx = points[i][0] - ndcX;
                    float dy = points[i][1] - ndcY;
                    if (sqrt(dx * dx + dy * dy) < clickThreshold) {
                        draggedPoint = i;
                        break;
                    }
                }
            }
            else if (currentTask == 6) {
                for (int i = 0; i < 8; i++) {
                    float dx = points6[i][0] - ndcX;
                    float dy = points6[i][1] - ndcY;
                    if (sqrt(dx * dx + dy * dy) < clickThreshold) {
                        draggedPoint6 = i;
                        break;
                    }
                }
            }
            else if (currentTask == 7) {
                for (int i = 0; i < 5; i++) {
                    float dx = points7[i][0] - ndcX;
                    float dy = points7[i][1] - ndcY;
                    if (sqrt(dx * dx + dy * dy) < clickThreshold) {
                        draggedPoint7 = i;
                        break;
                    }
                }
            }
            else if (currentTask == 9) {
                for (int i = 0; i < 4; i++) {
                    float dx = points9[i][0] - ndcX;
                    float dy = points9[i][1] - ndcY;
                    if (sqrt(dx * dx + dy * dy) < clickThreshold) {
                        draggedPoint9 = i;
                        break;
                    }
                }
            }
        }
        else if (action == GLFW_RELEASE) {
            draggedPoint = -1;
            draggedPoint6 = -1;
            draggedPoint7 = -1;
            draggedPoint9 = -1;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    float ndcX = (xPos / window_width) * 2.0f - 1.0f;
    float ndcY = 1.0f - (yPos / window_height) * 2.0f;

    if (currentTask == 5 && draggedPoint != -1) {
        points[draggedPoint][0] = ndcX;
        points[draggedPoint][1] = ndcY;
    }
    else if (currentTask == 6 && draggedPoint6 != -1) {
        points6[draggedPoint6][0] = ndcX;
        points6[draggedPoint6][1] = ndcY;
    }
    else if (currentTask == 7 && draggedPoint7 != -1) {
        points7[draggedPoint7][0] = ndcX;
        points7[draggedPoint7][1] = ndcY;
    }
    else if (currentTask == 9 && draggedPoint9 != -1) {
        points9[draggedPoint9][0] = ndcX;
        points9[draggedPoint9][1] = ndcY;
    }
}

int main(void) {
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(window_width, window_height, "ZH - Osszefogo", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);

    glfwSwapInterval(1);

    masterInit();

    while (!glfwWindowShouldClose(window)) {
        masterDisplay(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUpScene();
    return EXIT_SUCCESS;
}

void opengl_fuggveny_jegyzet_zh_ra() {

    // --- PUFFER KEZELÉS (VBO) ---

    // glGenBuffers(n, &VBO)
    // Létrehoz a videókártyán n darab memóriablokkot (puffert).
    // Az azonosítóját (ID) beleteszi a VBO változóba.
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // glBindBuffer(típus, VBO)
    // Kijelöli a megadott puffert 'aktívnak'.
    // Bármilyen műveletet végzünk ezután, az ezen a kijelölt pufferen fog megtörténni.
    // Típusok pl: GL_ARRAY_BUFFER (vertex adatok), GL_ELEMENT_ARRAY_BUFFER (index adatok).
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // glBufferData(típus, méret_byteban, &adat, mód)
    // Átmásolja a C++ tömb tartalmát a GPU aktív pufferébe.
    // Módok: GL_STATIC_DRAW (ha fix az adat), GL_DYNAMIC_DRAW (ha egérrel módosítjuk a pontokat).
    float vertices[] = { 0.0f, 0.0f, 0.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // --- VERTEX ATTRIBÚTUMOK ÉS STRUKTÚRA (VAO) ---

    // glGenVertexArrays(n, &VAO)
    // Létrehoz egy VAO-t, ami megjegyzi a vertexek felépítését és a hozzájuk rendelt VBO-kat.
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // glBindVertexArray(VAO)
    // Bekapcsolja a VAO-t, hogy elkezdhesse megjegyezni a konfigurációs lépéseket.
    glBindVertexArray(VAO);

    // glVertexAttribPointer(index, méret, típus, normalizálás, lépésköz, offset)
    // Megmondja a GPU-nak, hogyan kell darabolni a pufferben lévő nyers adatokat.
    // Az index megegyezik a vertex shaderben lévő layout (location = X) értékkel.
    // Méret: hány komponens alkotja (pl. 3 ha vec3 pozíció, 2 ha vec2 textúra koordináta).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // glEnableVertexAttribArray(index)
    // Engedélyezi a fent beállított indexű attribútumot (anélkül nem használja a GPU).
    glEnableVertexAttribArray(0);


    // --- SHADEREK ÉS UNIFORMOK ---

    // glUseProgram(program)
    // Aktiválja a lefordított shader programunkat. Enélkül nem fut le a shader a rajzolásnál.
    GLuint renderingProgram = 0;
    glUseProgram(renderingProgram);

    // glGetUniformLocation(program, "név")
    // Kikeresi, hogy a shaderben lévő uniform változó pontosan melyik memóriacímen található.
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offsetX");

    // glUniform1f(hely, érték)
    // Átküld egy darab float értéket a shaderbe a megadott helyre.
    // Típusok pl: glUniform1f (1 float), glUniform2f (2 float / vec2), glUniformMatrix4fv (mat4 mátrix).
    float offsetX = 0.5f;
    glUniform1f(offsetLoc, offsetX);


    // --- RAJZOLÁS ÉS RESET ---

    // glDrawArrays(rajzolási_mód, kezdő_index, darabszám)
    // A tényleges kirajzoló parancs. Beküldi a csúcspontokat a futószalagra.
    // Módok: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP.
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Puffer és VAO leválasztása (Biztonsági okokból, nehogy véletlenül felülíródjon)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}