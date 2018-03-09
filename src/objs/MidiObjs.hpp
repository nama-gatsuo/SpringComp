#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class NoteObj {
public:
    NoteObj(int midi, int velocity): midi(midi), velocity(velocity){
        
        mesh = ofMesh::icosphere(1, 0);
        ofMatrix4x4 m;
        float s = ((float)velocity / 128. + 1.) * 8.;
        m.glScale(s, s, s);
        
        ofFloatColor c;
        c.setHsb(0.5 + (float)midi / 256., 0.8, 1.);
        float colFactor = ((float)velocity / 128. + 1.) * 0.8;
        c.r *= colFactor;
        c.g *= colFactor;
        c.b *= colFactor;
        
        for (int i = 0; i < mesh.getNumVertices(); i++) {
            ofVec3f v = mesh.getVertex(i);
            
            mesh.setVertex(i, v * m);
            mesh.addColor(c);
        }
        
        pos = ofVec3f(-1000, 100. + (float)velocity, (midi - 64) * 20);
        q.makeRotate(ofRandom(TWO_PI), ofVec3f(ofRandom(1.), ofRandom(1.), ofRandom(1.)).getNormalized());
    };
    void update(float dt){
        life -= dt;
        pos.x += dt * 5.5;
        
    };
    
    bool isDead(){ return life < 0.; };
    
    void draw() {
        float a, x, y, z;
        q.getRotate(a, x, y, z);
        
        ofPushMatrix();
        
        ofTranslate(pos.x, pos.y, pos.z);
        ofRotate(a, x, y, z);
        
        mesh.draw();
        
        ofPopMatrix();
    };
    
private:
    int midi, velocity;
    float life = 400.;
    ofVboMesh mesh;
    ofVec3f pos;
    ofQuaternion q;
    
};

class MidiObjs : public ObjBase{
public:
    MidiObjs(){};
    void setup(){
        shader.load("shader/gbuffer.vert", "shader/customShader.frag");
    };
    void update(float dt){
        vector<NoteObj>::iterator it;
        for (it = notes.begin(); it < notes.end(); it++) {
            if (it->isDead()) {
                notes.erase(it);
            } else {
                it->update(dt);
            }
        }
    };
    void add(int midi, int velocity){
        notes.push_back(NoteObj(midi, velocity));
    };
    
    void draw(ofCamera& cam, bool isShadow){
        
        shader.begin();
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniform1f("farClip",  cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        
        vector<NoteObj>::iterator it;
        for (it = notes.begin(); it < notes.end(); it++) {
            it->draw();
        }
        shader.end();
        
    };
    void randomize(int i){};

private:
    ofShader shader;
    vector<NoteObj> notes;
};
