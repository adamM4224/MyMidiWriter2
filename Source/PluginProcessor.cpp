/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyMidiWriter2AudioProcessor::MyMidiWriter2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    // instantiate here
    patternID = 1;          // default pattern
    midiWriter.setPattern(patternID);
}

MyMidiWriter2AudioProcessor::~MyMidiWriter2AudioProcessor()
{
}

//==============================================================================
const juce::String MyMidiWriter2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyMidiWriter2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyMidiWriter2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyMidiWriter2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyMidiWriter2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyMidiWriter2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyMidiWriter2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyMidiWriter2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyMidiWriter2AudioProcessor::getProgramName (int index)
{
    return {};
}

void MyMidiWriter2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyMidiWriter2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MyMidiWriter2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyMidiWriter2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MyMidiWriter2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    midiMessages.clear();

    static bool sent = false;

    // EMIT THE EVENTS HERE
    midiWriter.process(
        getPlayHead(),
        midiMessages
    );

    if (true && !sent)
    {
        midiMessages.addEvent(
            juce::MidiMessage::noteOn(1, 60, (juce::uint8)100),
            0);

        midiMessages.addEvent(
            juce::MidiMessage::noteOff(1, 60),
            480);

        sent = true;
    }
    /*=========================================================================
    ===========================================================================
    
    SEND A SINGLE NOTE LOGIC - KEEP FOR DEBUGGING
    midiMessages.clear();

    static bool sent = false;

    if (!sent)
    {
        midiMessages.addEvent(
            juce::MidiMessage::noteOn(1, 60, (juce::uint8)100),
            0);

        midiMessages.addEvent(
            juce::MidiMessage::noteOff(1, 60),
            480);

        sent = true;
    }
    ---------------------------------------------------------------------------
    ===========================================================================
    */
}

//==============================================================================
bool MyMidiWriter2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

void MyMidiWriter2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);

    stream.writeInt(patternID);
}

// NOTE TO SELF: setState gets the state - getState sets the state. No joke.
void MyMidiWriter2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data,
                                   static_cast<size_t>(sizeInBytes),
                                   false);

    patternID = stream.readInt();

    midiWriter.setPattern(patternID);
}
juce::AudioProcessorEditor* MyMidiWriter2AudioProcessor::createEditor()
{
    return new MyMidiWriter2AudioProcessorEditor (*this);
}





// MY OVERRIDES 
// void MyMidiWriter2AudioProcessor::setPattern(int id)
// {
//     patternID = id;

//     midiWriter.setPattern(patternID);

//     juce::DynamicObject::Ptr obj = new juce::DynamicObject();
//     obj->setProperty("selectedPattern", patternID);

//     history.save(juce::var(obj));
// }

void MyMidiWriter2AudioProcessor::setPattern(int id)
{
    patternID = id;

    midiWriter.setPattern(patternID);

    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    obj->setProperty("selectedPattern", patternID);


}

void MyMidiWriter2AudioProcessor::reload() {
    midiWriter.reload();
}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyMidiWriter2AudioProcessor();
}


int MyMidiWriter2AudioProcessor::getPattern() const
{
    return patternID;
}



