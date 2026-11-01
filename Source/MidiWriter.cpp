
#include "MidiWriter.h"
#include <cmath>
MidiWriter::MidiWriter(PatternFetcher& f)
    : fetcher(f)
{

}
void MidiWriter::process(
    juce::AudioPlayHead* playHead,
    juce::MidiBuffer& midiMessages,
    int blockSize,
    double sampleRate
) {
    eventCount++;

    // Release any previously sounding notes.
    auto releaseNotes = [&]() {
        for (int channel = 1; channel <= 16; ++channel) {
            for (int note = 0; note <= 127; ++note) {
                midiMessages.addEvent(
                    juce::MidiMessage::noteOff(channel, note),
                    0
                );
            }
        }
    };

    // Handle missing transport information.
    if (playHead == nullptr) {
        if (wasPlaying) {
            releaseNotes();
        }

        wasPlaying = false;
        lastPPQ = 0.0;
        return;
    }

    auto position = playHead->getPosition();

    if (!position) {
        if (wasPlaying) {
            releaseNotes();
        }

        wasPlaying = false;
        lastPPQ = 0.0;
        return;
    }

    // Handle transport stop.
    if (!position->getIsPlaying()) {
        if (wasPlaying) {
            releaseNotes();
        }

        wasPlaying = false;
        lastPPQ = 0.0;
        return;
    }

    auto ppq = position->getPpqPosition();
    auto bpm = position->getBpm();

    if (!ppq.hasValue() ||
        !bpm.hasValue() ||
        *bpm <= 0.0 ||
        sampleRate <= 0.0 ||
        blockSize <= 0) {
        return;
    }

    const double currentPPQ = *ppq;
    const double samplesPerBeat = sampleRate * 60.0 / *bpm;

    const double blockStartPPQ = currentPPQ;
    const double blockDurationPPQ =
        static_cast<double>(blockSize) / samplesPerBeat;

    const double blockEndPPQ =
        blockStartPPQ + blockDurationPPQ;

    // Detect playback starting or a transport discontinuity.
    const bool starting = !wasPlaying;

    // lastPPQ stores the expected start of the next block.
    // Allow a small tolerance for host timing differences.
    const double tolerancePPQ = 2.0 / samplesPerBeat;

    const bool jumped =
        wasPlaying &&
        std::abs(currentPPQ - lastPPQ) > tolerancePPQ;

    if (starting || jumped) {
        releaseNotes();
    }

    wasPlaying = true;

    if (patternPath.isEmpty()) {
        lastPPQ = blockEndPPQ;
        return;
    }

    // Convert a musical position to a sample within this block.
    auto sampleOffsetForPPQ = [&](double eventPPQ) -> int {
        const double offset =
            (eventPPQ - blockStartPPQ) * samplesPerBeat;

        return juce::jlimit(
            0,
            blockSize - 1,
            static_cast<int>(std::round(offset))
        );
    };

    // First schedule note-offs.
    // This ensures consecutive notes of the same pitch are
    // released before being triggered again at the same sample.
    for (const auto& event : events) {
        if (event.type != MidiEvent::Type::Note) {
            continue;
        }

        if (event.endPPQ >= blockStartPPQ &&
            event.endPPQ < blockEndPPQ) {

            midiMessages.addEvent(
                juce::MidiMessage::noteOff(
                    event.channel,
                    event.note
                ),
                sampleOffsetForPPQ(event.endPPQ)
            );
        }
    }

    // Then schedule note-ons and controller events.
    for (const auto& event : events) {
        switch (event.type) {

            case MidiEvent::Type::Note:
            {
                if (event.startPPQ >= blockStartPPQ &&
                    event.startPPQ < blockEndPPQ) {

                    midiMessages.addEvent(
                        juce::MidiMessage::noteOn(
                            event.channel,
                            event.note,
                            static_cast<juce::uint8>(event.velocity)
                        ),
                        sampleOffsetForPPQ(event.startPPQ)
                    );
                }

                break;
            }

            case MidiEvent::Type::CC:
            {
                if (event.beat >= blockStartPPQ &&
                    event.beat < blockEndPPQ) {

                    midiMessages.addEvent(
                        juce::MidiMessage::controllerEvent(
                            event.channel,
                            event.controller,
                            event.value
                        ),
                        sampleOffsetForPPQ(event.beat)
                    );
                }

                break;
            }

            default:
                break;
        }
    }

    // Expected starting PPQ of the next processing block.
    lastPPQ = blockEndPPQ;
}


void MidiWriter::setPattern(juce::String fullFilePath) {
    patternPath = fullFilePath;
    juce::String message  = "full file path: " + fullFilePath;
    logger.write(message);
    events = fetcher.getPattern(patternPath);



    

}

void MidiWriter::reload() {
    events = fetcher.getPattern(patternPath);

}
void MidiWriter::emitEvent(
    const MidiEvent& event,
    juce::MidiBuffer& midiMessages
) {
    switch (event.type) {
        case MidiEvent::Type::Note:
            midiMessages.addEvent(
                juce::MidiMessage::noteOn(
                    event.channel,
                    event.note,
                    static_cast<juce::uint8>(event.velocity)
                ),
                0
            );
            break;

        case MidiEvent::Type::CC:
            midiMessages.addEvent(
                juce::MidiMessage::controllerEvent(
                    event.channel,
                    event.controller,
                    event.value
                ),
                0
            );
            break;
    }
}


void MidiWriter::write(juce::String message) {
    if(eventCount % 100 == 0) {
        logger.write(message);
    }
}