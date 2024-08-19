#version 430 core

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in vec2 fTexCoord;

out vec4 outColor;

void main(){
	outColor = texture(texture_diffuse1, fTexCoord);
}