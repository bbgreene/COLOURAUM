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
    treeState.addParameterListener("filters", this);
    treeState.addParameterListener("hiPass", this);
    treeState.addParameterListener("loPass", this);
    treeState.addParameterListener("chorus", this);
    treeState.addParameterListener("rate", this);
    treeState.addParameterListener("depth", this);
    treeState.addParameterListener("delay", this);
    treeState.addParameterListener("feedback", this);
    treeState.addParameterListener("chorusMix", this);
    treeState.addParameterListener("reverb", this);
    treeState.addParameterListener("er", this);
    treeState.addParameterListener("er type", this);
    treeState.addParameterListener("predelay", this);
    treeState.addParameterListener("er speed", this);
    treeState.addParameterListener("er depth", this);
    treeState.addParameterListener("size", this);
    treeState.addParameterListener("damp", this);
    treeState.addParameterListener("width", this);
    treeState.addParameterListener("blend", this);
    treeState.addParameterListener("freeze", this);
    treeState.addParameterListener("gate", this);
    treeState.addParameterListener("threshold", this);
    treeState.addParameterListener("ratio", this);
    treeState.addParameterListener("attack", this);
    treeState.addParameterListener("release", this);
    treeState.addParameterListener("main mix", this);
}

COLOURAUMAudioProcessor::~COLOURAUMAudioProcessor()
{
    treeState.removeParameterListener("filters", this);
    treeState.removeParameterListener("hiPass", this);
    treeState.removeParameterListener("loPass", this);
    treeState.removeParameterListener("chorus", this);
    treeState.removeParameterListener("rate", this);
    treeState.removeParameterListener("depth", this);
    treeState.removeParameterListener("delay", this);
    treeState.removeParameterListener("feedback", this);
    treeState.removeParameterListener("chorusMix", this);
    treeState.removeParameterListener("reverb", this);
    treeState.removeParameterListener("er", this);
    treeState.removeParameterListener("er type", this);
    treeState.removeParameterListener("predelay", this);
    treeState.removeParameterListener("er speed", this);
    treeState.removeParameterListener("er depth", this);
    treeState.removeParameterListener("size", this);
    treeState.removeParameterListener("damp", this);
    treeState.removeParameterListener("width", this);
    treeState.removeParameterListener("blend", this);
    treeState.removeParameterListener("freeze", this);
    treeState.removeParameterListener("gate", this);
    treeState.removeParameterListener("threshold", this);
    treeState.removeParameterListener("ratio", this);
    treeState.removeParameterListener("attack", this);
    treeState.removeParameterListener("release", this);
    treeState.removeParameterListener("main mix", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout COLOURAUMAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pFiltersOnOff = std::make_unique<juce::AudioParameterBool> ("filters", "Filters", false);
    auto pHighPassFreq = std::make_unique<juce::AudioParameterFloat> ("hiPass", "HiPass", 20.0, 2000.0, 20.0);
    auto pLowPassFreq = std::make_unique<juce::AudioParameterFloat> ("loPass", "LoPass", 5000.0, 20000.0, 20000.0);
    
    auto pChorusOnOff = std::make_unique<juce::AudioParameterBool> ("chorus", "Chorus", false);
    auto pRate = std::make_unique<juce::AudioParameterFloat>("rate", "Rate", juce::NormalisableRange<float>(0.0, 50.0, 0.1, 0.4), 0.7);
    auto pDepth = std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0, 1.0, 0.5);
    auto pDelay = std::make_unique<juce::AudioParameterFloat>("delay", "Delay", 0.0, 50.0, 9.0);
    auto pFeedback = std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", -1, 1.0, 0.3);
    auto pChorusMix = std::make_unique<juce::AudioParameterFloat>("chorusMix", "ChorusMix", 0, 1.0, 0.7);
    
    auto pReverbOnOff = std::make_unique<juce::AudioParameterBool> ("reverb", "Reverb", true);
    auto pEarlyOnOff = std::make_unique<juce::AudioParameterBool> ("er", "ER", true);
    auto pEarlySelection = std::make_unique<juce::AudioParameterInt>("er type", "ER Type", 0, 5, 0);
    auto pSize = std::make_unique<juce::AudioParameterFloat> ("size", "Size", 0.0, 1.0, 0.45);
    auto pPredelay = std::make_unique<juce::AudioParameterFloat> ("predelay", "Predelay", juce::NormalisableRange<float>(0.0, 200.0, 1.0, 1.0), 0.0);
    auto pPreSpeed = std::make_unique<juce::AudioParameterFloat> ("er speed", "ER Speed", 0.0, 200.0, 0.01);
    auto pPreDepth = std::make_unique<juce::AudioParameterFloat> ("er depth", "ER Depth", 0.0, 1000.0, 0.01);
    auto pDamp = std::make_unique<juce::AudioParameterFloat> ("damp", "Damp", 0.0, 1.0, 0.97);
    auto pWidth = std::make_unique<juce::AudioParameterFloat> ("width", "Width", 0.0, 1.0, 0.55);
    auto pBlend = std::make_unique<juce::AudioParameterFloat> ("blend", "Blend", 0.0, 1.0, 0.5); // reverb wet
    auto pFreeze = std::make_unique<juce::AudioParameterBool> ("freeze", "Freeze", false);
    
    auto pGateOnOff = std::make_unique<juce::AudioParameterBool> ("gate", "Gate", false);
    auto pThres = std::make_unique<juce::AudioParameterFloat> ("threshold", "Threshold", -20.0, 0.0, 0.0);
    auto pRatio = std::make_unique<juce::AudioParameterFloat> ("ratio", "Ratio", 1.0, 5.0, 1.0);
    auto pAtt = std::make_unique<juce::AudioParameterFloat> ("attack", "Attack", 1.0, 1000.0, 1.0);
    auto pRel = std::make_unique<juce::AudioParameterFloat> ("release", "Release", 1.0, 3000.0, 1.0);
    
    auto pMix = std::make_unique<juce::AudioParameterFloat> ("main mix", "Main Mix", 0.0, 1.0, 1.0); // overall mix
    
    params.push_back(std::move(pFiltersOnOff));
    params.push_back(std::move(pHighPassFreq));
    params.push_back(std::move(pLowPassFreq));
    params.push_back(std::move(pChorusOnOff));
    params.push_back(std::move(pRate));
    params.push_back(std::move(pDepth));
    params.push_back(std::move(pDelay));
    params.push_back(std::move(pFeedback));
    params.push_back(std::move(pChorusMix));
    params.push_back(std::move(pReverbOnOff));
    params.push_back(std::move(pEarlyOnOff));
    params.push_back(std::move(pEarlySelection));
    params.push_back(std::move(pPredelay));
    params.push_back(std::move(pPreSpeed));
    params.push_back(std::move(pPreDepth));
    params.push_back(std::move(pSize));
    params.push_back(std::move(pDamp));
    params.push_back(std::move(pWidth));
    params.push_back(std::move(pBlend));
    params.push_back(std::move(pFreeze));
    params.push_back(std::move(pGateOnOff));
    params.push_back(std::move(pThres));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pAtt));
    params.push_back(std::move(pRel));
    params.push_back(std::move(pMix));
    
    return { params.begin(), params.end() };
}

void COLOURAUMAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    // pre filters
    filtersOnOff = treeState.getRawParameterValue("filters")->load();
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
    
    // chorus params
    chorusOnOff = treeState.getRawParameterValue("chorus")->load();
    chorusModule.setRate(treeState.getRawParameterValue("rate")->load());
    chorusModule.setDepth(treeState.getRawParameterValue("depth")->load());
    chorusModule.setCentreDelay(treeState.getRawParameterValue("delay")->load());
    chorusModule.setFeedback(treeState.getRawParameterValue("feedback")->load());
    chorusModule.setMix(treeState.getRawParameterValue("chorusMix")->load());
    
    //reverb params
    reverbOnOff = treeState.getRawParameterValue("reverb")->load();
    earlyOnOff = treeState.getRawParameterValue("er")->load();
    if(parameterID == "er type")
    {
        erSelection = newValue;
        earlyTimesSelection(static_cast<int>(newValue));
    }
    erSpeed = treeState.getRawParameterValue("er speed")->load();
    erDepth = treeState.getRawParameterValue("er depth")->load();
    predelayMS.setTargetValue(treeState.getRawParameterValue("predelay")->load());
//    predelayMS = treeState.getRawParameterValue("predelay")->load();
    
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("blend")->load();
//    reverbParams.dryLevel = 1.0f - treeState.getRawParameterValue("mix")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
    
    //gate params
    gateOnOff = treeState.getRawParameterValue("gate")->load();
    gateModule.setThreshold(treeState.getRawParameterValue("threshold")->load());
    gateModule.setRatio(treeState.getRawParameterValue("ratio")->load());
    gateModule.setAttack(treeState.getRawParameterValue("attack")->load());
    gateModule.setRelease(treeState.getRawParameterValue("release")->load());
    
    // Mix module mix param
    mixModule.setWetMixProportion(treeState.getRawParameterValue("main mix")->load());
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
    
    // pre filters and prep
    filtersOnOff = treeState.getRawParameterValue("filters")->load();
    highPassFilter.prepare(spec);
    highPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    
    lowPassFilter.prepare(spec);
    lowPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
    
    // chorus params and prep
    chorusOnOff = treeState.getRawParameterValue("chorus")->load();
    chorusModule.prepare(spec);
    chorusModule.setRate(treeState.getRawParameterValue("rate")->load());
    chorusModule.setDepth(treeState.getRawParameterValue("depth")->load());
    chorusModule.setCentreDelay(treeState.getRawParameterValue("delay")->load());
    chorusModule.setFeedback(treeState.getRawParameterValue("feedback")->load());
    chorusModule.setMix(treeState.getRawParameterValue("chorusMix")->load());
    
    // for fractional delays
    Fs = sampleRate;
    
    //Early Reflections
    erSelection = treeState.getRawParameterValue("er type")->load();
    earlyTimesSelection(static_cast<int>(erSelection));
    earlyA.setFs(sampleRate);
    earlyA.setDelaySamples(0.0f);
    earlyB.setFs(sampleRate);
    earlyB.setDelaySamples(0.0f);
    earlyC.setFs(sampleRate);
    earlyC.setDelaySamples(0.0f);
    earlyD.setFs(sampleRate);
    earlyD.setDelaySamples(0.0f);
    earlyE.setFs(sampleRate);
    earlyE.setDelaySamples(0.0f);
    earlyF.setFs(sampleRate);
    earlyF.setDelaySamples(0.0f);
    earlyOnOff = treeState.getRawParameterValue("er")->load();
    erSpeed = treeState.getRawParameterValue("er speed")->load();
    erDepth = treeState.getRawParameterValue("er depth")->load();
    
    //Predelay
    predelay.setFs(sampleRate);
    predelay.setDelaySamples(0.0f);
    predelayMS.reset(sampleRate, 0.002);
    predelayMS.setCurrentAndTargetValue(treeState.getRawParameterValue("predelay")->load());
//    predelayMS = treeState.getRawParameterValue("predelay")->load();
    
    
    // reverb params and prep
    reverbOnOff = treeState.getRawParameterValue("reverb")->load();
    reverbModule.prepare(spec);
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("blend")->load();
//    reverbParams.dryLevel = 1.0f - treeState.getRawParameterValue("mix")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
    
    // gate params and prep
    gateOnOff = treeState.getRawParameterValue("gate")->load();
    gateModule.prepare(spec);
    gateModule.setThreshold(treeState.getRawParameterValue("threshold")->load());
    gateModule.setRatio(treeState.getRawParameterValue("ratio")->load());
    gateModule.setAttack(treeState.getRawParameterValue("attack")->load());
    gateModule.setRelease(treeState.getRawParameterValue("release")->load());
    
    // Mix module mix param
    mixModule.prepare(spec);
    mixModule.setWetMixProportion(treeState.getRawParameterValue("main mix")->load());
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
    
    //ERs
    earlyA.setDepth(erDepth);
    earlyA.setSpeed(erSpeed);
    float earlyASec = earlyAMS * 0.001;
    float earlyASamples = earlyASec * Fs;
    earlyA.setDelaySamples(earlyASamples);
    
    earlyB.setDepth(erDepth);
    earlyB.setSpeed(erSpeed);
    float earlyBSec = earlyBMS * 0.001;
    float earlyBSamples = earlyBSec * Fs;
    earlyB.setDelaySamples(earlyBSamples);
    
    earlyC.setDepth(erDepth);
    earlyC.setSpeed(erSpeed);
    float earlyCSec = earlyCMS * 0.001;
    float earlyCSamples = earlyCSec * Fs;
    earlyC.setDelaySamples(earlyCSamples);
    
    earlyD.setDepth(erDepth);
    earlyD.setSpeed(erSpeed);
    float earlyDSec = earlyDMS * 0.001;
    float earlyDSamples = earlyDSec * Fs;
    earlyD.setDelaySamples(earlyDSamples);
    
    earlyE.setDepth(erDepth);
    earlyE.setSpeed(erSpeed);
    float earlyESec = earlyEMS * 0.001;
    float earlyESamples = earlyESec * Fs;
    earlyE.setDelaySamples(earlyESamples);
    
    earlyF.setDepth(erDepth);
    earlyF.setSpeed(erSpeed);
    float earlyFSec = earlyFMS * 0.001;
    float earlyFSamples = earlyFSec * Fs;
    earlyF.setDelaySamples(earlyFSamples);
    //move this outside of process block?
        
    //Predelay
    predelay.setDepth(0.0);
    predelay.setSpeed(0.0);
    //move this outside of process block?
    float predelaySec = predelayMS.getNextValue() * 0.001;
    float predelaySamples = predelaySec * Fs;
    predelay.setDelaySamples(predelaySamples);
    
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("blend")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();
    
    mixModule.pushDrySamples(input);
    
    if (filtersOnOff) { highPassFilter.process(context); lowPassFilter.process(context); }
    if (chorusOnOff) { chorusModule.process(context); }
    
    if (reverbOnOff)
    {
        if(earlyOnOff)
        {// early reflections for loop for stereo placement// no need for channel outer for loop
            auto* leftData = block.getChannelPointer(0);
            auto* rightData = block.getChannelPointer(1);

            for(int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float left = leftData[sample];
                float right = rightData[sample];
                leftData[sample] = earlyA.processSample(left, 0 , 0.9) + earlyB.processSample(left, 0, 0.8) + earlyE.processSample(left, 0, 0.7) + earlyF.processSample(left, 0, 0.5);
                rightData[sample] = earlyC.processSample(right, 1, 0.9) + earlyD.processSample(right, 1, 0.8) + earlyE.processSample(right, 1, 0.7) + earlyF.processSample(right, 1, 0.6);
                leftData[sample] *= 0.5;
                rightData[sample] *= 0.5;
            }
        }
        
        //Pre delay for loop
        for (int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer(channel);

            for(int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float x = channelData[sample];
                channelData[sample] = predelay.processSample(x, channel, 1.0);
            }
        }
        reverbModule.process(context);
    }
    if (gateOnOff) { gateModule.process(context); }

    mixModule.mixWetSamples(output);
}

void COLOURAUMAudioProcessor::earlyTimesSelection(int selection)
{
    switch (selection)
    {
        case 0:
        earlyAMS = 10.0;
        earlyBMS = 25.0;
        earlyCMS = 55.0;
        earlyDMS = 76.0;
        earlyEMS = 23.0;
        earlyFMS = 90.0;
        break;
            
        case 1:
        earlyAMS = 7.0;
        earlyBMS = 15.0;
        earlyCMS = 23.0;
        earlyDMS = 33.0;
        earlyEMS = 50.0;
        earlyFMS = 55.0;
        break;
            
        case 2:
        earlyAMS = 20.0;
        earlyBMS = 41.0;
        earlyCMS = 50.0;
        earlyDMS = 70.0;
        earlyEMS = 53.0;
        earlyFMS = 98.0;
        break;
            
        case 3:
        earlyAMS = 25.0;
        earlyBMS = 35.0;
        earlyCMS = 60.0;
        earlyDMS = 82.0;
        earlyEMS = 23.0;
        earlyFMS = 90.0;
        break;
            
        default:
            break;
    }
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
    juce::MemoryOutputStream mos(destData, true);
    treeState.state.writeToStream(mos);
}

void COLOURAUMAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if ( tree.isValid() )
    {
        treeState.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new COLOURAUMAudioProcessor();
}
