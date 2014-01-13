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
    
    void update();
    void draw();
    void tap(float amt = 1.0f);
    void colorTap(float amt);
    void setBrightness(float amt);
    bool isActive();
    
    std::list<Triangle> subdivide();
    
  private:
    ci::Vec2f a, b, c;

    ci::Vec3f mColor; // HSV
    
    float mDecay;
    float mTresh;
    
    void init();
    
};