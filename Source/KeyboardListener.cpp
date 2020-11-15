/*
  ==============================================================================

    KeyboardListener.cpp
    Created: 25 Oct 2020 7:08:15pm
    Author:  chold

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyboardListener.h"



void KeyboardListener::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {


    juce::Logger::outputDebugString("Press "+std::to_string(midiNoteNumber));
}



void KeyboardListener::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) {

    juce::Logger::outputDebugString("Release " + std::to_string(midiNoteNumber));

}