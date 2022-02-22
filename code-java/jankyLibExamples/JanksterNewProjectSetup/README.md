# The Jankster Java Build Home Base

This folder contains all of the scripts and snippets required to allow the new build system to work in an elegant fashion.

As a student, the only likely file which matters is `setupNewProject.sh` which needs to be run for prepping a new project.
Note: This script only runs on Mac/Linux. It is not a batch file for Windows. For anyone on Windows or who wants to do things
without operating from a terminal / command-line, properly setting up a new project requires adding a couple lines of
script to two different files. You can edit these directly from VS Code in the editor.

## The file: gradlew

This file needs to be ammended by inserting the contents of `gradlew_Addition` AFTER the first line of gradlew.

## The file: gradlew.bat

This is the windows version of gradlew and it needs to be ammended by inserting the contents of `gradlewBAT_Addition`
at the beginning of gradlew.bat


* Once the setupNewProject.sh is complete or the manual changes are made, building the project the first time 
  will cause the rest of the build changes to occur.

