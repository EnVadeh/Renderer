#version 430 core

uniform vec4 fLightPos;
uniform vec3 fCamPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowRT;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

in vec2 fTexCoord;
//in vec3 fNorm;
in vec4 vPos;
in vec4 sPos;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNorm;

float shadowCalculation(vec4 sPos, float bias){
	vec3 projCoords = vec3(sPos/sPos.w);
	projCoords = projCoords * 0.5 + 0.5; 
	float closestDepth = texture(shadowRT, projCoords.xy).r; 
	//outColor = vec3(closestDepth);
	float currentDepth = projCoords.z;
	float shadow = currentDepth -0.0005> closestDepth  ? 1.0 : 0.0;  
	return shadow;
}

void main(){
	//outColor = vec3(texture(texture_diffuse1, fTexCoord));
	//outColor = vec4(fTexCoord.x, fTexCoord.y, 1.0, 1.0);
	//outColor = vec3(sPos.z/sPos.w);
	vec3 fNorm = vec3(texture(texture_normal1, fTexCoord));
	fNorm = -fNorm;
	vec3 lightDir = normalize(vec3(fLightPos) - vec3(vPos));
	float DiffPower = max(dot(normalize(fNorm), vec3(lightDir)), 0.0); 
	float bias = max(0.05 * (1.0 - dot(fNorm, lightDir)), 0.005);  
	vec3 cameraDir = normalize(fCamPos - vec3(vPos));
	vec3 refLightDir = normalize(reflect(-lightDir, normalize(fNorm)));
	float SpecPower = pow(max(dot(refLightDir, cameraDir), 0.0), 32);

	float fAmbient = 0.3;

	float shadow = shadowCalculation(sPos, bias);
	vec3 lighting = vec3(fAmbient) + (1.0 - shadow) * (vec3(DiffPower) + vec3(SpecPower));    
	//outColor = vec3(texture(texture_diffuse1, fTexCoord));
	//outColor = vec3(DiffPower + SpecPower) * fAmbient;

	//outColor = vec3(shadow);
	outColor = lighting * vec3(texture(texture_diffuse1, fTexCoord));

	//outColor = vec3(fAmbient * (1.0 -shadow));
	
	//outColor = vec3(texture(texture_diffuse1, fTexCoord));
	outNorm = normalize(fNorm);
	//outColor = vec3(1.0, 1.0, 1.0);
	
}