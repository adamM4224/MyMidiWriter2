#pragma once 

#include <JuceHeader.h>
#include "PatternFetcher.h"
#include "MidiEvent.h"
#include "MyLogger.h"
class MidiWriter {
    public:
        MidiWriter(PatternFetcher& fetcher);
        void process(
            juce::AudioPlayHead* playHead,
            juce::MidiBuffer& midiMessages,
            int blockSize,
            double sampleRate
        );
        void emitEvent(const MidiEvent& event, juce::MidiBuffer& midiMessages);
        void setPattern(juce::String fullFilePath);

        void reload();
        void write(juce::String message);

    private:
        int lastBeat = -1;
        int patternID = -1;
        juce::String patternPath;
        double lastPPQ = 0.0;
        bool seen = false;
        bool wasPlaying = false;
        

        std::vector<MidiEvent> events;
        PatternFetcher& fetcher;
        int eventCount = -1;
        MyLogger logger { "/Users/adamconn/dsp/plugins/MyMidiWriter2/debug.log" };

};