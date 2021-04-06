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

/*
Header file for the SamplerVoice Class.
This class generates the sound used by the synthesiser.
*/
class CustomSamplerVoice : public juce::SynthesiserVoice {

public:
    // constructor
    CustomSamplerVoice();

    // Returns if the sound object is correct
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    // Required implementation, Called when a midi note is played
    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound* sound, int currentPitchWheelPosition);

    // Required implementation, called when a midi note is released
    void stopNote(float velocity, bool allowTailOff);

    // Required implementation, Called when using modulation controls but not setup
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    // Main Audio processing function
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
        int startSample, int numSamples) override;

    // Setup the Sampler Voice, called at the begining from the Plugin Processor
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outPutChannels);

private:

    // imports all samples
    void importSamples();
    // import individual sample
    void readSample(const void* sourceData, int sourceDatasize, int index);


    

    // if prepare to play been executed sucessfully
    bool isPrepared = false;

    // Amplitude envelope & its parameters
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    // Temporary buffer to write samples to, used in render next block
    juce::AudioBuffer<float> synthBuffer;

    // Number of notes in the buffer
    static const int numNotes = 14;

    // array of Audio sample buffers containing the samples of each notes
    std::array<juce::AudioSampleBuffer, numNotes> notesBuffer;
    // Current possition in the array of samples
    int currentNoteIndex = 0;
    // position in the current sample buffer
    int position = 0;


};