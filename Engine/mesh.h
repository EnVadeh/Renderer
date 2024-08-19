#pragma once
#include "shader.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <iostream>
#include "stb_image.h"
#include "utils.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};


struct Texture {
	unsigned int id;
	std::string Type;
	std::string path;
};

class Mesh {
	GLuint VAO;
	GLuint Buffers[BufferAttribs::NumBuffers];
	void SetupMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(unsigned int shader);

};