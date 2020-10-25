/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KeyboardListener.h"


//==============================================================================
/**
*/
class SynthesisProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthesisProjectAudioProcessorEditor (SynthesisProjectAudioProcessor&);
    ~SynthesisProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthesisProjectAudioProcessor& audioProcessor;


    KeyboardListener keyboardListener;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;



    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesisProjectAudioProcessorEditor)
};
