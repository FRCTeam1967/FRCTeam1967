@rem Please UPDATE this version number ANY time changes are made to the file.
@echo Janksters Java-Build gradlew.bat mods Version 1.05

@rem Make sure the location exists for building to be successful
@if not exist .\src\main\java\org\ (
  @mkdir .\src\main\java\org
  @mkdir .\src\main\java\org\janksters
  @rem Copy the .gitignore file to its new home one time.
  @copy ..\JanksterNewProjectSetup\gitignoreForLinkArea .\src\main\java\.gitignore
)

@rem Now let's make sure the destination is writable
@attrib -R .\src\main\java\org\* /S
@rem remove both folders to allow for the possibility of deleted files from the source
@rmdir .\src\main\java\org\janksters\jankyLib /S /Q
@rmdir .\src\main\java\org\janksters\CommonClassesThisYear /S /Q
@mkdir .\src\main\java\org\janksters\jankyLib
@mkdir .\src\main\java\org\janksters\CommonClassesThisYear
@rem Now copy the two source areas into our newly created destinations
@xcopy ..\CommonClassesThisYear\*.* .\src\main\java\org\janksters\CommonClassesThisYear\ /S
@xcopy ..\..\jankyLib\*.* .\src\main\java\org\janksters\jankyLib\ /S
@rem Now let's make these areas read-only to protect form accidental editing of files here.
@attrib +R .\src\main\java\org\janksters\jankyLib\* /S
@attrib +R .\src\main\java\org\janksters\CommonClassesThisYear\* /S
