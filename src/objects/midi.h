#ifndef MIDI_H
#define MIDI_H

typedef struct {
    float start;
    float end;
    int pitch;
    int velocity;
} MidiNote;

void midi_load(const char *filename);
void midi_update(float time);
void midi_reset();
float midi_get_length(void);
void midi_seek(float target_time);

#endif
