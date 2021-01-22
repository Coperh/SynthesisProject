/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UilleannPipesAudioProcessorEditor::UilleannPipesAudioProcessorEditor (UilleannPipesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    addAndMakeVisible(keyboardComponent);


    addAndMakeVisible(droneToggle);
    droneToggle.setButtonText("Toggle Drone");
    // toggle when button is pressed
    droneToggle.onClick = [this]() { audioProcessor.toggleDrone(droneToggle.getToggleState()); };

    addAndMakeVisible(gainSlider);
    gainSlider.setRange(0.0, 1.0, 0.01);          // [1]
    gainSlider.setTextValueSuffix("%"); 
    gainSlider.setValue(0.8);



    setSize (400, 300);
}

UilleannPipesAudioProcessorEditor::~UilleannPipesAudioProcessorEditor()
{
}

//==============================================================================
void UilleannPipesAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void UilleannPipesAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    keyboardComponent.setBounds(0, 0, getWidth(), getHeight()/4);
    droneToggle.setBounds(0, (getHeight() / 8) * 2, getWidth(), getHeight() / 8);
    gainSlider.setBounds(0, (getHeight() / 8) * 3, getWidth(), getHeight() / 8);
}
