/*
  ==============================================================================

    KeyboardListener.h
    Created: 25 Oct 2020 7:01:33pm
    Author:  chold

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>



class KeyboardListener : public juce::MidiKeyboardState::Listener {
public:

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;


private:



};