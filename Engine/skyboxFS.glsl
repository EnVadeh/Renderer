#version 430 core

in vec3 texCoord;

uniform samplerCube skyBox;

layout (location = 0) out vec3 outColor;

void main(){
	outColor = vec3(texture(skyBox, texCoord));
	outColor = vec3(1.0, 1.0, 1.0);
}