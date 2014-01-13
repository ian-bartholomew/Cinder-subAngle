#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/FftProcessor.h"
#include "cinder/audio/PcmBuffer.h"
#include "cinder/params/Params.h"

#include <vector>

#include "Triangle.h"
#include "TriangleController.h"
#include "KissFFT.h"
#include "onsetsds.h"

#include "Resources.h"

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
    void shutdown();
    
    void keyDown( KeyEvent event );
    void mouseDown( MouseEvent event );
    
    void toggleFullscreen();
    void initTriangles();
    void initODF();
    void initAudio();
    long getCurrentTimeInMillis();
    
    bool                        mFullScreen = false;
    bool                        bDrawBeat   = false;
    bool                        bDrawParams = false;
    bool                        isOnset     = false;
    
    // PARAMS
	params::InterfaceGlRef      mParams;
    
    // Onset Dection
    OnsetsDS mOds;
    
	// Audio file
	ci::audio::SourceRef		mAudioSource;
	ci::audio::PcmBuffer32fRef	mBuffer;
	ci::audio::TrackRef			mTrack;
    
	// Analyzer
	KissRef						mFft;
    int32_t                     mDampen = 50; // time to wait between beats
    long                        timer;
    
    // Triangles
    TriangleController          mTriangleController;
    
};

//------------------------------------------------
#pragma mark -
#pragma mark lifecycle
#pragma mark -

void TrianglesApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( 1280, 1080 );
    settings->enableHighDensityDisplay();
    //    settings->setFullScreen(bIsFullScreen);
    //	settings->setFrameRate( mFPS );
}

void TrianglesApp::setup()
{
    mParams = params::InterfaceGl::create( "Triangles", Vec2i( 200, 310 ) );
    mParams->addParam( "Draw beat", &bDrawBeat);
    mParams->addParam( "Dampen", &mDampen);
    
    // Set up line rendering
	gl::enable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    
    gl::color( ColorAf::white() );

//    initTriangles();
    
	// Load and play audio
	mAudioSource = audio::load( loadResource( RES_TRACK ) );
	mTrack = audio::Output::addTrack( mAudioSource, false );
	mTrack->enablePcmBuffering( true );
	mTrack->play();
    
    initODF();
    
        
}

void TrianglesApp::resize()
{
    initTriangles();   
}

void TrianglesApp::update()
{
    // Check if track is playing and has a PCM buffer available
	if ( mTrack->isPlaying() && mTrack->isPcmBuffering() ) {
        
		// Get buffer
		mBuffer = mTrack->getPcmBuffer();
		if ( mBuffer && mBuffer->getInterleavedData() ) {
            
			// Get sample count
			uint32_t sampleCount = mBuffer->getInterleavedData()->mSampleCount;
			if ( sampleCount > 0 ) {
                
				// Initialize analyzer
				if ( !mFft ) {
					mFft = Kiss::create( sampleCount );
				}
                
				// Analyze data
				if ( mBuffer->getInterleavedData()->mData != 0 ) {
					mFft->setData( mBuffer->getInterleavedData()->mData );
				}
                
                isOnset = onsetsds_process(&mOds, mFft->getData());
                
			}
            
		}
        
	}
    
    if (isOnset && (getCurrentTimeInMillis() - timer > mDampen)){
        mTriangleController.tap();
        timer = getCurrentTimeInMillis();
    } else {
        isOnset = false;
    }
    
}

void TrianglesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::color( ColorAf::white() );
    
    mTriangleController.draw();
    
    if (isOnset && bDrawBeat) {
        gl::color(Color(CM_RGB, 1, 0, 0 ));
        gl::drawSolidCircle(Vec2f(20.0f, getWindowHeight() - 20.0f), 50.0f);
    }
    
    if (bDrawParams)
        mParams->draw();

}

void TrianglesApp::shutdown(){
    // Stop track
	mTrack->enablePcmBuffering( false );
	mTrack->stop();
	if ( mFft ) {
		mFft->stop();
	}
}

//------------------------------------------------
#pragma mark -
#pragma mark Events
#pragma mark -

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
        case KeyEvent::KEY_SPACE:
            bDrawParams = !bDrawParams;
            break;
        case KeyEvent::KEY_a:
            bDrawBeat = !bDrawBeat;
            break;
    }
}

void TrianglesApp::toggleFullscreen(){
    mFullScreen = !mFullScreen;
    setFullScreen( mFullScreen );
    
    mTriangleController.clearTriangles();
    
    resize();
}

//------------------------------------------------
#pragma mark -
#pragma mark Inits
#pragma mark -

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

void TrianglesApp::initODF(){
    // There are various types of onset detector available, we must choose one
//    onsetsds_odf_types odftype = onsetsds_odf_types::ODS_ODF_WPHASE;  // good for bass and low end
    onsetsds_odf_types odftype = onsetsds_odf_types::ODS_ODF_RCOMPLEX;
    
    // Allocate contiguous memory using malloc or whatever is reasonable.
    // FFT size of 512 (@44.1kHz), and a median span of 11.
    float* odsdata = (float*) malloc( onsetsds_memneeded(odftype, 1024, 11) );
    
    // Now initialise the OnsetsDS struct and its associated memory
    onsetsds_init(&mOds, odsdata, onsetsds_fft_types::ODS_FFT_SC3_POLAR, odftype, 1024, 11, 44100.0f);
    
    // start our timer for the damper
    timer = getCurrentTimeInMillis();
}


long TrianglesApp::getCurrentTimeInMillis()
{
    // get system time in milliseconds
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

CINDER_APP_NATIVE( TrianglesApp, RendererGl )
