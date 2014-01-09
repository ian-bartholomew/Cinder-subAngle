//
//  Poly.h
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/8/14.
//
//

#pragma once

#include "cinder/Vector.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Triangle {
  public:
	Triangle();
    Triangle(Vec2f a, Vec2f b, Vec2f c);
    
    ci::Vec2f a;
    ci::Vec2f b;
    ci::Vec2f c;
    
    ci::Color mColor;
    
    float mDecay;
    
    void update();
    void draw();
    std::list<Triangle> subdivide();
    
};