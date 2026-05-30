#pragma once 

#include <JuceHeader.h>
using namespace juce;

class MyLogger
{
public:
    MyLogger(const String& filename);

    void write(const String& message);

private:
    File file;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLogger)
};