//
//  Poly.cpp
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/8/14.
//
//

#include "Triangle.h"
#include "cinder/app/AppBasic.h"

#include "cinder/Rand.h"

Triangle::Triangle(){
    init();
}

Triangle::Triangle(Vec2f _a, Vec2f _b, Vec2f _c) {
    a       = _a;
    b       = _b;
    c       = _c;
    
    init();
}

void Triangle::init(){
    mColor = hsvToRGB(Vec3f(Rand::randFloat(0,1), Rand::randFloat(0,1), 0.0f));
    mDecay	= 0.99f;
    mTresh = 0.45f;
}

void Triangle::draw() {
    Vec3f hsv = cinder::rgbToHSV(mColor);
    
    mColor = Color(CM_HSV, hsv.x, hsv.y, hsv.z * mDecay);
    
	glColor4f( mColor );
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    
    gl::begin(GL_TRIANGLES);
    
        gl::vertex(a.x, a.y);
        gl::vertex(b.x, b.y);
        gl::vertex(c.x, c.y);
    
    gl::end();
        
}

// subdivide the current triangle into four equaliatral triangles
std::list<Triangle> Triangle::subdivide() {
    std::list<Triangle> rval;
    
    Vec2f a1 = Vec2f( (a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f );
    Vec2f b1 = Vec2f( (b.x + c.x) * 0.5f, (b.y + c.y) * 0.5f );
    Vec2f c1 = Vec2f( (c.x + a.x) * 0.5f, (c.y + a.y) * 0.5f );
    
    rval.push_back( Triangle(a, a1, c1) );
    rval.push_back( Triangle(b, b1, a1) );
    rval.push_back( Triangle(c, c1, b1) );
    rval.push_back( Triangle(a1, b1, c1) );
    
    return rval;
}

void Triangle::tap(){
    Vec3f hsv = cinder::rgbToHSV(mColor);
    mColor = hsvToRGB(Vec3f(hsv.x, hsv.y, 1.0f));
}

bool Triangle::isActive(){
    Vec3f hsv = cinder::rgbToHSV(mColor);
    return hsv.y > mTresh;
}