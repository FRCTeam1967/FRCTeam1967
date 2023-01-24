In order to use this example program, you need to downlaod some MIDI files,
convert them to CHRP files using Phoenix Tuner, and then save those CHRP files
to the src/main/deploy directory with the exact names.

To fetch the MIDI files, run the fetchMusic.sh script, which will download
the files to the midi directory. This script should be run from the project'save
directory (CommandExample).

To convert the .midi files into .chrp files, you need to use Phoenix Tuner:
1. Open Phoenix Tuner X
2. Select CHRP Generator
3. Click Import MIDI
4. Select the MIDI file to convert from the midi folder
5. Name the CHRP file as specified below
6. Choose the destination folder (src\main\deploy in this project)
7. Click save.

The chrp files must have the following names for the program to see them 
(or you can change the program, of course):

Beethoven's 5th: Fifth.chrp
Beethoven's Moonlight Sonata: Moonlight.chrp
Never Gonna Give You Up: NGGYU.chrp
YMCA: YMCA.chrp

If you make program changes, DO NOT add either the chrp or midi files to 
the repository. There is a .gitignore file in both the deploy and midi
directories that should help prevent this.

