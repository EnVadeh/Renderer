#version 430 core

uniform mat4 matWorld;
uniform mat4 matModel;
uniform mat4 matProjView;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 fTexCoord;

void main(){
	gl_Position = matWorld * matProjView * matModel * vec4(pos, 1.0);
	fTexCoord = TexCoord;
}
