#pragma once

#include <shared_processing_code/shared_processing_code.h>
#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include "background_thread.h"
#include "LockFreeQueue.h"

class PluginWithCustomModuleAudioProcessor : public PluginHelpers::ProcessorBase
{
public:
    BackgroundTask backgroundTask{};

    PluginWithCustomModuleAudioProcessor();
    ~PluginWithCustomModuleAudioProcessor() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    const juce::String getName() const override;

    juce::AudioProcessorEditor *createEditor() override;

    // queues
    unique_ptr<StaticLockFreeQueue<array<double, 9>, 128>> Bg2GuiQueuePtr{};

private:



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWithCustomModuleAudioProcessor)
};