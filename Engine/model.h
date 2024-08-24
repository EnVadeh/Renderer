#pragma once
#include "shader.h"
#include "mesh.h"

class Model {
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	Model(char* path)
	{
		loadModel(path);
	}
	void Draw(glm::vec3 Pos, glm::vec3 Size, GLuint shaderID, GLuint ShadowID);
private:
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};
