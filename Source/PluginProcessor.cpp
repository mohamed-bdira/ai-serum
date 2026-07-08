#include "PluginProcessor.h"
#include "PluginEditor.h"

CortexiaAudioProcessor::CortexiaAudioProcessor()
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

CortexiaAudioProcessor::~CortexiaAudioProcessor() {}

const juce::String CortexiaAudioProcessor::getName() const { return JucePlugin_Name; }

void CortexiaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}

void CortexiaAudioProcessor::releaseResources() {}

void CortexiaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

#if JucePlugin_IsSynth
    
    auto sampleRate = getSampleRate();

    // 1. PROCESS MIDI MESSAGES
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        
        if (message.isNoteOn()) // <--- Fixed typo here
        {
            // Fixed typo on "Hertz" here
            double frequency = juce::MidiMessage::getMidiNoteInHertz (message.getNoteNumber());
            phaseDelta = frequency / sampleRate;
            noteIsOn = true;
        }
        else if (message.isNoteOff())
        {
            noteIsOn = false;
        }
    }

    // 2. GENERATE AUDIO (SAWTOOTH WAVE)
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) // <--- Fixed "samples" typo here
    {
        float currentSampleValue = 0.0f;

        if (noteIsOn)
        {
            currentSampleValue = (float)(currentPhase * 2.0 - 1.0) * 0.1f;

            currentPhase += phaseDelta;
            if (currentPhase >= 1.0)
                currentPhase -= 1.0;
        }
        else
        {
            currentPhase = 0.0; 
        }

        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            buffer.getWritePointer(channel)[sample] = currentSampleValue;
        }
    }

#else

    // EFFECT LOGIC: Pass-through mode
    
#endif
}

bool CortexiaAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* CortexiaAudioProcessor::createEditor(){
    return new CortexiaAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){
    return new CortexiaAudioProcessor();
}