@set mypath=%~dp0
@echo %mypath%

@for /d %%d in (%mypath%\*) do (
  @if exist %%d\build.gradle (
    @echo Processing directory: %%d
    @cd %%d
    @call %mypath%\JanksterNewProjectSetup\WindowsModsScript.bat
  )
)

@pause
