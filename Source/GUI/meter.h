/*
  ==============================================================================

    meter.h
    Created: 22 Nov 2024 9:05:34pm
    Author:  charl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "bulb.h"

//==============================================================================
/*
*/
class meter  : public juce::Component, juce::Timer
{
public:
	meter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
	{
		startTimerHz(24);
	}

	~meter() 
	{
		
	}

	void paint(juce::Graphics & g) override
	{	//blub at 0 is bottom bulb => bulb at max is top bulb
		auto value = valueSupplier(); //gain reduction in decibles (will be negative)
		
		if ((value <= -40))
			bulbs[0]->setState(true);
		else
			bulbs[0]->setState(false);

		if ((value <= -30))
			bulbs[1]->setState(true);
		else
			bulbs[1]->setState(false);

		if ((value <= -20))
			bulbs[2]->setState(true);
		else
			bulbs[2]->setState(false);

		if ((value <= -10))
			bulbs[3]->setState(true);
		else
			bulbs[3]->setState(false);

		if ((value <= -7.5))
			bulbs[4]->setState(true);
		else
			bulbs[4]->setState(false);

		if ((value <= -5))
			bulbs[5]->setState(true);
		else
			bulbs[5]->setState(false);

		if ((value <= -4))
			bulbs[6]->setState(true);
		else
			bulbs[6]->setState(false);

		if ((value <= -3))
			bulbs[7]->setState(true);
		else
			bulbs[7]->setState(false);

		if ((value <= -2))
			bulbs[8]->setState(true);
		else
			bulbs[8]->setState(false);

		if ((value <= -1))
			bulbs[9]->setState(true);
		else
			bulbs[9]->setState(false);
		
	}

	void resized() override
	{
		const auto bounds = getLocalBounds().toFloat();
		juce::ColourGradient gradient{ juce::Colours::red, bounds.getBottomLeft(), juce::Colours::green, bounds.getTopLeft(), false };
		gradient.addColour(0.5, juce::Colours::yellow);

		const auto bulbHeight = getLocalBounds().getHeight() / totalNumberOfBulbs;
		auto bulbBounds = getLocalBounds();
		bulbs.clear();
		for (auto i = 0; i < totalNumberOfBulbs; i++)
		{
			auto blb = std::make_unique<bulb>(gradient.getColourAtPosition(static_cast<double>(i) / totalNumberOfBulbs));
			addAndMakeVisible(blb.get());
			blb->setBounds(bulbBounds.removeFromBottom(bulbHeight));
			bulbs.push_back(std::move(blb));
		}
	}

	void timerCallback()
	{
		repaint();
	}

private:
	std::function<float()> valueSupplier;
	std::vector<std::unique_ptr<bulb>> bulbs;
	const int totalNumberOfBulbs = 10;
	// bulb[9] = 1dB reduction (-1dB)
	// bulb[8] = 2dB reduction (-2dB)
	// bulb[7] = 3dB reduction (-3dB)
	// bulb[6] = 4dB reduction (-4dB)
	// bulb[5] = 5dB reduction (-5dB)
	// bulb[4] = 7.5dB reduction (-7.5dB)
	// bulb[3] = 10dB reduction (-10dB)
	// bulb[2] = 20dB reduction (-20dB)
	// bulb[1] = 30dB reduction (-30dB)
	// bulb[0] = 40dB reduction (-40dB)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (meter)
};
