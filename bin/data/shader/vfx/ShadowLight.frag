#version 400
#define precision highp;

uniform sampler2DRect tex;
uniform sampler2DRect lightDepthTex;
uniform sampler2DRect positionTex;
uniform sampler2DRect normalAndDepthTex;

uniform float linearDepthScalar;
uniform mat4 shadowTransMat;
uniform vec3 lightPosInViewSpace;

uniform vec4 ambient;
uniform vec4 diffuse;
// uniform vec4 specular;
uniform float darkness;
uniform float blend;

in vec2 vTexCoord;
out vec4 outputColor;

void main() {

    vec4 read = texture(tex, vTexCoord);
    vec4 posInViewSpace = texture(positionTex, vTexCoord); // in view space
    vec3 normal = texture(normalAndDepthTex, vTexCoord).rgb; // in view space

    vec3 lightDir = normalize(lightPosInViewSpace.xyz - posInViewSpace.xyz);
    float lambert = max(dot(normal, lightDir), 0.0);
    vec4 amb = vec4(vec3(0.0), 1.0), dif = vec4(vec3(0.0), 1.0);

    if (lambert > 0.0) {
        amb += ambient;
        dif += diffuse * lambert;
        // spe += specular * pow(max(dot(R,V), 0.0), 64);
    }

    vec4 shadowCoord = shadowTransMat * posInViewSpace;
    float shadow = 1.0;

    vec2 res = textureSize(lightDepthTex);
    float texel = texture(lightDepthTex, shadowCoord.xy * res.xy).r;

    if (texel < shadowCoord.z - 0.005) shadow = 1.0 - darkness;

    if (shadowCoord.x >= 1.0 || shadowCoord.y >= 1.0) shadow = 1.0;
    if (shadowCoord.x <= 0.0 || shadowCoord.y <= 0.0) shadow = 1.0;

    outputColor = mix(amb + dif, read, blend) * shadow;
    outputColor.a = 1.0;
}
