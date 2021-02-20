/*
  ==============================================================================

    SamplerVoice.h
    Created: 20 Feb 2021 3:30:59pm
    Author:  chold

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SamplerSound.h"

class CustomSamplerVoice : public juce::SamplerVoice{

public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound* sound, int currentPitchWheelPosition);

    void stopNote(float velocity, bool allowTailOff);


    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;


    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
        int startSample, int numSamples) override;





};