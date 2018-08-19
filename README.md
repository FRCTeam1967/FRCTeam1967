# FRC Team 1967: The Janksters

We are an all girls robotics team located in San Jose, California, and participate in FIRST (For Inspiration and Recognition in Science and Technology) Robotics Competition. Our code is written in C++, using WPI Lib, OpenCV, and other code libraries. If you would like to learn more about our team, you can visit us at our [website](http://team1967.ndsj.org).

## Prerequesites

Before you download this repository and use our code, make sure you have completed all of our prerequisites. Follow the instructions below to complete them.

### Installing Java SE

1. Go to [this](http://www.oracle.com/technetwork/java/javase/downloads/index.html) website
2. Under the JDK section, click install
    * This should take you to a page called Java SE Development Kit 10 Downloads
3. Accept the License Agreement, and then download it for Mac or Windows, depending on your device
4. Follow the instructions for installation when prompted

### Installing Eclipse

When creating and modifying our code, our team uses Eclipse Mars as an IDE. To install Eclipse, follow the steps below.
> **Note**: Please make sure you install Eclipse Mars.

1. Go to [this](https://eclipse.org/mars/) website
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

## Setting up the repository

To download our code repository, run the following command in your terminal shell:

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

## 2018 Code Overview

Our code for FIRST PowerUp, in 2018, is intended to be paired with their most recent robot, Lola. The objective of this robot was to pick up yellow Power Cubes and put them onto a low switch or a high scale. If you are interested, you can watch [this](https://www.youtube.com/watch?v=HZbdwYiCY74) video, which will explain the game.

Below, you can find an overview of Lola's code.

### Chassis

* The chassis on Lola is driven by four motors, with two on each side.
    * We have programmed two Logitech joysticks for the driver to controll the chassis, with each controlling one side of the robot.
    * Because our robot can extend up to 7 feet, we have built an anti-tipping algorithm into our code. 

### Game Components

* Lola has two main game components mechanisms.
    * The first mechanism is a claw that can open and close using pneumatic pistons. The claw can pivot in and out of the robot using a motor. It also has rollers, which can help bring a cube in and out of the claw quickly. 
    * The second mechanism is an elevator, which is driven by two motors. We use magnetic encoders to determine the height of the mechanism, how far it needs to move, and in what direction it should move. 
    * The game component code runs in a separate task from the other code, allowing it to run quickly and efficiently.
    * In order to manipulate Lola's mechanisms, we have programmed an Xbox 360 controller.

### Vision

* Our team uses a Jetson TX1 to process our vision software. 
    * Using the Microsoft Lifecam HD-3000, we are able to see retroreflective tape from across the field. Using this camera stream, we use OpenCV to isolate the tape, detect how far the robot is from it, and use this information to drive towards the tape. Then, using our distance calculation, we stop the robot when it is close enough to the tape.

### Autonomous

* Our team created 11 different autonomous modes. 
    * We are able to choose an autonomous mode to fit our needs, depending on where Lola starts out on the field at the beginning of the match.
    * We have integrated vision into our autonomous code, which allows our robot to be more precise with its movements.

## Licence

Our team has an open soure license. If you have any questions or concerns regarding licensing, please contect the team at team1967@ndsj.org.
