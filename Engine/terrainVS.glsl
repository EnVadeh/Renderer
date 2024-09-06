#version 430 core


layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

out vec4 vPos; 
out vec2 fTexCoord;
out vec3 fNorm;

void main(){
	//gl_Position = matProjView * matModel * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);
	fTexCoord = TexCoord;
	fNorm = Normal;
	//vPos = matModel * vec4(pos, 1.0); //dont change vPos for ssao, remember it's used for other things 
	vPos = vec4(pos, 1.0); //for tesselation
	//sPos = sunMatProjView * matModel * vec4(pos, 1.0); //for projection shadow
	//gl_Position = matSunOrthoView * matModel * vec4(pos, 1.0);
}
