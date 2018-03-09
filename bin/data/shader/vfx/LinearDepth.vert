#version 400
uniform mat4 modelViewProjectionMatrix; // oF Default
uniform mat4 modelViewMatrix; // oF Default
in vec4 position; // oF Default

out float vDepth;

void main(){
    gl_Position = modelViewProjectionMatrix * position;
    vDepth = (modelViewMatrix * position).z;
}
