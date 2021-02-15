/*
  ==============================================================================

    CustomOscilator.h
    Created: 6 Feb 2021 4:25:36pm
    Author:  chold

  ==============================================================================
*/

#pragma once



template <typename Type>
class Oscillators
{
public:


    static Type Saw(Type x) {

        return juce::jmap(x, Type(-juce::double_Pi), Type(juce::double_Pi), Type(-1), Type(1));

    }

    static Type Sine(Type x) {

        return std::sin(x);

    }


    static Type Triangle(Type x) {


        if (x < 0) return juce::jmap(x, Type(-juce::double_Pi), Type(0), Type(-1), Type(1));

        return juce::jmap(x, Type(0), Type(juce::double_Pi),  Type(1), Type(-1));

    }





};