# Common classes this year

This folder's purpose is to house any and all common Java classes which are used during the 2023 FRC season.

The typical situation that drives this use pattern is that a sub-group will often create a project to test some new device type or advancement. They do this in the 2023/ folder in their own project. However, the difficulty arises when that code is planned to be used in the "real robot" for competition. At that point, the main robot project needs access to the new class and the sub-group project may also need access to it for making changes as things evolve.

Both projects can easily use the class so long as the java class is placed in `2023/CommonClassesThisYear/`

All .java files found in this folder will get compiled as part of the build process for any and all 2023 projects so long as they have been configured per the README.md in the 2023/ folder.
