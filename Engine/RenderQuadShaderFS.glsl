#version 430 core

  
in vec2 fTex;

struct Materials{
	vec4 Albedo;
	float Mettalic;
	float Roughness;
};

layout(std430, binding = 0) buffer SSBO {   
	Materials Mat[];
} SS;

uniform sampler2D colorRT;
uniform sampler2D normalRT;
uniform sampler2D shadowRT;

out vec4 fragColor;

float sobelColor(vec2 uv)
{
    float offX = 1.0 / 1000;
    float offY = 1.0 / 1000;
    
    float s00 = texture(colorRT, uv + vec2(-offX, -offY)).a;
    float s01 = texture(colorRT, uv + vec2( 0.0,  -offY)).a;
    float s02 = texture(colorRT, uv + vec2( offX, -offY)).a;
    float s10 = texture(colorRT, uv + vec2(-offX,  0.0)).a;
    float s12 = texture(colorRT, uv + vec2( offX,  0.0)).a;
    float s20 = texture(colorRT, uv + vec2(-offX,  offY)).a;
    float s21 = texture(colorRT, uv + vec2( 0.0,   offY)).a;
    float s22 = texture(colorRT, uv + vec2( offX,  offY)).a;
    
    //sobel matrices operators
    float sobelX = s00 + 2.0 * s10 + s20 - s02 - 2.0 * s12 - s22;
    float sobelY = s00 + 2.0 * s01 + s02 - s20 - 2.0 * s21 - s22;
    float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
    
    //if current edgeValue > edge threshold then it's edge so it returns 1 - 1(result of the boolean expression)
    return 1.0 - float(edgeSqr > 0.07 * 0.07);
}

float sobelNormal(vec2 uv)
{
    float offX = 1.0 / 1000;
    float offY = 1.0 / 1000;
    float threshold = 0.1;

    vec3 c11 = texture(normalRT, uv).xyz;
    
    float s00 = max(0.0, dot(c11, texture(normalRT, uv + vec2(-offX, -offY)).xyz) - threshold);
    float s01 = max(0.0, dot(c11, texture(normalRT, uv + vec2( 0.0,  -offY)).xyz) - threshold);
    float s02 = max(0.0, dot(c11, texture(normalRT, uv + vec2( offX, -offY)).xyz) - threshold);
    float s10 = max(0.0, dot(c11, texture(normalRT, uv + vec2(-offX,  0.0)).xyz) - threshold);
    float s12 = max(0.0, dot(c11, texture(normalRT, uv + vec2( offX,  0.0)).xyz) - threshold);
    float s20 = max(0.0, dot(c11, texture(normalRT, uv + vec2(-offX,  offY)).xyz) - threshold);
    float s21 = max(0.0, dot(c11, texture(normalRT, uv + vec2( 0.0,   offY)).xyz) - threshold);
    float s22 = max(0.0, dot(c11, texture(normalRT, uv + vec2( offX,  offY)).xyz) - threshold);
    
    float sobelX = s00 + 2.0 * s10 + s20 - s02 - 2.0 * s12 - s22;
    float sobelY = s00 + 2.0 * s01 + s02 - s20 - 2.0 * s21 - s22;
    float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
    
    
    return 1.0 - float(edgeSqr > 0.5 * 0.5);
}


void main()
{             
    float sbl = sobelNormal(fTex) * sobelColor(fTex);
    if (sbl == 1.0)
	    fragColor = texture(colorRT, fTex);
    else 
	    fragColor = vec4(vec3(sbl, sbl, 0), 1.0);

    //FragColor = texture(colorRT, fTex);
    //FragColor = texture(shadowRT, fTex);
} 