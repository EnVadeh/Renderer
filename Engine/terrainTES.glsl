#version 430 core

uniform mat4 matModel;
uniform mat4 matProjView;
uniform mat4 sunMatProjView;
uniform mat4 matSunOrthoView;
uniform mat4 matView;
uniform unsigned int iTime;

layout (quads, equal_spacing, ccw) in;

in vec4 tcPosition[];
in vec2 tcTexCoord[];
in vec3 tcNormal[];

out float wH;
out vec4 vPos; 
out vec2 fTexCoord;
out vec3 fNorm;
out vec4 sPos;


float wave1(vec2 pos){
    float amplitude = 0.5;
    float frequency = 0.5;
    unsigned int tempTime = iTime/5;
    float phase = pos.x*frequency - tempTime * frequency;
    float temp_x = pos.x - amplitude * sin(pos.x * frequency - tempTime*sqrt(frequency));
    float temp_z = pos.y - amplitude * sin(pos.y * frequency - tempTime*sqrt(frequency));
    return amplitude * (cos(temp_x * frequency - tempTime * sqrt(frequency)) + cos(temp_z * frequency - tempTime * sqrt(frequency)))/2; 
}

float wave2(vec2 pos){
    float amplitude = 2.0;
    float frequency = 0.125;
    unsigned int tempTime = iTime/5 + 20;
    float phase = pos.x*frequency - tempTime * frequency;
    float temp_x = pos.x - amplitude * sin(pos.x * frequency - tempTime*sqrt(frequency));
    float temp_z = pos.y - amplitude * sin(pos.y * frequency - tempTime*sqrt(frequency));
    return amplitude * (cos(temp_x * frequency - tempTime * sqrt(frequency)) + cos(temp_z * frequency - tempTime * sqrt(frequency)))/2;
}

float waveHeight(vec2 pos){
    return wave1(pos) + wave2(pos);
}

vec3 calculateNormal(vec2 pos) {
    float epsilon = 0.03;
    
    float height = waveHeight(pos);
    float heightPosX = waveHeight(pos + vec2(epsilon, 0)); //height sampled right next to our current vertex on +x axis
    float heightNegX = waveHeight(pos + vec2(-epsilon, 0)); //height sampled right next to our current vertex on -x axis
    float heightPosZ = waveHeight(pos + vec2(0, epsilon)); //height sampled right next to our current vertex on +z axis
    float heightNegZ = waveHeight(pos + vec2(0, -epsilon)); //height sampled right next to our current vertex on -z axis
    
    float dydx = (heightPosX - heightNegX) / (2.0 * epsilon);
    float dydz = (heightPosZ - heightNegZ) / (2.0 * epsilon);
    
    // Construct the normal vector
    vec3 normal = vec3(-dydx, 1.0, -dydz);
    return normalize(normal);
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 pos1 = mix(tcPosition[0], tcPosition[1], u);
    vec4 pos2 = mix(tcPosition[3], tcPosition[2], u);
    vPos = mix(pos1, pos2, v);
    wH = waveHeight(vPos.xz);
    vPos.y += wH;
    vec2 tex1 = mix(tcTexCoord[0], tcTexCoord[1], u);
    vec2 tex2 = mix(tcTexCoord[3], tcTexCoord[2], u);
    fTexCoord = mix(tex1, tex2, v);

    //fNorm = calculateNormal(vPos.xz);
    fNorm = vec3(0, 1, 0);
    //because waves happening, need some shit

    sPos = matSunOrthoView * matModel * vPos; //for orthogonal shadow
    gl_Position = matProjView * matModel * vPos;
}