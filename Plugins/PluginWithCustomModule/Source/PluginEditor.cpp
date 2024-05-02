#include "PluginProcessor.h"
#include "PluginEditor.h"


PluginWithCustomModuleAudioProcessorEditor::PluginWithCustomModuleAudioProcessorEditor(PluginWithCustomModuleAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    Bg2GuiQueuePtr = processor.Bg2GuiQueuePtr.get();

    setSize(500, 400);

    // Adding text editors and connect button
    addAndMakeVisible(incomingPortInput);
    addAndMakeVisible(outgoingPortInput);
    addAndMakeVisible(delayInput);
    addAndMakeVisible(connectButton);

    // Setting text editors' justification
    incomingPortInput.setJustification(juce::Justification::centred);
    incomingPortInput.setText(juce::String(processor.backgroundTask.incomingPort));
    outgoingPortInput.setJustification(juce::Justification::centred);
    outgoingPortInput.setText(juce::String(processor.backgroundTask.outgoingPortDecoded));
    delayInput.setJustification(juce::Justification::centred);
    delayInput.setText(juce::String(processor.backgroundTask.timerDelayMS));

    // Adding labels
    addAndMakeVisible(incomingPortLabel);
    addAndMakeVisible(outgoingPortLabel);
    addAndMakeVisible(delayLabel);

    // Setting labels' text
    incomingPortLabel.setText("Server Port -> ", juce::dontSendNotification);
    outgoingPortLabel.setText("    Pd Port -> ", juce::dontSendNotification);
    delayLabel.setText(       "   Delay ms -> ", juce::dontSendNotification);

    // Ineditable text boxes
    initializeReadOnlyLabel(eegLeft, "EEG Left");
    initializeReadOnlyLabel(eegLeftVal, "---");
    initializeReadOnlyLabel(eegRight, "EEG Right");
    initializeReadOnlyLabel(eegRightVal, "---");
    initializeReadOnlyLabel(accX, "Acc X");
    initializeReadOnlyLabel(accXVal, "---");
    initializeReadOnlyLabel(accY, "Acc Y");
    initializeReadOnlyLabel(accYVal, "---");
    initializeReadOnlyLabel(accZ, "Acc Z");
    initializeReadOnlyLabel(accZVal, "---");
    initializeReadOnlyLabel(bodyTemp, "Body Temp");
    initializeReadOnlyLabel(bodyTempVal, "---");
    initializeReadOnlyLabel(battVolt, "Batt Volt");
    initializeReadOnlyLabel(battVoltVal, "---");
    initializeReadOnlyLabel(noise, "Noise");
    initializeReadOnlyLabel(noiseVal, "---");
    initializeReadOnlyLabel(light, "Light");
    initializeReadOnlyLabel(lightVal, "---");

    // Connect button callback
    connectButton.onClick = [this]() { this->connectButtonClicked(); };
    connectButton.setButtonText("Connect");

    layoutComponents();

    startTimerHz(30);
}



void PluginWithCustomModuleAudioProcessorEditor::initializeReadOnlyLabel(juce::Label& label, const juce::String& text) {
    addAndMakeVisible(label);
    label.setText(text, juce::dontSendNotification);
    label.setEditable(false);
}

void PluginWithCustomModuleAudioProcessorEditor::layoutComponents()
{
    auto area = getLocalBounds().reduced(10);

    area.removeFromTop(24);

    auto connect_area = area.removeFromTop(24);
    auto connect_width = connect_area.getWidth() / 6;
    incomingPortLabel.setBounds(connect_area.removeFromLeft(connect_width));
    incomingPortInput.setBounds(connect_area.removeFromLeft(connect_width));
    outgoingPortLabel.setBounds(connect_area.removeFromLeft(connect_width));
    outgoingPortInput.setBounds(connect_area.removeFromLeft(connect_width));
    delayLabel.setBounds(connect_area.removeFromLeft(connect_width));
    delayInput.setBounds(connect_area);

    area.removeFromTop(24);

    auto buttonArea = area.removeFromTop(24);
    auto buttonWidth = buttonArea.getWidth() / 5;
    buttonArea.removeFromLeft(buttonWidth*2);
    connectButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

    area.removeFromTop(48);

    auto eegArea = area.removeFromTop(24).reduced(50, 0);
    auto width  = eegArea.getWidth() / 4;
    eegLeft.setBounds(eegArea.removeFromLeft(width));
    eegLeftVal.setBounds(eegArea.removeFromLeft(width));
    eegRight.setBounds(eegArea.removeFromLeft(width));
    eegRightVal.setBounds(eegArea);

    area.removeFromTop(24);

    auto accArea = area.removeFromTop(24).reduced(50, 0);
    auto w = accArea.getWidth() / 6;
    accX.setBounds(accArea.removeFromLeft(w));
    accXVal.setBounds(accArea.removeFromLeft(w));
    accY.setBounds(accArea.removeFromLeft(w));
    accYVal.setBounds(accArea.removeFromLeft(w));
    accZ.setBounds(accArea.removeFromLeft(w));
    accZVal.setBounds(accArea);

    area.removeFromTop(24);

    auto restArea = area.removeFromTop(24).reduced(50, 0);
    auto w2 = restArea.getWidth() / 4;

    bodyTemp.setBounds(restArea.removeFromLeft(w2));
    bodyTempVal.setBounds(restArea.removeFromLeft(w2));
    battVolt.setBounds(restArea.removeFromLeft(w2));
    battVoltVal.setBounds(restArea.removeFromLeft(w2));

    area.removeFromTop(24);

    restArea = area.removeFromTop(24).reduced(50, 0);

    noise.setBounds(restArea.removeFromLeft(w2));
    noiseVal.setBounds(restArea.removeFromLeft(w2));
    light.setBounds(restArea.removeFromLeft(w2));
    lightVal.setBounds(restArea);

}

void PluginWithCustomModuleAudioProcessorEditor::connectButtonClicked()
{
    // put --- in the labels
    eegLeftVal.setText("---", juce::dontSendNotification);
    eegRightVal.setText("---", juce::dontSendNotification);
    accXVal.setText("---", juce::dontSendNotification);
    accYVal.setText("---", juce::dontSendNotification);
    accZVal.setText("---", juce::dontSendNotification);
    bodyTempVal.setText("---", juce::dontSendNotification);
    battVoltVal.setText("---", juce::dontSendNotification);
    noiseVal.setText("---", juce::dontSendNotification);
    lightVal.setText("---", juce::dontSendNotification);

    array<int, 3> values;
    values[0] = incomingPortInput.getText().getIntValue();
    values[1] = outgoingPortInput.getText().getIntValue();
    values[2] = delayInput.getText().getIntValue();

    processor.backgroundTask.connectButtonClicked(values);
}

void PluginWithCustomModuleAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}



void PluginWithCustomModuleAudioProcessorEditor::resized()
{
    layoutComponents();
}
void PluginWithCustomModuleAudioProcessorEditor::timerCallback()
{
    if (Bg2GuiQueuePtr != nullptr) {
            if (Bg2GuiQueuePtr->getNumReady() > 0) {
                auto data = Bg2GuiQueuePtr->getLatestOnly();
                eegLeftVal.setText(juce::String(data[0]), juce::dontSendNotification);
                eegRightVal.setText(juce::String(data[1]), juce::dontSendNotification);
                accXVal.setText(juce::String(data[2]), juce::dontSendNotification);
                accYVal.setText(juce::String(data[3]), juce::dontSendNotification);
                accZVal.setText(juce::String(data[4]), juce::dontSendNotification);
                bodyTempVal.setText(juce::String(data[5]), juce::dontSendNotification);
                battVoltVal.setText(juce::String(data[6]), juce::dontSendNotification);
                noiseVal.setText(juce::String(data[7]), juce::dontSendNotification);
                lightVal.setText(juce::String(data[8]), juce::dontSendNotification);
            }

    }
}
