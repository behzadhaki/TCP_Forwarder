#include "PluginProcessor.h"
#include "PluginEditor.h"


PluginWithCustomModuleAudioProcessorEditor::PluginWithCustomModuleAudioProcessorEditor(PluginWithCustomModuleAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(400, 300);

    addAndMakeVisible(incomingPortInput);
    addAndMakeVisible(outgoingPortInput);
    addAndMakeVisible(delayInput);
    addAndMakeVisible(startButton);

    incomingPortInput.setJustification(juce::Justification::centred);
    outgoingPortInput.setJustification(juce::Justification::centred);
    delayInput.setJustification(juce::Justification::centred);

    incomingPortInput.setText(juce::String(processor.incomingPort), juce::dontSendNotification);
    outgoingPortInput.setText(juce::String(processor.outgoingPort), juce::dontSendNotification);
    delayInput.setText(juce::String(processor.timerDelayMS), juce::dontSendNotification);

}

void PluginWithCustomModuleAudioProcessorEditor::layoutComponents()
{
    auto area = getLocalBounds().reduced(10);
    incomingPortInput.setBounds(area.removeFromTop(24).reduced(50, 0));
    outgoingPortInput.setBounds(area.removeFromTop(24).reduced(50, 0));
    delayInput.setBounds(area.removeFromTop(24).reduced(50, 0));
}


void PluginWithCustomModuleAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}



void PluginWithCustomModuleAudioProcessorEditor::resized()
{
    layoutComponents();
}

