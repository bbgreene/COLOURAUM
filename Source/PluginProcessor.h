/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/Reverb/ReverbDSP.h"
#include "../Source/Reverb/ReverbUtilities.h"
#include "../Source/Reverb/FractionalDelay.h"

//==============================================================================
/**
*/
class COLOURAUMAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    COLOURAUMAudioProcessor();
    ~COLOURAUMAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;
private:
    
    //Pre filters
    bool filtersOnOff { false };
    juce::dsp::LinkwitzRileyFilter<float> highPassFilter;
    juce::dsp::LinkwitzRileyFilter<float> lowPassFilter;
    
    //Tube distortion variables
    float tubeInPercentage { 0.0 };
    float tubeInGain { 0.0 };
    float rawInput { 1.0 };
    
    // distortion function and processing
    void processDistortion(juce::dsp::AudioBlock<float>& block);
    float tubeData(float samples);
    
    // softclip divisor. Creating this constexpr is more efficient than doing 2/pi for every sample in the audio block, because calculated at initialisation
    static constexpr float piDivisor = 2.0 / juce::MathConstants<float>::pi;
    
    //LFO Variables & functions
    juce::LinearSmoothedValue<float> depthOne { 0.0 };
    juce::LinearSmoothedValue<float> freqOne { 0.0 };
    juce::LinearSmoothedValue<float> lfoOnePhase { 0.0 };
    float inverseSampleRate; // for LFO phase update
    int waveform  { 0 };
    bool tremOnOff { true };
    bool tremPrePost { true };
    void tremoloProcessing(juce::AudioBuffer<float>& buffer);
    float lfoOne(float phase, int choice);
    float twoPiMultiplier = 2.f * M_PI;
    float myDepthOnePercentage;
    float myDepthOne;
    
    //Early Reflections
    FractionalDelay earlyA, earlyB, earlyC, earlyD , earlyE, earlyF;
    float erMixValue;
    juce::dsp::DryWetMixer<float> earlyMixModule;
    int erSelection { 0 };
    void earlyTimesSelection(int selection);
    float earlyAMS = 10.0;
    float earlyBMS = 25.0;
    float earlyCMS = 55.0;
    float earlyDMS = 76.0;
    float earlyEMS = 23.0;
    float earlyFMS = 90.0;
    float earlyAGain, earlyBGain, earlyCGain, earlyDGain, earlyEGain, earlyFGain;
    float earlyFilterValue;
    juce::dsp::LinkwitzRileyFilter<float> earlyLowPassFilter;
    void earlyReflectionsProcessing(juce::AudioBuffer<float>& buffer);
    float Fs = 44100.0;
    float erSpeed = 0.0;
    float erDepth = 0.0;
    void earlyReflectionsPrep();
    
    //Predelay object and params
    FractionalDelay predelay;
    juce::LinearSmoothedValue<float> predelayMS { 0.0 };
    void preDelayProcesing(juce::dsp::AudioBlock<float>& block);
    
    //Reverb and params
    float sizeValue;
    float dampValue;
    float widthValue;
    bbg_dsp::ReverbUtilities::Parameters reverbParams;
    bbg_dsp::Reverb reverbModule;
    
    //Gate
    bool gateOnOff { false };
    juce::dsp::NoiseGate<float> gateModule;
    
    //Main mix
    float mainMixValue;
    juce::dsp::DryWetMixer<float> mixModule;
    
    //Functions for param layout and changes
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    void updateParams();
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COLOURAUMAudioProcessor)
};
