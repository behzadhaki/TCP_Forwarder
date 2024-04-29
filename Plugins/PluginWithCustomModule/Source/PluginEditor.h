#pragma once

#include "PluginProcessor.h"

class PluginWithCustomModuleAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PluginWithCustomModuleAudioProcessorEditor (PluginWithCustomModuleAudioProcessor&);
    ~PluginWithCustomModuleAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void layoutComponents();
private:
    PluginWithCustomModuleAudioProcessor& processor;
    juce::TextEditor incomingPortInput, outgoingPortInput, delayInput;
    juce::TextButton startButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWithCustomModuleAudioProcessorEditor)
};
