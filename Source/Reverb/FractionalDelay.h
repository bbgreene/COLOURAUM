/*
  ==============================================================================

    FractionalDelay.h
    Created: 3 Aug 2022 5:03:35pm
    Author:  Brian Greene

  ==============================================================================
*/


#define _USE_MATH_DEFINES

#pragma once

#include <math.h>
using namespace std;

class FractionalDelay {

public:

    FractionalDelay();

    FractionalDelay(float delay, float speed);

    ~FractionalDelay();

    float processSample(float x, int channel, float gain);

    void setFs(float Fs);

    void setDelaySamples(float delay);

    void setSpeed(float speed);
    void setDepth(float depth);


private:

    float Fs = 48000.f;

    float delay = 5.f;

    const int MAX_BUFFER_SIZE = 96000;
    float delayBuffer[96000][2] = { 0.0f };
    int index[2] = { 0 };

    float speed = 1.0f; // Hz, frequency of LFO
    float depth = 10.0f; // percentage of intensity, control amp of LFO

    float twoPiMultiplier = 2.f * M_PI;
    float currentAngle[2] = { 0.0f };
    float angleChange = speed * (1.f / Fs) * twoPiMultiplier;
    
    

};
