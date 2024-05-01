#include "PluginProcessor.h"
#include "PluginEditor.h"

void PluginWithCustomModuleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)

{
        buffer.clear();
}


using namespace std;

PluginWithCustomModuleAudioProcessor::PluginWithCustomModuleAudioProcessor()
{
    Bg2GuiQueuePtr = make_unique<StaticLockFreeQueue<array<double, 9>, 128>>();
    backgroundTask.Bg2GuiQueuePtr = Bg2GuiQueuePtr.get();
    backgroundTask.startTask();
}

juce::AudioProcessorEditor *PluginWithCustomModuleAudioProcessor::createEditor() {
    auto editor = new PluginWithCustomModuleAudioProcessorEditor(*this);
    /*modelThread.addChangeListener(editor);*/
    return editor;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginWithCustomModuleAudioProcessor();
}


PluginWithCustomModuleAudioProcessor::~PluginWithCustomModuleAudioProcessor()
{
    backgroundTask.stopTask();
}

const juce::String PluginWithCustomModuleAudioProcessor::getName() const
{
    return "PluginWithCustomModule";
}

