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
    triangleOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));

    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
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
    triangleOsc.prepare(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.5f);

    adsrParams.attack = 0.1f;
    adsrParams.sustain = 1.0f;
    adsrParams.decay = 0.0f;
    adsrParams.release = 0.2f;


    adsr.setParameters(adsrParams);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.setCutoffFrequency(1000.0f);

    filter.prepare(spec);
    filter.reset();


   

    isPrepared = true;

}


void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);


    if (!isVoiceActive()) return;

    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    synthBuffer.clear();


    // allias for the output buffer

    juce::dsp::AudioBlock< float > audioBlock{ synthBuffer };
    sawOsc.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});

    triangleOsc.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});

    gain.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});

    filter.process(juce::dsp::ProcessContextReplacing<float>{audioBlock});

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    // adds buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel,0, numSamples);

        if (!adsr.isActive()) clearCurrentNote();
    
    
    }


}