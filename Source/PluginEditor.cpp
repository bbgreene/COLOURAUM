/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
COLOURAUMAudioProcessorEditor::COLOURAUMAudioProcessorEditor (COLOURAUMAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // BORDERS
    filterBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    filterBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    filterBorder.setTextLabelPosition(juce::Justification::centred);
    filterBorder.setText("Filter");
    addAndMakeVisible(filterBorder);
    
    earlyReflectionsBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    earlyReflectionsBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    earlyReflectionsBorder.setTextLabelPosition(juce::Justification::centred);
    earlyReflectionsBorder.setText("Early Reflections");
    addAndMakeVisible(earlyReflectionsBorder);
    
    reverbBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    reverbBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    reverbBorder.setTextLabelPosition(juce::Justification::centred);
    reverbBorder.setText("Reverb");
    addAndMakeVisible(reverbBorder);
    
    gateBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    gateBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    gateBorder.setTextLabelPosition(juce::Justification::centred);
    gateBorder.setText("Gate");
    addAndMakeVisible(gateBorder);
    
    tremoloBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    tremoloBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    tremoloBorder.setTextLabelPosition(juce::Justification::centred);
    tremoloBorder.setText("Tremolo");
    addAndMakeVisible(tremoloBorder);
    
    outBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::steelblue.brighter());
    outBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::whitesmoke);
    outBorder.setTextLabelPosition(juce::Justification::centred);
    outBorder.setText("Output");
    addAndMakeVisible(outBorder);

    setResizable(false, false);
    setSize (1005, 330);
}

COLOURAUMAudioProcessorEditor::~COLOURAUMAudioProcessorEditor()
{
}

//==============================================================================
void COLOURAUMAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
       g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::mediumpurple, getHeight() - getHeight(), juce::Colours::mediumpurple.darker(), getHeight()));
       g.fillRect(background);
}

void COLOURAUMAudioProcessorEditor::resized()
{
    /*
     height = 250
     small border = 80
     medium border = 160
     large border = 200
     in between gap = 15
     margin = 25
     
     2 small = 160
     2 medium = 320
     2 large = 400
     5 gaps = 75
     2 margins = 50
     
     total width = 1340
     
     */
    auto leftMarginGap = getWidth() * 0.031;
    auto borderWidthGap = getWidth() * 0.01492;
    auto smallBorderWidth = getWidth() * 0.0796;
    auto mediumBorderWidth = smallBorderWidth * 2.0;
    auto largeBorderWidth = getWidth() * 0.19901;
    auto borderHeight = getHeight() *  0.7576;
    auto borderY = getHeight() * 0.164436;

    filterBorder.setBounds(leftMarginGap, borderY, smallBorderWidth, borderHeight);
    earlyReflectionsBorder.setBounds(filterBorder.getRight() + borderWidthGap, borderY, largeBorderWidth, borderHeight);
    reverbBorder.setBounds(earlyReflectionsBorder.getRight() + borderWidthGap, borderY, largeBorderWidth, borderHeight);
    gateBorder.setBounds(reverbBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    tremoloBorder.setBounds(gateBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    outBorder.setBounds(tremoloBorder.getRight() + borderWidthGap, borderY, smallBorderWidth, borderHeight);

}
