
#include "MidiWriter.h"

MidiWriter::MidiWriter(PatternFetcher& f)
    : fetcher(f)
{

}
void MidiWriter::process(
    juce::AudioPlayHead* playHead,
    juce::MidiBuffer& midiMessages
) {
    eventCount++;

    if(playHead == nullptr) {
        return; 
    }

    auto position = playHead->getPosition();

    if(patternID < 0) {
        return;
    }
    if(!position) { return; }


    if(!position->getIsPlaying()) { 
        lastPPQ = 0.0;
        return; 
    }


    auto ppq = position->getPpqPosition();

    if(!ppq.hasValue()) {
        logger.write("return with !ppq");
        return;
    }



    double currentPPQ = *ppq;
    for(auto& event : events) {
        bool crossed = false;

        if(currentPPQ >= lastPPQ) {
            crossed = event.beat >= lastPPQ 
                && event.beat < currentPPQ;
        } 
        else {
            crossed = event.beat >= lastPPQ 
            || event.beat < currentPPQ;
        }

        if(!crossed) {
            continue;
        }
        if(!seen) {
            // DebugLogger::getInstance().log("EMITTING");
            seen = true;

        }

        emitEvent(
            event,
            midiMessages
        );
    }

    lastPPQ = currentPPQ;

}


void MidiWriter::setPattern(int newPatternID) {
    patternID = newPatternID;
    events = fetcher.getPattern(patternID);


    

}

void MidiWriter::reload() {
    events = fetcher.getPattern(patternID);

}

void MidiWriter::emitEvent(
    const MidiEvent& event,
    juce::MidiBuffer& midiMessages
) {


    switch(event.type) {
        case MidiEvent::Type::Note:
            
            midiMessages.addEvent(
                juce::MidiMessage::noteOn(
                    event.channel,
                    event.note,
                    (juce::uint8) event.velocity
                ),
                0
            );
            midiMessages.addEvent(
                juce::MidiMessage::noteOff(
                    event.channel,
                    event.note
                ),
                200
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