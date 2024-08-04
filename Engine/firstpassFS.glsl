#version 430 core

uniform sampler2D triangle0;
uniform sampler2D triangle1;

in vec2 fTexCoord;

out vec4 outColor;

void main(){
	outColor = texture(triangle0, fTexCoord);
}