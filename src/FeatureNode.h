//
//  FeatureNode.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/27/14.
//
//

#ifndef __vectorTileExperiment__FeatureNode__
#define __vectorTileExperiment__FeatureNode__

#include "ofMain.h"

class FeatureNode : public ofNode {
    
public:
    FeatureNode();
    virtual void draw();
    virtual void customDraw();
    virtual void printPosition(const string &prefix);
    
};

#endif /* defined(__vectorTileExperiment__FeatureNode__) */
