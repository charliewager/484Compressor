/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./LNF/custom_3d_knob_lnf.h"
#include "GUI/meter.h"

//==============================================================================
/**
*/
class _484CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _484CompressorAudioProcessorEditor (_484CompressorAudioProcessor&);
    ~_484CompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _484CompressorAudioProcessor& audioProcessor;

    // custom lnf for rotary knob
    Custom_3d_knob_lnf m_3dKnobLook;

    // sliders for parameters and labels for sliders
    juce::Slider thresh_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label t_label;
    juce::Slider atk_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label a_label;
    juce::Slider rels_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label rels_label;
    juce::Slider ratio_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label r_label;
    juce::Slider knee_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label k_label;
    juce::Slider makeup_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label mg_label;
    juce::Slider mix_knob { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label m_label;
    juce::ComboBox nl_choice;
    juce::Slider drive_knob{ juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label drive_label;
    
    // add parameter attatchments to each of these to have them affect the parameter values
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> atkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> relsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> makeupAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> nl_choiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    meter gr_meter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_484CompressorAudioProcessorEditor)
};