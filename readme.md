# 484Compressor - Compressor & Distortion Plugin

484Compressor is a versatile audio plugin built with the JUCE framework. It combines the functionality of a high-quality compressor with optional distortion effects, making it a powerful tool for shaping your sound.

## Features

- **Compressor**:
  - Adjustable threshold, ratio, attack, release, and makeup gain.
  - Transparent compression with precise dynamics control.

- **Distortion Options**:
  - **Distortion**: Hard clipping distortion.
  - **Hybrid**: Har clipping multiplied with the input signal.
  - **Overdrive**: Transistor overdrive emulation.
  - **Full Wave Rectifier**: Applies gain and absolute value operations to the signal.
  - **Half Wave Rectifier**: Applies gain and cuts out negative parts of the signal.

- **Mix**: Mix the distorted and compressed signal in with the original signal.

- **Intuitive UI**:
  - Visual feedback for compression and distortion levels.
  - Easy-to-use controls for quick parameter adjustments.

## Installation

No installation media has been created for this plugin yet, but it will be available soon.

To build this project yourself, do the following. Ensure that Visual Studio 2022 is installed for C++ development. Ensure that JUCE version 8.0.3 is installed. A tutorial to setup JUCE for Windows can be found in this video: https://www.youtube.com/watch?v=Mo0Oco3Vimo&pp=ygUHI2p1Y2V2Nw%3D%3D between timestamps 22:40 – 45:49. Once this is set up simply open the 484Compressor.jucer file in the Projucer application and click "Save and open in IDE". Once the Visual Studio IDE opens set the 484Compressor_VST3 solution as the startup project and build the solution. The solution can be built by navigating to the "Build" tab and clicking "Build Solution" or using the keyboard shortcut Ctrl+Shift+B. Building this project should copy the files to the proper directory. If any errors occur with copying the files it is most likely due to the permissions of the folder, these errors can be remedied by setting the permissions as done in the video above. The VST folder, once built, can be found in the 484Compressor/Builds/VisualStudio2022/x64/Debug/VST3 location.

## How to Use

1. Load the plugin in your DAW - you may have to look up where to place .vst3 files for your DAW to recognize them.
2. Adjust the **Compression** controls to shape your dynamics:
   - Set the **Threshold** to determine when compression starts.
   - Use the **Ratio** to control the level of compression.
   - Fine-tune with **Attack** and **Release** for responsiveness.
   - Apply **Makeup Gain** to compensate for output level changes.
3. Chose the **Distortion** mode to add additional grit and colour to the signal:
   - Adjust drive knob to taste.
4. Monitor the real-time visual feedback to perfect your sound.
5. Adjust the mix knob to achieve the desired effect depth.

## System Requirements

- **Operating System**: Windows 10+
- **DAW Compatibility**: Supports VST3 format.
- **Processor**: Dual-core CPU or better.
- **RAM**: 4 GB or more recommended.

## License

484Compressor is free to use for personal and commercial projects. Redistribution or modification of the plugin is subject to the terms of the LICENSE file.

## Acknowledgments

This plugin was developed using the [JUCE framework](https://juce.com), which powers countless professional-grade audio tools.
This plugin was developed using algorithms and principles derived from the following textbooks:

- **"Audio Effects: Theory, Implementation and Application" by Joshua D. Reiss and Andrew P. McPherson**: This book provided foundational concepts and advanced techniques for implementing audio effects.
- **"DAFX: Digital Audio Effects" by Udo Zölzer**: A comprehensive resource that informed the design and implementation of both the compressor and distortion effects. 

## Contact

For feedback, suggestions, or issues, please reach out to charlie.wagerr@gmail.com.

## Note

484Compressor is built with JUCE version 8.0.3 and is currently only tested for Windows machines, if you would like to clone this and build it yourself please ensure that version 8.0.3 of JUCE is installed and a Windows machine is used in order to avoid errors.
