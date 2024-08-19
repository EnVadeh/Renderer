#include "buffer.h"



//also add parametrs so that if it's the same size for all triangles, only once does it need to copy
void MeshBuffer::setVertices(GLfloat vertices[], GLfloat tex[], GLfloat Pos[], GLfloat Size[], GLuint shaderID, bool size, int number) {
	//now that I think about it, why am I even storing the vertex data here??? Maybe one day it may be useful? I dont' know
	for (size_t i = 0; i < number * 3; i++)
		vertex.push_back(vertices[i]);
	for (size_t i = 0; i < number * 2; i++)
		texCoord.push_back(tex[i]);
	samesize = size;
	glGenVertexArrays(BufferAttribs::NumVAOs, VAOs);
	glBindVertexArray(VAOs[0]);
	glGenBuffers(BufferAttribs::NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * number * 3, vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * number * 2, texCoord.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(temp_texcoord), temp_texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(1);
	//std::cout << "Vertex: " << vertex[0] << std::endl;
	//std::cout <<"Tex: " << texCoord[0];
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	int triangles = number / 3;
	for (size_t i = 0; i < triangles; i++) {
		this->Pos.push_back(glm::vec3 (Pos[i * 3], Pos[i * 3 + 1], Pos[i * 3 + 2]));
		this->Size.push_back(glm::vec3 (Size[i * 3], Size[i * 3 + 1], Size[i * 3 + 2]));
	}
	//std::cout << "VAO is: ";
	//std::cout << " " << Buffers[0];
	//std::cout << " " << Buffers[1];
	//DrawCall(shaderID, triangles);
	//std::cout << "Here's the full Pos: ";
	//for (size_t i = 0; i < 2; i++)
		//std::cout <<vertex[i]<<" ";
		//std::cout << this->texCoord[i]<<" ";
		//std::cout << this->Size[0][i] << " ";
	//std::cout << "Let's check to see if this has been printed or not! " << this->Pos[0][0];
}
//I need to work for uniforms and shit later
void MeshBuffer::DrawCall(GLuint shaderID, int number){
	glUseProgram(shaderID);
	//I'd love to use MultiDrawArrays I guess, so I'd have to generate firsts and count arrays, which isn't that hard tbh
	if (samesize) {
		for (size_t i = 0; i < number; i++) {
			glm::mat4 modeltoworld = createGeometricToWorldMatrix(Pos[i], glm::vec3(0, 0, 0), Size[0]);
			GLint mMpos = setUniform(shaderID, "matModel");
			glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
			glDrawArrays(GL_TRIANGLES, i * 3, 3);

		}
	}
	else {
		for (size_t i = 0; i < number; i++) {
			glm::mat4 modeltoworld = createGeometricToWorldMatrix(Pos[i], glm::vec3(0, 0, 0), Size[i]);
			GLint mMpos = setUniform(shaderID, "matModel");
			glUniformMatrix4fv(mMpos, 1, GL_FALSE, glm::value_ptr(modeltoworld));
			glDrawArrays(GL_TRIANGLES, i * 3, 3);
		}
	}

}