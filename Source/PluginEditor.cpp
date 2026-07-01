/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyMidiWriter2AudioProcessorEditor::MyMidiWriter2AudioProcessorEditor (MyMidiWriter2AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);

    DBG("INSIDE PLUGIN EDITOR");

    for (const auto& pattern : audioProcessor.getPatterns())
    {
        patternSelect.addItem(pattern.name, pattern.id);
    }

    patternSelect.addListener(this);
    addAndMakeVisible(patternSelect);

    // Restore the current selection from the processor
    patternSelect.setSelectedId(audioProcessor.getPattern(),
                                juce::dontSendNotification);

    addAndMakeVisible(reloadButton);

    reloadButton.onClick = [this]
    {
        audioProcessor.reload();
    };
}

MyMidiWriter2AudioProcessorEditor::~MyMidiWriter2AudioProcessorEditor()
{
    patternSelect.removeListener(this);
}

//==============================================================================
void MyMidiWriter2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Midi Writer 2", getLocalBounds(), juce::Justification::centred, 1);
}

void MyMidiWriter2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    patternSelect.setBounds(20, 20, 150, 30);

    reloadButton.setBounds(20, 50, 100, 30);
}

void MyMidiWriter2AudioProcessorEditor::comboBoxChanged(
    juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged != &patternSelect)
        return;

    int selected = patternSelect.getSelectedId();
    audioProcessor.setPattern(selected);

    // DBG("Pattern selected: " << selected);
}
