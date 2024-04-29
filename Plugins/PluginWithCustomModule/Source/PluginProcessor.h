#pragma once

#include <shared_processing_code/shared_processing_code.h>
#include <shared_plugin_helpers/shared_plugin_helpers.h>

class PluginWithCustomModuleAudioProcessor : public PluginHelpers::ProcessorBase, private juce::Timer
{
public:
    PluginWithCustomModuleAudioProcessor();
    ~PluginWithCustomModuleAudioProcessor() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    const juce::String getName() const override;

    bool startListening(int port);
    bool connectToHost(int port);
    void startServer();
    void timerCallback() override;
    juce::AudioProcessorEditor *createEditor() override;
    int incomingPort{8000};
    int outgoingPort{8050};
    int timerDelayMS{100};
private:

    juce::StreamingSocket sourceSocket, destinationSocket;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWithCustomModuleAudioProcessor)
};