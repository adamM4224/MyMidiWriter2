#pragma once

#include <JuceHeader.h>

struct PatternOption
{
    int id;
    juce::String name;
    juce::String filename;
};

struct PatternGroup
{
    juce::String name;

    std::vector<PatternOption> options;
};

class PatternOptions
{
public:

    PatternOptions();


    std::vector<PatternGroup> groups;

private:
    int currentID = 1;
};



