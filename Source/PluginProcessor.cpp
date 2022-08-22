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
    treeState.addParameterListener("er mix", this);
    treeState.addParameterListener("er type", this);
    treeState.addParameterListener("er speed", this);
    treeState.addParameterListener("er depth", this);
    treeState.addParameterListener("size", this);
    treeState.addParameterListener("freeze", this);
    treeState.addParameterListener("predelay", this);
    treeState.addParameterListener("damp", this);
    treeState.addParameterListener("gate", this);
    treeState.addParameterListener("threshold", this);
    treeState.addParameterListener("ratio", this);
    treeState.addParameterListener("attack", this);
    treeState.addParameterListener("release", this);
    treeState.addParameterListener("tremolo", this);
    treeState.addParameterListener("tremPrePost", this);
    treeState.addParameterListener("distortion", this);
    treeState.addParameterListener("wave", this);
    treeState.addParameterListener("lfo one depth", this);
    treeState.addParameterListener("lfo one rate", this);
    treeState.addParameterListener("main mix", this);
    treeState.addParameterListener("width", this);
}

COLOURAUMAudioProcessor::~COLOURAUMAudioProcessor()
{
    treeState.removeParameterListener("hiPass", this);
    treeState.removeParameterListener("loPass", this);
    treeState.removeParameterListener("er mix", this);
    treeState.removeParameterListener("er type", this);
    treeState.removeParameterListener("er speed", this);
    treeState.removeParameterListener("er depth", this);
    treeState.removeParameterListener("size", this);
    treeState.removeParameterListener("freeze", this);
    treeState.removeParameterListener("predelay", this);
    treeState.removeParameterListener("damp", this);
    treeState.removeParameterListener("gate", this);
    treeState.removeParameterListener("threshold", this);
    treeState.removeParameterListener("ratio", this);
    treeState.removeParameterListener("attack", this);
    treeState.removeParameterListener("release", this);
    treeState.removeParameterListener("tremolo", this);
    treeState.removeParameterListener("tremPrePost", this);
    treeState.removeParameterListener("distortion", this);
    treeState.removeParameterListener("wave", this);
    treeState.removeParameterListener("lfo one depth", this);
    treeState.removeParameterListener("lfo one rate", this);
    treeState.removeParameterListener("main mix", this);
    treeState.removeParameterListener("width", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout COLOURAUMAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //LFO One waveform names
    juce::StringArray waveformSelector = {"Sine", "Triangle", "Sloped Square", "Ring"};
    
    auto pHighPassFreq = std::make_unique<juce::AudioParameterFloat>("hiPass", "HiPass",
                                                                    juce::NormalisableRange<float> (20.0, 2000.0, 1.0, 0.45), 20.0,
                                                                    juce::String(),
                                                                    juce::AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) {return (value < 1000.0) ? juce::String (value, 0) + " Hz" : juce::String (value / 1000.0f, 2) + " kHz";},
                                                                    [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});;
    
    auto pLowPassFreq = std::make_unique<juce::AudioParameterFloat>("loPass", "LoPass", juce::NormalisableRange<float> (5000.0, 20000.0, 1.0, 0.45), 20000.0,
                                                                    juce::String(),
                                                                    juce::AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) {return (value < 10000.0) ? juce::String (value / 1000.0f, 2) + " kHz" : juce::String (value / 1000.0f, 1) + " kHz";},
                                                                    [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pEarlyMix = std::make_unique<juce::AudioParameterFloat> ("er mix", "ER Mix", juce::NormalisableRange<float> (0.0, 100.0, 1.0, 1.0),
                                                                  50.0,
                                                                  juce::String(),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value, 0) + " %";},
                                                                  [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pEarlySelection = std::make_unique<juce::AudioParameterInt>("er type", "ER Type", 0, 5, 1);
    
    auto pERSpeed = std::make_unique<juce::AudioParameterFloat> ("er speed", "ER Speed", juce::NormalisableRange<float>(0.01, 100.0, 0.01, 0.4),
                                                                 0.01,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {return (value < 10.0f) ? juce::String (value, 2) + " Hz" : ((value == 100.0f) ? juce::String (value, 0) + " Hz" : juce::String (value, 1) + " Hz" );},
                                                                 [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pERDepth = std::make_unique<juce::AudioParameterFloat> ("er depth", "ER Depth", juce::NormalisableRange<float> (0.0, 100.0, 1.0, 1.0),
                                                                 0.0,
                                                                 juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {return juce::String (value, 0) + " %";},
                                                                 [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    
    auto pSize = std::make_unique<juce::AudioParameterFloat> ("size", "Size", juce::NormalisableRange<float> (1.0, 100.0, 0.01, 1.0),
                                                              40.0,
                                                              juce::String(),
                                                              juce::AudioProcessorParameter::genericParameter,
                                                              [](float value, int) {return (value < 100.0) ? juce::String (value, 1) + " %" : juce::String (value, 0) + " %";},
                                                              [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});

    auto pFreeze = std::make_unique<juce::AudioParameterBool> ("freeze", "Freeze", false);
    
    auto pPredelay = std::make_unique<juce::AudioParameterFloat> ("predelay", "Predelay", juce::NormalisableRange<float>(0.0, 200.0, 1.0, 1.0),
                                                                  0.0,
                                                                  juce::String(),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value, 0) + " ms";},
                                                                  [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    
    auto pDamp = std::make_unique<juce::AudioParameterFloat> ("damp", "Damp", juce::NormalisableRange<float> (0.0, 100.0, 1.0, 1.0),
                                                              50.0,
                                                              juce::String(),
                                                              juce::AudioProcessorParameter::genericParameter,
                                                              [](float value, int) {return juce::String (value, 0) + " %";},
                                                              [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pGateOnOff = std::make_unique<juce::AudioParameterBool> ("gate", "Gate", false);
    auto pThres = std::make_unique<juce::AudioParameterFloat> ("threshold", "Threshold", juce::NormalisableRange<float> (-20.0, 0.0, 0.1, 1.0),
                                                               0.0,
                                                               juce::String(),
                                                               juce::AudioProcessorParameter::genericParameter,
                                                               [](float value, int) {return juce::String (value, 1) + " dB";},
                                                               [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pRatio = std::make_unique<juce::AudioParameterFloat> ("ratio", "Ratio", juce::NormalisableRange<float>(1.0, 5.0, 0.1, 1.0),
                                                               1.0,
                                                               juce::String(),
                                                               juce::AudioProcessorParameter::genericParameter,
                                                               [](float value, int) {return (value == 1.0) ? juce::String (value, 0) + ":1" :  juce::String (value, 1) + ":1";},
                                                               [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    auto pAtt = std::make_unique<juce::AudioParameterFloat> ("attack", "Attack", juce::NormalisableRange<float>(1.0, 200.0, 1.0, 1.0),
                                                             20.0,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {return juce::String (value, 0) + " ms";},
                                                             [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pRel = std::make_unique<juce::AudioParameterFloat> ("release", "Release", juce::NormalisableRange<float>(1.0, 3000.0, 1.0, 0.5),
                                                             300.0,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {return (value < 1000) ? juce::String (value, 0) + " ms" : juce::String (value / 1000, 2) + " s";},
                                                             [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pTremOnOff = std::make_unique<juce::AudioParameterBool>("tremolo", "Tremolo", true);
    auto pTremPrePost = std::make_unique<juce::AudioParameterBool>("tremPrePost", "TremPrePost", true);
    auto pTremDist = std::make_unique<juce::AudioParameterFloat> ("distortion", "Distortion", juce::NormalisableRange<float> (0.0, 100.0, 1.0, 0.5),
                                                             0.0,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value, 0) + " %";},
                                                             [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pWaveform = std::make_unique<juce::AudioParameterChoice>("wave", "Wave", waveformSelector, 0);
    auto pDepthOne = std::make_unique<juce::AudioParameterFloat>("lfo one depth",
                                                                     "LFO 1 Depth",
                                                                     juce::NormalisableRange<float>(0.00, 100.0, 1.0, 1.0),
                                                                     10.0,
                                                                     juce::String(),
                                                                     juce::AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {return juce::String (value, 0) + " %";},
                                                                     [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
        
    auto pFreqOne = std::make_unique<juce::AudioParameterFloat>("lfo one rate",
                                                                    "LFO 1 Rate",
                                                                    juce::NormalisableRange<float>(0.01, 100.0, 0.01, 0.4),
                                                                    8.00,
                                                                    juce::String(),
                                                                    juce::AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) {return (value < 10.0f) ? juce::String (value, 2) + " Hz" : ((value == 100.0f) ? juce::String (value, 0) + " Hz" : juce::String (value, 1) + " Hz" );},
                                                                    [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pMix = std::make_unique<juce::AudioParameterFloat> ("main mix", "Main Mix", juce::NormalisableRange<float> (0.0, 100.0, 0.01, 1.0),
                                                             100.0,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {return (value < 100.0) ? juce::String (value, 1) + " %" : juce::String (value, 0) + " %";},
                                                             [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pWidth = std::make_unique<juce::AudioParameterFloat> ("width", "Width", juce::NormalisableRange<float> (0.0, 100.0, 0.01, 1.0),
                                                               80.0,
                                                               juce::String(),
                                                               juce::AudioProcessorParameter::genericParameter,
                                                               [](float value, int) {return (value < 100.0) ? juce::String (value, 1) + " %" : juce::String (value, 0) + " %";},
                                                               [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    params.push_back(std::move(pHighPassFreq));
    params.push_back(std::move(pLowPassFreq));
    params.push_back(std::move(pEarlyMix));
    params.push_back(std::move(pEarlySelection));
    params.push_back(std::move(pERSpeed));
    params.push_back(std::move(pERDepth));
    params.push_back(std::move(pSize));
    params.push_back(std::move(pFreeze));
    params.push_back(std::move(pPredelay));
    params.push_back(std::move(pDamp));
    params.push_back(std::move(pGateOnOff));
    params.push_back(std::move(pThres));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pAtt));
    params.push_back(std::move(pRel));
    params.push_back(std::move(pTremOnOff));
    params.push_back(std::move(pTremPrePost));
    params.push_back(std::move(pTremDist));
    params.push_back(std::move(pWaveform));
    params.push_back(std::move(pDepthOne));
    params.push_back(std::move(pFreqOne));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pWidth));

    return { params.begin(), params.end() };
}

void COLOURAUMAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    predelayMS.setTargetValue(treeState.getRawParameterValue("predelay")->load());
    myDepthOnePercentage = treeState.getRawParameterValue("lfo one depth")->load(); //getting 0 - 100 from dial
    myDepthOne = juce::jmap(myDepthOnePercentage, 0.0f, 100.0f, 0.0f, 1.0f); // converting to 0 - 1
    depthOne.setTargetValue(myDepthOne);
    freqOne.setTargetValue(treeState.getRawParameterValue("lfo one rate")->load());
    if(parameterID == "wave")
    {
        waveform = newValue;
    }
    if(parameterID == "er type")
    {
        erSelection = newValue;
        earlyTimesSelection(static_cast<int>(newValue));
    }
    if (parameterID == "distortion")
    {
        tubeInPercentage = newValue;
        tubeInGain = juce::jmap(tubeInPercentage, 0.0f, 100.0f, 0.0f, 12.0f); //converting tube percentage to decibels
        rawInput = juce::Decibels::decibelsToGain(tubeInGain);
    }
    updateParams();
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
    
    //prep
    highPassFilter.prepare(spec);
    highPassFilter.reset();
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
    earlyLowPassFilter.prepare(spec);
    earlyLowPassFilter.reset();
    earlyMixModule.prepare(spec);
    earlyMixModule.reset();
    reverbModule.prepare(spec);
    reverbModule.reset();
    gateModule.prepare(spec);
    gateModule.reset();
    mixModule.prepare(spec);
    mixModule.reset();
    
    depthOne.reset(sampleRate, 0.0001);
    freqOne.reset(sampleRate, 0.0001);
    lfoOnePhase.reset(sampleRate, 0.001);
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
    predelay.setFs(sampleRate);
    predelay.setDelaySamples(0.0f);
    predelayMS.reset(sampleRate, 0.0001);
    earlyTimesSelection(static_cast<int>(erSelection));
    earlyReflectionsPrep();
    
    predelayMS.setCurrentAndTargetValue(treeState.getRawParameterValue("predelay")->load());
    erSelection = treeState.getRawParameterValue("er type")->load();
    myDepthOnePercentage = treeState.getRawParameterValue("lfo one depth")->load(); //getting 0 - 100 from dial
    myDepthOne = juce::jmap(myDepthOnePercentage, 0.0f, 100.0f, 0.0f, 1.0f);
    depthOne.setCurrentAndTargetValue(myDepthOne);
    freqOne.setCurrentAndTargetValue(treeState.getRawParameterValue("lfo one rate")->load());
    waveform = treeState.getRawParameterValue("wave")->load();
    lfoOnePhase = 0.0;
    inverseSampleRate = 1.0 / sampleRate;
    
    updateParams();

    // for fractional delays
    Fs = sampleRate;
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
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();
    
    mixModule.pushDrySamples(input);
    
    highPassFilter.process(context);
    lowPassFilter.process(context);
    
    if(tremPrePost)
    {
        preDelayProcesing(block);
        earlyReflectionsProcessing(buffer);
        reverbModule.process(context);
        if(gateOnOff) { gateModule.process(context); }
        if(tremOnOff) { if(tubeInPercentage > 0.0) { processDistortion(block); } tremoloProcessing(buffer); }
    }
    else
    {
        if(tremOnOff) { if(tubeInPercentage > 0.0) { processDistortion(block); } tremoloProcessing(buffer); }
        preDelayProcesing(block);
        earlyReflectionsProcessing(buffer);
        reverbModule.process(context);
        if(gateOnOff) { gateModule.process(context); }
    }
    
    mixModule.mixWetSamples(output);
}

void COLOURAUMAudioProcessor::earlyReflectionsPrep()
{
    //ER prep
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
    
}

void COLOURAUMAudioProcessor::preDelayProcesing(juce::dsp::AudioBlock<float>& block)
{
//    predelay.setDepth(erDepth);
//    predelay.setSpeed(erSpeed);
    float predelaySec = predelayMS.getNextValue() * 0.001;
    float predelaySamples = predelaySec * Fs;
    predelay.setDelaySamples(predelaySamples);
    
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = predelay.processSample(x, channel, 1.0);
        }
    }
}

void COLOURAUMAudioProcessor::earlyTimesSelection(int selection)
{
    switch (selection)
    {
        case 0: // small room
            earlyAMS = 3.0;
            earlyBMS = 7.0;
            earlyCMS = 4.0;
            earlyDMS = 10.0;
            earlyEMS = 35.0;
            earlyFMS = 55.0;
            earlyAGain = 0.9;
            earlyBGain = 0.9;
            earlyCGain = 0.9;
            earlyDGain = 0.8;
            earlyEGain = 0.6;
            earlyFGain = 0.6;
            earlyFilterValue = 16000.0;
            break;
            
        case 1: // medium room
            earlyAMS = 7.0;
            earlyBMS = 16.0;
            earlyCMS = 23.0;
            earlyDMS = 35.0;
            earlyEMS = 50.0;
            earlyFMS = 55.0;
            earlyAGain = 0.9;
            earlyBGain = 0.8;
            earlyCGain = 0.9;
            earlyDGain = 0.8;
            earlyEGain = 0.7;
            earlyFGain = 0.6;
            earlyFilterValue = 15000.0;
            break;
            
        case 2: // large room
            earlyAMS = 12.0;
            earlyBMS = 21.0;
            earlyCMS = 30.0;
            earlyDMS = 50.0;
            earlyEMS = 65.0;
            earlyFMS = 150.0;
            earlyAGain = 0.9;
            earlyBGain = 0.8;
            earlyCGain = 0.9;
            earlyDGain = 0.8;
            earlyEGain = 0.7;
            earlyFGain = 0.6;
            earlyFilterValue = 14000.0;
            break;
            
        case 3: // large Hall
            earlyAMS = 20.0;
            earlyBMS = 53.0;
            earlyCMS = 30.0;
            earlyDMS = 69.0;
            earlyEMS = 70.0;
            earlyFMS = 180.0;
            earlyAGain = 1.0;
            earlyBGain = 0.7;
            earlyCGain = 1.0;
            earlyDGain = 0.6;
            earlyEGain = 0.6;
            earlyFGain = 0.4;
            earlyFilterValue = 13000.0;
            break;
            
        case 4: // huge space
            earlyAMS = 40.0;
            earlyBMS = 60.0;
            earlyCMS = 55.0;
            earlyDMS = 85.0;
            earlyEMS = 130.0;
            earlyFMS = 280.0;
            earlyAGain = 0.9;
            earlyBGain = 0.8;
            earlyCGain = 0.9;
            earlyDGain = 0.8;
            earlyEGain = 0.7;
            earlyFGain = 0.6;
            earlyFilterValue = 12000.0;
            break;
            
        case 5: // extremely late
            earlyAMS = 80.0;
            earlyBMS = 100.0;
            earlyCMS = 90.0;
            earlyDMS = 150.0;
            earlyEMS = 170.0;
            earlyFMS = 350.0;
            earlyAGain = 0.9;
            earlyBGain = 0.8;
            earlyCGain = 0.9;
            earlyDGain = 0.8;
            earlyEGain = 0.7;
            earlyFGain = 0.6;
            earlyFilterValue = 11000.0;
            break;
            
        default:
            break;
    }
}

void COLOURAUMAudioProcessor::earlyReflectionsProcessing(juce::AudioBuffer<float> &buffer)
{
    juce::dsp::AudioBlock<float> erBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> erContext (erBlock);
    const auto& erInput = erContext.getInputBlock();
    const auto& erOutput= erContext.getOutputBlock();
    
    earlyMixModule.pushDrySamples(erInput);
    
    earlyReflectionsPrep();
    // early reflections for loop for stereo placement// no need for channel outer for loop
    float localWidth = treeState.getRawParameterValue("width")->load();
    localWidth = juce::jmap(localWidth, 0.0f, 100.0f, 0.0f, 1.0f);
    const auto coef_M = 1/std::fmax(1 + localWidth, 2);
    const auto coef_S = localWidth * coef_M;
    
    auto* leftData = erBlock.getChannelPointer(0);
    auto* rightData = erBlock.getChannelPointer(1);

    for(int sample = 0; sample < erBlock.getNumSamples(); ++sample)
    {
        float left = leftData[sample];
        float right = rightData[sample];
        
        leftData[sample] = earlyA.processSample(left, 0 , earlyAGain) + earlyB.processSample(left, 0, earlyBGain) + earlyE.processSample(left, 0, earlyEGain) + earlyF.processSample(left, 0, earlyFGain);
        rightData[sample] = earlyC.processSample(right, 1, earlyCGain) + earlyD.processSample(right, 1, earlyDGain) + earlyE.processSample(right, 1, earlyEGain) + earlyF.processSample(right, 1, earlyFGain);
        leftData[sample] *= 0.7;
        rightData[sample] *= 0.7;
        
        const auto mid = coef_M * (leftData[sample] + rightData[sample]);
        const auto side = coef_S * (rightData[sample] - leftData[sample]);

        leftData[sample] = mid - side;
        rightData[sample] = mid + side;
    }
    earlyLowPassFilter.process(erContext);
    earlyMixModule.mixWetSamples(erOutput);
}

//LFO one (main tremolo LFO) waveform selection
float COLOURAUMAudioProcessor::lfoOne(float phase, int choice)
{
    switch (choice) {
        case 0:
            // sine wave
            return 0.5 + 0.5 * sinf(twoPiMultiplier * phase);
            break;
        case 1:
            // Triangle
            if(phase < 0.25f)
                return 0.5f + 2.0f*phase;
            else if(phase < 0.75f)
                return 1.0f - 2.0f*(phase - 0.25f);
            else
                return 2.0f*(phase-0.75f);
            break;
        case 2:
            // Square with sloped edges
            if(phase < 0.48f)
                return 1.0f;
            else if(phase < 0.5f)
                return 1.0f - 50.0f*(phase - 0.48f);
            else if(phase < 0.98f)
                return 0.0f;
            else
                return 50.0f*(phase - 0.98f);
        case 3:
            // ring mod
            return sinf(twoPiMultiplier * phase);
            break;
            
        default:
            return 0.5f + 0.5f * sinf(twoPiMultiplier * phase);
            break;
    }
}

// distortion process
void COLOURAUMAudioProcessor::processDistortion(juce::dsp::AudioBlock<float>& block)
{
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer(channel);
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            data[sample] = tubeData(data[sample]);
        }
    }
}
// softclip algorithim (rounded)
float COLOURAUMAudioProcessor::tubeData(float samples)
{
        samples *= rawInput * 1.6;
        
        if (samples < 0.0)
        {
            samples = piDivisor * std::atan(samples);
        }
        else if (std::abs(samples) > 1.0)
        {
            // if true then this will output 1 (or -1)
            samples *= 1.0 / std::abs(samples);
        }
        samples = piDivisor * std::atan(samples);
    
//    samples = piDivisor * std::atan(samples);
    return samples;
}

void COLOURAUMAudioProcessor::tremoloProcessing(juce::AudioBuffer<float> &buffer)
{
    //LFO One parameters
    myDepthOnePercentage = *treeState.getRawParameterValue("lfo one depth"); //getting 0 - 100 from dial
    myDepthOne = juce::jmap(myDepthOnePercentage, 0.0f, 100.0f, 0.0f, 1.0f); // converting to 0 - 1
    depthOne.setTargetValue(myDepthOne);
    float myFreqOne = *treeState.getRawParameterValue("lfo one rate");
    freqOne.setTargetValue(myFreqOne);

    float currentDepthOne = depthOne.getNextValue();
    float currentFrequencyOne = freqOne.getNextValue();
    DBG(currentFrequencyOne);
    float phaseOne = lfoOnePhase.getNextValue();
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        phaseOne = lfoOnePhase.getNextValue();

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float in = channelData[sample];
            // Tremolo
            float out = in * (1 - currentDepthOne + currentDepthOne * lfoOne(phaseOne, waveform));
            channelData[sample] = out;

            // Update the carrier and LFO One phases, keeping them in the range 0-1.
            phaseOne += currentFrequencyOne * inverseSampleRate;
            if (phaseOne >= 1.0)
            phaseOne -= 1.0;
        }
    }
    lfoOnePhase = phaseOne;
}

void COLOURAUMAudioProcessor::updateParams()
{
    // pre filters
    highPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    lowPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
    
    // ERs
    earlyLowPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    earlyLowPassFilter.setCutoffFrequency(earlyFilterValue);
    erMixValue = treeState.getRawParameterValue("er mix")->load();
    earlyMixModule.setWetMixProportion(juce::jmap(erMixValue, 0.0f, 100.0f, 0.0f, 1.0f));
    erSpeed = treeState.getRawParameterValue("er speed")->load();
    erDepth = treeState.getRawParameterValue("er depth")->load();
    
    // reverb
    sizeValue = treeState.getRawParameterValue("size")->load();
    reverbParams.roomSize = juce::jmap(sizeValue, 1.0f, 100.0f, 0.0f, 1.0f);
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    dampValue = treeState.getRawParameterValue("damp")->load();
    reverbParams.damping = juce::jmap(dampValue, 0.0f, 100.0f, 0.0f, 1.0f);
    widthValue = treeState.getRawParameterValue("width")->load();
    reverbParams.width = juce::jmap(widthValue, 0.0f, 100.0f, 0.0f, 1.0f);
    reverbParams.wetLevel = 1.0;
    reverbModule.setParameters(reverbParams);
    
    //gate
    gateOnOff = treeState.getRawParameterValue("gate")->load();
    gateModule.setThreshold(treeState.getRawParameterValue("threshold")->load());
    gateModule.setRatio(treeState.getRawParameterValue("ratio")->load());
    gateModule.setAttack(treeState.getRawParameterValue("attack")->load());
    gateModule.setRelease(treeState.getRawParameterValue("release")->load());
    
    //tremolo
    tremOnOff = treeState.getRawParameterValue("tremolo")->load();
    tremPrePost = treeState.getRawParameterValue("tremPrePost")->load();
    tubeInPercentage = treeState.getRawParameterValue("distortion")->load();
    tubeInGain = juce::jmap(tubeInPercentage, 0.0f, 100.0f, 0.0f, 12.0f); //converting tube percentage to decibels
    rawInput = juce::Decibels::decibelsToGain(tubeInGain);
    
    // Main Mix
    mainMixValue = treeState.getRawParameterValue("main mix")->load();
    mixModule.setWetMixProportion(juce::jmap(mainMixValue, 0.0f, 100.0f, 0.0f, 1.0f));
}

//==============================================================================
bool COLOURAUMAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* COLOURAUMAudioProcessor::createEditor()
{
    return new COLOURAUMAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor (*this);
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
