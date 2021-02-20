/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Feb 2021 3:30:45pm
    Author:  chold

  ==============================================================================
*/


#include "SamplerVoice.h"


bool CustomSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SamplerSound*> (sound) != nullptr;
}


void CustomSamplerVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    
}

void CustomSamplerVoice::stopNote(float velocity, bool allowTailOff)
{

}



void CustomSamplerVoice::pitchWheelMoved(int newPitchWheelValue) {};
void CustomSamplerVoice::controllerMoved(int controllerNumber, int newControllerValue) {};



void CustomSamplerVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    

}