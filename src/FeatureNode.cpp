//
//  FeatureNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/27/14.
//
//

#include "FeatureNode.h"

FeatureNode::FeatureNode() {
    
}

void FeatureNode::customDraw() {
    
}

void FeatureNode::draw() {
    ofPushMatrix();
    ofMultMatrix(getLocalTransformMatrix());
    
    customDraw();
    
    ofPopMatrix();
}

void FeatureNode::printPosition(const string &prefix) {
    
}