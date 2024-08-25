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
#include "buffer.h"
#include "utils.h"

enum VAO_IDS { Particle, Light, Moveable, Textured_Particle, Triangle_Object, NumVAOs }; // numvaos has the value of how many elements are there in the enum. Can add other shit like {Triangles, Polygons, Circles, NumVAOs}, the enum value will be 3, starting form 0 of the left. This way we don't need to constantly update the value of How many number of vertices we need
enum Buffer_IDs { ArrayBuffer, ElementBuffer, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 }; // can use in vertedx attrib pointer 


GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

//so model coordinates are there, we have then the "position" coordinate which will take the center of the model and will transform the model coordinates to the world space

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

//glm::mat4 NDC = createNormalizationMatrix(-1000, 1000, -1000, 1000, -1000, 1000);
glm::mat4 matProj = glm::perspective(float(glm::radians(45.0f)), 1.0f, 0.1f, 10000.0f);

glm::vec4 lightPos = { 0, 50, -55, 1 };
glm::vec3 vEye = {90, 30.0, -50.0};
glm::vec3 vCenter = glm::vec3(0, 0, -1.0);//change later
glm::vec3 vUp = {0, 1, 0};
glm::mat4 matView = glm::lookAt(vEye, vCenter, vUp);
glm::mat4 matProjView = matProj * matView;
glm::vec3 vLightCenter = glm::vec3(0, 0, 0);
glm::mat4 matSunView = glm::lookAt(glm::vec3(lightPos), vLightCenter, vUp);

glm::mat4 matOrtho = glm::ortho(-50.f, 50.f, -50.f, 50.0f, 0.1f, 10000.0f);
glm::mat4 matOrthoView = matOrtho * matSunView;
glm::mat4 matProjViewSun = matProj * matSunView;

// I wanna make a realistic lighting 

std::string path = "E:/NEW_DOanload/backpack/backpack.obj";
//std::string path = "E:/NEW_DOanload/dusty_old_bookshelf_free/scene.gltf";
//std::string path = "E:/NEW_DOanload/hero_mountain/scene.gltf";
//std::string path = "C:/Users/broia/Downloads/ordinary-house/house.obj";
//std::string path = "C:/Users/broia/Downloads/85-cottage_obj.obj";
//std::string path = "E:/NEW_DOanload/TransCube.obj";
//std::string path = "E:/NEW_DOanload/simple-sphere/source/YourMesh.obj";
//std::string path = "E:/NEW_DOanload/round-meal-low-poly-free/source/Roundmeal.obj";
//std::string path = "C:/Users/broia/Downloads/tkf28u03u0ow-3dbuch/3dbuch/book.obj";

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Light position controls
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		vEye.x += 0.25;
		vCenter.x += 0.25;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		vEye.x -= 0.25;
		vCenter.x -= 0.25;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		vEye.y += 0.25;
		vCenter.y += 0.25;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		vEye.y -= 0.25;
		vCenter.y -= 0.25;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		vEye.z += 0.25;
		vCenter.z += 0.25;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vEye.z -= 0.5;
		vCenter.z -= 0.5;
	}
	//std::cout << "Camera Pos: " << vEye.x << " " << vEye.y << std::endl;
}

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

	ShaderSource source = ReadShaderCode("firstpassVS.glsl", "firstpassFS.glsl");
	GLuint firstpass = CreateShader(source.VertexSource, source.FragmentSource);

	source = ReadShaderCode("shadowmapVS.glsl", "shadowmapFS.glsl");
	GLuint shadowpass = CreateShader(source.VertexSource, source.FragmentSource);

	source = ReadShaderCode("RenderQuadShaderVS.glsl", "RenderQuadShaderFS.glsl");
	GLuint renderpass = CreateShader(source.VertexSource, source.FragmentSource);
	source = ReadShaderCode("terrainVS.glsl", "terrainFS.glsl");
	GLuint terrainpass = CreateShader(source.VertexSource, source.FragmentSource);


	glUseProgram(firstpass);
	
	GLint vecCamPos = setUniform(firstpass, "fCamPos");
	glUniform3fv(vecCamPos, 1, glm::value_ptr(vEye));
	GLint FBMatProjView = setUniform(shadowpass, "matProjView");
	glUniformMatrix4fv(FBMatProjView, 1, GL_FALSE, glm::value_ptr(matProjViewSun));
	GLint RPSunProjView = setUniform(firstpass, "sunMatProjView");
	glUniformMatrix4fv(RPSunProjView, 1, GL_FALSE, glm::value_ptr(matProjViewSun));
	GLint TPSunProjView = setUniform(terrainpass, "sunMatProjView");
	glUniformMatrix4fv(TPSunProjView, 1, GL_FALSE, glm::value_ptr(matProjViewSun));
	GLint FPSunOrthoView = setUniform(firstpass, "sunMatOrthoView");
	glUniformMatrix4fv(FPSunOrthoView, 1, GL_FALSE, glm::value_ptr(matOrthoView));
	GLint terrainProjView = setUniform(terrainpass, "matProjView");
	glUniformMatrix4fv(terrainProjView, 1, GL_FALSE, glm::value_ptr(matProjView));
	GLint terrainOrthoView = setUniform(terrainpass, "matSunOrthoView");
	glUniformMatrix4fv(terrainOrthoView, 1, GL_FALSE, glm::value_ptr(matOrthoView));
	GLint FBlightPos = setUniform(shadowpass, "fLightPos");
	glUniform4fv(FBlightPos, 1, glm::value_ptr(lightPos));
	GLint TPLightPos = setUniform(terrainpass, "fLightPos");
	glUniform4fv(TPLightPos, 1, glm::value_ptr(lightPos));

	Model NewModel(&path[0]);
	
	glm::vec3 posi = { 1, 5, -10 };
	glm::vec3 sizi = { 7, 7, 7 };

	glm::vec3 posi2 = { +1800, 0, 0 };
	glm::vec3 sizi2 = { 500, 500, 1000 };
	
	std::string TerrainTexName = "E:/NEW_DOanload/GroundTexture.jpg";
	std::vector<std::string> terrainTextures;
	terrainTextures.push_back(TerrainTexName);
	TerrainBuffer SimpleTerrain(200, 200, terrainTextures);

	ShadowMap SM;
	GLuint ShadowBuffer = SM.setupShadowFB();
	FrameBuffer Entities;
	GLuint EntitiesBuffer = Entities.setupFrameBuffer();
	ScreenQuad SQ;
	
	glfwSwapInterval(60);

	//put these things in a fucniton so i can just call the draw frame buffers and all these things type shit
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		GLenum err;
		useSB(ShadowBuffer);
		GLint vecLightPos = setUniform(firstpass, "fLightPos");
		glUniform4fv(vecLightPos, 1, glm::value_ptr(lightPos));
		matView = glm::lookAt(vEye, vCenter, vUp);
		matProjView = matProj * matView;
		GLint mPV = setUniform(firstpass, "matProjView");
		glUniformMatrix4fv(mPV, 1, GL_FALSE, glm::value_ptr(matProjView));
		matView = glm::lookAt(vEye, vCenter, vUp);
		GLint SBmOV= setUniform(shadowpass, "matOrthoView");
		glUniformMatrix4fv(SBmOV, 1, GL_FALSE, glm::value_ptr(matOrthoView));
		GLint TmPV= setUniform(terrainpass, "matProjView");
		glUniformMatrix4fv(TmPV, 1, GL_FALSE, glm::value_ptr(matProjView));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		SimpleTerrain.TerrainDraw(shadowpass, 0);
		NewModel.Draw(posi, sizi, shadowpass, 0);
		useFB(EntitiesBuffer);
		SimpleTerrain.TerrainDraw(terrainpass, SM.returnShadowRT());
		NewModel.Draw(posi, sizi, firstpass, SM.returnShadowRT());
		//reminder to change the draw functions so that they check to see if shadowMap exists or not and activates it!
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1000, 1000);
		glDisable(GL_CULL_FACE);
		Entities.ActivateRenderTexture(renderpass);
		//SM.activateshadowRT(renderpass); 
		SQ.drawQuad(renderpass);
		std::cout << "Drawn!" << std::endl;
		glfwSwapBuffers(window);
		while ((err = glGetError()) != GL_NO_ERROR)
				printf("OpenGL error: %d\n", err);
		glfwPollEvents();
		//glActiveTexture(GL_TEXTURE0);
	}
	glfwTerminate();
	return 0;
	//glDeleteBuffers(1, Buffers); Same names being used so would rather not do this rn
	//glDeleteVertexArrays(1, VAOs);
}