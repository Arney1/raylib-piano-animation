# Raylib Piano MIDI Animation
A MIDI visualizer built in C using Raylib. Loads a MIDI file that has been converted to CSV, falls note blocks down onto a live piano keyboard, and plays back synthesized audio in sync, with "seek"ing support.
![Preview](assets/preview/prev.png)

---

## Features

- Falling note blocks, which are notes spawn above the piano and fall in time with the music
- Live piano keyboard with interactivity such as keys that change color when pressed, with filled and outline render modes
- Real-time audio as a sine-wave voice synthesis with logarithmic decay, up to 32 simultaneous voices
- Seek control, you can scrub forward/backward through the song with arrow keys
- Note particles, they're musical note symbols that float up from the piano on each hit
- Dual render mode, you can toggle between filled and Bresenham/Midpoint outline

---

## Project Structure

```
.
├── assets/midis/output.csv     # midi data consumed by the app
├── midi-to-csv/                # python utility to convert .mid → .csv
│   ├── input.mid
│   ├── main.py
│   └── requirements.txt
├── src/
│   ├── audio/                  # audio stream, voice synth, frequency tables
│   ├── core/                   # app entry point and main loop
│   ├── graphics/               # bresenham line/rect, Midpoint circle/ellipse
│   ├── objects/                # piano, note block, note particle, midi parser, slider
│   ├── render/                 # render mode state (filled and outline)
│   ├── screens/                # menu, anim, objects, about screens
│   └── utils/                  # color palette, draw_utils, screen types
├── build/app                   # compiled binary output
└── Makefile
```

---

## Dependencies

- [Raylib 5.5](https://www.raylib.com/) — must be installed and linkable on your system
- GCC + Make
- Python 3 + dependencies in `midi-to-csv/requirements.txt` (only needed to convert MIDI)

---

## Build & Run

```bash
# build
make

# run
./build/app
```

---

## Converting a Custom MIDI File

If you want to use your own MIDI file instead of the bundled one:

```bash
cd midi-to-csv
pip install -r requirements.txt
python main.py          # reads input.mid, writes output.csv
cp output.csv ../assets/midis/output.csv
```

---

## Screens

- **Menu**
- **Animation**, that is a full MIDI playback with falling notes and piano animations
- **Objects**, that is a screen where you can inspect individual components (Piano, Note Block, MIDI Slider, Note Particle)
- **About**, my infos
