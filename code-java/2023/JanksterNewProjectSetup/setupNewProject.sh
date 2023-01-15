#!/bin/bash

# Usage: ./setupNewProject.sh <projectname>
#        If <projectname> is blank, presumes the current folder IS the project folder.
#        Will search for <projectname> and ../<projectname> automatically

# If no argument is given, assume we're running FROM the new project folder.
# If an argument is given, it might be the name of the project with or without ../
if [[ $# -eq 0 ]]; then
  d=.
  if [[ ! -e ./gradlew ]]; then
    echo "ERROR: Cannot detect project in this location."
    exit 1
  fi
else
  echo "Setting up Project: $1"
  if [[ -e $1/gradlew ]]; then
    d=$1
  else
    if [[ -e ../$1/gradlew ]]; then
      d=../$1
    else
      echo "ERROR: Could not detect location of project."
      exit 2
    fi
  fi
fi

echo "Actual project location is $d"
g=$d/gradlew
gb=$d/gradlew.bat

# modify gradlew if it has not previously been modified
if grep JanksterNewProjectSetup $g >/dev/null ; then
  echo "Skipping gradlew modification as it appears complete already."
else
  sed '1r ../JanksterNewProjectSetup/gradlew_Addition' $g >gradlew.tmp
  cp gradlew.tmp $g
  rm gradlew.tmp
fi

# modify gradlew.bat if it has not previously been modified
if grep JanksterNewProjectSetup $gb >/dev/null ; then
  echo "Skipping gradlew.bat modification as it appears complete already."
else
  sed '1r ../JanksterNewProjectSetup/gradlewBAT_Addition' $gb >gradlew.tmp
  cp gradlew.tmp $gb
  rm gradlew.tmp
fi

echo "Project setup complete."

