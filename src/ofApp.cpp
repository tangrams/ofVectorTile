#include "ofApp.h"
#include "JsonLoader.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetDataPathRoot("../Resources/data/");
    
    // Load map objects from JSON file
    JsonLoader jsonLoader = JsonLoader("vectorTile.json");
    rootNode = jsonLoader.loadNodeGraph();
    rootNode->setPosition(0, 0, 0);
    
    rootNode->printPosition("");
    
    // General graphics setup
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    // Camera setup
    camera = ofEasyCam();
    camera.setPosition(rootNode->getGlobalPosition());
    camera.move(0, 0, 300);
    camera.setTarget(rootNode->getGlobalPosition());
    
    // Lighting setup
    mainLight = ofLight();
    mainLight.setPointLight();
    mainLight.setGlobalPosition(-100, 0, 500);
    mainLight.setDiffuseColor(ofColor(35, 35, 35));
    mainLight.setSpecularColor(ofColor(170, 170, 170));
    
    ofLog(OF_LOG_VERBOSE, "Camera at " + ofToString(camera.getX()) + ", " + ofToString(camera.getY()) + ", " + ofToString(camera.getZ()));
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableLighting();
    
    camera.begin();
    
    mainLight.enable();
    
    rootNode->draw();
    
    mainLight.disable();
    
    camera.end();
    
    ofDisableLighting();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
