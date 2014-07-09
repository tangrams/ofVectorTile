//
//  FeatureLeafNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "FeatureLeafNode.h"

FeatureLeafNode::FeatureLeafNode(ofMesh newGeometry) {
    
    geometry = newGeometry;
    material = ofMaterial();
    material.setDiffuseColor(ofColor(50, 50, 50));
    material.setSpecularColor(ofColor(255, 255, 255));
    idString = "";
    
    setPosition(geometry.getCentroid());
    
}

FeatureLeafNode::FeatureLeafNode(ofMatrix4x4 newTransform) {
    
    setTransformMatrix(newTransform);
    geometry = ofMesh();
    material = ofMaterial();
    idString = "";
    
}

void FeatureLeafNode::customDraw() {
    
    material.begin();
    geometry.draw();
    material.end();

}

void FeatureLeafNode::printPosition(const string &prefix) {
    cout << "FeatureLeafNode at: " + ofToString(getPosition()) << endl;
}