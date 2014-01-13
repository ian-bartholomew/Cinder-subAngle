//
//  BeatDetect.cpp
//  Triangles
//
//  Created by HUGE | Ian Bartholomew on 1/10/14.
//
//

#include "BeatDetect.h"
#include "cinder/app/AppBasic.h"

#include <time.h>
#include <math.h>

BeatDetect::BeatDetect()
{
    mBandCount = 32;
    sampleRate = 44100;
    timeSize = 1024;
    initSEResources();
    //    initGraphs();
    algorithm = SOUND_ENERGY;
    sensitivity = 10;
    
    eBuffer.resize(mBandCount);
    dBuffer.resize(mBandCount);
    
}

BeatDetect::BeatDetect(int bandCount){
    mBandCount = bandCount;
    sampleRate = 44100;
    timeSize = 1024;
    initSEResources();
//    initGraphs();
    algorithm = SOUND_ENERGY;
    sensitivity = 10;
    
    eBuffer.resize(mBandCount);
    dBuffer.resize(mBandCount);
}

BeatDetect::~BeatDetect(){}

void BeatDetect::initSEResources()
{
    eIsOnset = false;
    timer = getCurrentTimeInMillis();
    insertAt = 0;
}

void BeatDetect::detect(float * buffer)
{
    switch (algorithm)
    {
        case SOUND_ENERGY:
            sEnergy(buffer);
            break;
//        case FREQ_ENERGY:
//            fEnergy(buffer);
//            break;
    }
}

void BeatDetect::sEnergy(float * samples)
{
    // compute the energy level
    float level = 0;
    for (int i = 0; i < mBandCount; i++)
    {
        level += (samples[i] * samples[i]);
    }
    level /= mBandCount;
    level = (float) sqrt(level);
    float instant = level * 100;
    
    // compute the average local energy
    float E = average(eBuffer);
    
    // compute the variance of the energies in eBuffer
    float V = variance(eBuffer, E);
    // compute C using a linear digression of C with V
    float C = (-0.0025714f * V) + 1.5142857f;
    // filter negaive values
    float diff = std::max(instant - C * E, 0.0f);
//    pushVal(diff);
    
    // find the average of only the positive values in dBuffer
    float dAvg = specAverage(dBuffer);
    // filter negative values
    float diff2 = std::max(diff - dAvg, 0.0f);
//    pushVar(diff2);
    
    // report false if it's been less than 'sensitivity'
    // milliseconds since the last true value
    if (getCurrentTimeInMillis() - timer < sensitivity)
    {
        eIsOnset = false;
    }
    // if we've made it this far then we're allowed to set a new
    // value, so set it true if it deserves to be, restart the timer
    else if (diff2 > 0 && instant > 2)
    {
        eIsOnset = true;
        timer = getCurrentTimeInMillis();
    }
    // OMG it wasn't true!
    else
    {
        eIsOnset = false;
    }
    
    eBuffer[insertAt] = instant;
    dBuffer[insertAt] = diff;
    insertAt++;
    if (insertAt == mBandCount)
        insertAt = 0;
    
}

bool BeatDetect::isOnset() {
    return eIsOnset;
}

long BeatDetect::getCurrentTimeInMillis()
{
    // get system time in milliseconds
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

float BeatDetect::average(vector<float> list)
{
    float bufferSum = 0.0f;
    for (float f : eBuffer) bufferSum += f;
    return bufferSum / eBuffer.size();

}

float BeatDetect::specAverage(vector<float> list)
{
    float avg = 0;
    float num = 0;
    for (float f : list)
    {
        if (f > 0)
        {
            avg += f;
            num++;
        }
    }
    if (num > 0)
    {
        avg /= num;
    }
    return avg;
}

float BeatDetect::variance(vector<float> list, float val)
{
    float V = 0;
    for (float f : list) V += (float)pow(f - val, 2);

    V /= list.size();
    return V;
}