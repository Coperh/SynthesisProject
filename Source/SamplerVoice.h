/*
  ==============================================================================

    SamplerVoice.h
    Created: 20 Feb 2021 3:30:59pm
    Author:  chold

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class CustomSamplerVoice : public juce::SynthesiserVoice {

public:

    CustomSamplerVoice();


    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound* sound, int currentPitchWheelPosition);

    void stopNote(float velocity, bool allowTailOff);


    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;


    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
        int startSample, int numSamples) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outPutChannels);

private:

    // imports all samples
    void importSamples();
    // import individual sample
    void readSample(const void* sourceData, int sourceDatasize, int index);


    float currentLevel = 0.0f, previousLevel = 0.0f;
    
    
    int position = 0;



    bool isPrepared = false;

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    juce::AudioBuffer<float> synthBuffer;


    // buffer for notes
    static const int numNotes = 14;

    std::array<juce::AudioSampleBuffer, numNotes> notesBuffer;

    int currentNoteIndex = 0;


    juce::AudioSampleBuffer * currentSampleBuffer;
};