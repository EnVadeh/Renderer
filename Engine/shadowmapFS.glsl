#version 430 core

uniform vec4 fLightPos;
uniform sampler2D texture_diffuse1;

in vec2 fTexCoord;
in vec3 fNorm;
in vec4 vPos;

//layout (location = 0) out vec3 outColor;
//layout (location = 1) out vec3 outNorm;

void main(){
	
	//outColor = vec4(fTexCoord.x, fTexCoord.y, 1.0, 1.0);
	//outColor = texture(texture_diffuse1, fTexCoord) * (DiffPower + SpecPower) * fAmbient;
	
	//outNorm = normalize(fNorm);
	//outColor = vec3(texture(texture_diffuse1, fTexCoord));
	//outColor = vec3(fTexCoord, 1.0);
	//outColor = vec3(DiffPower);

}