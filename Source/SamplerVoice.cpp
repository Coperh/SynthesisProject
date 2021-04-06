/*
  ==============================================================================

    SamplerVoice.cpp
    Created: 20 Feb 2021 3:30:45pm
    Author:  chold

  ==============================================================================
*/


#include "SamplerVoice.h"


/*
The implementation of the SamplerVoice Class.
This class generates the sound used by the synthesiser.
*/


CustomSamplerVoice::CustomSamplerVoice() {



    // import the samples for each note
    importSamples();

    //for(int i =0; i < 14; i++) juce::Logger::outputDebugString("samples " + std::to_string(notesBuffer[i].getNumSamples()));
    
}


bool CustomSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // returns true if sound can be cast
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}


void CustomSamplerVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // reset the position to 0
    position = 0;

    // crude way to implement switching note, does not change if note is not mentioned
    // switch current index to the buffer corresponding to the note
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

    // turn on adsr 
    adsr.noteOn();
}

void CustomSamplerVoice::stopNote(float velocity, bool allowTailOff)
{
    // turn off adsr
    adsr.noteOff();
}


// Required implementation, does nothing
void CustomSamplerVoice::pitchWheelMoved(int newPitchWheelValue) {};
void CustomSamplerVoice::controllerMoved(int controllerNumber, int newControllerValue) {};




void CustomSamplerVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outPutChannels) {

    // pass the sample rate to the adsr
    adsr.setSampleRate(sampleRate);

    // setup the envelope parameter
    adsrParams.attack = 0.1f;
    adsrParams.sustain = 1.0f;
    adsrParams.decay = 0.0f;
    adsrParams.release = 0.2f;
    // give parameters to the adsr
    adsr.setParameters(adsrParams);
    
    // should now be prepared
    isPrepared = true;

    //juce::Logger::outputDebugString("-- Is prepared");
}


void CustomSamplerVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    // assert that the prepare method was called
    jassert(isPrepared);
 
    // if voice is not active, do nothing and return
    if (!isVoiceActive()) return;
    

    // create temporary empty buffer the size of the output buffer
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // get number of channels for temporary and sample buffer
    auto numInputChannels = notesBuffer[currentNoteIndex].getNumChannels();
    auto numOutputChannels = synthBuffer.getNumChannels();

    //juce::Logger::outputDebugString("File: "+std::to_string(numInputChannels) +" Buff: "+ std::to_string(numOutputChannels));

    // if note is active
    if(adsr.isActive()){

        // for sample in the output buffer
        for (auto  sample = 0;  sample < synthBuffer.getNumSamples();  ++sample) {

            // for channels in the output bufer
            for (auto channel = 0; channel < numOutputChannels; ++channel) {

                // get a write pointer from the output
                auto writePointer = synthBuffer.getWritePointer(channel);

                // get a read pointer from the sample
                // possible that there might be more output channels than sample channels
                auto readPointer = notesBuffer[currentNoteIndex].getReadPointer(channel % numInputChannels);

                // copy sample to output
                writePointer[sample] = readPointer[position];

            }
       

            // next positon in sample, if the end is reached, go back to the start
            position++;
            if (position == notesBuffer[currentNoteIndex].getNumSamples())
                position = 0;
   
        }
    }

    // apply the adsr enveloped to the temp bufffer
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);


    // add the temp buffer to the output buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        outputBuffer.copyFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive()) clearCurrentNote();


    }
    
}


void CustomSamplerVoice::importSamples() {

    // hardcoded importing samples

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
    
    // create wavFormat and reader to read the sample .wav files
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(
        new juce::MemoryInputStream(sourceData, sourceDatasize, false), true));

    // if the file exists
    if (reader.get() != nullptr)
    {
        // get the duration - num samples / sample rate
        auto duration = (float)reader->lengthInSamples / reader->sampleRate;

        if (duration < 2)
        {
            // configure the sample buffer
            notesBuffer[index].setSize((int)reader->numChannels, (int)reader->lengthInSamples);
            // read values to sample buffer
            reader->read(&notesBuffer[index],
                0,
                (int)reader->lengthInSamples,
                0,
                true,
                true);
            position = 0;

        }
        else
        {
            juce::Logger::outputDebugString("-- File too large");

        }
        juce::Logger::outputDebugString("-- Read file");
    }
    else juce::Logger::outputDebugString("-- Could not read file");

}

