//
//  BeatDetect.h
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/10/14.
//
//

#pragma once

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class BeatDetect {
  public:
    BeatDetect();
	~BeatDetect();
    BeatDetect(int bandCount);
    BeatDetect(int timeSize, float sampleRate, int bandCount);
    
    
    static const int FREQ_ENERGY  = 0;
    static const int SOUND_ENERGY = 1;
    
    void detectMode(int algo);
    //void detect(AudioBuffer buffer);
    void    detect(float * buffer);
    int     dectectSize();
    float   getDetectCenterFrequency(int i);
    void    setSensitivity(int millis);
    
    bool    isOnset();
    bool    isOnset(int i);
    bool    isKick();
    bool    isSnare();
    bool    isHat();
    bool    isRange(int low, int high, int threshold);
    
  private:
    int             algorithm;
    int             sampleRate;
    int             timeSize;
    int             valCnt;
    vector<float>   valGraph;
    int             sensitivity;
    int             mBandCount;
    
    // for circular buffer support
    int             insertAt;
    
    // vars for sEnergy
    bool            eIsOnset;
    vector<float>   eBuffer;
    vector<float>   dBuffer;
    long            timer;
    
    // vars for fEnergy
    bool            fIsOnset;
    //FFT                                        spect;
    vector<vector<float>>   feBuffer;
    vector<vector<float>>   fdBuffer;
    vector<long>            fTimer;
    vector<float>           varGraph;
    int                     varCnt;
    
    long getCurrentTimeInMillis();
    void initSEResources();
    void sEnergy(float * buffer);
    void fEnergy(float * buffer);
    float average(vector<float> list);
    float variance(vector<float> list, float val);
    float specAverage(vector<float> list);
};