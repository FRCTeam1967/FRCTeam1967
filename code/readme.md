# How to Create & Organize Your Project

## How to Make A Project: WPILib Project Template

Use the WPILib Project Template to create a custom project for use in VS Code. Please do not modify the template, itself, and do not change its location. 

To create a project using the WPILib Project Template:

* Open this directory path: `~/FRCTeam1967/code/2019/projects`.
* Duplicate `Project Template`.
* Rename this copy to your project name.

### Open & Compile Project

To open and compile your new project:

* Within your new project, you should see a file called `TemplateWorkspace.code-workspace`. Rename this to `ProjectName.code-workspace`. 
* Open this workspace in VS Code. When you open this, you will see your project, as well as the `jankyLib` folder. This includes all of the reusable classes & includes from previous years. You will also see a `classes` and `include` folder in your workspace. These are where you will put your project's .h and .cpp files.
* Build your project. You can do this one of two different ways.
    * Way 1 - Press `Cmd + Shift + P`, and select `WPILib: Build Robot Code`.
    * Way 2 - Right-click on build.gradle, and select `WPILib: Build Robot Code`.
* When asked which project you would like to build, select your project. Hopefully, your code will compile, and you will see `Build Successful` in your terminal.

### Where to Put .h & .cpp Files

This year, we have reorganized our folder structure. Now, you **must** make sure your project include files are in the `~/FRCTeam1967/code/2019/include` folder, and your source files are in the `~/FRCTeam1967/code/2019/classes` folder. If you are making a project that can be reused, such as `jankyXboxJoystick`, put your include files in the `~/FRCTeam1967/code/jankyLib/include` folder, and put your source files in the `~/FRCTeam1967/code/jankyLib/classes` folder.

## How to Make A Project From Scratch

> **Note:** Making a project from a WPILib Project Template is the preferred method. Only make a project from scratch if you need something more than what the template offers.

To make a WPILib project from scratch:

* Press `Cmd + Shift + P`, and select `WPILib: Create New Project`.
* When prompted, select the project type to be: Template, cpp, Timed Robot.
* Enter your project name
* Enter your team number (1967)
* Press generate project

### Creating, Setting Up, and Editing your Workspace

Once you have created the project, open the project in VS Code. Then, follow these instructions to create, set up, and edit your workspace.

* Drag `~/FRCTeam1967/code/2019/classes`, `~/FRCTeam1967/code/2019/include`, and  `~/FRCTeam1967/code/jankyLib/` to the sidebar in VS Code, where you see your workspace. 
* Go to `File`, and select `Save Workspace As ...`.
* When it prompts you, name your workspace, and select the save location to be your project folder.
* You should see a file called `projectName.code-workspace`. Double click on this file to open your workspace.
* If you open this file in VS code, you can edit your workspace settings. In the file, edit all of the folder paths to be relative, rather than hardcoded. This will ensure that if someone else uses your workspace, everything will work correctly.
* Save this file.
* Now, you can reopen this workspace whenever you would like to use it, enabling you to see your project's code, as well as the includes and classes in one workspace.

### Build.gradle Setup

Once you have created the project and set up the workspace, please follow these instructions to set up your `build.gradle` file.

* Open the build.gradle file in your project.
* When scrolling down the file, you should find the `sources.cpp` section around line 54. Underneath `srcDir 'src/main/cpp'`, and above `include '**/*.cpp', '**/*.cc'`, add these two lines:
    * `srcDir '../../classes'`
    * `srcDir '../../../jankyLib/classes'`
* Below this, in the exported headers section, under `srcDir 'src/main/include'`, and above `if (includeSrcInIncludeRoot) {`, add these two lines:
    * `srcDir '../../include'`
    * `srcDir '../../../jankyLib/include'`
* These steps should make sure that all of your header and source files are found by the compiler.
* Save the `build.gradle` file, and build your project.

### Workspace setup

On the left pane, there are 'include' and 'classes' "folders" - these are defined in the workspace file. Unfortunately, the location of the folder is given in an absolute path which often contains a user name making it non-functional for multiple users loading workspaces. The workspace should instead have these are relative paths:
`
        {
            "path": "../../classes"
        },
        {
            "path": "../../include"
        }
`

## Common Include Problems

### Cannot find "ctre/Phoenix.h"

If you are getting this error, follow the instructions below:

* Right click on your `build.gradle` file
* Select `Manage Vendor Libraries`
* Select `Install New Library (online)`
* Paste this link when prompted: http://devsite.ctr-electronics.com/maven/release/com/ctre/phoenix/Phoenix-latest.json

### Cannot find include path

If your project cannot find an include path, go into the build.gradle file, and make sure the include paths are pointing to the correct directories. Also, make sure your `.h` and `.cpp` files are in the correct folder. If you need help, ask a mentor or student.