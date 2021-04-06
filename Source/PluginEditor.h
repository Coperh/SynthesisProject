/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


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

    // on screen keyboard component
    juce::MidiKeyboardComponent keyboardComponent;

    // toggle button for the drone
    juce::ToggleButton droneToggle;


    // Gain sliders and their lables
    juce::Label masterLabel;
    juce::Slider masterGainSlider;

    juce::Label chantLabel;
    juce::Slider chantGainSlider;

    juce::Label droneLabel;
    juce::Slider droneGainSlider;

    // selector for drone
    juce::ComboBox keySelector;

    // creates the label for the sliders
    void createLabelForSlider(juce::Label& label, const char* text, juce::Slider& slider);

    
    // attachments for the audio parameter tree in the pluggin proccessor
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr <SliderAttachment> masterGainAttachment;
    std::unique_ptr <SliderAttachment> droneGainAttachment;
    std::unique_ptr <SliderAttachment> chantGainAttachment;

    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> keySelectorAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UilleannPipesAudioProcessorEditor)
};
