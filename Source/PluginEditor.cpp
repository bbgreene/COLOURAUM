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
    
    // TITLE
    colouraumTitle.setFont(juce::Font (26.0f, juce::Font::plain));
    colouraumTitle.setJustificationType(juce::Justification::centredLeft);
    colouraumTitle.setColour(juce::Label::textColourId, juce::Colours::mintcream);
    addAndMakeVisible(colouraumTitle);
    
    colouraumVersion.setFont(juce::Font (15.0f, juce::Font::plain));
    colouraumVersion.setJustificationType(juce::Justification::centredLeft);
    colouraumVersion.setColour(juce::Label::textColourId, juce::Colours::lightslategrey.brighter());
    addAndMakeVisible(colouraumVersion);
    
    olumay.setFont(juce::Font (15.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::steelblue.brighter());
    addAndMakeVisible(olumay);

    setResizable(false, false);
    setSize (1065, 330);
}

COLOURAUMAudioProcessorEditor::~COLOURAUMAudioProcessorEditor()
{
}

//==============================================================================
void COLOURAUMAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
       g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::mediumpurple, getHeight() - getHeight(), juce::Colours::purple.darker(), getHeight()));
       g.fillRect(background);
}

void COLOURAUMAudioProcessorEditor::resized()
{
    /*
     height = 250
     small border = 90
     medium border = 180
     large border = 200
     in between gap = 15
     margin = 25
     
     2 small = 180
     2 medium = 360
     2 large = 400
     5 gaps = 75
     2 margins = 50
     
     total width = 1340
     
     */
    auto leftMarginGap = getWidth() * 0.027;
    auto borderWidthGap = getWidth() * 0.01408;
    auto smallBorderWidth = getWidth() * 0.08451;
    auto mediumBorderWidth = smallBorderWidth * 2.0;
    auto largeBorderWidth = getWidth() * 0.1878;
    auto borderHeight = getHeight() *  0.7576;
    auto borderY = getHeight() * 0.164436;

    filterBorder.setBounds(leftMarginGap, borderY, smallBorderWidth, borderHeight);
    earlyReflectionsBorder.setBounds(filterBorder.getRight() + borderWidthGap, borderY, largeBorderWidth, borderHeight);
    reverbBorder.setBounds(earlyReflectionsBorder.getRight() + borderWidthGap, borderY, largeBorderWidth, borderHeight);
    gateBorder.setBounds(reverbBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    tremoloBorder.setBounds(gateBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    outBorder.setBounds(tremoloBorder.getRight() + borderWidthGap, borderY, smallBorderWidth, borderHeight);
    
    auto colouraumTitleX = leftMarginGap;
    auto colouraumTitleY = 5;
    auto colouraumTitleWidth = 207;
    auto colouraumTitleHeight = 30;
    auto versionX = 233;
    auto versionY = 11;
    auto versionWidth = getWidth() * 0.169246;
    auto versionHeight = getHeight() * 0.068969;
    auto olumayX = getWidth() * 0.894;
    auto olumayY = getHeight() * 0.925818;
        
    colouraumTitle.setBounds(colouraumTitleX, colouraumTitleY, colouraumTitleWidth, colouraumTitleHeight);
    colouraumVersion.setBounds(versionX, versionY, versionWidth, versionHeight);
    olumay.setBounds(olumayX, olumayY, versionWidth, versionHeight);

}
