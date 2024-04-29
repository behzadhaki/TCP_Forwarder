#include "PluginProcessor.h"
#include "PluginEditor.h"

void PluginWithCustomModuleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)

{
        buffer.clear();
}


using namespace std;

PluginWithCustomModuleAudioProcessor::PluginWithCustomModuleAudioProcessor()
{
    // Default constructor
    startServer();
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
    return sourceSocket.bindToPort(port);
}

bool PluginWithCustomModuleAudioProcessor::connectToHost(int port)
{
    return destinationSocket.bindToPort(port);
}
void PluginWithCustomModuleAudioProcessor::startServer()
{

    if (!startListening(incomingPort))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                               "Error",
                                               "Failed to listen on incoming port " + juce::String(incomingPort),
                                               "OK");
    }

    if (!connectToHost(outgoingPort))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                               "Error",
                                               "Failed to connect to outgoing port " + juce::String(outgoingPort),
                                               "OK");
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
            cout << "Received " << bytesRead << " bytes from source" << endl;
//            destinationSocket.write(buffer, bytesRead);
        }
    }
}

