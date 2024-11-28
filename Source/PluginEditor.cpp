/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_484CompressorAudioProcessorEditor::_484CompressorAudioProcessorEditor (_484CompressorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), gr_meter([&]() { return audioProcessor.getGainReduction(); })
{
    // configure sliders
    thresh_knob.setLookAndFeel(&m_3dKnobLook);
    thresh_knob.setTextValueSuffix("dB");
    thresh_knob.setRange(-60.0, 12.0, 0.5);
    addAndMakeVisible(thresh_knob);

    // add attachment
    threshAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "thresh",
        thresh_knob
    );

    atk_knob.setLookAndFeel(&m_3dKnobLook);
    atk_knob.setTextValueSuffix("ms");
    atk_knob.setRange(0.50, 250.0, 0.25);
    addAndMakeVisible(atk_knob);

    // add attachment
    atkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "atk",
        atk_knob
    );
    
    rels_knob.setLookAndFeel(&m_3dKnobLook);
    rels_knob.setTextValueSuffix("ms");
    rels_knob.setRange(10.0, 1000.0, 0.25);
    addAndMakeVisible(rels_knob);

    // add attachment
    relsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "rels",
        rels_knob
    );
    
    ratio_knob.setLookAndFeel(&m_3dKnobLook);
    ratio_knob.setTextValueSuffix(":1");
    ratio_knob.setRange(1.0, 20.0, 0.5);
    addAndMakeVisible(ratio_knob);

    // add attachment
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "r",
        ratio_knob
    );
    
    knee_knob.setLookAndFeel(&m_3dKnobLook);
    knee_knob.setTextValueSuffix("dB");
    knee_knob.setRange(0.0, 1.0, 0.05);
    addAndMakeVisible(knee_knob);

    // add attachment
    kneeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "k_width",
        knee_knob
    );
    
    makeup_knob.setLookAndFeel(&m_3dKnobLook);
    makeup_knob.setTextValueSuffix("dB");
    makeup_knob.setRange(-30.0, 30.0, 0.5);
    addAndMakeVisible(makeup_knob);

    // add attachment
    makeupAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "m_gain",
        makeup_knob
    );
    
    mix_knob.setLookAndFeel(&m_3dKnobLook);
    mix_knob.setTextValueSuffix("%");
    mix_knob.setRange(0.0, 100.0, 0.5);
    addAndMakeVisible(mix_knob);

    // add attachment
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "mix",
        mix_knob
    );

    drive_knob.setLookAndFeel(&m_3dKnobLook);
    drive_knob.setTextValueSuffix("dB");
    drive_knob.setRange(0.0, 36.0, 0.25);
    addAndMakeVisible(drive_knob);

    // add attachment
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "drive",
        drive_knob
    );

    nl_choice.addItem("Distortion", 1);
    nl_choice.addItem("Hybrid", 2);
    nl_choice.addItem("Overdrive", 3);
    nl_choice.addItem("Full Wave Rectifier", 4);
    nl_choice.addItem("Half Wave Rectifier", 5);
    nl_choice.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(nl_choice);

    // add attachment
    nl_choiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts,
        "non_lin_choice",
        nl_choice
    );

    // configure labels
    t_label.setText("Threshold", juce::dontSendNotification);
    t_label.setJustificationType(juce::Justification::centred);
    t_label.attachToComponent(&thresh_knob, false);
    addAndMakeVisible(t_label);

    a_label.setText("Attack", juce::dontSendNotification);
    a_label.setJustificationType(juce::Justification::centred);
    a_label.attachToComponent(&atk_knob, false);
    addAndMakeVisible(a_label);

    rels_label.setText("Release", juce::dontSendNotification);
    rels_label.setJustificationType(juce::Justification::centred);
    rels_label.attachToComponent(&rels_knob, false);
    addAndMakeVisible(rels_label);

    r_label.setText("Ratio", juce::dontSendNotification);
    r_label.setJustificationType(juce::Justification::centred);
    r_label.attachToComponent(&ratio_knob, false);
    addAndMakeVisible(r_label);

    k_label.setText("Knee", juce::dontSendNotification);
    k_label.setJustificationType(juce::Justification::centred);
    k_label.attachToComponent(&knee_knob, false);
    addAndMakeVisible(k_label);

    mg_label.setText("Makeup Gain", juce::dontSendNotification);
    mg_label.setJustificationType(juce::Justification::centred);
    mg_label.attachToComponent(&makeup_knob, false);
    addAndMakeVisible(mg_label);

    m_label.setText("Mix", juce::dontSendNotification);
    m_label.setJustificationType(juce::Justification::centred);
    m_label.attachToComponent(&mix_knob, false);
    addAndMakeVisible(m_label);

    drive_label.setText("Drive", juce::dontSendNotification);
    drive_label.setJustificationType(juce::Justification::centred);
    drive_label.attachToComponent(&drive_knob, false);
    addAndMakeVisible(drive_label);

    addAndMakeVisible(gr_meter);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1600, 700);
     
}

_484CompressorAudioProcessorEditor::~_484CompressorAudioProcessorEditor()
{
}

//==============================================================================
void _484CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void _484CompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // Flexbox for over all layout
    juce::FlexBox overall;
    // grid for knob layout
    juce::FlexBox comp_knob_grid;
    // flex for non-linear section
    juce::FlexBox non_lin_control_strip;
    // flex for mix knob container
    juce::FlexBox mix_knob_strip;
    // meter contianer
    juce::FlexBox meters;

    auto bound = getLocalBounds();
    using fi = juce::FlexItem;

    //create compressor param knob grid
    comp_knob_grid.flexDirection = juce::FlexBox::Direction::row;
    comp_knob_grid.flexWrap = juce::FlexBox::Wrap::wrap;
    comp_knob_grid.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    comp_knob_grid.alignContent = juce::FlexBox::AlignContent::spaceAround;
    //add items
    float grid_item_h = bound.getHeight() / 3.75;
    float grid_item_w = (bound.getWidth() * 0.6) / 3.75;

    comp_knob_grid.items = { fi::FlexItem(thresh_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0)),
                             fi::FlexItem(ratio_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0)),
                             fi::FlexItem(knee_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0)),
                             fi::FlexItem(atk_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0)),
                             fi::FlexItem(rels_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0)),
                             fi::FlexItem(makeup_knob).withWidth(grid_item_w).withHeight(grid_item_h).withMargin(juce::FlexItem::Margin(0.0, 20.0, 0.0, 20.0))
                           };

    //make control strip layout
    non_lin_control_strip.flexDirection = juce::FlexBox::Direction::column;
    non_lin_control_strip.flexWrap = juce::FlexBox::Wrap::wrap;
    non_lin_control_strip.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    non_lin_control_strip.alignContent = juce::FlexBox::AlignContent::spaceAround;

    non_lin_control_strip.items = { fi::FlexItem(nl_choice).withWidth(bound.getWidth() * 0.175).withHeight(bound.getHeight() * 0.15),
                                    fi::FlexItem(drive_knob).withWidth(bound.getWidth() * 0.175).withHeight(bound.getHeight() * 0.35)
                                  };
    
    //mix knob strip layout
    mix_knob_strip.flexDirection = juce::FlexBox::Direction::row;
    mix_knob_strip.flexWrap = juce::FlexBox::Wrap::wrap;
    mix_knob_strip.justifyContent = juce::FlexBox::JustifyContent::center;
    mix_knob_strip.alignContent = juce::FlexBox::AlignContent::center;

    mix_knob_strip.items = { fi::FlexItem(mix_knob).withWidth(bound.getWidth() * 0.150).withHeight(bound.getHeight() * 0.35) };

    //make overall layout
    overall.flexWrap = juce::FlexBox::Wrap::noWrap;
    overall.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    overall.alignContent = juce::FlexBox::AlignContent::spaceAround;

    meters.items = { fi::FlexItem(gr_meter).withWidth(bound.getWidth() * 0.025).withHeight(bound.getHeight() * 0.9) };

    overall.items = { fi::FlexItem(non_lin_control_strip).withFlex(0.175f).withMargin(juce::FlexItem::Margin(0.0, 0.0, 0.0, 50.0)),
                      fi::FlexItem(comp_knob_grid).withFlex(0.65f).withMargin(juce::FlexItem::Margin(0.0, 15.0, 0.0, 15.0)), 
                      fi::FlexItem(meters).withFlex(0.025).withMargin(juce::FlexItem::Margin(50.0, 0.0, 50.0, 0.0)),
                      fi::FlexItem(mix_knob_strip).withFlex(0.150f).withMargin(juce::FlexItem::Margin(125.0, 50.0, 125.0, 25.0))
                    };
    
    overall.performLayout(getLocalBounds());
}
