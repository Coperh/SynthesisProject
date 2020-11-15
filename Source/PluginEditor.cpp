/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesisProjectAudioProcessorEditor::SynthesisProjectAudioProcessorEditor (SynthesisProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    addAndMakeVisible(keyboardComponent);


    setSize (400, 300);
}

SynthesisProjectAudioProcessorEditor::~SynthesisProjectAudioProcessorEditor()
{
}

//==============================================================================
void SynthesisProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthesisProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    keyboardComponent.setBounds(0, 0, getWidth(), getHeight()/4);
}
