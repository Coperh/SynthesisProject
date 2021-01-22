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
class UilleannPipesAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    UilleannPipesAudioProcessorEditor (UilleannPipesAudioProcessor&);
    ~UilleannPipesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UilleannPipesAudioProcessor& audioProcessor;


    KeyboardListener keyboardListener;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::ToggleButton droneToggle;

    juce::Slider gainSlider;


    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UilleannPipesAudioProcessorEditor)
};
