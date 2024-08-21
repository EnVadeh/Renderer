#include "buffer.h"

//also add parametrs so that if it's the same size for all triangles, only once does it need to copy


void MeshBuffer::setVertices(GLfloat vertices[], GLfloat tex[], GLfloat Pos[], GLfloat Size[], GLuint shaderID, bool size, int number) {
	//now that I think about it, why am I even storing the vertex data here??? Maybe one day it may be useful? I dont' know

	

	for (size_t i = 0; i < number * 3; i++)
		vertex.push_back(vertices[i]);
	for (size_t i = 0; i < number * 2; i++)
		texCoord.push_back(tex[i]);

	samesize = size;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(BufferAttribs::NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * number * 3, vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * number * 2, texCoord.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(temp_texcoord), temp_texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(1);
	std::cout << "Vertex: " << vertex[0] << std::endl;
	std::cout <<"Tex: " << texCoord[0];
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	int triangles = number / 3;
	for (size_t i = 0; i < triangles; i++) {
		this->Pos.push_back(glm::vec3(Pos[i * 3], Pos[i * 3 + 1], Pos[i * 3 + 2]));
		this->Size.push_back(glm::vec3(Size[i * 3], Size[i * 3 + 1], Size[i * 3 + 2]));
	}
}

//I need to work for uniforms and shit later

//i wanna change the size and position with resepect ot the model, not the mesh!
void MeshBuffer::DrawCall(GLuint shaderID, int number) {
	glUseProgram(shaderID);
	//I'd love to use MultiDrawArrays I guess, so I'd have to generate firsts and count arrays, which isn't that hard tbh
	glBindVertexArray(VAO);
	if (samesize) {
		for (size_t i = 0; i < number; i++) {
			glm::mat4 modeltoworld = createGeometricToWorldMatrix(Pos[i], glm::vec3(0, 0, 0), Size[0]);
			GLint mMpos = setUniform(shaderID, "matModel");
			glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
			glDrawArrays(GL_TRIANGLES, i * 3, 3);

		}
	}
	else {
		for (size_t i = 0; i < number; i++) {
			glm::mat4 modeltoworld = createGeometricToWorldMatrix(Pos[i], glm::vec3(0, 0, 0), Size[i]);
			GLint mMpos = setUniform(shaderID, "matModel");
			glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
			std::cout << "Value of i is: " << i << " ,the value of number is: " << number << std::endl;
			glDrawArrays(GL_TRIANGLES, i * 3, 3);
		}

	}
}

// basicallyy frame buffers need either a Render Buffer to render to or Textures, Textures for multisamppling and render buffers for multiple queues I think or fo rlike 
//parallel quues maybe? Like rendering frames ahead of others I thinks



GLuint FrameBuffer::setupFrameBuffer() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &render_texture);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //for da shadow
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, render_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);
	glDrawBuffers(1, DrawBuffers);

	//setupRenderBuffer();
	//std::cout << "WTF!!! FBO NOT COMPLETE!!!! OR NOT!!! " << ": " << glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return FBO;
}


GLuint FrameBuffer::renderTexture() {

	return render_texture;
}

void FrameBuffer::ActivateRenderTexture(GLuint shaderID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	GLint shaderLoc = glGetUniformLocation(shaderID, "RT");
	glUniform1i(shaderLoc, 0);
}

void FrameBuffer::setupRenderBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH, 2048, 2048);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH, GL_RENDERBUFFER, RBO);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void FrameBuffer::readFromBuffer() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

void ScreenQuad::drawQuad(GLuint shaderID) {
	glUseProgram(shaderID);
	float quadVertices[] = {
		// Positions   // Texture Coords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);;
	glGenBuffers(1, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[BufferAttribs::ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(BufferAttribs::vPos);
	glVertexAttribPointer(BufferAttribs::vPos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(BufferAttribs::vTex);
	glVertexAttribPointer(BufferAttribs::vTex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



	// Vertex data for a render quad
