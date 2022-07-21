/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
COLOURAUMAudioProcessor::COLOURAUMAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("size", this);
    treeState.addParameterListener("damp", this);
    treeState.addParameterListener("width", this);
    treeState.addParameterListener("mix", this);
    treeState.addParameterListener("freeze", this);
}

COLOURAUMAudioProcessor::~COLOURAUMAudioProcessor()
{
    treeState.removeParameterListener("size", this);
    treeState.removeParameterListener("damp", this);
    treeState.removeParameterListener("width", this);
    treeState.removeParameterListener("mix", this);
    treeState.removeParameterListener("freeze", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout COLOURAUMAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pSize = std::make_unique<juce::AudioParameterFloat> ("size",
                                                                 "Size",
                                                                 juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                                 0.5f,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {
                                                                    if (value * 100 < 10.0f)
                                                                        return juce::String (value * 100, 2);
                                                                    else if (value * 100 < 100.0f)
                                                                        return juce::String (value * 100, 1);
                                                                    else
                                                                        return juce::String (value * 100, 0); },
                                                                 nullptr);

    auto pDamp = std::make_unique<juce::AudioParameterFloat> ("damp",
                                                                 "Damp",
                                                                 juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                                 0.5f,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {
                                                                    if (value * 100 < 10.0f)
                                                                        return juce::String (value * 100, 2);
                                                                    else if (value * 100 < 100.0f)
                                                                        return juce::String (value * 100, 1);
                                                                    else
                                                                        return juce::String (value * 100, 0); },
                                                                 nullptr);


    auto pWidth = std::make_unique<juce::AudioParameterFloat> ("width",
                                                                 "Width",
                                                                 juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                                 0.5f,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {
                                                                    if (value * 100 < 10.0f)
                                                                        return juce::String (value * 100, 2);
                                                                    else if (value * 100 < 100.0f)
                                                                        return juce::String (value * 100, 1);
                                                                    else
                                                                        return juce::String (value * 100, 0); },
                                                                nullptr);

    auto pMix = std::make_unique<juce::AudioParameterFloat> ("mix",
                                                                 "Mix",
                                                                 juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                                 0.5f,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {
                                                                    if (value * 100 < 10.0f)
                                                                        return juce::String (value * 100, 2);
                                                                    else if (value * 100 < 100.0f)
                                                                        return juce::String (value * 100, 1);
                                                                    else
                                                                        return juce::String (value * 100, 0); },
                                                                 nullptr);

    auto pFreeze = std::make_unique<juce::AudioParameterBool> ("freeze", "Freeze", false);
    
    params.push_back(std::move(pSize));
    params.push_back(std::move(pDamp));
    params.push_back(std::move(pWidth));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pFreeze));
    
    return { params.begin(), params.end() };
}

void COLOURAUMAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    reverbParams.roomSize = *treeState.getRawParameterValue("size");
    reverbParams.damping = *treeState.getRawParameterValue("damp");
    reverbParams.width = *treeState.getRawParameterValue("width");
    reverbParams.wetLevel = *treeState.getRawParameterValue("mix");
    reverbParams.dryLevel = 1.0f - *treeState.getRawParameterValue("mix");
    reverbParams.roomSize = *treeState.getRawParameterValue("freeze");
}

//==============================================================================
const juce::String COLOURAUMAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool COLOURAUMAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool COLOURAUMAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool COLOURAUMAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double COLOURAUMAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int COLOURAUMAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int COLOURAUMAudioProcessor::getCurrentProgram()
{
    return 0;
}

void COLOURAUMAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String COLOURAUMAudioProcessor::getProgramName (int index)
{
    return {};
}

void COLOURAUMAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void COLOURAUMAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    leftReverb.prepare(spec);
    rightReverb.prepare(spec);
    
    reverbParams.roomSize = *treeState.getRawParameterValue("size");
    reverbParams.damping = *treeState.getRawParameterValue("damp");
    reverbParams.width = *treeState.getRawParameterValue("width");
    reverbParams.wetLevel = *treeState.getRawParameterValue("mix");
    reverbParams.dryLevel = 1.0f - *treeState.getRawParameterValue("mix");
    reverbParams.freezeMode = *treeState.getRawParameterValue("freeze");
}

void COLOURAUMAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool COLOURAUMAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void COLOURAUMAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    reverbParams.roomSize = *treeState.getRawParameterValue("size");
    reverbParams.damping = *treeState.getRawParameterValue("damp");
    reverbParams.width = *treeState.getRawParameterValue("width");
    reverbParams.wetLevel = *treeState.getRawParameterValue("mix");
    reverbParams.dryLevel = 1.0f - *treeState.getRawParameterValue("mix");
    reverbParams.freezeMode = *treeState.getRawParameterValue("freeze");
    
    leftReverb.setParameters(reverbParams);
    rightReverb.setParameters(reverbParams);
    
    juce::dsp::AudioBlock<float> block (buffer);
    
    auto leftBlock = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);

    juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);

    leftReverb.process (leftContext);
    rightReverb.process (rightContext);
}

//==============================================================================
bool COLOURAUMAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* COLOURAUMAudioProcessor::createEditor()
{
//    return new COLOURAUMAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void COLOURAUMAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void COLOURAUMAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new COLOURAUMAudioProcessor();
}
