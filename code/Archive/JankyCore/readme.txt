How to use a library

Alter the project properties to point to the include directory
Right click on the project and select ‘Properties’
Expand the C/C++ Build Section and select Settings
Expand the Cross G++ Compiler/Includes item
The right hand side of the window should show Include Paths.
Click the + button at the top of the screen to add this entry
“{workspace_loc:/JankyCore/include}”
(If you are feeling ambitous, you can select the "Filesystem" option and find the directory.

Alter the properties to point to the library directory and library we will use
Select the Libraries option under Cross G++ Linker item.
On the right-hand side of the window click the + items in the Libraries section.
Add the library JankyCore to the dialog that pops up
If JankyCore is not at the top of the list, select the wpi library and use the little down-arrow icon at the top of the table to move it down so the list is ordered as JankyCore followed by wpi
On the right-hand side of the window click the + items in the Library Search Path section.
Add this path to the dialog that pops up: $[workspace_loc:/JankyCore/Debug}”
(If you are feeling ambitous, you can select the "Filesystem" option and find the directory.

Press OK on the entire dialog to save everything.

Now you need to tell eclipse that your project depends on the library project so it will build it for you automatically.
Bring up that same Properties dialog
Under the C/C++ General item, select Project References.
Select the item JankyCore in the list of projects you see (make sure there is a check in the box)

Press OK on the entire dialog to save everything.

Build your project as usual, but don’t copy or link source files into your project -- that’s no longer required.

