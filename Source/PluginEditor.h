/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class COLOURAUMAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    COLOURAUMAudioProcessorEditor (COLOURAUMAudioProcessor&);
    ~COLOURAUMAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    // BORDERS
    juce::GroupComponent filterBorder;
    juce::GroupComponent earlyReflectionsBorder;
    juce::GroupComponent reverbBorder;
    juce::GroupComponent gateBorder;
    juce::GroupComponent tremoloBorder;
    juce::GroupComponent outBorder;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    COLOURAUMAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COLOURAUMAudioProcessorEditor)
};
