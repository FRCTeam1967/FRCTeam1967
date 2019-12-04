# FRC Team 1967: The Janksters

We are an all girls robotics team located in San Jose, California, and participate in FIRST (For Inspiration and Recognition in Science and Technology) Robotics Competition. Our code is written in C++, using WPI Lib, OpenCV, and other code libraries. If you would like to learn more about our team, you can visit us at our [website](http://team1967.ndsj.org).

## Setting up the repository

If you would like to download our code repository using the command line, run the following command in your terminal shell. Otherwise, ignore this command and follow the instructions to use SourceTree.

    $ git clone --recursive https://github.com/FRCTeam1967/FRCTeam1967.git

Many members of our team like to use SourceTree to manage their code. If you are using SourceTree, follow these instructions:

### Downloading SourceTree

Follow the steps below to download SourceTree.

1. Go to [this](https://www.sourcetreeapp.com/) website.
2. Click the green download button
3. Create an Atlassian account
4. Follow the instructions given to create it
5. Go back to SourceTree, and login with your Atlassian account
6. Click the Github icon, enter your GitHub information, and sign in

### Cloning our repository into SourceTree

1. Open SourceTree
2. Next to the searchbar that says "filter repositories," drop down the tab that says "New"
3. Select Clone from URL
4. Enter [this](https://github.com/FRCTeam1967/FRCTeam1967.git) source URL
5. Select the destination path and name the repository
6. Press clone

## Installations

Before you use our code, make sure you have completed all of these installations. Follow the instructions below to complete them.

### Installing Java SE

1. Go to [this](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) website
2. Under the JDK section, click install
    * This should take you to a page called Java SE Development Kit 8 Downloads
3. Accept the License Agreement, and then download it for Mac or Windows, depending on your device
4. Follow the instructions for installation when prompted

<!--
### Installing Eclipse

When creating and modifying our code, our team uses Eclipse Mars as an IDE. To install Eclipse, follow the steps below.
> **Note**: Please make sure you install Eclipse Mars.

1. Go to [this](https://eclipse.org/mars/) website
2. Click Download
3. Select Eclipse for C/C++
4. Follow the installation instructions when prompted
-->

### Installing VS Code
1. Go to [this](https://code.visualstudio.com/download) website
2. Click download. Depending on your operating system, you can download VS Code for Windows, Ubuntu, or Mac.

### Installing WPILib Plugin for VS Code 

1. Go to [this](https://github.com/wpilibsuite/allwpilib/releases/tag/v2019.4.1) website
2. Select the correct version of `2019.4.1` to install for your computer
3. Unzip / Open the download in your file system
4. Next steps for Mac
    * Rename your current `frc2019` folder to `frc2019_old` and create a new, empty folder in your home directory called `frc2019`
    * Ppen up the `WPILib_Mac-2019.4.1` folder. You should see a bunch of folders (`documentation`, `maven` etc)
    * Select all of these folders and move them into your new `frc2019` folder
    * Open up your terminal, and type the following: `cd frc2019/tools` & `python ToolsUpdater.py`
    * Open VSCode , and you should see a popup asking if you want to update WPILib. say yes.
    * ry building a working project (eg. 2019_DeepSpace)
5. Next steps for Windows
    * Run the installer (if you get "Windows Defender SmartScreen prevented an unrecognized app..." then click "More Info" and "Run Anyway")
    * If you've previously installed WPILib, only select the option to install "Gradle" (uncheck VScode, JDK, and the C++ compiler)
    * Once the installation is complete, try building a working project

### Installing CTRE Phoenix for VS Code
1. Open the VS Code project you are working
2. Right click on `build.gradle` in the project tree, then select `Manage Vendor Libraries`.
3. At the top of your screen, a menu will appear. Select `Install new libraries (offline)`.
    * Alternatively you can use “Install new libraries (online)” option with [this](http://devsite.ctr-electronics.com/maven/release/com/ctre/phoenix/Phoenix-latest.json) website. However this is not recommended as this requires a live Internet connection to use your FRC project
4. The menu will now display a list of vendor libraries you can install. Check `CTRE Phoenix`, then click `OK`.
5. If you are having trouble installing the Phoenix Library for VS Code, go to [this](https://phoenix-documentation.readthedocs.io/en/latest/ch05a_CppJava.html) website for more information & instructions.

<!--
### Installing the FRC Toolchains

Follow the [instructions](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#installing_the_c_toolchains_c_teams_only) below to install the FRC Toolchains. 

1. Go to [this](http://first.wpi.edu/FRC/roborio/toolchains/) website to install the toolchain
2. Install the most recent, 2018, toolchain(GCC 5.5)
3. Follow the instructions from [this](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#installing_the_c_toolchains_c_teams_only) website


### Installing Development Plugins in Eclipse

Follow the [instructions](https://wpilib.screenstepslive.com/s/4485/m/13810/l/145002-installing-eclipse-c-java#Installing-the-development-plugins---Option-1:-Onl) below to install the FRC development plugins in Eclipse.

1. Open Eclipse
2. In the top bar, select the Help menu
3. Click “Install New Software,” which is located near the bottom of the menu
4. Click “Add”
5. Type in the name: FRC Plugins
6. The location should be: http://first.wpi.edu/FRC/roborio/release/eclipse/ 
7. Click “OK”
8. Click the arrow to expand the WPILib Robot Development menu
9. Only select Robot C++ Development
10. Click next, next, accept, and finish
11. Click “OK” when prompted with a warning
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
-->

## 2019 Code Overview

Our code for the 2019 challenge, Destination: Deep Space, is intended to be paired with their most recent robot, Pandora. The objective of this robot was to pick up cargo balls and hatch panels from the ground and human player stations, delivering them to the cargo ship and rocket.

Below, you can find an overview of Pandora's code.

### Chassis

* The chassis on Pandora is driven by four motors, with two on each side.
    * We have programmed two Logitech joysticks for the driver to controll the chassis, with each controlling one side of the robot.

### Elevator

* Pandora's elevator mechanism utilizes a PID controller, which allows it to stop at certain positions.
    * We feed a distance into the PID controller, which allows the elevator to move up or down to its desired height.
    * The elevator has 8 preprogrammed heights, allowing the operator to bring the mechanism to the height of any goal on the field.
    * In addition to its preprogrammed heights, the elevator has manual controls, allowing the user to manually bring the elevator to any desired height up to 80 inches. 

### Cargo Manipulation
* Pandora's cargo manipulation mechanism has two main parts.
    * First, there are the rollers, which help roll the cargo ball into the robot. We have programmed a joystick for an operator to control the rollers.
    * Second, there is the pivot arm. To control the pivot arm, we input the desired angle into a PID controller, which allows the arm to go to a certain angle. There are three preprogrammed angles for the pivot arm, 0º, 45º, and 180º.

### Vision

* Our team uses a Jetson TX1 to process our vision software. 
    * Using the Microsoft Lifecam HD-3000, we are able to see retroreflective tape from across the field. Using this camera stream, we use OpenCV to isolate the tape, detect how far the robot is from it, and detect how offset the robot is from it. The distance and offset are then input into a PID controller, which drives the robot up to the target, stopping the robot when it is close enough.

## Licence

Our team has an open soure license. If you have any questions or concerns regarding licensing, please contect the team at team1967@ndsj.org.