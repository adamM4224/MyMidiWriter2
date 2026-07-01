#pragma once

#include <JuceHeader.h>
#include "MidiEvent.h"

#include "MyLogger.h"

struct PatternInfo {
    int id;
    juce::String name;
    juce::File file;
};

class PatternFetcher {
    public:
        PatternFetcher();
        std::vector<MidiEvent> getPattern(int patternID);
        void setDirectory(const juce::File& directory);
        const std::vector<PatternInfo>& getAvailablePatterns() const;
    private:
        std::vector<PatternInfo> patterns;
        juce::File patternDirectory;
        
        std::vector<MidiEvent> jsonToMidi(juce::var& json);
        MyLogger logger { "/Users/adamconn/dsp/plugins/MyMidiWriter2/debug.log" };
};  