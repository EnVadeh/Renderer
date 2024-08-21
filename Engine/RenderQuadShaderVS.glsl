#version 430 core



layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

out vec2 fTex;

void main(){
	gl_Position = vec4(pos,1.0,1.0);
	//gl_Position = vec4(pos, 1.0);
	fTex = tex;
}
