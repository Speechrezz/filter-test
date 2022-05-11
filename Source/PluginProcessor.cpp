/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterTestAudioProcessor::FilterTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    treeState(*this, nullptr, "PARAMETER", { createParameterLayout() })
#endif
{
    treeState.state = juce::ValueTree("savedParams");
}

FilterTestAudioProcessor::~FilterTestAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout FilterTestAudioProcessor::createParameterLayout() {
    // Add parameters to the paramater tree. Allows for saving/loading and automating of paramaters

    std::vector < std::unique_ptr <juce::RangedAudioParameter> > params;

    auto gainParam = std::make_unique<juce::AudioParameterFloat>(GAIN_KNOB_ID, GAIN_KNOB_NAME, 1.f, 16.f, 12.f);
    params.push_back(std::move(gainParam));

    auto noteParam = std::make_unique<juce::AudioParameterFloat>(NOTE_KNOB_ID, NOTE_KNOB_NAME, 40.f, 80.f, 69.f);
    params.push_back(std::move(noteParam));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String FilterTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();	

    // Loop through filters array
    for (int i = 0; i < filters.size(); i++) {

        filters[i].prepare(spec);
        filters[i].reset();
    }
}

void FilterTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FilterTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    updateFilters();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
	
    auto audioBlock = juce::dsp::AudioBlock<float>(buffer).getSubsetChannelBlock(0, (size_t)totalNumInputChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
	
	// Loop through filters array
    for (int i = 0; i < filters.size(); i++) {
        filters[i].process(context);
    }
	
}

//==============================================================================
bool FilterTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterTestAudioProcessor::createEditor()
{
    return new FilterTestAudioProcessorEditor (*this);
}

//==============================================================================
void FilterTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FilterTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    // 'If' statements are for error checking
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(treeState.state.getType())) {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }	
}

void FilterTestAudioProcessor::updateFilters()
{
    const float gain = treeState.getRawParameterValue(GAIN_KNOB_ID)->load();
    const int base_note = static_cast<int>(treeState.getRawParameterValue(NOTE_KNOB_ID)->load());

    if (prev_note == base_note && prev_gain == gain) return;

    // Loop through filters array
    for (int i = 0; i < filters.size(); i++) {
        const int note = base_note + minorKey[i];
        const float freq = juce::MidiMessage::getMidiNoteInHertz(note);

        *filters[i].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), freq, 40.f, gain);
        DBG(juce::String(gain) + ", " + juce::String(note) + ", " + juce::String(freq));
    }
    prev_note = base_note;
    prev_gain = gain;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterTestAudioProcessor();
}
