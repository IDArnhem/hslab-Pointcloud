#pragma once

#include "ofMain.h"
#include "ofxRealsense.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    bool acquiring;

    ofxRealsense rscam;

    ofTexture color;
    ofTexture depth;
    ofTexture infra;
};
