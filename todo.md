# Improve duration handling

Change the pattern class to have an actual tempo specified in beats per minute.
Unfortunately, this amount of flexibility can easily lead to running out of
space in the duration LUT.

[Optional] Move the pitch LUT to give more space for it.  It's currently fairly
limited in size.

Process each pattern to remove overlong notes.  Because of the way the envelope
on pulse channels works, notes over a certain length will not be
distinguishable.  In this case, truncate the note and add a rest afterward to
fill in the time.

For each pattern, collect the durations used.  A given pattern can only use
eight different durations.  Throw an error if more than eight are required.
Check if the durations required are available in an existing duration LUT row.
Barring that, check if there is a row with enough free space to add the missing
durations and prefer the one that requires the fewest additions.  In the worst
case, this will mean adding an entirely new row to the duration LUT.  If there
is no space left, throw an error.

# Automatic song table placement

Add checks to figure out the length of song data and determine if it interferes
with later data in the ROM.

Automatically move the song tables as necessary to fit the music.

# Envelope modifications

# Graphical interface
