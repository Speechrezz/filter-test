/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define NUM_FILTERS 20

#define GAIN_KNOB_ID "gain"
#define GAIN_KNOB_NAME "Gain"

#define NOTE_KNOB_ID "note"
#define NOTE_KNOB_NAME "Note"


#include <JuceHeader.h>

//==============================================================================
/**
*/
class FilterTestAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FilterTestAudioProcessor();
    ~FilterTestAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;

private:
    void updateFilters();

    std::array<int, 20> minorKey{ 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33 };

    int prev_note{ -1 };
    float prev_gain{ -1.f };
	
    //==============================================================================
	
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::array<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, NUM_FILTERS> filters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTestAudioProcessor)
};
