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
    keyboardComponent.setAvailableRange(62, 84);



    toggleAttachment = std::make_unique< juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "DRONETOGGLE", droneToggle);

    addAndMakeVisible(droneToggle);
    droneToggle.setButtonText("Toggle Drone");




    // toggle when button is pressed
    //droneToggle.onClick = [this]() { audioProcessor.toggleDrone(droneToggle.getToggleState()); };



    // master gain slider
    masterGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MASTERGAIN", masterGainSlider);
    masterGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    masterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 40);
    addAndMakeVisible(masterGainSlider);

    createLabelForSlider(masterLabel, "Master", masterGainSlider);


    // Drone Gaint sldier
    droneGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DRONEGAIN", droneGainSlider);
    droneGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    droneGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(droneGainSlider);

    createLabelForSlider(droneLabel, "Drone", droneGainSlider);

    // Chanter Gaint Sldier
    chantGainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "CHANTGAIN", chantGainSlider);
    chantGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chantGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(chantGainSlider);

    createLabelForSlider(chantLabel, "Chanter", chantGainSlider);



    addAndMakeVisible(keySelector);
    keySelector.addItem("G", 1);

    keySelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "KEYSELECTION", keySelector);



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

    




    // at the bottom, takes one fourth of the screen
    keyboardComponent.setBounds(0, (getHeight() - getHeight() / 4), getWidth(), getHeight()/4);




    droneToggle.setBounds(10, 10, getHeight()/4, getHeight() / 4);


    // right side, 3 fourths of the screen over, width of one fourth, height of 3 fourths
    masterGainSlider.setBounds( ((getWidth()/4)*3)+10, 
        30, 
        (getWidth() / 4) - 20, 
        ((getHeight() / 4) * 3) - 50);


    int sliderHorPos = ((getWidth() / 4) * 2) + 20;

    // half of 3 quarters of the height (h/4 * 3/2)
    int verticlePos = (getHeight() * 3) / 8;


    int sliderDim = verticlePos - 40;

    droneGainSlider.setBounds(sliderHorPos,
        30,
        sliderDim,
        sliderDim);

    chantGainSlider.setBounds(sliderHorPos,
        verticlePos +30,
        sliderDim,
        sliderDim);


    keySelector.setBounds(10, verticlePos+10, getHeight()/4, getHeight() / 8);
}




void UilleannPipesAudioProcessorEditor::createLabelForSlider(juce::Label& label, const char * text, juce::Slider & slider) {


    label.setText(text, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);

        


}