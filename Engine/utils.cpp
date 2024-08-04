#include "utils.h"

glm::mat4 createGeometricToWorldMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 Trans = glm::translate(glm::mat4(1.0f), position);
	return Trans * Scale;
}

GLint setUniform(GLuint shader, std::string uniformName) {
	glUseProgram(shader);
	const char* uniformNameptr = &uniformName[0];
	GLint pos = glGetUniformLocation(shader, uniformNameptr);
	return pos;
}