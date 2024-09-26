#include "buffer.h"

//also add parametrs so that if it's the same size for all triangles, only once does it need to copy
//change the name of the terrain texture so it can work with the other shaders too!
TerrainBuffer::TerrainBuffer(size_t width, size_t height, std::vector<std::string> texName) : terrain(1, name) {
	terrainW = width;
	terrainH = height;
	for (size_t z = 0; z < terrainH; z++) {
		for (size_t x = 0; x < terrainW; x++) {
			vertex v;
			v.Position = glm::vec3(static_cast<float>(x) - (terrainW / 2.0f), 0.0f, static_cast<float>(z) - (terrainH / 2.0f));
			v.texCoord = glm::vec2(static_cast<float>(x) / (terrainW - 1), static_cast<float>(z) / (terrainH - 1));
			v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices.push_back(v);
		}
	}
	for (size_t z = 0; z < terrainH - 1; z++) {
		for (size_t x = 0; x < terrainW - 1; x++) {
            size_t i0 = x + z * terrainW;
            size_t i1 = x + 1 + z * terrainW;
            size_t i2 = x + (z + 1) * terrainW;
            size_t i3 = x + 1 + (z + 1) * terrainW;

            glm::vec3 v0 = vertices[i2].Position - vertices[i0].Position;
            glm::vec3 v1 = vertices[i1].Position - vertices[i0].Position;

            glm::vec3 normal = glm::normalize(glm::cross(v0, v1));

			//std::cout << "The nromals: " << normal.x << " " << normal.y << " " << normal.z << std::endl;

            vertices[i0].Normal += normal;
            vertices[i1].Normal += normal;
            vertices[i2].Normal += normal;
            vertices[i3].Normal += normal;
			//std::cout<<"Normals of vertices: "<< vertices[i0].Normal.x << " " << vertices[i0].Normal.y << " " << vertices[i0].Normal.z << std::endl;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i1);

		}
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(BufferAttribs::NumBuffers, Buffers);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[BufferAttribs::ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[BufferAttribs::ElementBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Define the vertex attribute pointers
	glEnableVertexAttribArray(BufferAttribs::vPos);
	glVertexAttribPointer(BufferAttribs::vPos, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(BufferAttribs::vTex);
	glVertexAttribPointer(BufferAttribs::vTex, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texCoord));
	glEnableVertexAttribArray(BufferAttribs::vNormal);
	glVertexAttribPointer(BufferAttribs::vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
	terrain.load_textures_manual(texName);
}

void TerrainBuffer::TerrainDraw(GLuint shaderID, GLuint shadowID) {
	glUseProgram(shaderID);
	terrain.tex_to_shader(shaderID);
	if (shadowID != 0) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, shadowID);
		glUniform1i(glGetUniformLocation(shaderID, "shadowRT"), 1);
	}
	glm::vec3 Pos = {0, 0, -1};
	glm::vec3 Size = {1, 1, 1};
	glm::mat4 terrainMTW = createGeometricToWorldMatrix(Pos, glm::vec3(0, 0, 0), Size);

	GLint terrainMTWpos = setUniform(shaderID, "matModel");
	glUniformMatrix4fv(terrainMTWpos, 1, GL_FALSE, glm::value_ptr(terrainMTW));
	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4); 
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
}

// basicallyy frame buffers need either a Render Buffer to render to or Textures, Textures for multisamppling and render buffers for multiple queues I think or fo rlike 
//parallel quues maybe? Like rendering frames ahead of others I thinks

GLuint FrameBuffer::setupFrameBuffer() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	render_texture.resize(3);
	glGenTextures(3, render_texture.data());
	for(size_t i = 0; i < 3; i++){
	glBindTexture(GL_TEXTURE_2D, render_texture[i]);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //for da shadow
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, render_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, render_texture[i], 0);
	//std::cout << "Render Texture" << i << " is: " << render_texture[i] << std::endl;
	setupRenderBuffer();
	//std::cout << "WTF!!! FBO NOT COMPLETE!!!! OR NOT!!! " << ": " << glCheckFramebufferStatus(GL_FRAMEBUFFER);

	}
	glDrawBuffers(3, DrawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return FBO;
}

GLuint FrameBuffer::renderTexture() {
	return render_texture[0]; //useless right now
}

void FrameBuffer::ActivateRenderTexture(GLuint shaderID) {
	glUseProgram(shaderID);
	for (size_t i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, render_texture[i]);
		//std::string name = "RT" + std::to_string(i);
		GLint shaderLoc = glGetUniformLocation(shaderID, RenderTextureNames[i].c_str());
		glUniform1i(shaderLoc, i);
	}
}

void FrameBuffer::setupRenderBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, 2048, 2048);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::readFromBuffer() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

//std140 means each struct member get's its own space, so no packing
//std430 u can pack stuff but it's kinda tricky
SSBufferObject::SSBufferObject(std::vector<Materials> Temp) {
	this->Data.resize(Temp.size());
	this->Data = Temp;
	glGenBuffers(BufferAttribs::NumSSBs, SSBO);
	for (size_t i = 0; i < BufferAttribs::NumSSBs; i++) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Materials) * Data.size(), Data.data(), GL_DYNAMIC_COPY);
	}
}

void SSBufferObject::BindSSBOs() {
	for (size_t i = 0; i < BufferAttribs::NumSSBs; i++) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, SSBO[i]);
	}
}

void SSBufferObject::UnbindSSBOs() {
	for (size_t i = 0; i < BufferAttribs::NumSSBs; i++) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
	}
}

GLuint SSBufferObject::SSBOid(size_t index) {
	return SSBO[index];
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

GLuint ShadowMap::setupShadowFB() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &shadowRT);
	glBindTexture(GL_TEXTURE_2D, shadowRT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //for da shadow
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowRT, 0);
	//std::cout << "Render Texture" << i << " is: " << render_texture[i] << std::endl;
	//setupRenderBuffer();
	//std::cout << "WTF!!! FBO NOT COMPLETE!!!! OR NOT!!! " << ": " << glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return FBO;
}

void ShadowMap::activateshadowRT(GLuint shaderID) {
	glUseProgram(shaderID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowRT);
	GLint shadowMAPRT = glGetUniformLocation(shaderID, "shadowRT");
	glUniform1i(shadowMAPRT, 0);
}

GLuint ShadowMap::returnShadowRT() {
	return shadowRT;
}

skyBuffer::skyBuffer() {
	float temp[108] = {
		// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
	};
	std::copy(std::begin(temp), std::end(temp), std::begin(data));
	std::cout << "The shit is: " << data[20];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);;
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(BufferAttribs::vPos);
	glVertexAttribPointer(BufferAttribs::vPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void skyBuffer::draw(){
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}

void useFB(GLuint FB) {
	glViewport(0, 0, 2048, 2048);
	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

}
void bindFB(GLuint FB) {
	glViewport(0, 0, 2048, 2048);
	glBindFramebuffer(GL_FRAMEBUFFER, FB);

}
void useSB(GLuint SB) {
	glViewport(0, 0, 2048, 2048);
	glBindFramebuffer(GL_FRAMEBUFFER, SB);
	glClear(GL_DEPTH_BUFFER_BIT);
}


