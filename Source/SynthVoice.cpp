/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 Jan 2021 5:22:59pm
    Author:  chold

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    sawOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    squareOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));

    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    
    adsr.noteOff();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {};
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){};


void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outPutChannels) {

    adsr.setSampleRate(sampleRate);


    // give spects to
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outPutChannels;

    sawOsc.prepare(spec);
    squareOsc.prepare(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.1);

    
   

    isPrepared = true;

}


void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);


    // allias for the output buffer
    juce::dsp::AudioBlock< float > audioBlock{ outputBuffer };
    sawOsc.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
    squareOsc.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});
    //gain.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);


}