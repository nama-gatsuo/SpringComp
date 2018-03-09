#include "ofMain.h"
#include "CommonUtil.hpp"
#include "ObjBase.hpp"

class AffineParams {
public:
    AffineParams():weight(0.5){
        s.to(ofPoint(1.0, 1.0, 1.0));
        r.setSpeed(0.02);
        s.setSpeed(0.02);
        t.setSpeed(0.02);
    };
    void update(float dt){
        
        r.update(dt);
        s.update(dt);
        t.update(dt);
        
        mat = ofMatrix4x4();
        mat.glTranslate(t.x, t.y, t.z);
        mat.glScale(s.x, s.y, s.z);
        mat.glRotate(r.x, 1.0, 0.0, 0.0);
        mat.glRotate(r.y, 0.0, 1.0, 0.0);
        mat.glRotate(r.z, 1.0, 0.0, 1.0);
    };
    
    float weight;
    SmoothPoint r;
    SmoothPoint s;
    SmoothPoint t;
    ofMatrix4x4 mat;
};

class IfsPoints : public ObjBase {
public:
    void setup(){
        shader.load("shader/scene/IfsPoints.vert", "shader/scene/PointSprite.frag");
        
        shader.begin();
        shader.setUniform1f("scale", 200.);
        shader.end();
        
        mesh.setMode(OF_PRIMITIVE_POINTS);
        for (int i = 0; i < num; i++) {
            mesh.addVertex(ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0)));
            mesh.addColor(ofFloatColor(0.13, 0.34, 0.43 + ofRandom(0.3)));
        }
    };
    void update(float dt){
        ap[0].update(dt);
        ap[1].update(dt);
    };
    void draw(ofCamera& cam, bool isShadow){
        
        if (isShadow) return;
        
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofDisableDepthTest();
        
        shader.begin();
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("w0", ap[0].weight);
        shader.setUniformMatrix4f("mat0", ap[0].mat);
        shader.setUniform1f("w1", ap[1].weight);
        shader.setUniformMatrix4f("mat1", ap[1].mat);
        
        mesh.draw();
        
        shader.end();
        ofEnableDepthTest();
        ofDisableBlendMode();
    };
    void randomize(int i){
        
        float coin = ofRandom(1.);
        
        if (coin < 0.5) {
            ap[1].r.to(ofPoint(ofRandom(180.0), ofRandom(180.0), ofRandom(180.0)));
            ap[1].s.to(ofPoint(ofRandom(0.3, 1.1), ofRandom(0.3, 1.1), ofRandom(0.3, 1.1)));
            ap[1].t.to(ofPoint(ofRandom(-0.4, 0.4), ofRandom(-0.4, 0.4), ofRandom(-0.4, 0.4)));
            
            
        } else {
            ap[0].s.to(ofPoint(ofRandom(1.0)+0.3, ofRandom(1.0)+0.3, ofRandom(1.0)+0.3));
            ap[0].t.to(ofPoint(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5)));
            ap[0].t.to(ofPoint(ofRandom(-0.4, 0.4), ofRandom(-0.4, 0.4), ofRandom(-0.4, 0.4)));
        }
        
        
    };
private:
    ofShader shader;
    ofVboMesh mesh;
    int num = 200000;
    
    AffineParams ap[2];
    
};
