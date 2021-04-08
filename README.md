# Virtual Instrument of the Uilleann Pipes
## Overview
A virtual version of uilleann pipes intended to be used in digital audio workstations. It accepts midi input and has various parameters that can be automated.

The plugin consists of two parts: the chanter or the melody pipe that accepts midi input and the drone which can be manually toggled on and off or with automation.

Implemeted using the JUCE 6 audio application framework for C++. It can be compiled to many difference formats i.e. VST2/3, AU2/3.


## How to Use
* Go to the [JUCE website](https://juce.com/) and download JUCE.
* Unzip and run Projucer
* Open SynthesisProject.jucer
* Pick the exporter of your coice to compile

## Of Note
* The samples are not included because I do not have the rights for them
* Only midi note 62 and 84 change the sound, most sharps are unsupported
* Playing an unsupported note will just replay the last suportted one played
* There is no context menu, must use DAW's parameter selector to create automation clips
