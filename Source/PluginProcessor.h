/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class _484CompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    _484CompressorAudioProcessor();
    ~_484CompressorAudioProcessor() override;

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

    using APVTS = juce::AudioProcessorValueTreeState;
    static APVTS::ParameterLayout createParameterLayout();

    APVTS apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    // gain reduction for use with meter -> maybe set every 25 samples?
    juce::Atomic<float> gain_reduction;

private:
    // create pointers to params in createParameterLayout to optimize and simplify access
    // these can be accessed with param->get() and can be set with:
    //          x->setValueNotifyingHost(y) for setting value from editor
    //          x->setValue(y) for setting value anywhere else
    juce::AudioParameterFloat* thresh { nullptr };
    juce::AudioParameterFloat* atk { nullptr };
    juce::AudioParameterFloat* rels { nullptr };
    juce::AudioParameterFloat* r { nullptr };
    juce::AudioParameterFloat* k_width { nullptr };
    juce::AudioParameterFloat* m_gain{ nullptr };
    juce::AudioParameterFloat* mix { nullptr };
    juce::AudioParameterFloat* drive{ nullptr };
    juce::AudioParameterChoice* nl_choice{ nullptr };

    // gain reduction for use in algorithm
    std::array<float, 2> gainReduction;
    float TAV = 0.075;
    float samp_rate;
    // maybe add private functions for envolope/level detection (RMS), static gain computing and smoothing to simplify process function

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_484CompressorAudioProcessor)
};
