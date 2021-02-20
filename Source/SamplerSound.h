/*
  ==============================================================================

    SamplerSound.h
    Created: 20 Feb 2021 3:31:44pm
    Author:  chold

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomSamplerSound : public juce::SamplerSound {

public:

    using SamplerSound::SamplerSound;


    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }


};