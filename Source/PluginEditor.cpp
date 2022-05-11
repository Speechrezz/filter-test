/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterTestAudioProcessorEditor::FilterTestAudioProcessorEditor (FilterTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    freqKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, FREQ_KNOB_ID, freqKnob);

    freqKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
    freqKnob.setTextBoxIsEditable(true);
    freqKnob.setRange(20.f, 1000.f);
    freqKnob.setSkewFactorFromMidPoint(500.0f);

    freqKnobLabel.setText("Frequency", juce::dontSendNotification);
    freqKnobLabel.setJustificationType(juce::Justification::centred);
    freqKnobLabel.attachToComponent(&freqKnob, false);

    addAndMakeVisible(freqKnob);
	
}

FilterTestAudioProcessorEditor::~FilterTestAudioProcessorEditor()
{
}

//==============================================================================
void FilterTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FilterTestAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    freqKnob.setBounds(100, 100, 100, 100);
}
