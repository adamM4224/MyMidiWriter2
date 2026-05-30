#include "PatternFetcher.h"

PatternFetcher::PatternFetcher() {




}

std::vector<MidiEvent> PatternFetcher::getPattern(int patternID)
{
    juce::String filename;
    juce::String patternIDString = (juce::String) patternID;


    for (const auto& group : options.groups)
    {
        for (const auto& option : group.options)
        {
            if (option.id == patternID)
            {
                filename = option.filename;
                break;
            }
        }

        if (filename.isNotEmpty())
            break;
    }

    if (filename.isEmpty())
        return {};

    // keep your existing path handling
    filename = filename.replace(
        "~",
        juce::File::getSpecialLocation(
            juce::File::userHomeDirectory
        ).getFullPathName()
    );

    juce::File file(filename);

    if (!file.existsAsFile())
    {
        logger.write(filename + " DOES NOT EXIST...crashing");
        return {};
    }

    juce::String contents = file.loadFileAsString();



    juce::var parsed = juce::JSON::parse(contents);

    return jsonToMidi(parsed);
}


std::vector<MidiEvent> PatternFetcher::jsonToMidi(
    juce::var& json
){
    std::vector<MidiEvent> midiEvents;
    auto* root =
        json.getDynamicObject();

    if (root == nullptr)
    {
        logger.write("root == nullptr -- ensure json file has property 'events': [...]");
        return midiEvents;
    }

    auto eventsVar =
        root->getProperty("events");

    if (!eventsVar.isArray())
    {
        return midiEvents;
    }


    auto* events =
        eventsVar.getArray();

    for (auto& eventVar : *events)
    {

        auto* event =
            eventVar.getDynamicObject();

        if (event == nullptr)
        {
            continue;
        }

        MidiEvent midiEvent;

        midiEvent.beat = event->hasProperty("beat")
            ? (double) event->getProperty("beat")
            : 0.0;

        midiEvent.channel = event->hasProperty("channel")
            ? (int) event->getProperty("channel")
            : 1;

        juce::String type =
            event->hasProperty("type")
                ? event->getProperty("type").toString()
                : "";


        if (type == "Note")
        {
            midiEvent.type =
                MidiEvent::Type::Note;

            midiEvent.note =
                event->hasProperty("note")
                    ? (int) event->getProperty("note")
                    : 60;


            midiEvent.velocity =
                event->hasProperty("velocity")
                    ? (int) event->getProperty("velocity")
                    : 100;


            midiEvent.duration =
                event->hasProperty("duration")
                    ? (double) event->getProperty("duration")
                    : 0.25;
            midiEvent.startPPQ = midiEvent.beat;
            midiEvent.endPPQ = midiEvent.beat + midiEvent.duration;
        }
        else if (type == "CC")
        {
            midiEvent.type =
                MidiEvent::Type::CC;

            midiEvent.controller =
                event->hasProperty("controller")
                    ? (int) event->getProperty("controller")
                    : 0;



            midiEvent.value =
                event->hasProperty("value")
                    ? (int) event->getProperty("value")
                    : 0;

        }

        midiEvents.push_back(
            midiEvent
        );
    }

    return midiEvents;
}