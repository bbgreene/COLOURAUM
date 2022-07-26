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
    bbg_gui::bbg_Dial earlyVolume { "", 0.0, 100.0, 1.0, 50.0, 0.0 };
    bbg_gui::bbg_Menu erType;
    bbg_gui::bbg_Dial earlyRate { "", 0.0, 200.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial earlyDepth { "", 0.0, 100.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Dial size { "", 1.0, 100.0, 0.01, 40.0, 0.0 };
    bbg_gui::bbg_PushButton freeze { "Freeze" };
    bbg_gui::bbg_Dial predelay { "", 0.0, 200.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Dial damp { "", 0.0, 100.0, 1.0, 50.0, 0.0 };
    bbg_gui::bbg_PushButton GateOnOffButton { "On" };
    bbg_gui::bbg_Dial threshold { "", -20.0, 0.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial ratio { "", 1.0, 5.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial attack { "", 1.0, 200.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Dial release { "", 1.0, 3000.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Menu waveMenu;
    bbg_gui::bbg_PushButton tremOnOffButton { "On" };
    bbg_gui::bbg_PushButton tremPrePostButton { "Post" };
    bbg_gui::bbg_Dial tremoDistortion { "", 0.0, 100.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Dial tremoRate { "", 0.0, 100.0, 0.01, 8.0, 0.0 };
    bbg_gui::bbg_Dial tremoDepth { "", 0.0, 100.0, 1.0, 10.0, 0.0 };
    bbg_gui::bbg_Dial mix { "", 0.0, 100.0, 0.01, 100.0, 0.0 };
    bbg_gui::bbg_Dial width { "", 0.0, 100.0, 0.01, 80.0, 0.0 };
    
    // ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> earlyVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> erTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> earlyRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> earlyDepthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freezeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> predelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tremoloAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tremoloPrePostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremoDistortionAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveMenuAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremoDepthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    // LABELS
    bbg_gui::bbg_dialLabel highPassLabel { "High" };
    bbg_gui::bbg_dialLabel lowPassLabel { "Low" };
    bbg_gui::bbg_dialLabel earlyVolumeLabel { "Amount" };
    bbg_gui::bbg_menuLabel erTypeLabel { "Type" };
    bbg_gui::bbg_dialLabel earlyRateLabel { "Mod Rate" };
    bbg_gui::bbg_dialLabel earlyDepthLabel { "Mod Depth" };
    bbg_gui::bbg_dialLabel sizeLabel { "Size" };
    bbg_gui::bbg_dialLabel predelayLabel { "Predelay" };
    bbg_gui::bbg_dialLabel dampLabel { "Damp" };
    bbg_gui::bbg_dialLabel thresholdLabel { "Threshold" };
    bbg_gui::bbg_dialLabel ratioLabel { "Ratio" };
    bbg_gui::bbg_dialLabel attackLabel { "Attack" };
    bbg_gui::bbg_dialLabel releaseLabel { "Release" };
    bbg_gui::bbg_dialLabel tremoDistortionLabel { "Distortion" };
    bbg_gui::bbg_menuLabel waveLabel { "Wave" };
    bbg_gui::bbg_dialLabel tremoRateLabel { "Rate" };
    bbg_gui::bbg_dialLabel tremoDepthLabel { "Depth" };
    bbg_gui::bbg_dialLabel mixLabel { "Mix" };
    bbg_gui::bbg_dialLabel widthLabel { "Width" };
    
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
