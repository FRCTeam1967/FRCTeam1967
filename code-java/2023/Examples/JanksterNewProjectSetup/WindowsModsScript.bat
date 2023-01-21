@rem Please UPDATE this version number ANY time changes are made to the file.
@echo Janksters Java-Build gradlew.bat mods Version 2.1

@rem Make sure the location exists for building to be successful
@if not exist .\src\main\java\org\ (
  @mkdir .\src\main\java\org
  @mkdir .\src\main\java\org\janksters
)

@rem Short circuit: If both paths exist, and they're both reparse points (symlinks), we can skip the rest
@if exist .\src\main\java\org\janksters\jankyLib (
  @>nul fsutil reparsepoint query .\src\main\java\org\janksters\jankyLib
  @if %ERRORLEVEL% == 0 (
    @rem jankyLib is a symlink!
    @if exist .\src\main\java\org\janksters\CommonClassesThisYear (
      @>nul fsutil reparsepoint query .\src\main\java\org\janksters\CommonClassesThisYear
      @if %ERRORLEVEL% == 0 (
        @rem CommonClassesThisYear is also a symlink. We're good!!
        @echo NTFS symlinks are already in place!
        @exit /b
      )
    )
  )
)

@rem Now let's make sure the destination is writable
@attrib -R .\src\main\java\org\* /S

@rem If the directories aren't there, we know we need to do something. If they are there, we
@rem need to figure out if they're symlinks (in which case we don't really need to do anything)
@rem or if they're copies that need to be updated. Recreating the symlinks should be quick, so 
@rem we'll take the easy way out and always remove them.
@rmdir .\src\main\java\org\janksters\jankyLib /S /Q
@rmdir .\src\main\java\org\janksters\CommonClassesThisYear /S /Q

@rem If we're on NTFS, we can create symlinks directly; otherwise, we need to make read-only copies
mklink /D src\main\java\org\janksters\CommonClassesThisYear ..\..\..\..\..\..\..\CommonClassesThisYear
@if %ERRORLEVEL% NEQ 0 (
  @echo Unable to create symlinks on the current filesystem with current permissions. #sad
  @rem remove both folders to allow for the possibility of deleted files from the source
  @rem (This was handled above, but if we get smarter about symlinks, we'll need to do this here)
  @mkdir .\src\main\java\org\janksters\jankyLib
  @mkdir .\src\main\java\org\janksters\CommonClassesThisYear
  @rem Now copy the two source areas into our newly created destinations
  @xcopy ..\..\CommonClassesThisYear\*.* .\src\main\java\org\janksters\CommonClassesThisYear\ /S
  @xcopy ..\..\..\jankyLib\*.* .\src\main\java\org\janksters\jankyLib\ /S
  @rem Now let's make these areas read-only to protect form accidental editing of files here.
  @attrib +R .\src\main\java\org\janksters\jankyLib\* /S
  @attrib +R .\src\main\java\org\janksters\CommonClassesThisYear\* /S
) else (
  @echo Creating symlinks!
  @mklink /D src\main\java\org\janksters\jankyLib ..\..\..\..\..\..\..\..\jankyLib
)

