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
    
    treeState.addParameterListener("wave", this);
    treeState.addParameterListener("lfo one depth", this);
    treeState.addParameterListener("lfo one rate", this);

    treeState.addParameterListener("reverb", this);
    treeState.addParameterListener("er", this);
    treeState.addParameterListener("er type", this);
    treeState.addParameterListener("er mix", this);
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
    
    treeState.removeParameterListener("wave", this);
    treeState.removeParameterListener("lfo one depth", this);
    treeState.removeParameterListener("lfo one rate", this);

    treeState.removeParameterListener("reverb", this);
    treeState.removeParameterListener("er", this);
    treeState.removeParameterListener("er type", this);
    treeState.removeParameterListener("er mix", this);
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
    
    //LFO One waveform names
    juce::StringArray waveformSelector = {"Sine", "Triangle", "Sloped Square", "Ring"};
    
    auto pFiltersOnOff = std::make_unique<juce::AudioParameterBool> ("filters", "Filters", false);
    auto pHighPassFreq = std::make_unique<juce::AudioParameterFloat> ("hiPass", "HiPass", 20.0, 2000.0, 20.0);
    auto pLowPassFreq = std::make_unique<juce::AudioParameterFloat> ("loPass", "LoPass", 5000.0, 20000.0, 20000.0);
    
    auto pWaveform = std::make_unique<juce::AudioParameterChoice>("wave", "Wave", waveformSelector, 0);
    auto pDepthOne = std::make_unique<juce::AudioParameterFloat>("lfo one depth",
                                                                     "LFO 1 Depth",
                                                                     juce::NormalisableRange<float>(0.00, 100.0, 0.1, 1.0),
                                                                     0.0,
                                                                     juce::String(),
                                                                     juce::AudioProcessorParameter::genericParameter,
                                                                     [](float value, int) {return (value < 100.0f) ? juce::String (value, 1) + " %" : juce::String (value, 0) + " %";},
                                                                     [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
        
    auto pFreqOne = std::make_unique<juce::AudioParameterFloat>("lfo one rate",
                                                                    "LFO 1 Rate",
                                                                    juce::NormalisableRange<float>(0.01, 100.0, 0.01, 0.4),
                                                                    0.01,
                                                                    juce::String(),
                                                                    juce::AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) {return (value < 10.0f) ? juce::String (value, 2) + " Hz" : ((value == 100.0f) ? juce::String (value, 0) + " Hz" : juce::String (value, 1) + " Hz" );},
                                                                    [](juce::String text) {return text.dropLastCharacters (3).getFloatValue();});
    
    auto pReverbOnOff = std::make_unique<juce::AudioParameterBool> ("reverb", "Reverb", true);
    auto pEarlyOnOff = std::make_unique<juce::AudioParameterBool> ("er", "ER", true);
    auto pEarlySelection = std::make_unique<juce::AudioParameterInt>("er type", "ER Type", 0, 5, 0);
    auto pEarlyMix = std::make_unique<juce::AudioParameterFloat> ("er mix", "ER Mix", 0.0, 1.0, 0.5);
    auto pPredelay = std::make_unique<juce::AudioParameterFloat> ("predelay", "Predelay", juce::NormalisableRange<float>(0.0, 200.0, 1.0, 1.0), 0.0);
    auto pPreSpeed = std::make_unique<juce::AudioParameterFloat> ("er speed", "ER Speed", 0.0, 200.0, 0.01);
    auto pPreDepth = std::make_unique<juce::AudioParameterFloat> ("er depth", "ER Depth", 0.0, 1000.0, 0.01);
    auto pSize = std::make_unique<juce::AudioParameterFloat> ("size", "Size", 0.0, 1.0, 0.45);
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
    
    params.push_back(std::move(pWaveform));
    params.push_back(std::move(pDepthOne));
    params.push_back(std::move(pFreqOne));

    params.push_back(std::move(pReverbOnOff));
    params.push_back(std::move(pEarlyOnOff));
    params.push_back(std::move(pEarlySelection));
    params.push_back(std::move(pEarlyMix));
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
    updateParams();
    
    predelayMS.setTargetValue(treeState.getRawParameterValue("predelay")->load());
    depthOne.setTargetValue(treeState.getRawParameterValue("lfo one depth")->load());
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
    lowPassFilter.prepare(spec);
    reverbModule.prepare(spec);
    gateModule.prepare(spec);
    mixModule.prepare(spec);
    earlyMixModule.prepare(spec);
    
    depthOne.reset(sampleRate, 0.001);
    freqOne.reset(sampleRate, 0.001);
    lfoOnePhase.reset(sampleRate, 0.001);
    earlyTimesSelection(static_cast<int>(erSelection));
    earlyReflectionsPrep();
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
    predelayMS.reset(sampleRate, 0.002);
    
    updateParams();
    
    predelayMS.setCurrentAndTargetValue(treeState.getRawParameterValue("predelay")->load());
    erSelection = treeState.getRawParameterValue("er type")->load();
    depthOne.setCurrentAndTargetValue(treeState.getRawParameterValue("lfo one depth")->load());
    freqOne.setCurrentAndTargetValue(treeState.getRawParameterValue("lfo one rate")->load());
    waveform = treeState.getRawParameterValue("wave")->load();
    lfoOnePhase = 0.0;
    inverseSampleRate = 1.0 / sampleRate;

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
    
    if (filtersOnOff) { highPassFilter.process(context); lowPassFilter.process(context); }
    preDelayProcesing(block);
    if(earlyOnOff){ earlyReflectionsProcessing(buffer); }
    if (reverbOnOff) { reverbModule.process(context); }
    if (gateOnOff) { gateModule.process(context); }
    tremoloProcessing(buffer);

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
    predelay.setDepth(0.0);
    predelay.setSpeed(0.0);
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
            break;
            
        case 3: // large Hall
            earlyAMS = 20.0;
            earlyBMS = 53.0;
            earlyCMS = 30.0;
            earlyDMS = 69.0;
            earlyEMS = 70.0;
            earlyFMS = 180.0;
            earlyAGain = 0.9;
            earlyBGain = 0.6;
            earlyCGain = 0.9;
            earlyDGain = 0.6;
            earlyEGain = 0.5;
            earlyFGain = 0.4;
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
        leftData[sample] *= 0.5;
        rightData[sample] *= 0.5;
        
        const auto mid = coef_M * (leftData[sample] + rightData[sample]);
        const auto side = coef_S * (rightData[sample] - leftData[sample]);

        leftData[sample] = mid - side;
        rightData[sample] = mid + side;
    }
    earlyMixModule.mixWetSamples(erOutput);
}

//LFO one (main tremolo LFO) waveform selection
float COLOURAUMAudioProcessor::lfoOne(float phase, int choice)
{
    switch (choice) {
        case 0:
            // sine wave
            return 0.5 + 0.5 * sinf(2.0 * M_PI * phase);
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
            return sinf(2.0 * M_PI * phase);
            break;
            
        default:
            return 0.5f + 0.5f * sinf(2.0 * M_PI * phase);
            break;
    }
}

void COLOURAUMAudioProcessor::tremoloProcessing(juce::AudioBuffer<float> &buffer)
{
    //LFO One parameters
    float myDepthOnePercentage = *treeState.getRawParameterValue("lfo one depth"); //getting 0 - 100 from dial
    float myDepthOne = juce::jmap(myDepthOnePercentage, 0.0f, 100.0f, 0.0f, 1.0f); // converting to 0 - 1
    depthOne.setTargetValue(myDepthOne);
    float myFreqOne = *treeState.getRawParameterValue("lfo one rate");
    freqOne.setTargetValue(myFreqOne);

    float currentDepthOne = depthOne.getNextValue();
    float currentFrequencyOne = freqOne.getNextValue();
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
    filtersOnOff = treeState.getRawParameterValue("filters")->load();
    highPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilter.setCutoffFrequency(treeState.getRawParameterValue("hiPass")->load());
    lowPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilter.setCutoffFrequency(treeState.getRawParameterValue("loPass")->load());
    
    // ERs
    earlyOnOff = treeState.getRawParameterValue("er")->load();
    earlyMixModule.setWetMixProportion(treeState.getRawParameterValue("er mix")->load());
    erSpeed = treeState.getRawParameterValue("er speed")->load();
    erDepth = treeState.getRawParameterValue("er depth")->load();
    
    // reverb
    reverbOnOff = treeState.getRawParameterValue("reverb")->load();
    reverbParams.roomSize = treeState.getRawParameterValue("size")->load();
    reverbParams.damping = treeState.getRawParameterValue("damp")->load();
    reverbParams.width = treeState.getRawParameterValue("width")->load();
    reverbParams.wetLevel = treeState.getRawParameterValue("blend")->load();
    reverbParams.freezeMode = treeState.getRawParameterValue("freeze")->load();
    reverbModule.setParameters(reverbParams);
    
    //gate
    gateOnOff = treeState.getRawParameterValue("gate")->load();
    gateModule.setThreshold(treeState.getRawParameterValue("threshold")->load());
    gateModule.setRatio(treeState.getRawParameterValue("ratio")->load());
    gateModule.setAttack(treeState.getRawParameterValue("attack")->load());
    gateModule.setRelease(treeState.getRawParameterValue("release")->load());
    
    // Main Mix
    mixModule.setWetMixProportion(treeState.getRawParameterValue("main mix")->load());
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
