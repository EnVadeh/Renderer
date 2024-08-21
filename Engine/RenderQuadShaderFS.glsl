#version 430 core

uniform sampler2D RT;

in vec2 fTex;
out vec4 outColor;

void main(){
	vec2 uv = fTex;
	uv.y = 1 - uv.y;
	outColor = texture(RT, uv);
	
	//outColor = vec4(fTexCoord.x, fTexCoord.y, 1.0, 1.0);
	//outColor = texture(texture_diffuse1, fTexCoord) * (DiffPower + SpecPower) * fAmbient;
	//outColor = vec4(DiffPower);
	//outColor = vec4(1.0,1.0, 1.0, 1.0);
}