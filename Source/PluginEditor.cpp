/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyMidiWriter2AudioProcessorEditor::MyMidiWriter2AudioProcessorEditor (MyMidiWriter2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    DBG("INSIDE PLUGIN EDITOR");

        // add patterns to choose from
    for(const auto& group : patternOptions.groups) {
        patternSelect.addSectionHeading(group.name);
        for(const auto& option : group.options) {
            patternSelect.addItem(option.name, option.id);
        }
    }
    // DebugLogger::getInstance().log("adding pattern select");
    patternSelect.addListener( this );
    addAndMakeVisible(patternSelect);

        // DebugLogger::getInstance().log("adding reload buttong");
    addAndMakeVisible(reloadButton);
    reloadButton.onClick = [this] {
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
