#version 430 core

layout (vertices = 4) out;

in vec4 vPos[];
in vec2 fTexCoord[];
in vec3 fNorm[];

out vec4 tcPosition[];
out vec2 tcTexCoord[];
out vec3 tcNormal[];


void main()
{
    tcPosition[gl_InvocationID] = vPos[gl_InvocationID];
    tcTexCoord[gl_InvocationID] = fTexCoord[gl_InvocationID];
    tcNormal[gl_InvocationID] = fNorm[gl_InvocationID];
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 2;
        gl_TessLevelOuter[1] = 2;
        gl_TessLevelOuter[2] = 2;
        gl_TessLevelOuter[3] = 2;
        gl_TessLevelInner[0] = 2;
        gl_TessLevelInner[1] = 2;
    }
}