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

void TriangleController::update(){}

void TriangleController::draw(){
    for( list<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
		t->draw();
	}
}

void TriangleController::subdivide(){
    // make a new list
    list<Triangle> newList;
    // go through the current triangles
    for( list<Triangle>::iterator t = mTriangles.begin(); t != mTriangles.end(); ++t ){
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

void TriangleController::tap(){
    
    uint16_t randTriangle = Rand::randInt(0, mTriangles.size() - 1);
        console() << "--- tapping " << randTriangle << endl;
    if (mTriangles.size() > randTriangle)
    {
        std::list<Triangle>::iterator it = std::next(mTriangles.begin(), randTriangle);
        it->tap();
    }

    
}