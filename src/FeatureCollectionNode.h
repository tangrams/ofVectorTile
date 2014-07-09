//
//  FeatureCollectionNode.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#ifndef __vectorTileExperiment__FeatureCollectionNode__
#define __vectorTileExperiment__FeatureCollectionNode__

#include <vector>
#include "ofMain.h"
#include "FeatureNode.h"

#endif /* defined(__vectorTileExperiment__FeatureCollectionNode__) */

class FeatureCollectionNode : public FeatureNode {
    
public:
    vector<FeatureNode*> children;
    
    FeatureCollectionNode(vector<FeatureNode*> newChildren);
    FeatureCollectionNode(ofMatrix4x4 newTransform);
    //TODO: Other ways of initializing with just position
    
    virtual void customDraw();
    virtual void printPosition(const string &prefix);
    void addChild(FeatureNode* newChild);
    bool removeChild(FeatureNode* childToRemove);
    
private:
    ofVec3f getAnchorFromChildren(vector<FeatureNode*> children);
    
};