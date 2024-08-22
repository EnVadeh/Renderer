#version 430 core


out vec4 FragColor;
  
in vec2 fTex;

uniform sampler2D shadowRT;

void main()
{             
    float depthValue = texture(shadowRT, fTex).r;
    FragColor = vec4(vec3(depthValue), 1.0);
    //FragColor = texture(shadowRT, fTex);
} 