#pragma once 

#include <JuceHeader.h>
#include "PatternFetcher.h"
#include "MidiEvent.h"
#include "MyLogger.h"
class MidiWriter {
    public:
        MidiWriter();
        void process(
            juce::AudioPlayHead* playHead, 
            juce::MidiBuffer& midiMessage
        );
        void emitEvent(const MidiEvent& event, juce::MidiBuffer& midiMessages);
        void setPattern(int newPatternID);
        void reload();
        void write(juce::String message);
    private:
        int lastBeat = -1;
        int patternID = -1;
        double lastPPQ = 0.0;
        bool seen = false;
        

        std::vector<MidiEvent> events;
        PatternFetcher fetcher;
        int eventCount = -1;
        MyLogger logger { "/Users/adamconn/dsp/plugins/MyMidiWriter2/debug.log" };

};