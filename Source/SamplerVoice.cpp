/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Feb 2021 3:30:45pm
    Author:  chold

  ==============================================================================
*/


#include "SamplerVoice.h"

CustomSamplerVoice::CustomSamplerVoice() {



    // setup sample
    importSamples();


    currentSampleBuffer = &notesBuffer[0];
    for(int i =0; i < 14; i++) juce::Logger::outputDebugString("samples " + std::to_string(notesBuffer[i].getNumSamples()));
    
}


bool CustomSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}


void CustomSamplerVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    position = 0;


    // switch index to coresepsonding note 
    switch (midiNoteNumber) {
    
    case 62:
        currentNoteIndex = 0;
        break;
    case 64:
        currentNoteIndex = 1;
        break;
    case 65:
        currentNoteIndex = 2;
        break;
    case 67:
        currentNoteIndex = 3;
        break;
    case 69:
        currentNoteIndex = 4;
        break;
    case 71:
        currentNoteIndex = 5;
        break;
    case 72:
        currentNoteIndex = 6;
        break;
    case 73:
        currentNoteIndex = 7;
        break;
    case 74:
        currentNoteIndex = 8;
        break;
    case 76:
        currentNoteIndex = 9;
        break;
    case 77:
        currentNoteIndex = 10;
        break;
    case 79:
        currentNoteIndex = 11;
        break;
    case 81:
        currentNoteIndex = 12;
        break;
    case 84:
        currentNoteIndex = 13;
        break;
    }


    adsr.noteOn();
}

void CustomSamplerVoice::stopNote(float velocity, bool allowTailOff)
{
    
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


    auto numInputChannels = notesBuffer[currentNoteIndex].getNumChannels();
    auto numOutputChannels = synthBuffer.getNumChannels();

    //juce::Logger::outputDebugString("File: "+std::to_string(numInputChannels) +" Buff: "+ std::to_string(numOutputChannels));


    if(adsr.isActive()){

        for (auto  sample = 0;  sample < synthBuffer.getNumSamples();  ++sample) {
 

            // Highest multiple of the offset below the max
            // number of samples divided by the offset floored multiplied by the offset
            

            for (auto channel = 0; channel < numOutputChannels; ++channel) {

                auto writePointer = synthBuffer.getWritePointer(channel);
                // possible that there might be more output channels
                auto readPointer = notesBuffer[currentNoteIndex].getReadPointer(channel);

                writePointer[sample] = readPointer[position];

            }
       


            position++;

            if (static_cast <int> (std::ceil(position)) == notesBuffer[currentNoteIndex].getNumSamples())
                position = 0;
   
        }
    }


    adsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

    // adds buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        outputBuffer.copyFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive()) clearCurrentNote();


    }
    
}



void CustomSamplerVoice::importSamples() {



    // d5.wav
    readSample(BinaryData::d5_wav, BinaryData::d5_wavSize, 0);

    // e5.wav
    readSample(BinaryData::e5_wav, BinaryData::e5_wavSize, 1);
    // f5.wav
    readSample(BinaryData::f5_wav, BinaryData::f5_wavSize, 2);
    // g5.wa
    readSample(BinaryData::g5_wav, BinaryData::g5_wavSize, 3);
    // a5.wav
    readSample(BinaryData::a5_wav, BinaryData::a5_wavSize, 4);
    // b5.wav
    readSample(BinaryData::b5_wav, BinaryData::b5_wavSize, 5);
    // c6.wav
    readSample(BinaryData::c6_wav, BinaryData::c6_wavSize, 6);
    // cS6.wav
    readSample(BinaryData::cS6_wav, BinaryData::cS6_wavSize, 7);
    // d6.wav
    readSample(BinaryData::d6_wav, BinaryData::d6_wavSize, 8);
    // e6.wav
    readSample(BinaryData::e6_wav, BinaryData::e6_wavSize, 9);
    // f6.wav
    readSample(BinaryData::f6_wav, BinaryData::f6_wavSize, 10);
    // g6.wav
    readSample(BinaryData::g6_wav, BinaryData::g6_wavSize, 11);
    // a6.wav
    readSample(BinaryData::a6_wav, BinaryData::a6_wavSize, 12);
    // c7.wav
    readSample(BinaryData::c7_wav, BinaryData::c7_wavSize, 13);





}





void CustomSamplerVoice::readSample(const void * sourceData, int sourceDatasize, int index) {
    juce::WavAudioFormat wavFormat;

    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(
        new juce::MemoryInputStream(sourceData, sourceDatasize, false), true));


    if (reader.get() != nullptr)
    {
        auto duration = (float)reader->lengthInSamples / reader->sampleRate;

        if (duration < 2)
        {

            notesBuffer[index].setSize((int)reader->numChannels, (int)reader->lengthInSamples);
            reader->read(&notesBuffer[index],
                0,
                (int)reader->lengthInSamples,
                0,
                true,
                true);
            position = 0.0;

        }
        else
        {
            

        }
        juce::Logger::outputDebugString("-- Read file");
    }
    else juce::Logger::outputDebugString("-- Could not read file");

}

