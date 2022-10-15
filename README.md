# Karel the Robot learns C - Command Line

This is a command line version of "Karel the Robot learns C" by Markus Heckner ([git repo by Florian Heinz](https://github.com/fwheinz?tab=repositories), [Handout](https://fbim.oth-regensburg.de/~hem38149/files/karel/Karel_the_Robot_Reader.pdf)), originating from "Karel the Robot learn Java" by Eric Roberts ([Handout](https://cs.stanford.edu/people/eroberts/karel-the-robot-learns-java.pdf)). 
The goal of this projeect was to create a simpler, easier to install and cross-plattform version of Karel, only requiring a C compiler.
Inspired by [Maximilian Schecklmann](https://github.com/Narrenschlag/KingKarel)

<img src="https://media.giphy.com/media/T5y8Ke9iC97tOV7ZnF/giphy.gif" height="500" width="450"/>

## Setup
### All Platforms
Download this repo by clicking on the green Code-Button in the right upper corner and clicking on "Download as ZIP". Unzip it, and you should be presented with `karel.h`, `HelloWorld.c` and this README.
### MacOS
The best way of compiling it on MacOS is by using Clang. To use Clang, install the XCode-Commandline-Tools by typing `xcode-select --install` in your terminal. Click `Okay` in the pop up and wait till the installation has finished. 
Confim that Clang is installed by typing `clang --version` in your terminal to list the installed version.
### Windows
To install the gcc, go to [MinGW](https://sourceforge.net/projects/mingw/) and install the MinGW installer. Follow the instructions listed [here](https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j).
Confirm your installation by typing `gcc --version` in your terminal.
  ### Linux
idk, i dont have linux; install gcc or another compiler however you want

## Run Karel

To run your programm, first you need to open a terminal either in your IDE or a normal terminal. Then yoou have to navigate to the place where your Karel-files are by typing `cd You/Path/To/The/Programm/Folder` and pressing enter. You can list the files in the folder by typing `ls` (unix) or `dir` (windows).

Now type `clang yourProgram.c`, or `gcc yourProgram.c` depending on your OS and a new file called `a.out` (on MacOS) will be created in your folder. To run this file simply type `./a.out` in your terminal (on MacOS at least). Now you should see Karel in action.

## Customization
> NOTE: these featurs are exclusively for this Karel-version, and are may (very likely!!) not supported in other versions.
If you want to be fancy and have more fun with karel, there are a few settings you can play with, including creating own worlds.
### Speed
As shown in the `HelloWorld.c` example (that uses the LivingRoom world), you can use  `setSpeed(int speed)` to set the speed between frames in milliseconds. This value is set per default to 500ms. Also you can change the speed while the code is executed by using the `setSpeed()` function in components that are called in your `run()`.
### Karels look
You can set own characters for Karel by going into `karel.h` and set under the comment `//  ✨ Beauty ✨` the characters that Karel (and the beepers) should use.
### Own Worlds
To encorporate your own worlds (or change existing worlds) there are three steps to be made (all in `karel.h`): 
#### Wrting the world
To write your world, go to the end of the `karel.h` file and youll find all the other already written worlds.
On top of them there is a comment including a preset called `void worldPreset()`. Copy that function and paste it under the other worlds. 
Now you can change the world settings.
##### hasUlimitedBeeper
Does Karel have unlimited beepers? true/false
##### numberOfBeeper
How much Beepers does karel have at the start? int
##### setSpeed()
Default speed of the world. int
##### worldDimension[0] & worldDimension[1]
The size of your world. [0]>Y, [1]>x; Remember that the number you put here has to be `(ActuallSizeOfTheWorld)-1`. int
##### karelPosition[0] & karelPosition[1]
The position where karel starts. [0]>Y, [1]>X; Same sizing-rule as `worldDimension[]`. int
##### karelRotation
Karels starting rotation. 0>North,1>East,2>south,3>west. int
##### Walls
The walls are a bit tricky. All walls are stored in the array `walls[][4]`. Each wall is another array with 4 Elements: `{field1Y,field1X,field2Y,field2X}`. The wall is created between `field1` and `field2`. 
##### Beeper
The beepers are stored in the array `beeper[][3]`. Each beeper is another array with 3 elements: `{fieldY,fieldX,numberOfBeepers}`. `numberOfBeepers` is th eamount of beepers stored on this field. 
#### Adding it to the Selection
To ad your world to the world selection go into the `void loadWorld()` function and under `//world selection:` youll find plenty of if-elseif statements. To add your world paste
```c
else if (!strcmp(worldName, "howYouWanToCallItFromYourFile"))
    {
        worldYourWorldName();
    }
``` 
onto the end of said chain of statements.
Note: `howYouWanToCallItFromYourFile` is the name your world will have, and that you use to load it by using `loadWorld("yourWorldName)`.
#### Pre-Declare it
To pre-declare your world, add it to `// WORLDS` (somewhere around line 80) like the other worlds already have: `void worldYourWorldName();`
#### Using it
Now you can use your world by typing `loadWorld("yourWorldName")` into the `void setup() {}` function.

## Todo:
- Farbe
- Action-History
- Check Setup/etc for Windows & Linux