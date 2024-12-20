#version 430 core

in vec3 texCoord;

uniform samplerCube skyBox;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outRadiance;

void main(){
	outColor = vec3(texture(skyBox, texCoord.xyz));
}