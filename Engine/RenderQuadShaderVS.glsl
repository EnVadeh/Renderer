#version 430 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

uniform mat4 matProj;

out vec2 fTex;
out mat4 mProj;

void main(){
	gl_Position = vec4(pos,0.0,1.0);
	//gl_Position = vec4(pos, 1.0);
	fTex = tex;
	mProj = matProj;
}
