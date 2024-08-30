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

void texture::load_textures_manual(std::vector<std::string> texture_names) {
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
	size_t texNum = 1;
	for (size_t i = 0; i < number; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
		std::string uniformName = texName + std::to_string(texNum++);
		GLint shaderLoc = glGetUniformLocation(shader_id, uniformName.c_str());
		glUniform1i(shaderLoc, i);
		//std::cout << "textures are named: " << uniformName<<""<<i << std::endl;
	}
}

void texture::bind() {
	for (size_t i = 0; i < number; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
	}
}


GLuint load_individual_texture(const char* path, const std::string& directory){
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

cubeMap::cubeMap(std::vector<std::string> paths) {
	this->paths = paths;
	glGenTextures(1, &cubeTex);
	glBindBuffer(GL_TEXTURE_CUBE_MAP, cubeTex);
	for (size_t i = 0; i < paths.size(); i++) {
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			std::cout << "The face loaded right now is!" << paths[i] << std::endl;
			stbi_image_free(data);
		}
		else
			std::cout << "The " << i << "th cubemap face wasn't found!" << std::endl;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void cubeMap::bind(GLuint shaderID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTex);
	std::string name = "skyBox";
	GLint shaderLoc = glGetUniformLocation(shaderID, name.c_str());
	glUniform1i(shaderLoc, 0);
}