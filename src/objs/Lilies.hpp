#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class Lily {
public:
    Lily(ofVec3f pos){
        
        float s = 3 + ofRandom(1);
        
        col.setHsb(ofRandom(0.9, 1.), ofRandom(0.2, 0.5), 1.);
        
        ofMatrix4x4 m;
        m.glTranslate(pos.x, pos.y, pos.z);
        m.glScale(s, s, s);
        m.glRotate(ofRandom(30), 1,0,0);
        m.glRotate(ofRandom(360), 0,1,0);
        createRing(m);
        
        center = ofMesh::sphere(1., 3);
        
        m.glTranslate(0, 10, 0);
        
        for (int i = 0; i < center.getNumVertices(); i++){
            center.setVertex(i, center.getVertex(i) * m);
            center.addColor(ofFloatColor(col.r * 2., col.g * 2., col.b * 2.));
        }
        
        lev.setGravity(0.001);
    };
    
    void update(float dt){
        
        lev.update(dt);
        
        if (lev.get() > 0.) {
            float t1 = lev.get();
            float t0 = 1 - t1;
            
            for (int i = 0; i < mesh.getNumVertices(); i++) {
                mesh.setVertex(i, v0[i] * t0 + v1[i] * t1);
            }
            
        }
        
    };
    
    void draw(){
        
        mesh.draw();
        center.draw();
    };
    
    void randomize(float f){
        
        lev.addForce(f);
        
    };
    
    void createPetal(ofMatrix4x4 m){
        
        float hSpace = 2;
        float radX = 0.14;
        float radY = -0.1;
        
        ofVec3f tv0[17] = {
            ofVec3f(0,  0, 0),
            
            ofVec3f(2,  hSpace*1, -0.5).rotateRad(PI*0.3 - radX, radY, 0),
            ofVec3f(0,  hSpace*1, 0).rotateRad(PI*0.3 - radX, radY, 0),
            ofVec3f(-2, hSpace*1, -0.5).rotateRad(PI*0.3 - radX, radY, 0),
            
            ofVec3f(3.2,hSpace*2, -0.4).rotateRad(PI*0.3 - radX*2, radY*2, 0),
            ofVec3f(0,  hSpace*2, 0).rotateRad(PI*0.3 - radX*2, radY*2, 0),
            ofVec3f(-3.2,hSpace*2, -0.4).rotateRad(PI*0.3 - radX*2, radY*2, 0),
            
            ofVec3f(4,  hSpace*3, -0.3).rotateRad(PI*0.3 - radX*3, radY*3, 0),
            ofVec3f(0,  hSpace*3, 0).rotateRad(PI*0.3 - radX*3, radY*3, 0),
            ofVec3f(-4, hSpace*3, -0.3).rotateRad(PI*0.3 - radX*3, radY*3, 0),
            
            ofVec3f(3.2,hSpace*4, -0.2).rotateRad(PI*0.3 - radX*4, radY*4, 0),
            ofVec3f(0,  hSpace*4, 0).rotateRad(PI*0.3 - radX*4, radY*4, 0),
            ofVec3f(-3.2,hSpace*4, -0.2).rotateRad(PI*0.3 - radX*4, radY*4, 0),
            
            ofVec3f(2,  hSpace*5, -0.1).rotateRad(PI*0.3 - radX*5, radY*5, 0),
            ofVec3f(0,  hSpace*5, 0).rotateRad(PI*0.3 - radX*5, radY*5, 0),
            ofVec3f(-2, hSpace*5, -0.1).rotateRad(PI*0.3 - radX*5, radY*5, 0),
            
            ofVec3f(0,  hSpace*6, 0).rotateRad(PI*0.3 - radX*6, radY*6, 0)
        };
        for (int i = 0; i < 17; i++) {
            v0.push_back(tv0[i] * m);
        }
        
        radX = 0.13 + ofRandom(0.1);
        radY = 0.1;
        hSpace = 3;
        ofVec3f tv1[17] = {
            ofVec3f(0,  0, 0),
            
            ofVec3f(2,  hSpace*1, -0.5).rotateRad(radX, radY, 0),
            ofVec3f(0,  hSpace*1, 0).rotateRad(radX, radY, 0),
            ofVec3f(-2, hSpace*1, -0.5).rotateRad(radX, radY, 0),
            
            ofVec3f(3.2,hSpace*2, -0.4).rotateRad(radX*2, radY*2, 0),
            ofVec3f(0,  hSpace*2, 0).rotateRad(radX*2, radY*2, 0),
            ofVec3f(-3.2,hSpace*2, -0.4).rotateRad(radX*2, radY*2, 0),
            
            ofVec3f(4,  hSpace*3, -0.3).rotateRad(radX*3, radY*3, 0),
            ofVec3f(0,  hSpace*3, 0).rotateRad(radX*3, radY*3, 0),
            ofVec3f(-4, hSpace*3, -0.3).rotateRad(radX*3, radY*3, 0),
            
            ofVec3f(3.2,hSpace*4, -0.2).rotateRad(radX*4, radY*4, 0),
            ofVec3f(0,  hSpace*4, 0).rotateRad(radX*4, radY*4, 0),
            ofVec3f(-3.2,hSpace*4, -0.2).rotateRad(radX*4, radY*4, 0),
            
            ofVec3f(2,  hSpace*5, -0.1).rotateRad(radX*5, radY*5, 0),
            ofVec3f(0,  hSpace*5, 0).rotateRad(radX*5, radY*5, 0),
            ofVec3f(-2, hSpace*5, -0.1).rotateRad(radX*5, radY*5, 0),
            
            ofVec3f(0,  hSpace*6, 0).rotateRad(radX*6, radY*6, 0)
        };
        for (int i = 0; i < 17; i++) {
            v1.push_back(tv1[i] * m);
        }
        
        int startNum = mesh.getNumVertices();
        
        for (int i = 0; i < 17; i++) {
            mesh.addVertex(tv0[i] * m);
            mesh.addColor(col);
        }
        
        for (int i = 0; i < 60; i++) {
            mesh.addIndex(ofIndexType(iArray[i] + startNum));
        }
        
    };
    
    void createRing(ofMatrix4x4 m) {
        
        ofMatrix4x4 _m = m;
        for (int i = 0; i < 6; i++) {
            
            _m.glRotate(60, 0, 1, 0);
            createPetal(_m);
            
        }
        
    }
    
protected:
    ofVboMesh mesh;
    ofVboMesh center;
    ofShader shader;
    
    vector<ofVec3f> v0;
    vector<ofVec3f> v1;
    ofFloatColor col;
    
    bool isInterp = false;
    PhysicValue lev;
    
    int iArray[60] = {
        0,1,2,    0,2,3,
        1,4,2,    2,4,5,    2, 5,6,   2,6,3,
        4,7,5,    5,7,8,    5, 8,9,   5,9,6,
        7,10,8,   8,10,11,  8, 11,12, 8,12,9,
        10,13,11, 11,13,14, 11,14,15, 11,15,12,
        13,16,14, 14,16,15
    };
    
};

class LilySystem : public ObjBase {
public:
    LilySystem(){};
    
    void setup(){
        shader.load("shader/gbuffer.vert", "shader/customShader.frag");
        
        for (int i = 0; i < 200; i++) {
            
            float x = ofRandom(-800, 800);
            float z = ofRandom(-800, 800);
            float d = sqrt(x*x + z*z) * 0.4;
            float y = - d;
            
            ls.push_back(Lily(ofVec3f(x,y,z)));
            
        }
        
        plane = ofMesh::plane(3000,3000, 30,30);
        for (int i = 0; i < plane.getNumVertices(); i++) {
            
            ofVec3f v = plane.getVertex(i);
            v.rotate(-90, ofVec3f(1,0,0));
            
            float d = v.length() * 0.4;
            v.y = - d;
            
            plane.setVertex(i, v);
            plane.addColor(ofFloatColor(2. / d + 0.1));
        }
        
    };
    
    void update(float dt){
        
        vector<Lily>::iterator it;
        for (it = ls.begin(); it < ls.end(); it++) {
            it->update(dt);
        }
        
    };
    
    void draw(ofCamera& cam, bool isShadow){
        
        shader.begin();
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniform1f("farClip",  cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        
        vector<Lily>::iterator it;
        for (it = ls.begin(); it < ls.end(); it++) {
            it->draw();
        }
        
        plane.draw();
        
        shader.end();
        
    };
    
    void randomize(int i){
        
        if (i == 0) {
            vector<Lily>::iterator it;
            for (it = ls.begin(); it < ls.end(); it++) {
                it->randomize(ofRandom(0.3));
            }
        }
        
    };
    
    
protected:
    ofShader shader;
    ofVboMesh plane;
    vector<Lily> ls;
};



