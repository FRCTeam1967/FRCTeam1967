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

### Installing VS Code
1. Go to [this](https://code.visualstudio.com/download) website
2. Click download. Depending on your operating system, you can download VS Code for Windows, Ubuntu, or Mac.

### Installing WPILib Plugin for VS Code 

1. Go to [this](https://github.com/wpilibsuite/allwpilib/releases) website
2. Select the correct version of `WPILib 2020.2.2 Update` to install for your computer
3. Unzip / Open the download in your file system
4. Next steps for Mac
    * Rename your current `wpiLib` folder to `wpiLib2020` and create a new, empty folder in your home directory called `wpiLib`.
    * Once downloaded, rename the `WPILib 2020.2.2 Update` folder to `2020`.
    * Move the `2020` folder that you downloaded into your new `wpiLib` folder.
    * Open up your terminal, and type the following: `cd wpiLib/2020/tools` & `python ToolsUpdater.py`.
    * Open VSCode , and you should see a popup asking if you want to update WPILib. say yes.
    * Try building a working project (eg. 2020_Infinite_Recharge).
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

## Licence

Our team has an open soure license. If you have any questions or concerns regarding licensing, please contect the team at team1967@ndsj.org.