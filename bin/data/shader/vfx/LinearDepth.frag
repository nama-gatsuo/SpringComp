#version 400

uniform float linearDepthScalar;

in float vDepth;
out float outputDepth;

void main(){
    outputDepth = abs(vDepth);
}
