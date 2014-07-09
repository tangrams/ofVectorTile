//
//  FeatureLeafNode.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#ifndef __vectorTileExperiment__FeatureLeafNode__
#define __vectorTileExperiment__FeatureLeafNode__

#include <iostream>
#include <string>
#include "ofMain.h"
#include "FeatureNode.h"

#endif /* defined(__vectorTileExperiment__FeatureLeafNode__) */

class FeatureLeafNode : public FeatureNode {
    
public:
    ofMesh geometry;
    string idString;
    
    FeatureLeafNode(ofMesh newGeometry);
    FeatureLeafNode(ofMatrix4x4 newTransform);
    
    virtual void customDraw();
    virtual void printPosition(const string &prefix);
    
private:
    ofMaterial material;
    
};