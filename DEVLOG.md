# Updated the Select to persist in memory.
- plugged into audioProcessor->getState and setState to load values from history.
- This also fixed the json file not persisting. which was nice.
- Note - the getState sets the state and setState gets the state, and least in how I would think of those names.

