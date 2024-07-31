#include "texture.h"
#include "iostream"

texture::texture(int num, std::string name) {
	texName = name;
	number = num;
	texture_id.resize(number);
	width.resize(number);
	height.resize(number);
	nrChannels.resize(number);
	glGenTextures(number, texture_id.data());
	setup();
}

void texture::setup() {
	for (size_t i = 0; i < number; i++) {
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //s is x coord
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t is y coord
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void texture::load_texture(std::vector<std::string> texture_names) {
	stbi_set_flip_vertically_on_load(true);
	for (size_t i = 0; i < number; i++) {
		char* tex_name = &texture_names[i][0];
		unsigned char* data = stbi_load(tex_name, &width[i], &height[i], &nrChannels[i], 0);
		if (data) {	
			glBindTexture(GL_TEXTURE_2D, texture_id[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
	}
}

void texture::tex_to_shader(GLuint shader_id) {
	for (size_t i = 0; i < number; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
		std::string uniformName = texName + std::to_string(i);
		GLint shaderLoc = glGetUniformLocation(shader_id, uniformName.c_str());
		glUniform1i(shaderLoc, i);
	}
	//glBindTexture(GL_TEXTURE_2D, texture_id[0]); //I think this assumes that glactivei(the lastone)
	//is being boudn to this texture!
}

void texture::bind() {
	for (size_t i = 0; i < number; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
	}
}