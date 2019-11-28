#pragma once

#include "ofMain.h"
//#include "ofxAsio.h"
//#include <ofxRealSense2.h>
#include <librealsense2/rs.hpp>

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

        void acquirePointCloud();

    private:
        rs2::pipeline pipe;
        rs2::device device;

        rs2::points points;
        rs2::pointcloud pc;

        bool acquiring;

        ofVboMesh mesh;
        ofEasyCam cam;

//        std::shared_ptr<ofxAsio::UdpSender> mSender;
};
