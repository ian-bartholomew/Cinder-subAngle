//
//  TriangleController.h
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/8/14.
//
//

#pragma once

#include "Triangle.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TriangleController {
  public:
	TriangleController();

    void update();
    void draw();
    
    void subdivide();
    void addTriangle(Triangle triangle);
    
    std::list<Triangle> mTriangles;
};