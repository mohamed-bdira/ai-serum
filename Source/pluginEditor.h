#pragma once 
#include <JuceHeader.h>
#include "PluginProcessor.h"

class CortexiaAudioProcessorEditor   :public juce::AudioProcessorEditor{
    public:
        CortexiaAudioProcessorEditor(CortexiaAudioProcessor&);
        ~CortexiaAudioProcessorEditor() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        CortexiaAudioProcessor& audioProcessor;
        juce::WebBrowserComponent webComponent;

        double currentPhase = 0.0;
        double phaseDelta = 0.0;
        bool noteIsOn = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CortexiaAudioProcessorEditor)
};