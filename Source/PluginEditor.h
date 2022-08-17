/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Source/bbg_gui/Dial.h"
#include "../Source/bbg_gui/Toggle.h"
#include "../Source/bbg_gui/PushButton.h"
#include "../Source/bbg_gui/Menu.h"
#include "../Source/bbg_gui/Label.h"

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
    
    bbg_gui::bbg_dialLabel olumay { "Olumay dsp" };
    bbg_gui::bbg_dialLabel colouraumTitle { "C O L O U R A U M" };
    bbg_gui::bbg_dialLabel colouraumVersion { "version 1.1" };
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    COLOURAUMAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COLOURAUMAudioProcessorEditor)
};
