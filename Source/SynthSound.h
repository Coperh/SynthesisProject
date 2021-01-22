/*
  ==============================================================================

    SynthSound.h
    Created: 28 Oct 2020 4:14:29pm
    Author:  chold

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct SynthSound : public juce::SynthesiserSound
{
public:

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};