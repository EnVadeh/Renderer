#version 430 core

layout (location = 0) in vec3 pos;

uniform mat4 matProjView;

out vec3 texCoord;

void main(){
	texCoord = pos;
	gl_Position = (matProjView * vec4(pos, 1.0)).xyww;
}