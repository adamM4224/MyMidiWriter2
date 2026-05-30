/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PatternOptions.h"
#include "MyLogger.h"

//==============================================================================
/**
*/
class MyMidiWriter2AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           public juce::ComboBox::Listener
                                        
{
public:
    MyMidiWriter2AudioProcessorEditor (MyMidiWriter2AudioProcessor&);
    ~MyMidiWriter2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyMidiWriter2AudioProcessor& audioProcessor;
    PatternOptions patternOptions;
    juce::TextButton reloadButton { "Reload" };
    juce::ComboBox patternSelect;
    MyLogger logger { "/Users/adamconn/dsp/plugins/MyMidiWriter2/debug.log" };



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyMidiWriter2AudioProcessorEditor)
};
