
#pragma once
#include <JuceHeader.h>

struct MidiEvent
{
    enum class Type
    {
        Note,
        CC,
        PitchBend,
        Aftertouch,
        PolyAftertouch,
        ProgramChange,
        ChannelPressure
    };

    Type type;

    double beat;

    int channel = 1;

    // Note
    int note = 0;
    int velocity = 0;
    double duration = 0.0;

    // DERIVED
    double startPPQ = 0.0;
    double endPPQ = 0.0;

    // CC
    int controller = 0;
    int value = 0;

    // Pitch bend
    int bend = 8192;

    // Poly aftertouch
    int pressureNote = 0;
    int pressureValue = 0;

    // Channel pressure
    int channelPressure = 0;

    // Program
    int program = 0;

    juce::String toString() const
    {
        return "beat=" + juce::String(beat)
            + ", channel=" + juce::String(channel)
            + ", note=" + juce::String(note)
            + ", velocity=" + juce::String(velocity)
            + ", duration=" + juce::String(duration);
    }
};