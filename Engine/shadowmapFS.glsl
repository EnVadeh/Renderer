#version 430 core

uniform sampler2D texture_diffuse1;

in vec2 fTexCoord;
in vec3 fNorm;
in vec4 vPos;

layout (location = 0) out vec3 outColor;

void main(){
	//outColor = vec3(texture(texture_diffuse1, fTexCoord));
	
	//outColor = vec4(fTexCoord.x, fTexCoord.y, 1.0, 1.0);
	//outColor = texture(texture_diffuse1, fTexCoord) * (DiffPower + SpecPower) * fAmbient;
	//outColor = vec4(DiffPower);
	outColor = vec3(1.0, 1.0, 1.0);
}