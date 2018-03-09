#version 400
uniform mat4 modelViewMatrix; // oF Default
uniform mat4 modelViewProjectionMatrix; // oF Default
uniform mat4 normalMatrix; // Pass from C++
in vec4 position; // oF Default
in vec4 normal; // oF Default
in vec4 color; // oF Default
in vec2 texcoord; // oF Default

uniform float farClip;
uniform float nearClip;

uniform sampler2DRect soundTex;
uniform int historySize;
uniform int fftSize;

out vec4 vPosition;
out float vDepth;
out vec4 vColor;

void main(){
    vec4 p = position;

    vec2 size = textureSize(soundTex);

    float instanceX = mod(gl_InstanceID, size.x);
    float instanceY = floor(gl_InstanceID / size.x);

    float val = texture(soundTex, vec2(instanceX+0.5, instanceY+0.5)).r;
    vec2 boxSize = vec2(9., 10.);

    vec2 offset = vec2(0.);
    p.x *= boxSize.x;
    p.z *= boxSize.y;
    p.y *= boxSize.x;
    p.y += val * 120.0;

    p.x += (boxSize.x + offset.y) * (instanceY - size.y * 0.5);
    p.z += (boxSize.y + offset.x) * (instanceX - size.x * 0.5);

    gl_Position = modelViewProjectionMatrix * p;

    vec4 viewPos = modelViewMatrix * p;
    vDepth = - viewPos.z / (farClip - nearClip);
    vColor = vec4(vec3(0.2,0.28,0.3) * (0.7 + val * 3.2), 1.0);
    vPosition = viewPos;
}
