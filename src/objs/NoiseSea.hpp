#include "ofMain.h"
#include "ObjBase.hpp"
#include "CommonUtil.hpp"

class NoiseSea : public ObjBase {
public:
    void setup(){
        shader.setGeometryInputType(GL_TRIANGLES);
        shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        shader.setGeometryOutputCount(3);
        shader.load("shader/scene/NoiseSea.vert",
                    "shader/scene/NoiseSea.frag",
                    "shader/scene/NoiseSea.geom");
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        ofMatrix4x4 m = ofMatrix4x4();
        m.glRotate(90, 1, 0, 0);
        
        int step = 60;
        float y = - 200;
        
        for (int w = -1500; w < 1500; w += step) {
            for (int h = -1500; h < 1500; h += step) {
                ofVec3f v, n;
                n = ofVec3f(0, 1, 0);
                
                v = ofVec3f(w, y, h);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
                v = ofVec3f(w+step, y, h);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
                v = ofVec3f(w+step, y, h+step);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
                v = ofVec3f(w+step, y, h+step);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
                v = ofVec3f(w, y, h+step);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
                v = ofVec3f(w, y, h);
                mesh.addVertex(v);
                mesh.addNormal(n);
                mesh.addColor(ofFloatColor(1.));
                
            }
        }
        
        randomize(0);
        randomize(1);
    };
    void update(float dt){
        
        heightFactor.update(dt);
        colFactor.update(dt);
        noiseFactor.update(dt);
        wireWidth.update(dt);
        time += 0.03 * dt;
        
    };
    void draw(ofCamera& cam, bool isShadow){
        shader.begin();
        shader.setUniform1i("isShadow", isShadow?1:0);
        shader.setUniform1f("farClip", cam.getFarClip());
        shader.setUniform1f("nearClip", cam.getNearClip());
        shader.setUniform1f("colFactor", colFactor.get());
        shader.setUniform1f("heightFactor", heightFactor.get());
        shader.setUniform1f("noiseFactor", noiseFactor.get());
        shader.setUniform1f("wireWidth", 1.05 - wireWidth.get());
        shader.setUniform1f("time", time);
        
        mesh.draw(OF_MESH_FILL);
        shader.end();
    };
    void randomize(int i){
        if (i == 0) {
            heightFactor.addForce(0.6);
            noiseFactor.to(ofRandom(0.01, 0.05));
            
            if (ofRandom(1.) < 0.4) {
                noiseFactor.to(ofRandom(0.01, 0.05));
            }
            if (ofRandom(1.) < 0.2) {
                wireWidth.addForce(ofRandom(0.6, 0.85));
            }
        } else if (i == 1) {
            colFactor.addForce(0.5);
        }
    };
    
private:
    ofVboMesh mesh;
    ofShader shader;
    float time;
    
    PhysicValue colFactor;
    PhysicValue heightFactor;
    SmoothValue noiseFactor;
    PhysicValue wireWidth;
    
};
