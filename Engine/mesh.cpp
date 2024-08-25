#include "mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	SetupMesh();
}

void Mesh::SetupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(BufferAttribs::NumBuffers, Buffers);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[BufferAttribs::ArrayBuffer]);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); //vertices.size is the number of structs, and we mulitply by size of the struct itself

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[BufferAttribs::ElementBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(BufferAttribs::vPos);
	glVertexAttribPointer(BufferAttribs::vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(BufferAttribs::vTex);
	glVertexAttribPointer(BufferAttribs::vTex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(BufferAttribs::vNormal);
	glVertexAttribPointer(BufferAttribs::vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	
	glEnableVertexAttribArray(BufferAttribs::vTangent);
	glVertexAttribPointer(BufferAttribs::vTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(BufferAttribs::vBiTangent);
	glVertexAttribPointer(BufferAttribs::vBiTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
	//std::cout << "The buffers are set up!" << std::endl;
}

void Mesh::Draw(unsigned int shader, GLuint shadowID) {
	glUseProgram(shader);
	//Different maps
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (size_t i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].Type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		//std::cout<<"Activated texture: "<<i<<std::endl;
		//std::cout << "Textures named: " << name + number << std::endl;
	}

	if (shadowID != 0) {
		glActiveTexture(GL_TEXTURE0 + textures.size());
		glBindTexture(GL_TEXTURE_2D, shadowID);
		glUniform1i(glGetUniformLocation(shader, "shadowRT"), textures.size());

		//std::cout << "shadowRT for Mesh: "<<textures.size() << std::endl;
	}

	//std::cout << "The size of textures::"<<textures.size()<<std::endl;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//for (size_t i = 0; i < textures.size(); i++) {
	//	glActiveTexture(GL_TEXTURE0 + i);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	//
	//if (shadowID != 0) {
	//	glActiveTexture(GL_TEXTURE0 + textures.size());
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	glBindVertexArray(0);
}