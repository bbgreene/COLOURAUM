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
    
    // pre filters
    bool filtersOnOff { false };
    juce::dsp::LinkwitzRileyFilter<float> highPassFilter;
    juce::dsp::LinkwitzRileyFilter<float> lowPassFilter;
    
    // chorus object
    juce::dsp::Chorus<float> chorusModule;
    bool chorusOnOff { false };
    
    FractionalDelay predelay;
    float predelayMS = 0.0;
    float Fs = 44100.0;
    
    //reverb and params
    bool reverbOnOff { true };
    bbg_dsp::ReverbUtilities::Parameters reverbParams;
    bbg_dsp::Reverb reverbModule;
//    juce::dsp::Reverb::Parameters reverbParams;
//    juce::dsp::Reverb reverbModule;
    
    // gate
    bool gateOnOff { false };
    juce::dsp::NoiseGate<float> gateModule;
    
    //main mix
    juce::dsp::DryWetMixer<float> mixModule;
    
    //Functions for param layout and changes
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COLOURAUMAudioProcessor)
};
