#include "PluginProcessor.h"

class PluginWithCustomModuleAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    PluginWithCustomModuleAudioProcessorEditor(PluginWithCustomModuleAudioProcessor&);
    ~PluginWithCustomModuleAudioProcessorEditor() override = default;
    void initializeReadOnlyLabel(juce::Label& label, const juce::String& text);
    void paint(juce::Graphics&) override;
    void resized() override;
    void layoutComponents();
    void connectButtonClicked();
    void timerCallback() override;

    StaticLockFreeQueue<array<double, 9>, 128>* Bg2GuiQueuePtr{nullptr};

private:
    PluginWithCustomModuleAudioProcessor& processor;
    juce::TextEditor incomingPortInput, outgoingPortInput, shouldPrintRcvdMsgs;
    juce::Label incomingPortLabel, outgoingPortLabel, shouldPrintLabel;
    juce::TextButton connectButton;
    juce::Label eegLeft, eegRight, accX, accY, accZ, bodyTemp, battVolt, noise, light;
    juce::Label eegLeftVal, eegRightVal, accXVal, accYVal, accZVal, bodyTempVal, battVoltVal, noiseVal, lightVal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWithCustomModuleAudioProcessorEditor)
};
