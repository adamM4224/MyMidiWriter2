#include <JuceHeader.h>
#include "MidiEvent.h"
#include "PatternOptions.h"
#include "MyLogger.h"


class PatternFetcher {
    public:
        PatternFetcher();
        std::vector<MidiEvent> getPattern(int patternID);
    private:
        PatternOptions options;
        
        std::vector<MidiEvent> jsonToMidi(juce::var& json);
        MyLogger logger { "/Users/adamconn/dsp/plugins/MyMidiWriter2/debug.log" };
};  