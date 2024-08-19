#include "buffer.h"

//also add parametrs so that if it's the same size for all triangles, only once does it need to copy
void MeshBuffer::setVertices(std::vector<vertex> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;


	glGenVertexArrays(1, &VAO);
	
	glGenBuffers(BufferAttribs::NumBuffers, Buffers);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[BufferAttribs::ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(BufferAttribs::vPos);
	glVertexAttribPointer(BufferAttribs::vPos, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0));
	glEnableVertexAttribArray(BufferAttribs::vTex);
	glVertexAttribPointer(BufferAttribs::vTex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, texCoord)));
	glEnableVertexAttribArray(BufferAttribs::vNormal);
	glVertexAttribPointer(BufferAttribs::vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, Normal)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[BufferAttribs::ElementBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

}
//I need to work for uniforms and shit later

//i wanna change the size and position with resepect ot the model, not the mesh!
void MeshBuffer::DrawCall(GLuint shaderID){
	glUseProgram(shaderID);
	//I'd love to use MultiDrawArrays I guess, so I'd have to generate firsts and count arrays, which isn't that hard tbh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}