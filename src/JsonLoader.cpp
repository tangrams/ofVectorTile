//
//  JsonLoader.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "JsonLoader.h"
#include <math.h>

JsonLoader::JsonLoader(std::string fileName) {
    
    if (jsonRoot.open(fileName)) {
        ofLog(OF_LOG_VERBOSE, "Json parsed successfully");
    } else {
        ofLog(OF_LOG_VERBOSE, "Json could not be parsed!");
    }
    
    tessellator = ofTessellator();
    
}

FeatureNode* JsonLoader::loadNodeGraph() {
    
    vector<FeatureNode*> layers;
    
    ofLog(OF_LOG_VERBOSE, "Loading Layers:");
    
    std::string layerNames[5] = {
        "buildings",
        "earth",
        "landuse",
        "roads",
        "water"
    };
    
    ofColor layerColors[5] = {
        ofColor::moccasin,
        ofColor::lawnGreen,
        ofColor::khaki,
        ofColor::ivory,
        ofColor::deepSkyBlue
    };
    
    float layerHeights[5] = {
        0.5f,
        0.2f,
        0.3f,
        0.4f,
        0.0f
    };
    
    for (int i = 0; i < 5; i++) {
        layerColor = layerColors[i];
        FeatureCollectionNode* newLayer = parseFeatureCollectionNode(jsonRoot[layerNames[i]]);
        newLayer->move(0, 0, layerHeights[i]);
        layers.push_back(newLayer);
        ofLog(OF_LOG_VERBOSE, layerNames[i] + " layer parsed with " + ofToString(newLayer->children.size()) + " children");
    }
    
    FeatureCollectionNode* rootNode = new FeatureCollectionNode(layers);
    
    return rootNode;
}

FeatureNode* JsonLoader::parseNode(ofxJSONElement nodeJson) {
    
    std::string nodeType = nodeJson["type"].asString();
    
    if (nodeType.compare("Feature") == 0) {
        return parseFeatureNode(nodeJson);
    } else if (nodeType.compare("FeatureCollection") == 0) {
        return parseFeatureCollectionNode(nodeJson);
    } else {
        ofLog(OF_LOG_WARNING, "Couldn't match Feature type; returning empty node");
        return new FeatureNode();
    }
    
}

FeatureCollectionNode* JsonLoader::parseFeatureCollectionNode(ofxJSONElement collectionJson) {
    
    ofxJSONElement features = collectionJson["features"];
    
    vector<FeatureNode*> featureNodes;
    
    for (int i = 0; i < features.size(); i++) {
        featureNodes.push_back(parseNode(features[i]));
    }
    
    FeatureCollectionNode* newFeatureCollection = new FeatureCollectionNode(featureNodes);
    
    ofLog(OF_LOG_VERBOSE, "Feature collection of " + ofToString(features.size()) + " features parsed with position " +
          ofToString(newFeatureCollection->getX()) + ", " +
          ofToString(newFeatureCollection->getY()));
    
    return newFeatureCollection;
    
}

FeatureNode* JsonLoader::parseFeatureNode(ofxJSONElement featureJson) {
    
    ofxJSONElement coords = featureJson["geometry"]["coordinates"];
    ofxJSONElement props = featureJson["properties"];
    
    ofMesh newMesh = ofMesh();
    
    ofVec3f anchor;
    
    std::string type = featureJson["geometry"]["type"].asString();
    
    if (type.compare("Point") == 0) {
        
        newMesh.setMode(OF_PRIMITIVE_POINTS);
        newMesh.setupIndicesAuto();
        newMesh.addVertex(ofVec3f(0, 0, 0));
        
        anchor = parsePointInProjectedCoords(coords);
        
    } else if (type.compare("MultiPoint") == 0) {
        
        vector<ofVec3f> verts = parsePointArrayInProjectedCoords(coords);
        
        anchor = getCentroidFromPoints(verts);
        
        movePoints(&verts, -(anchor));
        
        newMesh.setMode(OF_PRIMITIVE_POINTS);
        newMesh.addVertices(verts);
        newMesh.setupIndicesAuto();
        
    } else if (type.compare("LineString") == 0) {
        
        parseLineGeometry(coords, props, &newMesh, &anchor);
        
    } else if (type.compare("MultiLineString") == 0) {
        
        for (int i = 0; i < coords.size(); i++) {
            parseLineGeometry(coords[0], props, &newMesh, &anchor);
        }
        
    } else if (type.compare("Polygon") == 0) {
        
        parsePolygonGeometry(coords, props, &newMesh, &anchor);
        
        ofLog(OF_LOG_VERBOSE, "Polygon mesh with vertices: " + ofToString(newMesh.getVertices()));
        
    } else if (type.compare("MultiPolygon") == 0) {
        
        for (int i = 0; i < coords.size(); i++) {
            ofMesh subMesh = ofMesh();
            parsePolygonGeometry(coords[0], props, &subMesh, &anchor);
            newMesh.append(subMesh);
        }
        
    } else if (type.compare("GeometryCollection") == 0) {
        
        // Ignoring this for now
        
    } else {
        
        ofLog(OF_LOG_WARNING, "Couldn't match geometry type; returning empty mesh");
        
    }
     
    newMesh.setColorForIndices(0, newMesh.getNumIndices(), layerColor);
    
    FeatureLeafNode* newNode = new FeatureLeafNode(newMesh);
    newNode->idString = featureJson["id"].asString();
    newNode->setPosition(anchor);
    
    ofLog(OF_LOG_VERBOSE, "Parsed geometry type: " + type + ", with centroid: " + ofToString(anchor));
    
    return newNode;
    
}

ofVec3f JsonLoader::parsePointInProjectedCoords(ofxJSONElement pointJson) {
    
    double x = lon2x(pointJson[0].asFloat());
    double y = lat2y(pointJson[1].asFloat());
    
    return ofVec3f(x, y, 0);
    
}

vector<ofVec3f> JsonLoader::parsePointArrayInProjectedCoords(ofxJSONElement pointArrayJson) {
    
    vector<ofVec3f> pts;
    
    for (int i = 0; i < pointArrayJson.size(); i++) {
        pts.push_back(parsePointInProjectedCoords(pointArrayJson[i]));
    }
    
    return pts;
    
}

void JsonLoader::parseLineGeometry(ofxJSONElement lineJson, ofxJSONElement propsJson, ofMesh* meshToFill, ofVec3f* anchor) {
    
    vector<ofVec3f> verts = parsePointArrayInProjectedCoords(lineJson);
    
    *anchor = getCentroidFromPoints(verts);
    
    movePoints(&verts, -(*anchor));
    
    meshToFill->setMode(OF_PRIMITIVE_LINE_STRIP);
    meshToFill->addVertices(verts);
    meshToFill->setupIndicesAuto();
    
    for (int i = 0; i < meshToFill->getNumVertices(); i++) {
        meshToFill->addNormal(ofVec3f(0, 0, 1));
    }
    
}

void JsonLoader::parsePolygonGeometry(ofxJSONElement polygonJson, ofxJSONElement propsJson, ofMesh *meshToFill, ofVec3f *anchor) {
    
    float height = 0;
    float minHeight = 0;
    
    if (propsJson["height"] != Json::nullValue) {
        height = propsJson["height"].asFloat();
    }
    
    if (propsJson["min_height"] != Json::nullValue) {
        minHeight = propsJson["min_height"].asFloat();
    }
    
    vector<ofPolyline> polyLines;
    
    for (int i = 0; i < polygonJson.size(); i++) {
        
        vector<ofVec3f> verts = parsePointArrayInProjectedCoords(polygonJson[i]);
        
        if (i == 0) {
            *anchor = getCentroidFromPoints(verts);
        }
        
        movePoints(&verts, ofVec3f(-(anchor->x), -(anchor->y), height));
        
        polyLines.push_back(verts);
        
    }
    
    tessellator.tessellateToMesh(polyLines, OF_POLY_WINDING_ODD, *meshToFill, false);
    
    // Add normals for the top surface
    for (int i = 0; i < meshToFill->getNumVertices(); i++) {
        meshToFill->addNormal(ofVec3f(0, 0, 1));
    }
    
    if (height != 0) {
        // Extrude outermost (first) polyLine down to minHeight
        vector<ofVec3f> outlineVerts = polyLines[0].getVertices();
        
        for (int i = 0; i < outlineVerts.size() - 1; i++) {
            
            int nV = meshToFill->getNumVertices();
            
            ofVec3f norm = (meshToFill->getNormals()[i]).crossed(outlineVerts[i+1] - outlineVerts[i]);
            
            meshToFill->addVertex(outlineVerts[i]);
            meshToFill->addNormal(norm);
            meshToFill->addVertex(outlineVerts[i+1]);
            meshToFill->addNormal(norm);
            meshToFill->addVertex(ofVec3f(outlineVerts[i].x, outlineVerts[i].y, minHeight));
            meshToFill->addNormal(norm);
            meshToFill->addTriangle(nV, nV + 1, nV + 2);
            meshToFill->addVertex(ofVec3f(outlineVerts[i+1].x, outlineVerts[i+1].y, minHeight));
            meshToFill->addNormal(norm);
            meshToFill->addTriangle(nV + 1, nV + 3, nV + 2);
            
        }

    }

}

void JsonLoader::movePoints(vector<ofVec3f> *pts, ofVec3f offset) {
    
    for (int i = 0; i < pts->size(); i++) {
        (*pts)[i] += offset;
    }
}

ofVec3f JsonLoader::getCentroidFromPoints(vector<ofVec3f> pts) {
    ofVec3f centroid;
    
    for (int i = 0; i < pts.size(); i++) {
        centroid += pts[i] / pts.size();
    }
    
    return centroid;
}