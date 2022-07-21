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
    treeState.addParameterListener("hiPass", this);
    treeState.addParameterListener("loPass", this);
    treeState.addParameterListener("size", this);
    treeState.addParameterListener("damp", this);
    treeState.addParameterListener("width", this);
    treeState.addParameterListener("mix", this);
    treeState.addParameterListener("freeze", this);
}

COLOURAUMAudioProcessor::~COLOURAUMAudioProcessor()
{
    treeState.addParameterListener("hiPass", this);
    treeState.addParameterListener("loPass", this);
    treeState.removeParameterListener("size", this);
    treeState.removeParameterListener("damp", this);
    treeState.removeParameterListener("width", this);
    treeState.removeParameterListener("mix", this);
    treeState.removeParameterListener("freeze", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout COLOURAUMAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pHighPassFreq = std::make_unique<juce::AudioParameterFloat> ("hiPass", "HiPass", 20.0, 2000.0, 20.0);
    auto pLowPassFreq = std::make_unique<juce::AudioParameterFloat> ("loPass", "LoPass", 5000.0, 20000.0, 20000.0);
    auto pSize = std::make_unique<juce::AudioParameterFloat> ("size", "Size", 0.0, 1.0, 0.0);
    auto pDamp = std::make_unique<juce::AudioParameterFloat> ("damp", "Damp", 0.0, 1.0, 0.0);
    auto pWidth = std::make_unique<juce::AudioParameterFloat> ("width", "Width", 0.0, 1.0, 0.0);
    auto pMix = std::make_unique<juce::AudioParameterFloat> ("mix", "Mix", 0.0, 1.0, 0.0);
    auto pFreeze = std::make_unique<juce::AudioParameterBool> ("freeze", "Freeze", false);
    
    params.push_back(std::move(pHighPassFreq));
    params.push_back(std::move(pLowPassFreq));
    params.push_back(std::move(pSize));
    params.push_back(std::move(pDamp));
    params.push_back(std::move(pWidth));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pFreeze));
    
    return { params.begin(), params.end() };
}

void COLOURAUMAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
    
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("mix")->load();
    reverbParams.dryLevel = 1.0f - treeState.getRawParameterValue("mix")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
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
    
    reverbModule.prepare(spec);
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("mix")->load();
    reverbParams.dryLevel = 1.0f - treeState.getRawParameterValue("mix")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
    
    highPassFilter.prepare(spec);
    highPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    
    lowPassFilter.prepare(spec);
    lowPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
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

    
    
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("mix")->load();
    reverbParams.dryLevel = 1.0f - treeState.getRawParameterValue("mix")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    
    reverbModule.setParameters(reverbParams);
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    highPassFilter.process(context);
    lowPassFilter.process(context);
    reverbModule.process(context);

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
