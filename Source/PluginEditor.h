/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FilterTestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FilterTestAudioProcessorEditor (FilterTestAudioProcessor&);
    ~FilterTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterTestAudioProcessor& audioProcessor;

    juce::Slider freqKnob;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> freqKnobAttach;
    juce::Label freqKnobLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTestAudioProcessorEditor)
};
