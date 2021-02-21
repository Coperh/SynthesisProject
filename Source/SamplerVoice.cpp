/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Feb 2021 3:30:45pm
    Author:  chold

  ==============================================================================
*/


#include "SamplerVoice.h"

CustomSamplerVoice::CustomSamplerVoice() {

    //createWavetable();

    // setup sample


    
    juce::WavAudioFormat wavFormat;

    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(
        new juce::MemoryInputStream(BinaryData::sine441_wav, BinaryData::sine441_wavSize, false), true));

    baseFreqency = 440.0;
    
    if (reader.get() != nullptr)
    {
        auto duration = (float)reader->lengthInSamples / reader->sampleRate;

        if (duration < 2)
        {


            fileBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples); 
            reader->read(&fileBuffer,                                                     
                0,                                                              
                (int)reader->lengthInSamples,                                    
                0,                                                                
                true,                                                             
                true);                                                           
            position = 0.0;                                                                   

        }
        else
        {
            // handle the error that the file is 2 seconds or longer..
        }
    }
    else juce::Logger::outputDebugString("-- Could not read file");

}


bool CustomSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}


void CustomSamplerVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // get note as a playback rate
    offset = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / baseFreqency;

    adsr.noteOn();
}

void CustomSamplerVoice::stopNote(float velocity, bool allowTailOff)
{
    position = 0;
    adsr.noteOff();
}



void CustomSamplerVoice::pitchWheelMoved(int newPitchWheelValue) {};
void CustomSamplerVoice::controllerMoved(int controllerNumber, int newControllerValue) {};




void CustomSamplerVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outPutChannels) {

    
   

    adsr.setSampleRate(sampleRate);

    // setup adsr
    adsrParams.attack = 0.1f;
    adsrParams.sustain = 1.0f;
    adsrParams.decay = 0.0f;
    adsrParams.release = 0.2f;

    adsr.setParameters(adsrParams);
    
    isPrepared = true;

    juce::Logger::outputDebugString("-- Is prepared");
}


void CustomSamplerVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    // assert that the prepare method was called
    jassert(isPrepared);
 
    if (!isVoiceActive()) return;
    

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    synthBuffer.clear();


    auto numInputChannels = fileBuffer .getNumChannels();
    auto numOutputChannels = synthBuffer.getNumChannels();

    //juce::Logger::outputDebugString("File: "+std::to_string(numInputChannels) +" Buff: "+ std::to_string(numOutputChannels));




    if(adsr.isActive()){

        for (auto  sample = 0;  sample < synthBuffer.getNumSamples();  ++sample) {
    
            int index = static_cast <int> (std::floor(position));

            // Highest multiple of the offset below the max
            // number of samples divided by the offset floored multiplied by the offset
            

            for (auto channel = 0; channel < numOutputChannels; ++channel) {

                auto writePointer = synthBuffer.getWritePointer(channel);
                // possible that there might be more output channels
                auto readPointer = fileBuffer.getReadPointer(channel);

                writePointer[sample] = readPointer[index];

            }
       





            position = position + offset;

            if (static_cast <int> (std::ceil(position)) == fileBuffer.getNumSamples())
                position = 0.0;
   
        }
    }


    adsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

    // adds buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        outputBuffer.copyFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive()) clearCurrentNote();


    }
    
}


