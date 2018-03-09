#pragma once
#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"
#include "ofxFFTLive.h"

class MassVis : public ObjBase {
public:
    void setup(){
        fftLive.setMirrorData(false);
        fftLive.setup();
        
        for (int i = 0; i < 2; i++) {
            soundTex[i].allocate(fftSize, historySize, GL_R16);
            soundTex[i].begin();
            ofClear(0);
            soundTex[i].end();
        }
        
        updateShader.load("shader/scene/MassVis/Update");
        
        mesh = ofMesh::box(1.0, 1.0, 1.0, 1, 1, 1);
        shader.load("shader/scene/MassVis/MassVis.vert", "shader/customShader.frag");
    };
    void update(float dt){
        
        fftLive.update();
        vector<float> val = fftLive.getFftRawData();
        
        soundTex[current].begin();
        ofClear(0);
        
        updateShader.begin();
        updateShader.setUniform1i("ffSize", fftSize);
        
        updateShader.setUniform1fv("rawFft", val.data(), fftSize);
        
        soundTex[1-current].draw(0,0);
        
        updateShader.end();
        soundTex[current].end();
        
        current = 1 - current;
        
    };
    void draw(ofCamera& cam, bool isShadow){
        shader.begin();
        shader.setUniformTexture("soundTex", soundTex[current].getTexture(), 1);
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        mesh.drawInstanced(OF_MESH_FILL, historySize * fftSize);
        shader.end();
    };
    
private:
    ofxFFTLive fftLive;
    
    const int historySize = 256;
    const int fftSize = 128;
    
    int current = 0;
    ofFbo soundTex[2];
    ofShader updateShader;
    
    ofShader shader;
    ofVboMesh mesh;
    
};

