#include "PostEffect.hpp"

void PostEffect::setup(){
    
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGB32F;
    
    base.allocate(s);
    
    composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
    
    complexConv.load("shader/passThru.vert", "shader/pfx/ComplexConv.frag");
    complexConv.begin();
    complexConv.setUniform2f("res", ofGetWidth(), ofGetHeight());
    complexConv.end();
    
    mirrorConv.load("shader/passThru.vert", "shader/pfx/MirrorConv.frag");
    mirrorConv.begin();
    mirrorConv.setUniform2f("res", ofGetWidth(), ofGetHeight());
    mirrorConv.end();
    
    colorConv.load("shader/passThru.vert", "shader/pfx/ColorConv.frag");
}

void PostEffect::begin(){
    
    base.begin();
    ofClear(0,0);
    
}
void PostEffect::end(){
    
    base.end();
    
    composite.begin();
    ofClear(0,0);
    ofSetColor(255);
    
    switch (mode) {
        case 0: break;
        case 1: {
            mirrorConv.begin();
            mirrorConv.setUniform1i("mode", 0); // horizontal
            break;
        }
        case 2: {
            mirrorConv.begin();
            mirrorConv.setUniform1i("mode", 1); // vertical
            break;
        }
        case 3: {
            complexConv.begin();
            complexConv.setUniform1f("s1", params[0]);
            complexConv.setUniform1f("s2", params[1]);
            break;
        }
        default: break;
    }
    
    base.draw(0,0);
    
    switch (mode) {
        case 0: break;
        case 1: mirrorConv.end(); break;
        case 2: mirrorConv.end(); break;
        case 3: complexConv.end(); break;
        default: break;
    }
    composite.end();
    
    
}
void PostEffect::draw(){
    
    ofSetColor(255);
    
    if (isColor) {
        colorConv.begin();
        colorConv.setUniform1i("isNega", isNega?1:0);
        colorConv.setUniform4f("theme", theme);
    }
    
    composite.draw(0,0);
    
    if (isColor) {
        colorConv.end();
    }
    
}

void PostEffect::bang() {
    
    float coin = ofRandom(1.0);
    
    if (coin < 0.1) isNega = true;
    else isNega = false;
    
    coin = ofRandom(1.0);
    
    if (coin < 0.4) mode = 0;
    else if (coin < 0.6) mode = 1;
    else if (coin < 0.8) mode = 2;
    else mode = 3;
    
}

void PostEffect::setMode(int mode) { this->mode = mode; }
void PostEffect::setParam(int ch, float val) { params[ch] = val; }

void PostEffect::enableColor(ofFloatColor col, bool isNega) {
    isColor = true;
    theme = col;
    this->isNega = isNega;
}

void PostEffect::disableColor() {
    isColor = false;
}


