#pragma once
#include <GL/glew.h>
#include "stb_image.h"
#include <string>
#include <vector>

class texture {
public:
	std::vector<GLuint> texture_id;
	std::vector<int> width;
	std::vector<int> height;
	std::vector<int> nrChannels;
	size_t number;
	texture(int num);
	void bindTexture();
	void load_texture(std::vector<std::string> texture_names);
	void tex_to_shader(GLuint shader_id);
};

//note for future, do we want each texture class to have n number of textures and make an array of
//texture objects? or do we want to just have all textures in teh same class? if so then we can just 
//make a struct I guess, what's the point of classes? I will think later