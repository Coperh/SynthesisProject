/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SamplerVoice.h"
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


    // toggles the drone to the passed state
    void toggleDrone(bool state);


    // connects to the on screen keyboard to get input
    juce::MidiKeyboardState keyboardState;

    // contains the parameters used in automation
    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================


    juce::AudioProcessorValueTreeState::ParameterLayout  createParameters();


    juce::AudioFormatManager formatManager;


    //juce::dsp::Oscillator<float> droneHighPulse{ [](float x) { return x < 0.0f ? -1.0f : 1.0f;  } };
    //juce::dsp::Oscillator<float> droneHighSaw{ [](float x) { return x / juce::MathConstants<float>::pi;  } };
    //juce::dsp::Oscillator<float> droneLowPulse{ [](float x) { return x < 0.0f ? -1.0f : 1.0f;  } };
    //juce::dsp::Oscillator<float> droneLowSaw{ [](float x) { return x / juce::MathConstants<float>::pi;  } };

    float droneFrequency = 146.83f;


    int num_voices = 3;

    juce::Synthesiser syntheiser;



    // Drone Sample things
    void readDroneSample();

    bool samplesRead = false;
    int dronePosition;

    bool droneLastState;

    // drone adsr
    juce::ADSR droneAdsr;
    juce::ADSR::Parameters droneAdsrParams;


    // temp buffer and file buffer
    juce::AudioBuffer<float> droneBuffer;
    juce::AudioBuffer<float> fileBuffer;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UilleannPipesAudioProcessor)

};



