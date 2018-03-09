#pragma once
#include "ofMain.h"

class SmoothPoint : public ofPoint{
public:
    SmoothPoint():mSpeed(0.05){};
    void update(float dt){
        this->x += (mTarget.x - this->x) * mSpeed * dt;
        this->y += (mTarget.y - this->y) * mSpeed * dt;
        this->z += (mTarget.z - this->z) * mSpeed * dt;
    };
    void to(ofPoint target){
        mTarget = target;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    };
    bool isMoving(){
        float d2 = pow(mTarget.x - this->x, 2.) + pow(mTarget.y - this->y, 2.) + pow(mTarget.z - this->z, 2.);
        return sqrt(d2) > 100.;
    };
    
private:
    ofPoint mTarget;
    float mSpeed;
};

class SmoothValue {
public:
    SmoothValue():mSpeed(0.05),mValue(0),mTarget(0){}
    
    void update(float dt){
        mValue += (mTarget - mValue) * mSpeed * dt;
    };
    void to(float target){
        mTarget = target;
    };
    float get(){
        return mValue;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    };
    
private:
    float mValue;
    float mTarget;
    float mSpeed;
};

class PhysicValue {
public:
    PhysicValue():pos(0.),vel(0.),acce(0.),gravity(0.002){};
    void update(float dt){
        
        vel -= gravity;
        pos += vel * dt;
        if (vel > .3) {
            vel = .3;
        }
        
        if (pos < 0.) {
            vel = 0.;
            pos = 0.;
        } else if (pos > 1.) {
            pos = 0.99;
        }
        
    };
    void setGravity(float g) {gravity = g;};
    void addForce(float force){
        acce += force * 0.1;
        vel += acce;
        if (vel > 0.2) vel = 0.2;
        acce = 0.;
    };
    float get(){ return pos; };
private:
    float pos;
    float vel;
    float acce;
    float gravity;
};

class WireFrameShader : public ofShader {
public:
    void setup(){
        setGeometryInputType(GL_TRIANGLES);
        setGeometryOutputType(GL_TRIANGLE_STRIP);
        setGeometryOutputCount(3);
        load("shader/scene/wireframe.vert",
             "shader/scene/wireframe.frag",
             "shader/scene/wireframe.geom");
    };
    void begin(ofCamera &cam, float wireWidth, bool isDrawFace, bool isShadow){
        ofShader::begin();
        setUniform1f("nearClip", cam.getNearClip());
        setUniform1f("farClip", cam.getFarClip());
        setUniform1f("wireWidth", wireWidth);
        setUniform1i("isDrawFace", isDrawFace?1:0);
        setUniform1i("isShadow", isShadow?1:0);
    };
    
};

class MovingCam : public ofCamera {
public:
    MovingCam(){
        pos.setSpeed(0.01);
        up = ofVec3f(0,1,0);
        bang();
    };
    void update(float dt){
        
        pos.update(dt);
        look.update(dt);
        
        ofCamera::setPosition(pos);
        ofCamera::lookAt(look, up);
        
    };
    void top(){
        pos.to(ofPoint(0, 1200, 0));
        look.to(ofPoint(0., 0., 0.));
        up = ofVec3f(0,0,1.);
    };
    
    void bang(){
        
        if (!pos.isMoving()) {
            float coin = ofRandom(1.);
            if (coin < 0.7) {
                pos.to(ofPoint(ofRandom(-800, 800), ofRandom(50, 300), ofRandom(-800, 800)));
                look.to(ofPoint(ofRandom(-50, 50), ofRandom(10, 10), ofRandom(-50, 50)));
                up = ofVec3f(0,1,0);
            } else if (coin < 0.9) {
                // looking down on
                pos.to(ofPoint(ofRandom(-200, 200), ofRandom(800, 1000), ofRandom(-200, 200)));
                look.to(ofPoint(0., 0., 0.));
                up = ofVec3f(0,1,0);
            } else {
                // look from near center
                pos.to(ofPoint(ofRandom(-80, 80), ofRandom(10, 80), ofRandom(-80, 80)));
                look.to(ofPoint(ofRandom(-50, 50), ofRandom(-10, 10), ofRandom(-50, 50)));
                up = ofVec3f(0,1,0);
            }
        }
    };
    
private:
    SmoothPoint pos;
    SmoothPoint look;
    ofVec3f up;
};

struct PingPongBuffer {
public:
    void allocate( int _width, int _height){
        
        for(int i = 0; i < 2; i++){
            
            ofFbo::Settings s;
            
            s.width = _width;
            s.height = _height;
            s.internalformat = GL_RGB32F;
            s.numColorbuffers = 2;
            
            FBOs[i].allocate(s);
            FBOs[i].createAndAttachTexture(GL_RGBA32F, 1);
            FBOs[i].createAndAttachTexture(GL_RGBA32F, 2);
            
            FBOs[i].getTexture(0).setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            FBOs[i].getTexture(1).setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }
        
        clear();
    }
    
    void swap(){
        std::swap(src,dst);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    ofFbo *src = &FBOs[0];
    ofFbo *dst = &FBOs[1];
    
private:
    ofFbo FBOs[2];
};
