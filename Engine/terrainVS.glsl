#version 430 core

uniform mat4 matModel;
uniform mat4 matProjView;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Norm;

out vec4 vPos; 
out vec3 fNorm;
out vec2 fTexCoord;

void main(){
	gl_Position = matProjView * matModel * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);
	fTexCoord = TexCoord;
	vPos = matModel * vec4(pos, 1.0);
	fNorm = normalize(Norm);
}
