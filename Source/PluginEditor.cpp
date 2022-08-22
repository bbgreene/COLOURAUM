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
    // SET DEFAULT FONT
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Avenir Next");
    
    // DIALS, BUTTONS, MENUS & PARAMETER ATTACHMENTS
    highPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    highPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "hiPass", highPass);
    addAndMakeVisible(highPass);
    lowPass.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    lowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "loPass", lowPass);
    addAndMakeVisible(lowPass);
    earlyVolume.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    earlyVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "er mix", earlyVolume);
    addAndMakeVisible(earlyVolume);
    erType.setText("Small");
    erType.addItem("Small", 1);
    erType.addItem("Medium", 2);
    erType.addItem("Large", 3);
    erType.addItem("Hall", 4);
    erType.addItem("Concert", 5);
    erType.addItem("Huge", 6);
    erTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "er type", erType);
    addAndMakeVisible(erType);
    earlyRate.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(earlyRate);
    earlyRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "er speed", earlyRate);
    earlyDepth.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    earlyDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "er depth", earlyDepth);
    addAndMakeVisible(earlyDepth);
    size.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "size", size);
    addAndMakeVisible(size);
    freeze.setClickingTogglesState(true);
    freezeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "freeze", freeze);
    addAndMakeVisible(freeze);
    predelay.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "predelay", predelay);
    addAndMakeVisible(predelay);
    damp.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    dampAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "damp", damp);
    addAndMakeVisible(damp);
    
    if(static_cast<int>(GateOnOffButton.getToggleState())) { GateOnOffButton.setButtonText("On"); }
    else { GateOnOffButton.setButtonText("Off"); }
    GateOnOffButton.onClick = [this]() { if(static_cast<int>(GateOnOffButton.getToggleState())) { GateOnOffButton.setButtonText("On"); }
    else { GateOnOffButton.setButtonText("Off"); } };
    gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "gate", GateOnOffButton);
    addAndMakeVisible(GateOnOffButton);
    threshold.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "threshold", threshold);
    addAndMakeVisible(threshold);
    ratio.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "ratio", ratio);
    addAndMakeVisible(ratio);
    attack.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "attack", attack);
    addAndMakeVisible(attack);
    release.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "release", release);
    addAndMakeVisible(release);
    
    tremoDistortion.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    tremoDistortionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "distortion", tremoDistortion);
    addAndMakeVisible(tremoDistortion);
    
    if(static_cast<int>(tremOnOffButton.getToggleState())) { tremOnOffButton.setButtonText("On"); }
    else { tremOnOffButton.setButtonText("Off"); }
    tremOnOffButton.onClick = [this]() { if(static_cast<int>(tremOnOffButton.getToggleState())) { tremOnOffButton.setButtonText("On"); }
    else { tremOnOffButton.setButtonText("Off"); } };
    tremoloAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "tremolo", tremOnOffButton);
    addAndMakeVisible(tremOnOffButton);
    
    if(static_cast<int>(tremPrePostButton.getToggleState())) { tremPrePostButton.setButtonText("Post"); }
    else { tremPrePostButton.setButtonText("Pre"); }
    
    tremPrePostButton.onClick = [this]() { if(static_cast<int>(tremPrePostButton.getToggleState())) { tremPrePostButton.setButtonText("Post"); }
    else { tremPrePostButton.setButtonText("Pre"); } };
    tremoloPrePostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "tremPrePost", tremPrePostButton);
    addAndMakeVisible(tremPrePostButton);
    
    waveMenu.setText("Sine");
    waveMenu.addItem("Sine", 1);
    waveMenu.addItem("Triangle", 2);
    waveMenu.addItem("Square", 3);
    waveMenu.addItem("Ring", 4);
    waveMenuAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "wave", waveMenu);
    addAndMakeVisible(waveMenu);

    tremoRate.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    tremoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "lfo one rate", tremoRate);
    addAndMakeVisible(tremoRate);
    tremoDepth.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    tremoDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "lfo one depth", tremoDepth);
    addAndMakeVisible(tremoDepth);
    
    mix.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "main mix", mix);
    addAndMakeVisible(mix);
    width.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "width", width);
    addAndMakeVisible(width);
    
    //DIAL LABEL ATTACHMENTS
    highPassLabel.attachToComponent(&highPass, false);
    lowPassLabel.attachToComponent(&lowPass, false);
    earlyVolumeLabel.attachToComponent(&earlyVolume, false);
    erTypeLabel.attachToComponent(&erType, false);
    earlyRateLabel.attachToComponent(&earlyRate, false);
    earlyDepthLabel.attachToComponent(&earlyDepth, false);
    sizeLabel.attachToComponent(&size, false);
    predelayLabel.attachToComponent(&predelay, false);
    dampLabel.attachToComponent(&damp, false);
    thresholdLabel.attachToComponent(&threshold, false);
    ratioLabel.attachToComponent(&ratio, false);
    attackLabel.attachToComponent(&attack, false);
    releaseLabel.attachToComponent(&release, false);
    tremoDistortionLabel.attachToComponent(&tremoDistortion, false);
    waveLabel.attachToComponent(&waveMenu, false);
    tremoRateLabel.attachToComponent(&tremoRate, false);
    tremoDepthLabel.attachToComponent(&tremoDepth, false);
    mixLabel.attachToComponent(&mix, false);
    widthLabel.attachToComponent(&width, false);
    
    // BORDERS
    filterBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    filterBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    filterBorder.setTextLabelPosition(juce::Justification::centred);
    filterBorder.setText("FILTER");
    addAndMakeVisible(filterBorder);
    
    earlyReflectionsBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    earlyReflectionsBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    earlyReflectionsBorder.setTextLabelPosition(juce::Justification::centred);
    earlyReflectionsBorder.setText("EARLY REFLECTIONS");
    addAndMakeVisible(earlyReflectionsBorder);
    
    reverbBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    reverbBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    reverbBorder.setTextLabelPosition(juce::Justification::centred);
    reverbBorder.setText("REVERB");
    addAndMakeVisible(reverbBorder);
    
    gateBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    gateBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    gateBorder.setTextLabelPosition(juce::Justification::centred);
    gateBorder.setText("GATE");
    addAndMakeVisible(gateBorder);
    
    tremoloBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    tremoloBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    tremoloBorder.setTextLabelPosition(juce::Justification::centred);
    tremoloBorder.setText("TREMOLO");
    addAndMakeVisible(tremoloBorder);
    
    outBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::purple.brighter());
    outBorder.setColour(juce::GroupComponent::ColourIds::textColourId, CustomColours::creamWhite.darker());
    outBorder.setTextLabelPosition(juce::Justification::centred);
    outBorder.setText("OUTPUT");
    addAndMakeVisible(outBorder);
    
    // TITLE
    colouraumTitle.setFont(juce::Font (32.0f, juce::Font::plain));
    colouraumTitle.setJustificationType(juce::Justification::centredLeft);
    colouraumTitle.setColour(juce::Label::textColourId, juce::Colours::lightblue.brighter());
    addAndMakeVisible(colouraumTitle);
    
    colouraumVersion.setFont(juce::Font (18.0f, juce::Font::plain));
    colouraumVersion.setJustificationType(juce::Justification::centredLeft);
    colouraumVersion.setColour(juce::Label::textColourId, juce::Colours::lightslategrey.brighter());
    addAndMakeVisible(colouraumVersion);
    
    olumay.setFont(juce::Font (18.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::lightslategrey.brighter());
    addAndMakeVisible(olumay);

    setResizable(false, false);
    setSize (1045, 330);
}

COLOURAUMAudioProcessorEditor::~COLOURAUMAudioProcessorEditor()
{
}

//==============================================================================
void COLOURAUMAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
       g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::purple.darker(), getHeight() - getHeight(), juce::Colours::purple.darker(), getHeight()));
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
     3 medium = 540
     2 large = 200
     5 gaps = 75
     2 margins = 50
     
     total width = 1045
     
     */
    auto leftMarginGap = getWidth() * 0.02393;
    auto borderWidthGap = getWidth() * 0.01436;
    auto smallBorderWidth = getWidth() * 0.0862;
    auto mediumBorderWidth = smallBorderWidth * 2.0;
    auto largeBorderWidth = getWidth() * 0.1914;
    auto borderHeight = getHeight() *  0.7576;
    auto borderY = getHeight() * 0.164436;

    filterBorder.setBounds(leftMarginGap, borderY, smallBorderWidth, borderHeight);
    earlyReflectionsBorder.setBounds(filterBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    reverbBorder.setBounds(earlyReflectionsBorder.getRight() + borderWidthGap, borderY, largeBorderWidth, borderHeight);
    gateBorder.setBounds(reverbBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    tremoloBorder.setBounds(gateBorder.getRight() + borderWidthGap, borderY, mediumBorderWidth, borderHeight);
    outBorder.setBounds(tremoloBorder.getRight() + borderWidthGap, borderY, smallBorderWidth, borderHeight);
    
    auto colouraumTitleX = leftMarginGap;
    auto colouraumTitleY = 13.2951;
    auto colouraumTitleWidth = 207;
    auto colouraumTitleHeight = 30;
    auto versionX = 233;
    auto versionY = 20.0;
    auto versionWidth = getWidth() * 0.169246;
    auto versionHeight = getHeight() * 0.068969;
    auto olumayX = getWidth() * 0.894;
    auto olumayY = getHeight() * 0.925818;
        
    colouraumTitle.setBounds(colouraumTitleX, colouraumTitleY, colouraumTitleWidth, colouraumTitleHeight);
    colouraumVersion.setBounds(versionX, versionY, versionWidth, versionHeight);
    olumay.setBounds(olumayX, olumayY, versionWidth, versionHeight);
    
    auto smallDialSize = getWidth() * 0.07;
    auto bigDialSize = getWidth() * 0.102412;
    auto onOffButtonHeight = getHeight() * 0.073715;
    auto onOffButtonWidth = getWidth() * 0.03;
    auto waveMenuWidth = getWidth() * 0.07;
    auto waveMenuHeight = getHeight() * 0.07;
    auto topRowY = getHeight() * 0.313055;
    auto bottomRowY = getHeight() * 0.634681;

    auto filtersX = filterBorder.getX() * 1.32125;
    auto earlyVolumeX = earlyReflectionsBorder.getX() * 1.1;
    auto erTypeX = earlyReflectionsBorder.getX() * 1.1;
    auto erTypeY = earlyReflectionsBorder.getY() * 4.2815;
    auto earlyModParamsX = earlyReflectionsBorder.getX() * 1.7;
    auto sizeX = reverbBorder.getX() * 1.02;
    auto sizeY = reverbBorder.getY() * 1.91164;
    auto freezeX = reverbBorder.getX() * 1.07;
    auto freezeY = reverbBorder.getY() * 4.30707;
    auto predelayDampX = reverbBorder.getX() * 1.343;
    auto gateOnOffX = gateBorder.getX() * 1.00428;
    auto gateOnOffY = gateBorder.getY() * 0.632637;
    auto thresRatioX = gateBorder.getX() * 1.027;
    auto attRelX = gateBorder.getX() * 1.17;
    auto tremOnOffX = tremoloBorder.getX() * 1.003;
    auto tremOnOffY = tremoloBorder.getY() * 0.632637;
    auto tremPrePostX = tremoloBorder.getX() * 1.2;
    auto waveMenuX = tremoloBorder.getX() * 1.018;
    auto waveMenuY = tremoloBorder.getY() * 4.26691;
    auto tremoDistX = tremoloBorder.getX() * 1.02;
    auto tremoRateDepthX = tremoloBorder.getX() * 1.125;
    auto mixWidthX = outBorder.getX() * 1.01;
    
    highPass.setBounds(filtersX, topRowY, smallDialSize, smallDialSize);
    lowPass.setBounds(filtersX, bottomRowY, smallDialSize, smallDialSize);
    
    earlyVolume.setBounds(earlyVolumeX, topRowY, smallDialSize, smallDialSize);
    erType.setBounds(erTypeX, erTypeY, waveMenuWidth, waveMenuHeight);
    earlyRate.setBounds(earlyModParamsX, topRowY, smallDialSize, smallDialSize);
    earlyDepth.setBounds(earlyModParamsX, bottomRowY, smallDialSize, smallDialSize);
    
    size.setBounds(sizeX, sizeY, bigDialSize, bigDialSize);
    freeze.setBounds(freezeX, freezeY, waveMenuWidth, waveMenuHeight);
    predelay.setBounds(predelayDampX, topRowY, smallDialSize, smallDialSize);
    damp.setBounds(predelayDampX, bottomRowY, smallDialSize, smallDialSize);
    
    GateOnOffButton.setBounds(gateOnOffX, gateOnOffY, onOffButtonWidth, onOffButtonHeight);
    threshold.setBounds(thresRatioX, topRowY, smallDialSize, smallDialSize);
    ratio.setBounds(thresRatioX, bottomRowY, smallDialSize, smallDialSize);
    attack.setBounds(attRelX, topRowY, smallDialSize, smallDialSize);
    release.setBounds(attRelX, bottomRowY, smallDialSize, smallDialSize);
    
    tremOnOffButton.setBounds(tremOnOffX, tremOnOffY, onOffButtonWidth, onOffButtonHeight);
    tremPrePostButton.setBounds(tremPrePostX, tremOnOffY, onOffButtonWidth, onOffButtonHeight);
    tremoDistortion.setBounds(tremoDistX, topRowY, smallDialSize, smallDialSize);
    waveMenu.setBounds(waveMenuX, waveMenuY, waveMenuWidth, waveMenuHeight);
    tremoRate.setBounds(tremoRateDepthX, topRowY, smallDialSize, smallDialSize);
    tremoDepth.setBounds(tremoRateDepthX, bottomRowY, smallDialSize, smallDialSize);
    
    mix.setBounds(mixWidthX, topRowY, smallDialSize, smallDialSize);
    width.setBounds(mixWidthX, bottomRowY, smallDialSize, smallDialSize);

}
