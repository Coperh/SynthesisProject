/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

using juce::Synthesiser;

//==============================================================================
/**
*/
class UilleannPipesAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    UilleannPipesAudioProcessor();
    ~UilleannPipesAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;



    void toggleDrone(bool state);


    juce::MidiKeyboardState keyboardState;


    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================


    juce::AudioProcessorValueTreeState::ParameterLayout  createParameters();





    juce::dsp::Oscillator<float> droneHighPulse{ [](float x) { return x < 0.0f ? -1.0f : 1.0f;  } };
    juce::dsp::Oscillator<float> droneHighSaw{ [](float x) { return x / juce::MathConstants<float>::pi;  } };
    juce::dsp::Oscillator<float> droneLowPulse{ [](float x) { return x < 0.0f ? -1.0f : 1.0f;  } };
    juce::dsp::Oscillator<float> droneLowSaw{ [](float x) { return x / juce::MathConstants<float>::pi;  } };

    float droneFrequency = 146.83;



    juce::Synthesiser syntheiser;

    
    
    bool droneEnabled;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UilleannPipesAudioProcessor)

};
