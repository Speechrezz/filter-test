/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define NUM_FILTERS 20

#define FREQ_KNOB_ID "freq"
#define FREQ_KNOB_NAME "Frequency"

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
	
    //==============================================================================
	
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::array<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, NUM_FILTERS> filters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterTestAudioProcessor)
};
