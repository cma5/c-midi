# c-midi

# Midi Parser in C

This is a very simple program which prints out the content of a in a human readable format on the console.

Installation: For Win32 64bit I provide a pre built .exe in the build directory

For Linux or Mac, you have to build it yourself:
The 2 header files are stored in the incl. subdirectory and referenced in the code as relative paths.
The main.c is located in src.

You can compile it with probably most c compilers.
I used GCC. 

Download the repository
Extract the .zip file at your desired location
Navigate to the src folder in your command line and run:
gcc -o c-midi main.c
Your executable will be located in the src folder.

Usage:

this program takes one and only one argument which ist the path to your MIDI-File.
cmidi [path to .mid file]

Since this is very much a work in progress, I wouldnt recommend you to use this for any serious application or product.
