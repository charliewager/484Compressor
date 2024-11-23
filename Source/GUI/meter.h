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
		for (int i = (totalNumberOfBulbs - 1); i >= 0; i--) {
			bulbs[i]->setState(true);
			//doesnt work
			//if ((i == 0) && (value <= -40))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 1) && (value <= -30))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 2) && (value <= -20))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 3) && (value <= -10))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 4) && (value <= -7.5))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 5) && (value <= -5))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 6) && (value <= -4))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 7) && (value <= -3))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 8) && (value <= -2))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);

			//if ((i == 9) && (value <= -1))
			//	bulbs[i]->setState(true);
			//else
			//	bulbs[i]->setState(false);
		}
		
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
