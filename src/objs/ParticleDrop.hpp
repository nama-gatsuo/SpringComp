#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class ParticleDrop : public ObjBase {
public:
    void setup(){
        
        shader.load("shader/scene/ParticleDrop.vert", "shader/scene/PointSprite.frag");
        
        mesh.setMode(OF_PRIMITIVE_POINTS);
        for (int i = 0; i < num; i++) {
            
            // y: random seed
            mesh.addVertex(ofVec3f(ofRandom(-1600., 1600.), ofRandom(1.), ofRandom(-1600., 1600.)));
            
        }
        col.to(ofPoint(1.0, 0.5, 0.9));
        rot.setSpeed(0.01);
        
    };
    void update(float dt){
        t += dt * 0.5;
        rot.update(dt);
        col.update(dt);
    };
    void draw(ofCamera& cam, bool isShadow){
        
        if (isShadow) return;
        
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
//        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        shader.begin();
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("time", t);
        shader.setUniform3f("rot", rot);
        shader.setUniform3f("col", col * 2.);
        
        mesh.draw();
        
        shader.end();
//        ofEnableDepthTest();
        ofDisableBlendMode();
    };
    void randomize(int i){
        
        if (i == 0) {
            if (!rot.isMoving()){
                if (ofRandom(1.) < 0.1) {
                    rot.to(ofPoint(PI, rot.y, rot.z));
                } else {
                    rot.to(ofPoint(0.));
                }
            }
        } else if (i == 1) {
            ofFloatColor c;
            c.setHsb(ofRandom(1.), 0.4, 1.);
            col.to(ofPoint(c.r, c.g, c.b));
        }
        
        
    };
private:
    
    int num = 300000;
    ofVboMesh mesh;
    ofShader shader;
    SmoothPoint rot;
    SmoothPoint col;
    
    float t = 0.;
    
};


