# FRC Team 1967: The Janksters
> **Note**: This readme is not complete yet.

We are an all girls robotics team located in San Jose, California, and participate in FIRST (For Inspiration and Recognition in Science and Technology) Robotics Competition. Our code is written in C++, using WPI Lib, OpenCV, and other code libraries.

## Prerequesites

Before you download our repository and use our code, make sure you have completed all of our prerequisites. Follow the instructions below to complete them.

### Installing Java SE

1. Go to http://www.oracle.com/technetwork/java/javase/downloads/index.html 
2. Click install under JDK. This should take you to a page called Java SE Development Kit 9 Downloads
3. Accept the License Agreement, and then download it for Mac or Windows depending on your device
4. Follow instructions for installation when prompted

### Installing Eclipse

When creating and modifying our code, our team uses Eclipse Mars as an IDE. To install Eclipse, follow these steps.
> **Note**: Please make sure you install Eclipse Mars.

1. Go to https://eclipse.org/mars/
2. Click Download
3. Select Eclipse for C/C++
4. Follow the installation instructions when prompted

### Installing the FRC Toolchains

Follow the [instructions](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#installing_the_c_toolchains_c_teams_only) below to install the FRC Toolchains. 

1. Go to [this](http://first.wpi.edu/FRC/roborio/toolchains/) website to install the toolchain
2. Install the most recent, 2018, toolchain(GCC 5.5)
3. Follow the instructions from [this](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#installing_the_c_toolchains_c_teams_only) website

### Installing Development Plugins in Eclipse

Follow the [instructions](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#Installing-the-development-plugins---Option-1:-Onl) below to install the FRC development plugins in Eclipse.

1. Open Eclipse
2. Choose “Help” (on the top bar)
3. Click “Install New Software” (near the bottom)
4. Click “Add” (on the top right)
5. Name: FRC Plugins
6. Location: http://first.wpi.edu/FRC/roborio/release/eclipse/ 
7. Click “OK”
8. Click the arrow to expand the WPILib Robot Development menu
9. Only select Robot C++ Development
10. Click next, next, accept, and Finish
11. Click “OK” on the warning
12. Restart Eclipse when prompted

### Installing CAN Talon SRX Software

1. Go to [this](http://www.ctr-electronics.com/control-system/hro.html#product_tabs_technical_resources) website
2. Under Stable Installer on the left side, download based on your computer
    * Macs - Download CTRE Toolsuite (No Installer) package 4.4.1.14 (.zip)
    * PCs - Download CTRE Toolsuite Installer 4.4.1.14 (.zip)
3. Unzip the file
4. Open a new Finder window
5. Go to your wpilib folder -> user -> cpp -> lib
    * On a mac: 
        * Command + Shift + G
        * Type “/Users/” and click “Go”
        * Click on your user
6. Name this cpp folder “cpp-[date]” (ex. cpp-Nov9)
7. Drag the cpp folder you just downloaded (inside the into the “user” folder of your Finder
8. Restart Eclipse after

## Setting up the repository

To download our code repository, run the following command in your terminal shell:

    $ git clone --recursive https://github.com/FRCTeam1967/FRCTeam1967.git

If you are using SourceTree, follow these instructions:

TODO

## 2018 Overview

Our code for FIRST PowerUp, in 2018, is intended to be paired with their most recent robot, Lola. TODO

### Chassis

TODO

### Game Components

TODO

### Vision

TODO

### Autonomous

TODO

## Licence

Our has an open soure license. If you have any questions or concerns regarding licensing, please contect the team at team1967@ndsj.org.