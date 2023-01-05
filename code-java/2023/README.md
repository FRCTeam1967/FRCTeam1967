# Janksters - How to setup a new project

The Jankters' Github repository contains code which is shared year to year and within a given year. The background of this topic and how the build system of gradle works can be found in the /docs/ folder of our repository for those wanting a bit more depth on the topic.

In order to properly build a project with such shared code "configured" in the project, you must do one of the following:

## Automated Script
This is a place-holder for how we'll utilize a command line script to automate the process of customizing a new project. #comingsoon

## Copy /code-java/2023/2023StartingTemplate/ folder
The project in `code-java/2023/2023StartingTemplate/` is fully configured and ready to go as a base class of `TimedRobot`. It is valid to use this as a generic starting point by duplicating (or copy/paste in Windows Explorer) the folder and renaming it to be your own.
- Go to Finder or Explorer (or a command line) and duplicate the folder. Keep the duplicated version in the 2023/ folder.
- Rename the duplicated folder. For this example, let's call the project "JankySolenoidTester"
	- At this point, you see in the 2023/ folder there is a folder called 2023/JankySolenoidTester
- Now, in your `JankySolenoidTester/` folder, you'll see there is a file called `2023StartingTemplate.code-workspace`. This file should be renamed to match your project name and so it should become: `JankySolenoidTester.code-workspace`. Be sure you don't change the (long) extension name of '.code-workspace'

At this point, the new project is ready to use. To use it you may double-click on the `JankySolenoidTester.code-workspace` which will bring up VSCode and the project. Or you can use VSCode and the File->Open Workspace From File...

## Create Project Manually
For those who wish to play with the wide variety of WPI supplied sample or template projects, this method will work fine. It allows the user to create a project and then add in the common folders all by hand. Don't fret - it's not a super complex gig, really!

- Create the project using the WPI supplied "Create New Project" command. 
	- The only caution is that the project should be placed in a folder under the `2023/` folder.
- Once you have the project created, you'll be in VSCode with the project open and usually with the files view on the left hand side. Great.
- Add 'CommonClassesThisYear' folder - Go to File->Add Folder to Workspace... and navigate up to the 2023/ folder level and down into the 'CommonClassesThisYear' folder and click 'Add'
- Add 'jankyLib' folder - Go to File->Add Folder to Workspace... and navigate up two levels to the code-java/ folder level and down into the 'jankyLib' folder and click 'Add'
- Now you need to save this workspace by using File->Save Workspace As... and save the workspace file in the same folder as the project (2023/NewCoolProject/) and preferrably name the workspace file the same as your project name. The end result should be a file like `2023/NewCoolProject/NewCoolProject.code-workspace`

##### Finally: Modify build.gradle
Any WPI project that is created will have a build.gradle file in the project folder.  To add support for our common file areas, one must edit the file by clicking on it on the left hand side and after the line reading:

```
def ROBOT_MAIN_CLASS = "frc.robot.Main"`
```

the user must add the following lines:

```
sourceSets {
  main {
    java {
      srcDir "../../jankyLib"
      srcDir "../CommonClassesThisYear"
    }
  }
}
```

Then save the build.gradle file (Ctrl-S / Command-S) or File->Save
