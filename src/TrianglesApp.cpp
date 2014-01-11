#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/FftProcessor.h"
#include "cinder/audio/PcmBuffer.h"

#include <vector>

#include "Triangle.h"
#include "TriangleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TrianglesApp : public AppNative {
  public:
	void prepareSettings( Settings *settings );
	void setup();
    void resize();
	void update();
	void draw();
    
    void keyDown( KeyEvent event );
    void mouseDown( MouseEvent event );
    
    void toggleFullscreen();
    void initTriangles();

    bool                    mFullScreen = false;
    
    // Audio
    audio::TrackRef         mTrack;
	audio::PcmBuffer32fRef  mPcmBuffer;
	uint16_t                mBandCount = 32;
    
    std::shared_ptr<float>  mFftRef;
    
    // Triangles
    TriangleController      mTriangleController;
    
};

void TrianglesApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( 1280, 1080 );
    //    settings->setFullScreen(bIsFullScreen);
    //	settings->setFrameRate( mFPS );
}

void TrianglesApp::setup()
{
    initTriangles();
}

void TrianglesApp::resize()
{
    initTriangles();
}

void TrianglesApp::update()
{
}

void TrianglesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    mTriangleController.draw();

}

void TrianglesApp::mouseDown( MouseEvent event )
{
    mTriangleController.subdivide();
}

void TrianglesApp::keyDown(KeyEvent event){
    switch( event.getCode() ) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        case KeyEvent::KEY_f:
            toggleFullscreen();
            break;
    }
}

void TrianglesApp::toggleFullscreen(){
    mFullScreen = !mFullScreen;
    setFullScreen( mFullScreen );
    
    mTriangleController.clearTriangles();
    
    resize();
}

void TrianglesApp::initTriangles(){
    list<Triangle> tempTriangles;
    
    Vec2f a,b,c;
    
    // check to make sure that we are drawing a base triangle that will fit in the window
    if (getWindowWidth() > getWindowHeight()){
        // get the height of the triangle we want to draw
        Vec2f h1 = Vec2f(getWindowWidth() * 0.5f, getWindowHeight() - 50.0f);
        Vec2f h2 = Vec2f(getWindowWidth() * 0.5f, 50.0f);
        
        // get the height
        float h = abs(h1.y - h2.y);
        
        // calculate the length of the sides
        float _a = h/(sqrt(3) * 0.5f);
        
        a = Vec2f(h2.x - (_a * 0.5f), h2.y);
        b = Vec2f(h2.x + (_a * 0.5f), h2.y);
        c = h1;
        
    } else {
        // creates a triangle given with width
        // -60 for inverted triangles
        double s60 = sin(-60 * M_PI / 180.0);
        double c60 = cos(-60 * M_PI / 180.0);
        
        float x1 = 50;
        float x2 = getWindowWidth() - 50;
        float y1 = 10.0f;
        float y2 = 10.0f;
        
        a = Vec2f(x1, y1);
        b = Vec2f(x2, y2);
        c = Vec2f(c60 * (x1 - x2) - s60 * (y1 - y2) + x2,
                  s60 * (x1 - x2) + c60 * (y1 - y2) + y2);
        
    }
    
    mTriangleController.addTriangle(Triangle(a,b,c));
    mTriangleController.subdivide();

}

CINDER_APP_NATIVE( TrianglesApp, RendererGl )
