#version 430 core

uniform mat4 matModel;
uniform mat4 matProjView;
uniform mat4 sunMatProjView;
uniform mat4 sunMatOrthoView;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

layout(std430, binding = 0) buffer AlbedoBuffer {   
	vec3 data[];
} Albedos;


out vec4 vPos; 
out vec2 fTexCoord;
out vec3 vNorm;
out vec4 sPos;
out mat3 TBN;

void main(){
	gl_Position = matProjView * matModel * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);
	fTexCoord = TexCoord;
	vPos = matModel * vec4(pos, 1.0);
	sPos = sunMatOrthoView * matModel * vec4(pos, 1.0); //for orthogonal shadow
	//sPos = sunMatProjView * matModel * vec4(pos, 1.0); //for projection shadow
	vNorm = normalize(Normal);
	vec3 N = normalize(vec3(matModel*vec4(Normal, 0.0)));
	vec3 T = normalize(vec3(matModel*vec4(tangent, 0.0)));
	vec3 B = normalize(vec3(matModel*vec4(biTangent, 0.0)));
	TBN = mat3(T, B, N);
	//gl_Position = sunMatOrthoView * matModel * vec4(pos, 1.0);
}
