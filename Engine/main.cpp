#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/exponential.hpp>
#include <sstream>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"

enum VAO_IDS { Particle, Light, Moveable, Textured_Particle, Triangle_Object, NumVAOs }; // numvaos has the value of how many elements are there in the enum. Can add other shit like {Triangles, Polygons, Circles, NumVAOs}, the enum value will be 3, starting form 0 of the left. This way we don't need to constantly update the value of How many number of vertices we need
enum Buffer_IDs { ArrayBuffer, ElementBuffer, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 }; // can use in vertedx attrib pointer 


GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

//so model coordinates are there, we have then the "position" coordinate which will take the center of the model and will transform the model coordinates to the world space

glm::mat4 createGeometricToWorldMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
 	
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), scale);
	//model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Trans = glm::translate(glm::mat4(1.0f), position);
	return Trans * Scale;
}

glm::mat4 createNormalizationMatrix(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(
		2.0f / (maxX - minX),
		2.0f / (maxY - minY),
		2.0f / (maxZ - minZ)
	));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(
		-(maxX + minX) / 2.0f,
		-(maxY + minY) / 2.0f,
		-(maxZ + minZ) / 2.0f
	));

	return scale * translate;
}

//glm::mat4 matProj = glm::frustum(-1, +1, -1, 1, -1, 1);
glm::mat4 NDC = createNormalizationMatrix(-1000, 1000, -1000, 1000, -1000, 1000);
glm::mat4 matProj = glm::perspective(float(glm::radians(90.0f)), 1.0f, 0.1f, 150.0f);


glm::vec3 vEye = {0.0, 30.0, 5.0};
glm::vec3 vCenter = {0, 30, -100};//change later
glm::vec3 vUp = {0, 1, 0};
glm::mat4 matView = glm::lookAt(vEye, vCenter, vUp);

glm::mat4 matOrtho = glm::ortho(-1, 1, -1, 1);

GLint setUniform(GLuint shader, std::string uniformName) {
	glUseProgram(shader);
	const char* uniformNameptr = &uniformName[0];
	GLint pos = glGetUniformLocation(shader, uniformNameptr);
	return pos;	
}



float vertices[9] = {
	0.5, 0.2, 0.0,
	-0.5, 0.2, 0.0,
	0.0, 0.8, 0.0
};


// I wanna make a realistic lighting 

int main() {
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);
	glGenBuffers(1, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(vertices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(vertices), vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0));
	glEnableVertexAttribArray(0);

	

	ShaderSource source = ReadShaderCode("firstpassVS.glsl", "firstpassFS.glsl");
	GLuint firstpass = CreateShader(source.VertexSource, source.FragmentSource);

	glUseProgram(firstpass);
	GLint mNorm = setUniform(firstpass, "matWorld");
	glUniformMatrix4fv(mNorm, 1, GL_FALSE, glm::value_ptr(NDC));
	GLint mView = setUniform(firstpass, "matView");
	glUniformMatrix4fv(mView, 1, GL_FALSE, glm::value_ptr(matView));
	GLint mProj = setUniform(firstpass, "matProj");
	glUniformMatrix4fv(mProj, 1, GL_FALSE, glm::value_ptr(matProj));


	texture triangle(2);
	std::vector<std::string>triangle_name;
	triangle_name.push_back("test.jpg");
	triangle_name.push_back("test2.jpg");
	triangle.load_texture(triangle_name);

	glfwSwapInterval(20);
	//glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 PosTri = { 10, -0.2, 0 };
		glm::vec3 SizTri = { 1000, 1000, 1 };

		glm::mat4 modeltoworld = createGeometricToWorldMatrix(PosTri, glm::vec3(0, 0, 0), SizTri);
		GLint mMpos = setUniform(firstpass, "matModel");
		glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
		//glUseProgram(shader_textured);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		//glBindVertexArray(VAOs[0]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			printf("OpenGL error: %d\n", err);
		}
		glUseProgram(firstpass);
		//glActiveTexture(GL_TEXTURE0); // if I change to texture1, it will use thee previosu texture? I guess it's cyclign? 
		//glBindTexture(GL_TEXTURE_2D, texture_id[1]);
		//glm::mat4 model = glm::mat4(1.0f);

		//ourModel.Draw(shader);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(VAOs[2]);
		PosTri = { -2000, 200, 1 };
		modeltoworld = createGeometricToWorldMatrix(PosTri, glm::vec3(0, 0, 0), SizTri);
		mMpos = setUniform(firstpass, "matModel");
		glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
		glDrawArrays(GL_TRIANGLES, 3, 3);

		//glDrawArrays(GL_TRIANGLES, 3, 3);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, 3);
		//glMultiDrawElements(GL_TRIANGLES, first_vp, GL_UNSIGNED_INT, triangle_multielements_offsets, 4);
		//glMultiDrawArrays(GL_TRIANGLES, first_vp, count_vp, 4);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 2);
		//GLint source_DiffusePos = glGetUniformLocation(shader, "material.vDiffuse_Pos");
		//GLint source_view = glGetUniformLocation(shader, "mView");
		//glUniformMatrix4fv(source_view, 1, GL_FALSE, glm::value_ptr(mView));
		//glUniform3fv(source_DiffusePos, 1, vDiffPos);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
	//glDeleteBuffers(1, Buffers); Same names being used so would rather not do this rn
	//glDeleteVertexArrays(1, VAOs);
}