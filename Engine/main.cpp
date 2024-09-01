#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/exponential.hpp>
#include <sstream>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <random>
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "buffer.h"
#include "utils.h"
#include "SSBOLoader.h"


enum VAO_IDS { Particle, Light, Moveable, Textured_Particle, Triangle_Object, NumVAOs }; // numvaos has the value of how many elements are there in the enum. Can add other shit like {Triangles, Polygons, Circles, NumVAOs}, the enum value will be 3, starting form 0 of the left. This way we don't need to constantly update the value of How many number of vertices we need
enum Buffer_IDs { ArrayBuffer, ElementBuffer, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 }; // can use in vertedx attrib pointer 


GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

//so model coordinates are there, we have then the "position" coordinate which will take the center of the model and will transform the model coordinates to the world space

glm::mat4 matProj = glm::perspective(float(glm::radians(45.0f)), 1.0f, 0.1f, 10000.0f);

glm::vec4 lightPos = { 0, 50, -55, 1 };
glm::vec3 vEye = {90, 10.0, -100.0};
glm::vec3 vCenter = glm::vec3(0, 30, -1.0);//change later
glm::vec3 vUp = {0, 1, 0};
glm::mat4 matView = glm::lookAt(vEye, vCenter, vUp);
glm::mat4 matProjView = matProj * matView;
glm::vec3 vLightCenter = glm::vec3(0, 0, 0);
glm::mat4 matSunView = glm::lookAt(glm::vec3(lightPos), vLightCenter, vUp);

glm::mat4 matOrtho = glm::ortho(-50.f, 50.f, -50.f, 50.0f, 0.1f, 10000.0f);
glm::mat4 matOrthoView = matOrtho * matSunView;
glm::mat4 matProjViewSun = matProj * matSunView;
glm::mat4 matSkyView = glm::mat4(glm::mat3(matView));
glm::mat4 matSkyProjView = matProj * matSkyView;


std::string path = "E:/NEW_DOanload/backpack/backpack.obj";
std::vector<std::string> cubePaths;

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

	source = ReadShaderCode("skyboxVS.glsl", "skyboxFS.glsl");
	GLuint skypass = CreateShader(source.VertexSource, source.FragmentSource);
	
	std::vector<Materials> SSBOMat;
	Materials Mat1{ glm::vec4{1, 1, 1, 1}, 0.3f, 0.7f };
	Materials Mat2{ glm::vec4{1, 1, 1, 1}, 0.2f, 0.75f };
	Materials Mat3{ glm::vec4{1, 1, 1, 1}, 0.75f, 0.2f };

	SSBOMat.push_back(Mat1);
	SSBOMat.push_back(Mat2);
	SSBOMat.push_back(Mat3);

	SSBufferObject BufferObj(SSBOMat);
	BufferObj.BindSSBOs();

	skyBuffer skyB;
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/right.jpg");
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/left.jpg");
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/top.jpg");
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/bottom.jpg");
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/front.jpg");
	cubePaths.push_back("C:/Users/broia/Downloads/skybox/skybox/back.jpg");


	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	std::vector<glm::vec4> SSAOkernel;
	
	for (size_t i = 0; i < 64; ++i) {
		glm::vec4 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator), 0.0);
		float scale = float(i) / 64.0;
		scale = 0.1 + scale * scale * (1 - 0.1);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		sample *= scale;
		SSAOkernel.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// OpenGL buffer setup
	//also I'd actually love to change the Tangent and Bitangent claculation to the vertex shader rather than the CPU

	GLuint ssaoBuffer;
	glGenBuffers(1, &ssaoBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssaoBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * SSAOkernel.size(), SSAOkernel.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssaoBuffer);

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
	GLint skyProjView = setUniform(skypass, "matProjView");
	glUniformMatrix4fv(skyProjView, 1, GL_FALSE, glm::value_ptr(matSkyProjView));
	GLint terrainOrthoView = setUniform(terrainpass, "matSunOrthoView");
	glUniformMatrix4fv(terrainOrthoView, 1, GL_FALSE, glm::value_ptr(matOrthoView));
	GLint FBlightPos = setUniform(shadowpass, "fLightPos");
	glUniform4fv(FBlightPos, 1, glm::value_ptr(lightPos));
	GLint TPLightPos = setUniform(terrainpass, "fLightPos");
	glUniform4fv(TPLightPos, 1, glm::value_ptr(lightPos));

	Model NewModel(&path[0]);
	
	glm::vec3 posi = { 1, 5, -10 };
	glm::vec3 sizi = { 7, 7, 7 };
	
	std::string TerrainTexName = "E:/NEW_DOanload/GroundTexture.jpg";
	std::vector<std::string> terrainTextures;
	terrainTextures.push_back(TerrainTexName);
	cubeMap cskyBox(cubePaths);


	TerrainBuffer SimpleTerrain(150, 150, terrainTextures);
	ShadowMap SM;
	GLuint ShadowBuffer = SM.setupShadowFB();
	FrameBuffer Entities;
	GLuint EntitiesBuffer = Entities.setupFrameBuffer();
	ScreenQuad SQ;

	glfwSwapInterval(60);

	//put these things in a fucniton so i can just call the draw frame buffers and all these things type shit

	while (!glfwWindowShouldClose(window)) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		processInput(window);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		GLenum err;
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
		GLint ProjLoc = setUniform(renderpass, "matProj");
		glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, glm::value_ptr(matProj));
		GLint VLoc = setUniform(firstpass, "matView");
		glUniformMatrix4fv(VLoc, 1, GL_FALSE, glm::value_ptr(matView));
		GLint VLoc2 = setUniform(terrainpass, "matView");
		glUniformMatrix4fv(VLoc2, 1, GL_FALSE, glm::value_ptr(matView));
		useSB(ShadowBuffer);
		SimpleTerrain.TerrainDraw(shadowpass, 0);
		NewModel.Draw(posi, sizi, shadowpass, 0);
		useFB(EntitiesBuffer);
		cskyBox.bind(skypass);
		skyB.draw();
		SimpleTerrain.TerrainDraw(terrainpass, SM.returnShadowRT());
		NewModel.Draw(posi, sizi, firstpass, SM.returnShadowRT());
		//reminder to change the draw functions so that they check to see if shadowMap exists or not and activates it!
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glViewport(0, 0, 1000, 1000);
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		Entities.ActivateRenderTexture(renderpass);
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, noiseTexture);
		//GLint noiseLoc = glGetUniformLocation(renderpass, "noiseT");
		//glUniform1i(noiseLoc, 3);
		//SM.activateshadowRT(renderpass); 
		SQ.drawQuad(renderpass);
		glfwSwapBuffers(window);
		while ((err = glGetError()) != GL_NO_ERROR)
				printf("OpenGL error: %d\n", err);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}