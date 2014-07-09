//
//  FeatureCollectionNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "FeatureCollectionNode.h"

FeatureCollectionNode::FeatureCollectionNode(vector<FeatureNode*> newChildren) {
    
    ofVec3f anchor = getAnchorFromChildren(newChildren);
    
    setGlobalPosition(anchor);
    
    for (int i = 0; i < newChildren.size(); i++) {
        addChild(newChildren[i]);
    }

}

FeatureCollectionNode::FeatureCollectionNode(ofMatrix4x4 newTransform) {
    
    setTransformMatrix(newTransform);
    children = vector<FeatureNode*>();
    
}

void FeatureCollectionNode::customDraw() {
    
    for (int i = 0; i < children.size(); i++) {
        children[i]->draw();
        //ofDrawArrow(ofVec3f(0, 0, 0), children[i]->getPosition() * getLocalTransformMatrix(), 5);
    }
    //ofCircle(ofVec3f(0, 0, 0), 20);
    
}

void FeatureCollectionNode::addChild(FeatureNode* newChild) {
    ofVec3f oldPos = newChild->getGlobalPosition();
    newChild->setParent(*this, false);
    newChild->setGlobalPosition(oldPos);
    children.push_back(newChild);
}

bool FeatureCollectionNode::removeChild(FeatureNode* childToRemove) {
    int childIndex = -1;
    
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == childToRemove) {
            childIndex = i;
        }
    }
    
    if (childIndex == -1) {
        return false;
    } else {
        children.erase(children.begin()+childIndex);
        return true;
    }
}

ofVec3f FeatureCollectionNode::getAnchorFromChildren(vector<FeatureNode*> children) {
    
    ofVec3f anchor;
    
    for (int i = 0; i < children.size(); i++) {
        anchor += children[i]->getGlobalPosition() / children.size();
    }
    
    return anchor;
}

void FeatureCollectionNode::printPosition(const string &prefix) {
    
    cout << "FeatureCollectionNode at: " + ofToString(getPosition()) << endl;
    
    string newPrefix = prefix + "-";
    
    for (int i = 0; i < children.size(); i++) {
        cout << newPrefix;
        children[i]->printPosition(newPrefix);
    }
}
