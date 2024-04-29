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

    incomingPortInput.setText("12345", juce::dontSendNotification);
    outgoingPortInput.setText("54321", juce::dontSendNotification);
    delayInput.setText("100", juce::dontSendNotification);

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

