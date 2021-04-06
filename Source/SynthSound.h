/*
  ==============================================================================

    SynthSound.h
    Created: 28 Oct 2020 4:14:29pm
    Author:  chold

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*
    Simple Class required by the Synthesiser class which describes a sound the synthesiser voice can make.
    It does not need to contain any data.
*/
struct SynthSound : public juce::SynthesiserSound
{
public:

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};