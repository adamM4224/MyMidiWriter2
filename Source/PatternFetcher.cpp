
#include "PatternFetcher.h"

PatternFetcher::PatternFetcher() {




}

const std::vector<PatternInfo>& PatternFetcher::getAvailablePatterns() const
{
    return patterns;
}
std::vector<MidiEvent> PatternFetcher::getPattern(int patternID)
{
    juce::File file;


    for (const auto& pattern : patterns)
    {
        if (pattern.id == patternID)
        {
            file = pattern.file;
            break;
        }
    }


    if (!file.existsAsFile())
    {
        logger.write("Pattern file not found");
        return {};
    }


    auto contents = file.loadFileAsString();

    auto json = juce::JSON::parse(contents);

    return jsonToMidi(json);
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
        logger.write(midiEvent.toString());
        midiEvents.push_back(
            midiEvent
        );
    }

    return midiEvents;
}


void PatternFetcher::setDirectory(const juce::File& directory)
{
    patternDirectory = directory;

    patterns.clear();

    if (!patternDirectory.isDirectory())
    {
        logger.write("Pattern directory does not exist");
        return;
    }


    auto files = patternDirectory.findChildFiles(
        juce::File::findFiles,
        false,
        "*.json"
    );


    int id = 1;

    for (auto& file : files)
    {
        PatternInfo info;

        info.id = id++;
        info.name = file.getFileNameWithoutExtension();
        info.file = file;

        patterns.push_back(info);
    }
}