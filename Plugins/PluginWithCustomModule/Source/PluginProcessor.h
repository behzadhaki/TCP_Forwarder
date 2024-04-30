#pragma once

#include <shared_processing_code/shared_processing_code.h>
#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include "background_thread.h"

class PluginWithCustomModuleAudioProcessor : public PluginHelpers::ProcessorBase
{
public:
    BackgroundTask backgroundTask{};

    PluginWithCustomModuleAudioProcessor();
    ~PluginWithCustomModuleAudioProcessor() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    const juce::String getName() const override;

    juce::AudioProcessorEditor *createEditor() override;

private:



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWithCustomModuleAudioProcessor)
};