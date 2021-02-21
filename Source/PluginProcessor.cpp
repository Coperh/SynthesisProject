/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UilleannPipesAudioProcessor::UilleannPipesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters() )
#endif
{



    syntheiser.addSound(new SynthSound());
    syntheiser.addVoice(new CustomSamplerVoice());
    
    
    readDroneSample();
    droneEnabled = false;
}

UilleannPipesAudioProcessor::~UilleannPipesAudioProcessor()
{
}

//==============================================================================
const juce::String UilleannPipesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UilleannPipesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UilleannPipesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UilleannPipesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UilleannPipesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UilleannPipesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UilleannPipesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UilleannPipesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String UilleannPipesAudioProcessor::getProgramName (int index)
{
    return {};
}

void UilleannPipesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void UilleannPipesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{


    syntheiser.setCurrentPlaybackSampleRate(sampleRate);


    for (int i = 0; i < syntheiser.getNumVoices(); i++) {

        if (auto voice = dynamic_cast<CustomSamplerVoice*>(syntheiser.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
        }
    }



    // setup adsr for drone
    droneAdsr.setSampleRate(sampleRate);

    droneAdsrParams.attack = 0.1f;
    droneAdsrParams.sustain = 1.0f;
    droneAdsrParams.decay = 0.0f;
    droneAdsrParams.release = 0.2f;

    droneAdsr.setParameters(droneAdsrParams);


}

void UilleannPipesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UilleannPipesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void UilleannPipesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    int numSamples = buffer.getNumSamples();


    // gets midi buffer from keyboar state and inserts it into current buffer
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples,true);
 
    syntheiser.renderNextBlock(buffer, midiMessages, 0, numSamples);


    


    if (droneAdsr.isActive())
    {

        droneBuffer.setSize(buffer.getNumChannels(), numSamples, false, false, true);

        droneBuffer.clear();

        auto numInputChannels = droneBuffer.getNumChannels();
        auto numOutputChannels = fileBuffer.getNumChannels();

       
        for (auto sample = 0; sample < droneBuffer.getNumSamples(); ++sample) {


            // Highest multiple of the offset below the max
            // number of samples divided by the offset floored multiplied by the offset


            for (auto channel = 0; channel < numOutputChannels; ++channel) {

                auto writePointer = droneBuffer.getWritePointer(channel);
                // possible that there might be more output channels
                auto readPointer = fileBuffer.getReadPointer(channel);

                writePointer[sample] = readPointer[dronePosition];

            }

            dronePosition += 1;



            if (  dronePosition == fileBuffer.getNumSamples())
                dronePosition = 0;

        }
   
        // apply to buffer.
        droneAdsr.applyEnvelopeToBuffer(droneBuffer, 0, numSamples);

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {

           buffer.addFrom(channel, 0, droneBuffer, channel, 0, numSamples);

        }


    
    }
    

   



    //auto g = apvts.getRawParameterValue("KEYSELECTION");
    //juce::Logger::outputDebugString(std::to_string(g->load()));

}

//==============================================================================
bool UilleannPipesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* UilleannPipesAudioProcessor::createEditor()
{
    return new UilleannPipesAudioProcessorEditor (*this);
}

//==============================================================================
void UilleannPipesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UilleannPipesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UilleannPipesAudioProcessor();
}

void UilleannPipesAudioProcessor::toggleDrone(bool state) {
    droneEnabled = state;


    if (state){ 
        droneAdsr.noteOn();
        dronePosition = 0;
    }
    else droneAdsr.noteOff();
    

    juce::Logger::outputDebugString("Drone: " + std::to_string(droneEnabled));


}

juce::AudioProcessorValueTreeState::ParameterLayout  UilleannPipesAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // combo box switch drone key

    

    params.push_back(std::make_unique<juce::AudioParameterChoice>("KEYSELECTION", "Key Selection", juce::StringArray{ "D","A" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.8f));


    return { params.begin(), params.end() };
};


void UilleannPipesAudioProcessor::readDroneSample(){



    juce::WavAudioFormat wavFormat;

    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(
        new juce::MemoryInputStream(BinaryData::sine441_wav, BinaryData::sine441_wavSize, false), true));


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
            dronePosition = 0.0;

        }
        else
        {
            // handle the error that the file is 2 seconds or longer..
        }
    }
    else juce::Logger::outputDebugString("-- Could not read file");



}



