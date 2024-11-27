/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    
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

    samp_rate = sampleRate;
    gr = 1;
    rms_0 = 0;
    rms_1 = 0;
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
    // 
    //get number of samples
    auto numSamples = buffer.getNumSamples();
    float sample_0 = 0;
    float sample_0p = 0;
    float sample_1 = 0;
    float sample_1p = 0;
    rms_0 = 0;
    rms_1 = 0;
    float curr_sample_RMS_dB = 0;
    float G = 0;    //gain computation for scaling factor 
    float static_g = 0;    //static gain

    float makeupDB = m_gain->get();
    float makeup = pow(10.0f, (makeupDB/20.0f));
    float loc_thresh = thresh->get();
    float loc_r = r->get();
    float loc_atk = atk->get();
    float loc_rels = rels->get();
    float mix_r = (mix->get()) / 100;
    float loc_k_width = k_width->get();

    //calculate AT and RT (in samples)
    //note that loc_atk is in ms
    double AT = exp(-1 / (0.001 * samp_rate * loc_atk));
    double RT = exp(-1 / (0.001 * samp_rate * loc_rels));

    for (int i = 0; i < numSamples; i++) {
        // do channel dependent processing
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            // calculate rms lvl and apply distortion to each channel sample
            if (channel == 0) {
                sample_0 = buffer.getSample(channel, i);
                sample_0p = applyOD_or_DIST(sample_0);

                if (i == 0) {
                    rms_0 = sample_0p * sample_0p;
                }
                else {
                    rms_0 = ((1 - TAV) * rms_0) + (TAV * (sample_0p * sample_0p));
                }
            } else if (channel == 1) {
                sample_1 = buffer.getSample(channel, i);
                sample_1p = applyOD_or_DIST(sample_1);

                if (i == 0) {
                    rms_1 = sample_1p * sample_1p;
                }
                else {
                    rms_1 = ((1 - TAV) * rms_1) + (TAV * (sample_1p * sample_1p));
                }
            }
        }
        // use max rms for calculation
        if (rms_1 > rms_0) {
            curr_sample_RMS_dB = 10.0f * log10(rms_1); // change to dB
        } else {
            curr_sample_RMS_dB = 10.0f * log10(rms_0); // change to dB
        }
        // Determine if we have a soft or hard knee and calculate static gain
        if (loc_k_width == 0) {
            //Hard Knee algortihm
            if (curr_sample_RMS_dB <= loc_thresh) {
                G = 0;
            } else {
                G = (1 - (1 / (loc_r))) * (loc_thresh - curr_sample_RMS_dB);     //G = CS * (CT - X)] (see Zolzer pg 110)
            }
        }
        else {
            // Soft Knee Algorithm
            float abs_val = fabsf(curr_sample_RMS_dB - loc_thresh);
            if ((2 * (curr_sample_RMS_dB - loc_thresh)) < ((-1) * loc_k_width)) {
                G = 0;
            } else if ((2 * abs_val) <= (loc_k_width)) {
                G = curr_sample_RMS_dB + (1 - (1 / (loc_r))) * (curr_sample_RMS_dB - loc_thresh + (loc_k_width / 2)) / (2 * loc_k_width);
            } else if ((2 * (curr_sample_RMS_dB - loc_thresh)) > loc_k_width) {
                G = (1 - (1 / (loc_r))) * (loc_thresh - curr_sample_RMS_dB);
            }
        }
        // change back to amplitude
        static_g = pow(10.0f, (G / 20.0f));
        // smooth gain reduction with attack and release times
        if (static_g <= gr) {
            gr = (AT * gr) + ((1 - AT) * static_g);
        }
        else {
            gr = (RT * gr) + ((1 - RT) * static_g);
        }
        // apply gain reduction to each channel
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            // apply makeup gain as well
            if (channel == 0) {
                sample_0p = makeup * (sample_0p * gr);
                buffer.setSample(channel, i, ((mix_r * sample_0p) + ((1 - mix_r) * sample_0)));
            }
            else if (channel == 1) {
                sample_1p = makeup * (sample_1p * gr);
                buffer.setSample(channel, i, ((mix_r * sample_1p) + ((1 - mix_r) * sample_1)));
            }
        }
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
    // attack parameter in units of ms - lower top range to be maybe 200
    param_layout.add(std::make_unique<AudioParameterFloat>("atk", "Attack", NormalisableRange<float>(0.50f, 250.0f, 0.25f, 1.0f), 5));
    // release parameter in units of ms bring up bottom of range to 10 ms
    param_layout.add(std::make_unique<AudioParameterFloat>("rels", "Release", NormalisableRange<float>(10.0f, 1000.0f, 0.25f, 1.0f), 25));
    // x:1 ratio parameter
    param_layout.add(std::make_unique<AudioParameterFloat>("r", "Ratio", NormalisableRange<float>(1.0f, 20.0f, 0.5f, 1.0f), 2));
    // knee width param in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("k_width", "Knee", NormalisableRange<float>(0.0f, 1.0f, 0.05f, 1.0f), 0.2));
    // makeup gain param in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("m_gain", "Makeup Gain", NormalisableRange<float>(-30.0f, 30.0f, 0.5f, 1.0f), 0));
    // mix param in %
    param_layout.add(std::make_unique<AudioParameterFloat>("mix", "Mix", NormalisableRange<float>(0.0f, 100.0f, 0.5f, 1.0f), 100));
    // non-linear effect selection param
    param_layout.add(std::make_unique<AudioParameterChoice>("non_lin_choice", "Dist Type Choice", StringArray("Distortion", "Hybrid", "Overdrive", "Full Wave Rectifier", "Half Wave Rectifier"), 0));
    // non-linear effect amount (or gain) in dB
    param_layout.add(std::make_unique<AudioParameterFloat>("drive", "Drive", NormalisableRange<float>(0.0f, 36.0f, 0.25f, 1.0f), 0));

    return param_layout;
}

float _484CompressorAudioProcessor::getGainReduction() {
    return juce::Decibels::gainToDecibels(gr);
}

float _484CompressorAudioProcessor::applyOD_or_DIST(float in_level)
{
    //put od/dist alg here

    //distortion variables
    float input_gain_dB = drive->get();
    float input_gain = pow(10.0f, (input_gain_dB / 20.0f));
    auto d_type = nl_choice->getCurrentChoiceName(); //distortion type

    float d_out; //distortion out

    //thresholds for Overdrive:
    float o_thresh1 = 1.0f / 3.0f;
    float o_thresh2 = 2.0f / 3.0f;

    //this distortion code is adapted from Reiss pg 183

    //Apply distortion based on type
    in_level = in_level * input_gain;
    if (d_type == "Distortion") {

        //simple hard clipping
        if (in_level > 1) {

            d_out = 1;

        } else if (in_level < -1) {

            d_out = -1;

        } else {

            d_out = in_level;

        }


        return d_out;

    } else if(d_type == "Hybrid") {

        //simple hard clipping
        if (in_level > 1) {

            d_out = 1;

        }
        else if (in_level < -1) {

            d_out = -1;

        }
        else {

            d_out = in_level;

        }

        return d_out * in_level;

    }
    else if (d_type == "Overdrive") {

        //should emulate the clipping diodes of a common overdrive pedal
        
        if(in_level > 0){
            d_out = 1.0f - expf((-1) * in_level);

        }
        else {
            d_out = -1.0f + expf(in_level);
        }

        return d_out;

    }else if(d_type == "Full Wave Recifiter"){ 
        
        d_out = fabsf(in_level);
        return d_out;

    }else{

        /*Half Wave Rectifier*/

        if (in_level > 0) {

            d_out = in_level;

        }
        else {

            d_out = 0;

        }

        return d_out;
    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _484CompressorAudioProcessor();
}
