#include "texture.h"

void texture::bindTexture() {
	for(unsigned int i = 0; i < number; i++)
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //s is x coord
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t is y coord
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

texture::texture(int num) {
	number = num;
	glGenTextures(number, &texture_id[0]);
	bindTexture();
}

void texture::load_texture(std::string texture_name) {
	char* tex_name = &texture_name[0];
	unsigned char* data = stbi_load(tex_name, &width, &height, &nrChannels, 0);
	if (data) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//stbi_image_free(data);
}