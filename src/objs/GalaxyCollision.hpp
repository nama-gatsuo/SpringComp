#include "ofMain.h"
#include "CommonUtil.hpp"
#include "ObjBase.hpp"

class GalaxyCollision : public ObjBase{
public:
    void setup(){
        mesh.setMode(OF_PRIMITIVE_POINTS);
        for (int h = 0; h < hSize; h++) {
            
            ofFloatColor c;
            if (h < hSize / 2) {
                c = ofFloatColor(0.24, 0.6, 0.8);
            } else {
                c = ofFloatColor(0.72, 0,3, 0.6);
            }
            
            for (int w = 0; w < wSize; w++) {
                mesh.addVertex(ofVec3f(0,0,0));
                mesh.addTexCoord(ofVec2f(w, h));
                mesh.addColor(c);
            }
        }
        
        createData.load("shader/passThru.vert", "shader/scene/GalaxyCollision/createData.frag");
        updateData.load("shader/passThru.vert", "shader/scene/GalaxyCollision/updateData.frag");
        updateRenderer.load("shader/scene/GalaxyCollision/render.vert", "shader/scene/PointSprite.frag");
        
        pp.allocate(wSize, hSize);
        
        randomize(0);
    };
    void update(float dt){
        dt *= 2.;
        float d = p1.distance(p0);
        if (d > 1.0) {
            a0 = (p1 - p0).normalize() * GM / d;
            a1 = - a0 * massRatio;
            
            v0 += a0 * dt;
            v1 += a1 * dt;
            
            p0 += v0 * dt;
            p1 += v1 * dt;
        }
        
        pp.dst->begin();
        ofClear(0);
        pp.dst->activateAllDrawBuffers();
        
        updateData.begin();
        updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
        updateData.setUniformTexture("velData", pp.src->getTexture(1), 1);
        updateData.setUniform1f("GM", GM);
        updateData.setUniform1f("dt", dt);
        updateData.setUniform1f("massRatio", massRatio);
        updateData.setUniform3f("p0", p0);
        updateData.setUniform3f("p1", p1);
        
        pp.src->draw(0,0);
        
        updateData.end();
        pp.dst->end();
        
        pp.swap();
        
    };
    void draw(ofCamera& cam, bool isShadow){
        
        if (isShadow) return;
        
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        updateRenderer.begin();
        updateRenderer.setUniform1f("farClip", cam.getFarClip());
        updateRenderer.setUniform1f("nearClip", cam.getNearClip());
        updateRenderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);
        
        mesh.draw();
        
        updateRenderer.end();
        ofEnableDepthTest();
        ofDisableBlendMode();
    };
    void randomize(int i){
        
        if (i == 0) {
            GM = ofRandom(1.0, 10.0);
            massRatio = ofRandom(0.3, 1.0);
            distance = ofRandom(20, 600);
            bulgeRad[0] = ofRandom(50, 300);
            diskRad[0] = ofRandom(200, 500);
            bulgeRad[1] = ofRandom(50, 300);
            diskRad[1] = ofRandom(200, 500);
            speed = sqrt(GM / 2.0);
            
            angleX[0] = ofRandom(-45, 45);
            angleX[1] = ofRandom(-45, 45);
            angleY[0] = ofRandom(-45, 45);
            angleY[1] = ofRandom(-45, 45);
            
            p0 = ofVec3f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)).normalize() * distance;
            v0 = ofVec3f(speed, 0.0, 0.0);
            p1 = (- massRatio) * p0;
            v1 = (- massRatio) * v0;
            
            // create initial value in GPU
            pp.dst->begin();
            ofClear(0);
            pp.dst->activateAllDrawBuffers();
            
            createData.begin();
            createData.setUniform1f("GM", GM);
            updateData.setUniform2f("res", 1.0 * wSize, 1.0 * hSize);
            createData.setUniform1f("massRatio", massRatio);
            createData.setUniform3f("p0", p0.x, p0.y, p0.z);
            createData.setUniform3f("p1", p1.x, p1.y, p1.z);
            createData.setUniform3f("v0", v0.x, v0.y, v0.z);
            createData.setUniform3f("v1", v1.x, v1.y, v1.z);
            createData.setUniform1fv("bulgeRad", bulgeRad, 2);
            createData.setUniform1fv("diskRad", diskRad, 2);
            createData.setUniform1fv("angleX", angleX, 2);
            createData.setUniform1fv("angleY", angleY, 2);
            
            pp.src->draw(0,0);
            
            createData.end();
            pp.dst->end();
            
            pp.swap();
        
        } else if (i == 1) {
            
            
        }
        
    };
    
    
private:
    
    ofVboMesh mesh;
    ofShader updateData;
    ofShader updateRenderer;
    ofShader createData;
    
    int wSize = 256, hSize = 512;
    PingPongBuffer pp;
    
    ofVec3f p0, p1, v0, v1, a0, a1;
    
    float GM;
    float diskRad[2];
    float bulgeRad[2];
    float ratio[2];
    
    float distance = 80.0;
    float massRatio = 0.07;
    float speed = sqrt(GM / 2.0);
    
    float angleX[2] = { 20.0, - 45.0 };
    float angleY[2] = { 10.0, 0.0 };
};


