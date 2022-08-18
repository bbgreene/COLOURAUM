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
    erType.setText("Small Room");
    erType.addItem("Small Room", 1);
    erType.addItem("Medium Room", 2);
    erType.addItem("Large Room", 3);
    erType.addItem("Hall", 4);
    erType.addItem("Large Hall", 5);
    erType.addItem("Huge", 6);
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
    
    tremoloAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "tremolo", tremOnOffButton);
    addAndMakeVisible(tremOnOffButton);
    tremoloPrePostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "tremPrePost", tremPrePostButton);
    addAndMakeVisible(tremPrePostButton);
    
    sineButton.setClickingTogglesState(true);
    sineButton.setRadioGroupId(1);
    addAndMakeVisible(sineButton);
    
    triButton.setClickingTogglesState(true);
    triButton.setRadioGroupId(1);
    addAndMakeVisible(triButton);
    
    squareButton.setClickingTogglesState(true);
    squareButton.setRadioGroupId(1);
    addAndMakeVisible(squareButton);
    
    ringButton.setClickingTogglesState(true);
    ringButton.setRadioGroupId(1);
    addAndMakeVisible(ringButton);

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
    waveLabel.attachToComponent(&sineButton, false);
    tremoRateLabel.attachToComponent(&tremoRate, false);
    tremoDepthLabel.attachToComponent(&tremoDepth, false);
    mixLabel.attachToComponent(&mix, false);
    widthLabel.attachToComponent(&width, false);
    
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
     
     total width = 1065
     
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
    
    auto smallDialSize = getWidth() * 0.07;
    auto bigDialSize = getWidth() * 0.102412;
    auto erTypeWidth = getWidth() * 0.095;
    auto erTypeHeight = getHeight() * 0.109492;
    auto waveButtonWidth = getWidth() * 0.0660485;
    auto waveButtonHeight = getHeight() * 0.073715;
    auto onOffButtonWidth = getWidth() * 0.0437792;
    auto topRowY = getHeight() * 0.313055;
    auto bottomRowY = getHeight() * 0.634681;

    auto filtersX = filterBorder.getX() * 1.32125;
    auto earlyVolumeX = earlyReflectionsBorder.getX() * 1.11316;
    auto erTypeX = earlyReflectionsBorder.getX() * 1.08;
    auto erTypeY = earlyReflectionsBorder.getY() * 4.2815;
    auto earlyModParamsX = earlyReflectionsBorder.getX() * 1.859;
    auto sizeX = reverbBorder.getX() * 1.03;
    auto sizeY = reverbBorder.getY() * 1.91164;
    auto freezeX = reverbBorder.getX() * 1.03457;
    auto freezeY = reverbBorder.getY() * 4.30707;
    auto predelayDampX = reverbBorder.getX() * 1.343;
    auto gateOnOffX = gateBorder.getX() * 1.00501;
    auto gateOnOffY = gateBorder.getY() * 0.632637;
    auto thresRatioX = gateBorder.getX() * 1.03;
    auto attRelX = gateBorder.getX() * 1.17;
    auto tremOnOffX = tremoloBorder.getX() * 1.003;
    auto tremOnOffY = tremoloBorder.getY() * 0.632637;
    auto tremPrePostX = tremoloBorder.getX() * 1.177;
    auto waveButtonX = tremoloBorder.getX() * 1.02546;
    auto waveButtonY = tremoloBorder.getY() * 2.29699;
    auto waveButtonGapY = tremoloBorder.getY() * 0.263916;
    auto tremoRateDepthX = tremoloBorder.getX() * 1.125;
    auto mixWidthX = outBorder.getX() * 1.01;
    
    highPass.setBounds(filtersX, topRowY, smallDialSize, smallDialSize);
    lowPass.setBounds(filtersX, bottomRowY, smallDialSize, smallDialSize);
    
    earlyVolume.setBounds(earlyVolumeX, topRowY, smallDialSize, smallDialSize);
    erType.setBounds(erTypeX, erTypeY, erTypeWidth, erTypeHeight);
    earlyRate.setBounds(earlyModParamsX, topRowY, smallDialSize, smallDialSize);
    earlyDepth.setBounds(earlyModParamsX, bottomRowY, smallDialSize, smallDialSize);
    
    size.setBounds(sizeX, sizeY, bigDialSize, bigDialSize);
    freeze.setBounds(freezeX, freezeY, erTypeWidth, erTypeHeight);
    predelay.setBounds(predelayDampX, topRowY, smallDialSize, smallDialSize);
    damp.setBounds(predelayDampX, bottomRowY, smallDialSize, smallDialSize);
    
    GateOnOffButton.setBounds(gateOnOffX, gateOnOffY, onOffButtonWidth, waveButtonHeight);
    threshold.setBounds(thresRatioX, topRowY, smallDialSize, smallDialSize);
    ratio.setBounds(thresRatioX, bottomRowY, smallDialSize, smallDialSize);
    attack.setBounds(attRelX, topRowY, smallDialSize, smallDialSize);
    release.setBounds(attRelX, bottomRowY, smallDialSize, smallDialSize);
    
    tremOnOffButton.setBounds(tremOnOffX, tremOnOffY, onOffButtonWidth, waveButtonHeight);
    tremPrePostButton.setBounds(tremPrePostX, tremOnOffY, onOffButtonWidth, waveButtonHeight);
    sineButton.setBounds(waveButtonX, waveButtonY, waveButtonWidth, waveButtonHeight);
    triButton.setBounds(waveButtonX, sineButton.getBottom() + waveButtonGapY, waveButtonWidth, waveButtonHeight);
    squareButton.setBounds(waveButtonX, triButton.getBottom() + waveButtonGapY, waveButtonWidth, waveButtonHeight);
    ringButton.setBounds(waveButtonX, squareButton.getBottom() + waveButtonGapY, waveButtonWidth, waveButtonHeight);
    tremoRate.setBounds(tremoRateDepthX, topRowY, smallDialSize, smallDialSize);
    tremoDepth.setBounds(tremoRateDepthX, bottomRowY, smallDialSize, smallDialSize);
    
    mix.setBounds(mixWidthX, topRowY, smallDialSize, smallDialSize);
    width.setBounds(mixWidthX, bottomRowY, smallDialSize, smallDialSize);

}
