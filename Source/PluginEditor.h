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
    
    // DIALS, BUTTONS AND MENUS
    bbg_gui::bbg_Dial highPass { "", 20, 2000.0, 1.0, 20.0, 0.0 };
    bbg_gui::bbg_Dial lowPass { "", 5000.0, 20000.0, 1.0, 20000.0, 0.0 };
    bbg_gui::bbg_Dial earlyVolume { "", 0.0, 1.0, 0.01, 0.5, 0.0 };
    bbg_gui::bbg_Dial earlyRate { "", 0.0, 200.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial earlyDepth { "", 0.0, 100.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial size { "", 0.0, 1.0, 0.01, 0.2, 0.0 };
    bbg_gui::bbg_Dial predelay { "", 0.0, 200.0, 0.01, 0.0, 0.0 };
    bbg_gui::bbg_Dial damp { "", 0.0, 1.0, 0.01, 0.0, 0.0 };
    
    // LABELS
    bbg_gui::bbg_dialLabel highPassLabel { "High" };
    bbg_gui::bbg_dialLabel lowPassLabel { "Low" };
    bbg_gui::bbg_dialLabel earlyVolumeLabel { "Amount" };
    bbg_gui::bbg_dialLabel earlyRateLabel { "Mod Rate" };
    bbg_gui::bbg_dialLabel earlyDepthLabel { "Mod Depth" };
    bbg_gui::bbg_dialLabel sizeLabel { "Size" };
    bbg_gui::bbg_dialLabel predelayLabel { "Predelay" };
    bbg_gui::bbg_dialLabel dampLabel { "Damp" };
    
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
