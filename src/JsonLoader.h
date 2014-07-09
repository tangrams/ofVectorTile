//
//  JsonLoader.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#ifndef __vectorTileExperiment__JsonLoader__
#define __vectorTileExperiment__JsonLoader__
#define R_EARTH 6378137

#include <iostream>
#include "ofxJSONElement.h"
#include "FeatureCollectionNode.h"
#include "FeatureLeafNode.h"

#endif /* defined(__vectorTileExperiment__JsonLoader__) */

class JsonLoader {
    
public:
    ofxJSONElement jsonRoot;
    
    JsonLoader(std::string fileName);
    FeatureNode* loadNodeGraph();
    FeatureNode* parseNode(ofxJSONElement nodeJson);
    FeatureCollectionNode* parseFeatureCollectionNode(ofxJSONElement collectionJson);
    FeatureNode* parseFeatureNode(ofxJSONElement featureJson);
    ofVec3f parsePointInProjectedCoords(ofxJSONElement pointJson);
    vector<ofVec3f> parsePointArrayInProjectedCoords(ofxJSONElement pointArrayJson);
    
    double y2lat(double y) { return ofRadToDeg(2 * atan(exp(ofDegToRad(y))) - PI / 2); }
    double x2lon(double x) { return ofRadToDeg(x / R_EARTH); }
    double lat2y(double lat) { return R_EARTH * log(tan(PI / 4 + ofDegToRad(lat) / 2)); }
    double lon2x(double lon) { return ofDegToRad(lon) * R_EARTH; }
    
private:
    ofTessellator tessellator;
    ofColor layerColor;
    
    ofVec3f getCentroidFromPoints(vector<ofVec3f> pts);
    void parseLineGeometry(ofxJSONElement lineJson, ofxJSONElement propsJson, ofMesh* meshToFill, ofVec3f* anchor);
    void parsePolygonGeometry(ofxJSONElement polygonJson, ofxJSONElement propsJson, ofMesh* meshToFill, ofVec3f* anchor);
    void movePoints(vector<ofVec3f> *pts, ofVec3f offset);
    
};