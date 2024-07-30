#version 430 core

uniform sampler2D texture0;

in vec2 fTexCoord;

out vec4 outColor;

void main(){
	outColor = texture(texture0, fTexCoord);
}