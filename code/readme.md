# How to Create & Organize Your Project

## Copy WPILib Project Template

Please follow these instructions, outlining how to copy the WPILib Project Template.

* Open this directory path: `~/FRCTeam1967/code/2019/projects`.
* Duplicate `Project Template`.
* Rename this copy to your project name.

## Open & Compile Project

Please follow these instructions, outlining how to open and compile your new project.

* Within your new project, you should see a file called `TemplateWorkspace.code-workspace`. Rename this to `ProjectName.code-workspace`. 
* Open this workspace. When you open this, you will see your project, as well as the `jankyLib` folder. This includes all of the reusable classes & includes from previous years. You will also see a `classes` and `include` folder in your workspace. These are where you will put your project's .h and .cpp files.
* Build your project. You can do this one of two different ways.
    * Way 1 - Press `Cmd + Shift + P`, and select `WPILib: Build Robot Code`.
    * Way 2 - Right click on build.gradle, and select `WPILib: Build Robot Code`.
* When prompted which project you would like to build, select your project. Hopefully, your code will compile, and you will see `Build Successful` in your terminal.

## Where to Put .h & .cpp Files

This year, we have reorganized our folder structure. Now, you **must** make sure your project include files are in the `~/FRCTeam1967/code/2019/include` folder, and your source files are in the `~/FRCTeam1967/code/2019/classes` folder. If you are making a project that can be reused, such as jankyXboxJoystick, put your include files in the `~/FRCTeam1967/code/jankyLib/include` folder, and put your source files in the `~/FRCTeam1967/code/jankyLib/classes` folder.

## Common Include Problems

### Cannot find "ctre/Phoenix.h"

If you are getting this error, follow the instructions below:

* Right click on your `build.gradle` file
* Select `Manage Vendor Libraries`
* Select `Install New Library (online)`
* Paste this link when prompted: http://devsite.ctr-electronics.com/maven/release/com/ctre/phoenix/Phoenix-latest.json

### Cannot find include path

If your project cannot find an include path, go into the build.gradle file, and make sure the include paths are pointing to the correct directories. Also, make sure your .h and .cpp files are in the correct folder. If you need help, ask a mentor or student.