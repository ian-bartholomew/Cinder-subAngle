            //
//  TriangleController.cpp
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/8/14.
//
//

#include "TriangleController.h"
#include "cinder/app/AppBasic.h"

#include "cinder/Rand.h"

TriangleController::TriangleController(){}

void TriangleController::update(){
    for( vector<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
		t->update();
	}
}

void TriangleController::draw(){
    for( vector<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
		t->draw();
	}
}

void TriangleController::subdivide(){
    // make a new list
    list<Triangle> newList;
    // go through the current triangles
    for( vector<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
        // get the new ones
        list<Triangle> newTriangles = t->subdivide();
        // add them to our new list
        for( list<Triangle>::iterator t1 = newTriangles.begin(); t1 != newTriangles.end(); ++t1 ){
            newList.push_back(*t1);
        }
	}
    
    // clear out all the old ones
    mTriangles.clear();
    // add the new ones to our list
    for( list<Triangle>::iterator t = newList.begin(); t != newList.end(); ++t ){
        mTriangles.push_back(*t);
    }
    
}

void TriangleController::addTriangle(Triangle triangle){
    mTriangles.push_back(triangle);
}

void TriangleController::clearTriangles(){
    mTriangles.clear();
}


void TriangleController::tap(float amt){
    uint16_t randTriangle = Rand::randInt(0, mTriangles.size());
    for( vector<Triangle>::iterator t = std::next(mTriangles.begin(), randTriangle); t != mTriangles.end(); ++t ){
        if (!t->isActive()){
            t->tap(amt);
            break;
        }
    }
}

void TriangleController::colorTap(float amt){
//    uint16_t randTriangle = Rand::randInt(0, mTriangles.size());
//    for( vector<Triangle>::iterator t = std::next(mTriangles.begin(), randTriangle); t != mTriangles.end(); ++t ){
    for( vector<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
        if (t->isActive()){
            t->colorTap(amt);
        }
    }
}

void TriangleController::setBrightness(float amt){
    for( vector<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
        if (t->isActive())
            t->setBrightness(amt);
    }
}