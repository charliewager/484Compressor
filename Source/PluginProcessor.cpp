/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    LOGAN IS COOL
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_484CompressorAudioProcessor::_484CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // initialize param pointers
    thresh = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("thresh"));
    // assert param is not null
    jassert(thresh != nullptr);
    
    atk = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("atk"));
    // assert param is not null
    jassert(atk != nullptr);

    rels = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("rels"));
    // assert param is not null
    jassert(thresh != nullptr);

    r = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("r"));
    // assert param is not null
    jassert(rels != nullptr);

    k_width = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("k_width"));
    // assert param is not null
    jassert(k_width != nullptr);

    m_gain = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("m_gain"));
    // assert param is not null
    jassert(m_gain != nullptr);

    mix = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("mix"));
    // assert param is not null
    jassert(mix != nullptr);

    drive = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("drive"));
    // assert param is not null
    jassert(drive != nullptr);

    nl_choice = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("non_lin_choice"));
    // assert param is not null
    jassert(nl_choice != nullptr);

}

_484CompressorAudioProcessor::~_484CompressorAudioProcessor()
{
}

//==============================================================================
const juce::String _484CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _484CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _484CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _484CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _484CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _484CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _484CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _484CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _484CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void _484CompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _484CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{   // this is called before sound playback starts
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // clear dynamic gain and maybe even input rms level here
}

void _484CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _484CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _484CompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

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
        // will need to iterate over each sample in the block 
        // Roadmap:
        //  - Apply non_lin processing (do before rms measurement)
        //  - Measure rms of sample
        //  - Calculate static gain
        //  - Smooth static gain by applying attack and release time logic (this may
        //      need to be applied across multiple processBlock invocations - maybe
        //      keep a record of applied (dynamic) gain potentially as a public var
        //      - this will need to be reset whenerver the audio is stopped
        //  - Apply gain to sample
        //  - Applut makeup gain
    }
}

//==============================================================================
bool _484CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _484CompressorAudioProcessor::createEditor()
{
    return new _484CompressorAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void _484CompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mem_out(destData, true);
    apvts.state.writeToStream(mem_out);
}

void _484CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout _484CompressorAudioProcessor::createParameterLayout() {
    // initialize param obj
    APVTS::ParameterLayout param_layout;
    // use namespace to eliminate the need to type juce::
    using namespace juce;
    // threshold parameter in units of dB
    param_layout.add(std::make_unique<AudioParameterFloat>("thresh", "Threshold", NormalisableRange<float>(-60.0f, 12.0f, 0.5f, 1.0f), 0));
    // attack parameter in units of ms
    param_layout.add(std::make_unique<AudioParameterFloat>("atk", "Attack", NormalisableRange<float>(0.50f, 500.0f, 0.05f, 1.0f), 5));
    // release parameter in units of ms
    param_layout.add(std::make_unique<AudioParameterFloat>("rels", "Release", NormalisableRange<float>(1.0f, 1000.0f, 0.05f, 1.0f), 25));
    // x:1 ratio parameter
    param_layout.add(std::make_unique<AudioParameterFloat>("r", "Ratio", NormalisableRange<float>(1.0f, 20.0f, 0.5f, 1.0f), 2));
    // knee width param in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("k_width", "Knee", NormalisableRange<float>(0.0f, 1.0f, 0.05f, 1.0f), 0.2));
    // makeup gain param in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("m_gain", "Makeup Gain", NormalisableRange<float>(-30.0f, 30.0f, 0.5f, 1.0f), 0));
    // mix param in %
    param_layout.add(std::make_unique<AudioParameterFloat>("mix", "Mix", NormalisableRange<float>(0.0f, 100.0f, 0.5f, 1.0f), 100));
    // non-linear effect selection param
    param_layout.add(std::make_unique<AudioParameterChoice>("non_lin_choice", "Dist Type Choice", StringArray("Distortion", "Overdrive"), 0));
    // non-linear effect amount (or gain) in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("drive", "Drive", NormalisableRange<float>(0.0f, 24.0f, 0.25f, 1.0f), 0));

    return param_layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _484CompressorAudioProcessor();
}
