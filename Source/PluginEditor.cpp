/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UilleannPipesAudioProcessorEditor::UilleannPipesAudioProcessorEditor (UilleannPipesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    addAndMakeVisible(keyboardComponent);


    addAndMakeVisible(droneToggle);
    droneToggle.setButtonText("Toggle Drone");
    // toggle when button is pressed
    droneToggle.onClick = [this]() { audioProcessor.toggleDrone(droneToggle.getToggleState()); };

    gainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(gainSlider);


    addAndMakeVisible(keySelector);

    keySelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (audioProcessor.apvts, "OSC", keySelector);



    setSize (400, 300);
}

UilleannPipesAudioProcessorEditor::~UilleannPipesAudioProcessorEditor()
{
}

//==============================================================================
void UilleannPipesAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void UilleannPipesAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    keyboardComponent.setBounds(0, 0, getWidth(), getHeight()/4);
    droneToggle.setBounds(0, (getHeight() / 8) * 2, getWidth(), getHeight() / 8);
    gainSlider.setBounds(getWidth()/2 - 100, (getHeight() / 8) * 3, 200, 100);
    keySelector.setBounds(0, (getHeight() / 2), 100, 50);
}
