/*
  ==============================================================================

    bulb.h
    Created: 22 Nov 2024 9:20:49pm
    Author:  charl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class bulb  : public juce::Component
{
public:
	bulb(const juce::Colour& c) : colour(c) {}
	void paint(juce::Graphics& g) override
	{
		if (isOn)
		{
			g.setColour(colour);

		}
		else
		{
			g.setColour(juce::Colours::black);
		}
		const auto delta = 4.f;
		const auto bounds = getLocalBounds().toFloat().reduced(delta);
		const auto side = juce::jmin(bounds.getWidth(), bounds.getHeight());
		const auto bulbFillBounds = juce::Rectangle<float>{ bounds.getX(), bounds.getY(), side, side };
		g.fillEllipse(bulbFillBounds);
		g.setColour(juce::Colours::black);
		g.drawEllipse(bulbFillBounds, 1.f);
		if (isOn)
		{
			g.setGradientFill(juce::ColourGradient{ colour.withAlpha(0.3f), bulbFillBounds.getCentre(), colour.withLightness(1.5f).withAlpha(0.f), {}, true });
			g.fillEllipse(bulbFillBounds.expanded(delta));
		}

	}
	void setState(const bool state) { isOn = state; }

private:
	bool isOn = false;
	juce::Colour colour{};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (bulb)
};
