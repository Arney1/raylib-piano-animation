import pretty_midi
import csv

def midi_to_csv(midi_file_path, csv_file_path):
   # load
    try:
        midi_data = pretty_midi.PrettyMIDI(midi_file_path)
    except Exception as e:
        print(f"Error loading MIDI file: {e}")
        return

    # check if the midi is empty
    # checking if there are any notes across all instruments
    total_notes = sum(len(instrument.notes) for instrument in midi_data.instruments)

    if total_notes == 0:
        print(f"The MIDI file '{midi_file_path}' is empty (contains 0 notes). Aborting conversion.")
        return
    else:
        print(f"Success! The MIDI file is not empty. Found {total_notes} notes.")

    with open(csv_file_path, mode='w', newline='') as csv_file:
        writer = csv.writer(csv_file)

        writer.writerow(['Instrument_Program', 'Is_Drum', 'Start_Time', 'End_Time', 'Pitch', 'Velocity'])

        # loop through every instrument and every note
        for instrument in midi_data.instruments:
            for note in instrument.notes:
                writer.writerow([
                    instrument.program,  # midi program number
                    instrument.is_drum,
                    round(note.start, 4), # start time in seconds
                    round(note.end, 4),   # end time in seconds
                    note.pitch,          # midi pitch number (60 = C, etc)
                    note.velocity
                ])

    print(f"Data successfully saved to '{csv_file_path}'")

midi_to_csv('input.mid', 'output.csv')
