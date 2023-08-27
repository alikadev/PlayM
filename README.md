# PlayM

## A terminal music player

This application is a music player for your terminal. All your musics added to your playlist and listen in every environement (if you have a terminal with posix).


<!-- ======================================== -->


# Quick start

## Building the application

You will need `make`, `gcc`, `SDL2` and `SDL_Mixer`.

- Open a terminal
- Do `git clone https://github.com/alikadev/PlayM.git && cd PlayM`
- `make`

## Run it

- Open a terminal next to the `playm` binary
- Do `./playm`
- Use it!

# General informations

## Commands

- `quit`
- `help`
- `play`
- `pause`
- `vol volume`
- `set time`
- `play`
- `load path`
- `ldir path` `[ext]`
- `list`
- `music`
- `ren`
- `renlist`

## File format

PlayM uses the `M3U` format to save its playlists.

<!-- ======================================== -->


## Previsions / TODOs

Theses are the previsions for the next versions.

### (0.1.\*) - Basic functionnalities

Probablity: __++__

Add all the basic functionnalities

- Multiple playlists
- Write/Save playlists
- Support for terminal arguments

### (0.2.\*) - Advanced UI

Probablity: __+__

Advanced CLI or a basic TUI

### (0.3.\*) - Advanced UX

Probablity: __~__

Verify the user experience and upgrade it

### (0.4.\*) - Advanced functionnalities

Probablity: __~__

Advanced functionnalities (theses are only idea)

- Customisation
- Configuration
- Scripting
- Online communications (Spotify, SoundCloud, Youtube)

### (1.0.0) - Official release

Probablity: __--__

Everything is there, it works and is easy to use


<!-- ======================================== -->


## Version list

### 0.1.5 (devel)

- Change the default music name (remove extension)
- Create command `save` (to save to m3u)

### 0.1.4

_More commands and bug fixing_

- Create command `ren`
- Create command `renlist`
- Rename command `playlist` to `list`
- Add a _trace_ building mode
- Fix bug
  - Linked list remove

### 0.1.3

_Stability and better command formatting_

- Add commands (internally)
- Command `load` support `*` or nothing as ext
- Add support for `space` in path
  - `"Example and more"`
  - `Example\ and\ more`

### 0.1.2 - Single playlist

_Nothing visible, but a lot in the background_

- Reformat of the codebase
  - Better naming
  - Reformat code-data relation
- Rename `resume` to `play`
- Rename `play` to `start`
- Rename `list` to `playlist`
- Rename `info` to `music`
- Remove `help` coloring 
  - (Because will be official during next weeks)
- Remove `rand`
- Create a REAL `Playlist` structure
- Use `OrderedLinkedList` instead of dynamic array
  - Remove Quicksort because `OrderedLinkedList` sort at insertion

### 0.1.1

_Better stability and a bit of reformatting_

- Fix some bugs
  - Input handling crash
  - Halt music before quitting
- Ignore SIGINT

### 0.1.0 - First usable version

_This version is really bad but it works..._

- Create the github page
- Split project into multiple files
- Simple terminal experience (crash because of fgets)
- Single playlist
- Rename command `load` `ldir`
- Create command `load` (single file)
- Create command `help` (implemented only global help)
- Create command `set`
- Create command `load`
- Create command `rand`
- Create command `info`

### 0.0.5 - Musics

_Listen and load musics!_

- Add command `load` (load a directory)
- Add command `next`
- Add command `prev`
- Add command `list`

### 0.0.4 - SDL Mixer

_Goodbye Raylib, welcome SDL Mixer!_

- Remove command `stop`
- Add command `vol`
- Add a wrapper for each API (SDL Mixer) call.

### 0.0.3

_Loop the preloaded music_

Yea, it's only loop it back...

### 0.0.2 - Music

_Play a predefined music!_

- Listen to music!
- Add command `quit`
- Add command `play`
- Add command `stop`
- Add command `pause`
- Add command `resume`

### 0.0.1 - First version

The "Hello world!" of the CLI/TLI app.

- Read input from users
- Check if the input is "`quit`"
  - Quit the app


<!-- ======================================== -->


# Contact me

You can contact me via this address: dev@alikadev.com
