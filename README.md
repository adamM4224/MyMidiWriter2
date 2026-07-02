

Adding file persistence. 
There is an issue with the json files when you close an open logic, they don't reload. 
Need to add a storage file to reset all the values and persist the select on top of it.


- And as of now the file is fixed 


I think I can initialize everything here in PlugInEditor.h
//==============================================================================
MyMidiWriter2AudioProcessorEditor::MyMidiWriter2AudioProcessorEditor (MyMidiWriter2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    DBG("INSIDE PLUGIN EDITOR");

        // add patterns to choose from
    for(const auto& group : patternOptions.groups) {
        patternSelect.addSectionHeading(group.name);
        for(const auto& option : group.options) {
            patternSelect.addItem(option.name, option.id);
        }
    }
    // DebugLogger::getInstance().log("adding pattern select");
    patternSelect.addListener( this );
    addAndMakeVisible(patternSelect);

        // DebugLogger::getInstance().log("adding reload buttong");
    addAndMakeVisible(reloadButton);
    reloadButton.onClick = [this] {
        audioProcessor.reload();
    };
}


1) Need to read the file
2) Need to set the selects with the current file.
3) Need to load the json 
4) When switching patterns - need to save the previous.



Instead of adding files, we should take a directory, and from that directory parse the json files. I think that is a lot for now though.



