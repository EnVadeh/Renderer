#version 430 core

const float PI = 3.14159265359;

uniform vec4 fLightPos;
uniform vec3 fCamPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowRT;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

in vec2 fTexCoord;
in vec4 vPos;
in vec3 vNorm;
in vec4 sPos;
in mat3 TBN;

struct Materials{
	vec4 Albedo;
	float Mettalic;
	float Roughness;
	float padding;
	float paddin2;
};

layout(std430, binding = 0) buffer SSBO {   
	Materials Mat[];
} SS;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNorm;
layout (location = 2) out vec3 outPos;


float distribution(float iso, float roughness){
	float a = roughness * roughness;
	float a2 = a*a;
	float iso2 = iso*iso;
	float D = iso2 * (a2 - 1.0) + 1.0;
	D = PI * D * D;
	return a2/max(D, 0.0000001);
}

float geoSchlick(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = (r*r)/8.0;
	float D = NdotV * (1.0 - k) + k;
	return NdotV/D;
}

float geoSmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = geoSchlick(NdotV, roughness);
	float ggx2 = geoSchlick(NdotL, roughness);
	return ggx1 * ggx2;
}


vec3 fres(float cosTheta, vec3 F){
	return F + (1.0 - F) * pow(1.0 - cosTheta, 5.0); //how muych surface actually refelcts
}

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
	float fAmbient = 0.3;
	vec3 fNorm = vec3(texture(texture_normal1, fTexCoord));
	fNorm = fNorm * 2.0 - 1.0;
	fNorm = normalize(fNorm * TBN);
	float Met = clamp(SS.Mat[1].Mettalic, 0.0, 1.0);
	float Rough = clamp(SS.Mat[1].Roughness, 0.0, 1.0);
	//vec3 N = normalize(vNorm);
	vec3 N = fNorm;
	vec3 lightDir = normalize(vec3(fLightPos) - vec3(vPos));
	vec3 cameraDir = normalize(fCamPos - vec3(vPos));
	vec3 fColor = vec3(texture(texture_diffuse1, fTexCoord));
	vec3 AlbedoAmbient = fAmbient * fColor;
	vec3 reflectivity = mix(vec3(0.03), fColor, Met); //interpolation kinda
	vec3 halfDir = normalize(lightDir + cameraDir);
	float distance = length(fLightPos - vPos); 
	//float lightIntensity = 20; 
	//float attenuation = lightIntensity / (1.0 + distance * distance);
	float attenuation = 1;  //directional light
	vec3 radiance = vec3(7.50) * attenuation; //Basically Flux per unit solid angle
	float Fresnel = dot(halfDir, lightDir); //atleast for raytraing useful iirc
	float isoMicro = max(dot(halfDir, N), 0.0);
	float bias = max(0.05 * (1.0 - dot(fNorm, lightDir)), 0.005);  
	float shadow = shadowCalculation(sPos, bias);
	
	float NDF = distribution(isoMicro, Rough);
	float G = geoSmith(N, cameraDir, lightDir, Rough);
	//vec3 F = fres(max(dot(halfDir, cameraDir), 0.0), reflectivity);
	vec3 F = fres(max(dot(halfDir, lightDir), 0.0), reflectivity);

	vec3 nominator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, cameraDir), 0.0) * max(dot(N, lightDir), 0.0);
	vec3 specular = nominator/max(denominator, 0.001);

	//How much is specular reflection
	vec3 kS = F;
	//how much is diffuse reflection
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - Met;

	float NdotL = max(dot(N, lightDir), 0.0);
	//vec3 Lo = (1.0 - shadow) * (kD * fColor/ PI + specular) * NdotL;//outgoign radiance ignoring attenuation cause my lightsource too fat away
	vec3 Lo = (kD * fColor/ PI + specular) * NdotL * radiance;//outgoign radiance ignoring attenuation cause my lightsource too fat away
	//vec3 Lo = (kD * fColor/ PI + specular) * NdotL;//outgoign radiance ignoring attenuation cause my lightsource too fat away
	vec3 color = clamp(Lo, 0.0, 1.0);
	//color = radiance;
	//color = color/ (color + vec3(1.0)); //tone mapping
	//color = pow(color, vec3(1.0/2.2));
	
	//float DiffPower = max(dot(fNorm, vec3(lightDir)), 0.0);
	//vec3 refLightDir = normalize(reflect(-lightDir, fNorm));
	//float SpecPower = pow(max(dot(refLightDir, cameraDir), 0.0), 32);

	//vec3 lighting = vec3(fAmbient) + (1.0 - shadow) * (vec3(DiffPower) + vec3(SpecPower));    
	//vec3 lighting = (vec3(DiffPower) + vec3(SpecPower));    
	//outColor = lighting * vec3(texture(texture_diffuse1, fTexCoord));
	outColor = color;
	outNorm = normalize(vNorm);
	outPos = vec3(vPos);
	//outColor = vec3(index) * lighting;	
}