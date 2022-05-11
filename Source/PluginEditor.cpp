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

    gainKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, GAIN_KNOB_ID, gainKnob);

    gainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
    gainKnob.setTextBoxIsEditable(true);
    gainKnob.setRange(1.f, 16.f);
    //gainKnob.setSkewFactorFromMidPoint(500.0f);

    gainKnobLabel.setText("Gain", juce::dontSendNotification);
    gainKnobLabel.setJustificationType(juce::Justification::centred);
    gainKnobLabel.attachToComponent(&gainKnob, false);

    addAndMakeVisible(gainKnob);
	

	// ================================================================
    noteKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, NOTE_KNOB_ID, noteKnob);

    noteKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    noteKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
    noteKnob.setTextBoxIsEditable(true);
    noteKnob.setRange(40.f, 80.f, 1.f);

    noteKnobLabel.setText("Note", juce::dontSendNotification);
    noteKnobLabel.setJustificationType(juce::Justification::centred);
    noteKnobLabel.attachToComponent(&noteKnob, false);

    addAndMakeVisible(noteKnob);
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

    gainKnob.setBounds(100, 100, 100, 100);

    noteKnob.setBounds(200, 100, 100, 100);
}
