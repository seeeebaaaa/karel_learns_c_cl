# Karel the Robot learns C - Command Line

This is a command line version of "Karel the Robot learns C" by Markus Heckner ([git repo by Florian Heinz](https://github.com/fwheinz?tab=repositories), [Handout](https://fbim.oth-regensburg.de/~hem38149/files/karel/Karel_the_Robot_Reader.pdf)), originating from "Karel the Robot learn Java" by Eric Roberts ([Handout](https://cs.stanford.edu/people/eroberts/karel-the-robot-learns-java.pdf)). 
The goal of this projeect was to create a simpler, easier to install and cross-plattform version of Karel, only requiring a C compiler.

## Setup
### All Platforms
Download this repo by clicking on the green Code-Button in the right upper corner and clicking on "Download as ZIP". Unzip it, and you should be presented with `karel.h`, `HelloWorld.c`, this README and a folder called `worlds` containing the worlds.
### MacOS
The best way of compiling it on MacOS is by using Clang. To use Clang, install the XCode-Commandline-Tools by typing `xcode-select --install` in your terminal. Click `Okay` in the pop up and wait till the installation has finished. 
Confim that Clang is installed by typing `clang --version` in your terminal to list the installed version.
### Windows
To install the gcc, go to [MinGW](https://sourceforge.net/projects/mingw/) and install the MinGW installer. Follow the instructions listed [here](https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j).
Confirm your installation by typing `gcc --version` in your terminal.
  ### Linux
idk, i dont have linux

## Run Karel

To run your programm, first you need to open a terminal either in your IDE or a normal terminal. Then yoou have to navigate to the place where your Karel-files are by typing `cd You/Path/To/The/Programm/Folder` and pressing enter. You can list the files in the folder by typing `ls`.

Now type `clang yourProgram.c`, or `gcc yourProgram.c` depending on your OS and a new file called `a.out` (on MacOS) will be created in your folder. To run this file simply type `./a.out` in your terminal. Now you should see Karel in action.
