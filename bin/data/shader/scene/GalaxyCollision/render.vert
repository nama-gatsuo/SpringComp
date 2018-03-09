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

out vec3 vColor;
out vec4 vPosition;
out float vDepth;

uniform sampler2DRect posData;

void main() {

    vec3 p = texture(posData, texcoord).xyz;

    vec4 viewPos = modelViewMatrix * vec4(p, 1.0);
    vDepth = - viewPos.z / (farClip - nearClip);
    vPosition = viewPos;
    vColor = color.rgb;

    gl_Position = modelViewProjectionMatrix * vec4(p, 1.0);
    gl_PointSize = .1 / vDepth;
}
