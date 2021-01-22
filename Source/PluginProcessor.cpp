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
                       )
#endif
{


    for (int i = 0; i < 4; i++) {
        synthesiser.addVoice(new SynthVoice());
    }

    synthesiser.addSound(new SynthSound());


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

    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
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

    // gets midi buffer from keyboar state and inserts it into current buffer
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),true);

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
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


