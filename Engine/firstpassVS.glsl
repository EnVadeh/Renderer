#version 430 core

uniform mat4 matWorld;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

layout(location = 0) in vec3 pos;

void main(){
	gl_Position =  matWorld * matProj * matView * matModel * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);

}