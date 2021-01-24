/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UilleannPipesAudioProcessor::UilleannPipesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters() )
#endif
{
    syntheiser.addSound(new SynthSound());

    syntheiser.addVoice(new SynthVoice());



    droneEnabled = false;
}

UilleannPipesAudioProcessor::~UilleannPipesAudioProcessor()
{
}

//==============================================================================
const juce::String UilleannPipesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UilleannPipesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UilleannPipesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UilleannPipesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UilleannPipesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UilleannPipesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UilleannPipesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UilleannPipesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String UilleannPipesAudioProcessor::getProgramName (int index)
{
    return {};
}

void UilleannPipesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void UilleannPipesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    syntheiser.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < syntheiser.getNumVoices(); i++) {
        
        if (auto voice = dynamic_cast<SynthVoice*>(syntheiser.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
        }
    }


    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getNumOutputChannels();

    droneHighPulse.prepare(spec);
    droneHighPulse.setFrequency(droneFrequency);
    droneHighSaw.prepare(spec);
    droneHighSaw.setFrequency(droneFrequency);

    droneLowPulse.prepare(spec);
    droneLowPulse.setFrequency(droneFrequency/2);
    droneLowSaw.prepare(spec);
    droneLowSaw.setFrequency(droneFrequency / 2);

}

void UilleannPipesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UilleannPipesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void UilleannPipesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    
    // gets midi buffer from keyboar state and inserts it into current buffer
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),true);

    
    for (int i = 0; i < syntheiser.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(syntheiser.getVoice(i))) {
            //adsr
        }
    }
    
    syntheiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock< float > audioBlock{ buffer };


    if (droneEnabled) 
    {
       
        droneHighPulse.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
        droneHighSaw.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
        droneLowPulse.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
        droneLowSaw.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
    
    }


    auto g = apvts.getRawParameterValue("OSC");
   

}

//==============================================================================
bool UilleannPipesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* UilleannPipesAudioProcessor::createEditor()
{
    return new UilleannPipesAudioProcessorEditor (*this);
}

//==============================================================================
void UilleannPipesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UilleannPipesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UilleannPipesAudioProcessor();
}

void UilleannPipesAudioProcessor::toggleDrone(bool state) {
    droneEnabled = state;


    juce::Logger::outputDebugString("Drone: " + std::to_string(droneEnabled));
}

juce::AudioProcessorValueTreeState::ParameterLayout  UilleannPipesAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // combo box switch drone key

    

    //params.push_back(std::make_unique<juce::AudioParameterChoice>("KEYSELECTION", "Key", juce::StringArray{ "D","A" }, 0));

    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.8f));


    return { params.begin(), params.end() };
};
