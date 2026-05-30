#include "PatternOptions.h"

PatternOptions::PatternOptions()
{

    /*
    ADD DRUM OPTIONS
    */
    PatternGroup drumGroup;
    drumGroup.name = "Drum Options";
    std::vector<PatternOption> drumOptions = {
        {currentID++, "Drums 0001", "~/dsp/songs/drums/drums0001.json"}
    };
    drumGroup.options = drumOptions;

    /*
    ADD GENERAL OPTIONS
    */
    PatternGroup generalGroup;
    generalGroup.name = "General Options";
    std::vector<PatternOption> generalOptions = {
        {currentID++, "General 0001", "~/dsp/songs/general/general0001.json"}
    };
    generalGroup.options = generalOptions;

    groups.push_back(drumGroup);
    groups.push_back(generalGroup);

    /*
    ADD SONG SPECIFIC
    */
    std::vector<juce::String> songs = {
        "songA"
    };

    for (auto &song : songs){
        PatternGroup group;

        group.name = song;

        for (int i = 0; i < 10; i++){
            PatternOption option;

            option.id = currentID++;

            option.filename =
                "~/dsp/songs/" + song + "/track" + juce::String((i + 1)).paddedLeft('0', 4) + ".json";
            option.name = "track " + juce::String((i+1)).paddedLeft('0', 4);

            group.options.push_back(
                option);
        }

        groups.push_back(group);
    }
}