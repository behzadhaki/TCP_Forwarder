#include "PluginProcessor.h"
#include "PluginEditor.h"

void PluginWithCustomModuleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)

{
        buffer.clear();
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginWithCustomModuleAudioProcessor::PluginWithCustomModuleAudioProcessor()
{
    // Default constructor
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
    sourceSocket.close();
    destinationSocket.close();
}

const juce::String PluginWithCustomModuleAudioProcessor::getName() const
{
    return "PluginWithCustomModule";
}

bool PluginWithCustomModuleAudioProcessor::startListening(int port)
{
    return sourceSocket.createListener(port);
}

bool PluginWithCustomModuleAudioProcessor::connectToHost(const juce::String& host, int port)
{
    return destinationSocket.connect(host, port);
}
void PluginWithCustomModuleAudioProcessor::startServer()
{

    if (!startListening(incomingPort))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                               "Error",
                                               "Failed to listen on incoming port!",
                                               "OK");
        return;
    }

    if (!connectToHost("localhost", outgoingPort))
    {
        juce::Logger::writeToLog("Failed to connect to destination port!");
        return;
    }

    startTimer(timerDelayMS);
}

void PluginWithCustomModuleAudioProcessor::timerCallback()
{
    static const juce::Colour green = juce::Colours::green;
    static const juce::Colour red = juce::Colours::red;

    if (sourceSocket.isConnected())
    {
        char buffer[1024] = {0};
        const int bytesRead = sourceSocket.read(buffer, sizeof(buffer), false);

        if (bytesRead > 0)
        {
            destinationSocket.write(buffer, bytesRead);
        }
    }
}

