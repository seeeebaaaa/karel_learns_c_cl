//-jhint options
/*jshint unused:true,leanswitch:true,asi:true,esversion:8,expr:true,-W008*/

const { readFileSync } = require('fs');

let world = {
    dimension: 0,       // X Y
    walls: [],  // X Y, index starts at 1
    beepers: [],        // X Y , index starts at 1
    karel: [[], 0],     // X Y, index starts at 1
    BeeperBag: [0, false]
}

let worldFromFile;
function syncReadFile(filename) {
    // if youre reading this, and want to convert your own .w files just change this path and down somwhere the file name; or just put the whole path into this
    const contents = readFileSync("../karel_clion_macosx_m1-main/data/worlds/" + filename + ".w", 'utf-8');

    const arr = contents.split(/\r?\n/);

    return arr;
}


// CHANGE WORLD NAME HERE
worldFromFile = syncReadFile('worldArea3');

worldFromFile.pop(); //remove speed setting

//extract world settings
for (const elemt in worldFromFile) {
    if (worldFromFile[elemt][0] == "D") {
        let dimString = worldFromFile[0].split("(")[1];
        dimString = dimString.substring(0, dimString.length - 1);
        let values = dimString.split(",")
        values[1] = values[1].split(" ")
        world.dimension = [values[0], values[1][1]]
        //world.walls += []
        continue;
    }
    if (worldFromFile[elemt][0] == "W") {
        let wallStringO = worldFromFile[elemt].split("(")[1]
        wallString = wallStringO.split(")")[0]
        let values = wallString.split(",")
        values[1] = values[1].split(" ")
        wallPos = [parseInt(values[0]), parseInt(values[1][1])]
        rotString = wallStringO.split(")")[1]
        rotString = rotString.substring(1);
        if (rotString == "south" || rotString == "South") {
            world.walls = [...world.walls, [wallPos[0], wallPos[1] - 1, ...wallPos]]
        } else {
            world.walls = [...world.walls, [wallPos[0] - 1, wallPos[1], ...wallPos]]
        }
        //world.walls += []
        continue;
    }
    if (worldFromFile[elemt][0] == "B" && worldFromFile[elemt][6] == ":") {
        let beeperStringO = worldFromFile[elemt].split("(")[1]
        beeperString = beeperStringO.split(")")[0]
        let values = beeperString.split(",")
        values[1] = values[1].split(" ")
        beepPos = [parseInt(values[0]), parseInt(values[1][1])]
        numString = beeperStringO.split(")")[1]
        numString = parseInt(numString.substring(1))
        world.beepers = [...world.beepers, [...beepPos, numString]]
    }
    if (worldFromFile[elemt][0] == "K") {
        let karelStringO = worldFromFile[elemt].split("(")[1]
        karelString = karelStringO.split(")")[0]
        let values = karelString.split(",")
        values[1] = values[1].split(" ")
        karelPos = [parseInt(values[0]), parseInt(values[1][1])]
        rotString = karelStringO.split(")")[1]
        rotString = rotString.substring(1);
        switch (true) {
            case rotString == "north" || rotString == "North":
                world.karel = [...karelPos, 0]
                break;
            case rotString == "east" || rotString == "East":
                world.karel = [...karelPos, 1]
                break;
            case rotString == "south" || rotString == "South":
                world.karel = [...karelPos, 2]
                break;
            case rotString == "west" || rotString == "West":
                world.karel = [...karelPos, 3]
                break;
        }
        //world.walls += []
        continue;
    }
    if (worldFromFile[elemt][0] == "B" && worldFromFile[elemt][6] != ":") {
        let number = worldFromFile[elemt].split(" ")[1]
        if (number == "INFINITE") {
            world.BeeperBag = [0, true]
        } else {
            world.BeeperBag = [parseInt(number), false]
        }
    }
}
//control log
console.log(world);

//conver world settings to c code
let validCCode = ""

//beeperBag
validCCode += "hasUlimitedBeeper = " + world.BeeperBag[1] + ";"
validCCode += "numberOfBeeper = " + world.BeeperBag[0] + ";"
//speed of world
validCCode += "setSpeed(500);"
//dimension
validCCode += "worldDimension[0] = " + world.dimension[1] + " - 1;" //y in program, x in world file
validCCode += "worldDimension[1] = " + world.dimension[0] + " - 1;" //x in program, y in world file
//karel position
validCCode += "karelPosition[0] = " + world.karel[1] + " - 1;"
validCCode += "karelPosition[1] = " + world.karel[0] + " - 1;"
//karel rotation
validCCode += "karelRotation = " + world.karel[2] + ";"
//walls
let i = 0;
for (const wall of world.walls) {
    validCCode += "walls[" + i + "][0] = " + wall[1] + " - 1;"
    validCCode += "walls[" + i + "][1] = " + wall[0] + " - 1;"
    validCCode += "walls[" + i + "][2] = " + wall[3] + " - 1;"
    validCCode += "walls[" + i + "][3] = " + wall[2] + " - 1;"
    i++
}
i = 0;
for (const beep of world.beepers) {
    validCCode += "beeper[" + i + "][0] = " + beep[1] + " - 1;"
    validCCode += "beeper[" + i + "][1] = " + beep[0] + " - 1;"
    validCCode += "beeper[" + i + "][2] = " + beep[2] + ";"
    i++
}

console.log("======VALID C CODE======");
console.log(validCCode)
//beeper
