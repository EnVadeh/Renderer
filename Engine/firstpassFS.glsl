#version 430 core

uniform vec4 fLightPos;
uniform vec3 fCamPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in vec2 fTexCoord;
in vec3 fNorm;
in vec4 vPos;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNorm;

void main(){
	//outColor = texture(texture_diffuse1, fTexCoord);
	//outColor = vec4(fTexCoord.x, fTexCoord.y, 1.0, 1.0);

	vec3 LightDir = normalize(vec3(fLightPos) - vec3(vPos));
	float DiffPower = max(dot(normalize(fNorm), vec3(LightDir)), 0.0); 

	vec3 cameraDir = normalize(fCamPos - vec3(vPos));
	vec3 refLightDir = normalize(reflect(-LightDir, normalize(fNorm)));
	float SpecPower = pow(max(dot(refLightDir, cameraDir), 0.0), 32);


	float fAmbient = 0.5;

	outColor = vec3(texture(texture_diffuse1, fTexCoord) * (DiffPower + SpecPower) * fAmbient);
	//outColor = vec3(DiffPower + SpecPower) * fAmbient;
	outNorm = normalize(fNorm);
	//outColor = vec3(1.0, 1.0, 1.0);
	
}