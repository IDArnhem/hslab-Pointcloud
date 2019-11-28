#include "ofApp.h"
#include "alvinutil.h"

void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);

    acquiring = false;

    rs2::context ctx;
    std::vector<rs2::pipeline>  pipelines;

    // enumerate available devices
    for(auto &&dev : ctx.query_devices()) {
        rs2::config cfg;
        ofLogNotice() << "RealSense device found with ID " << dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
        cfg.enable_device( dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) );
        acquiring = true; // seems like we have a data source
        pipe.start( cfg ); // open it
        pipelines.push_back(pipe); // keep track of this pipeline
    }

    // show result of camera scan
    if(acquiring) {
        ofLogNotice() << "Number of camera pipelines " << pipelines.size();
    } else {
        ofLogWarning() << "No RealSense cameras were found! make sure you have one plugged in";
    }
/*
    mSender = std::make_shared<UdpSender>();
    mSender->addOnSendFn([](std::shared_ptr<Datagram> datagram) {
        std::printf("Sent message!");
    });
*/
}

void ofApp::acquirePointCloud() {
    // Get depth data from camera
    auto frames = pipe.wait_for_frames();
    auto color = frames.get_color_frame();
    auto depth = frames.get_depth_frame();

    // tell pointcloud object to map colors from video frame
    pc.map_to(color);
    points = pc.calculate(depth);

    /*
    auto textureCoordinates = points.get_texture_coordinates();
    const unsigned char* colorData = static_cast<const unsigned char*>(color.get_data());

    for (int i = 0; i < rs_points.size(); i++) {
        int x = static_cast<int>(textureCoordinates[i].u * width);
        int y = static_cast<int>(textureCoordinates[i].v * height);
        int colorLocation = y * color_intrinsic.width + x;

        float px = rs_vertices[i].x;
        float py = rs_vertices[i].y;
        float pz = rs_vertices[i].z;
        unsigned char r = colorData[colorLoation];
        unsigned char g = colorData[colorLocation + 1];
        unsigned char b = colorData[colorLocation + 2];
        save(px, py, pz, r, g, b);
    }
    */

    // Create oF mesh
    mesh.clear();
    int n = points.size();
    if(n!=0){
        const rs2::vertex * vs = points.get_vertices();
        for(int i=0; i<n; i++){
            if(vs[i].z){
                const rs2::vertex v = vs[i];
                glm::vec3 v3(v.x,v.y,v.z);
                mesh.addVertex(v3);
                mesh.addColor(ofFloatColor(0,0,ofMap(v.z, 2, 6, 1, 0), 0.8));
            } // if
        } // for
    } // if
}

//--------------------------------------------------------------
void ofApp::update(){
/*
    std::shared_ptr<Message> datagram = std::make_shared<Message>("Hello world", "192.168.0.115", 8080);
    mSender->send(datagram);
*/

    if(!acquiring) {
        // do nothing
        return;
    } else {
        acquirePointCloud();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(200);
    cam.begin();
    float s = 200;
    ofScale(s,-s,-s);
    ofDrawAxis(1);

    ofPushMatrix();
    ofTranslate(0, 1, 0);
    ofRotateZDeg(90);
    ofSetColor(0,200);
    ofDrawGridPlane(1, 5, true);
    ofPopMatrix();

    mesh.drawVertices();

    cam.end();
}
