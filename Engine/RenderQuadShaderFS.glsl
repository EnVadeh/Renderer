#version 430 core
  
in vec2 fTex;
in mat4 mProj;

layout(std430, binding = 1) buffer ssaoKernelBuffer {   
	vec4 samples[];
} ssaoK;

const vec2 noiseScale = vec2(1000.0/4.0, 1000.0/4.0);

uniform sampler2D colorRT;
uniform sampler2D normalRT;
uniform sampler2D depthRT;
uniform sampler2D noiseT;

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
    
    
    return 1.0 - float(edgeSqr > 0.75 * 0.75);
}


void main()
{             
    float sbl = sobelNormal(fTex) * sobelColor(fTex);
    if (sbl == 1.0)
	    fragColor = texture(colorRT, fTex);
    else 
	    fragColor = vec4(vec3(sbl, sbl, 0), 1.0);
/*    vec3 fragPos   = texture(depthRT, fTex).xyz;
    vec3 normal    = texture(normalRT, fTex).rgb;
    vec3 randomVec = texture(noiseT, fTex * noiseScale).xyz; 
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < 64; ++i) {
    // get sample position:
    vec3 s = TBN* vec3(ssaoK.samples[i]);
        s = s * 0.5 + fragPos;
        // project sample position:
        vec4 offset = vec4(s, 1.0);
        offset = mProj * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;
  
        //get sample depth:
        float sampleDepth = texture(depthRT, offset.xy).z;
  
        // range check & accumulate:
        float rangeCheck = smoothstep(0.0, 1.0, 0.5 / abs(fragPos.z - sampleDepth));
        occlusion       += (sampleDepth >= s.z + 0.07 ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / 64.0);
    fragColor = vec4(occlusion);*/
    //fragColor = texture(depthRT, fTex);
    //FragColor = texture(colorRT, fTex);
} 