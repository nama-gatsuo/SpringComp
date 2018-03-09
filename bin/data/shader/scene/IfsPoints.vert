#version 400
uniform mat4 modelViewMatrix; // oF Default
uniform mat4 modelViewProjectionMatrix; // oF Default
// uniform mat4 normalMatrix; // Pass from C++
in vec4 position; // oF Default
// in vec4 normal; // oF Default
in vec4 color; // oF Default
in vec2 texcoord; // oF Default

uniform float farClip;
uniform float nearClip;

uniform float w0;
uniform mat4 mat0;
uniform float w1;
uniform mat4 mat1;

out vec4 vPosition;
out float vDepth;
out vec3 vColor;

float PI = 3.1415926535897932384626433832795;

float random(inout vec2 randomVec2) {

    float r = fract(sin(dot(randomVec2, vec2(12.9898, 78.233))) * 43758.5453);
    randomVec2[0] = randomVec2[1];
    randomVec2[1] = r;

    return r;

}

void main() {

    vec4 p = vec4(position.xyz, 1.0);

    vec2 randomVec2 = vec2(p.x, p.y);
    float r;

    for (int i = 0; i < 15; i++) {
        r = random(randomVec2);

        if (r < w0) {
            p = mat0 * p;
        } else if (r < w0 + w1) {
            p = mat1 * p;
        }

    }

    vColor = color.rgb;
    p = 400. * p;

    vec4 viewPos = modelViewMatrix * vec4(p.xyz, 1.0);
    vDepth = - viewPos.z / (farClip - nearClip);
    vPosition = viewPos;
    vColor = vec3(1., 0.5, 0.9);

    gl_Position = modelViewProjectionMatrix * vec4(p.xyz, 1.0);
    gl_PointSize = .1 / vDepth;

}
