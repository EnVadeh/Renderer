#pragma once
#include <GL/glew.h>
#include "stb_image.h"
#include <string>
#include <vector>

class texture {
public:
	std::vector<GLuint> texture_id;
	int width;
	int height;
	int nrChannels;
	int number;
	texture(int num);
	void bindTexture();
	void load_texture(std::string texture_name);
};

//note for future, do we want each texture class to have n number of textures and make an array of
//texture objects? or do we want to just have all textures in teh same class? if so then we can just 
//make a struct I guess, what's the point of classes? I will think later