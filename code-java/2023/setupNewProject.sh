#!/bin/bash

#
# HISTORY
#   Project setup automation for 2023
#   Adds sourceset{} to build.gradle
#   Creates .code-workspace including common folders for viewing/editing
#

# Usage: ./setupNewProject.sh <projectname>
#        If <projectname> is blank, presumes the current folder IS the project folder.
#        Will search for <projectname> and ../<projectname> automatically

# If no argument is given, assume we're running FROM the new project folder.
# If an argument is given, it might be the name of the project with or without ../
if [[ $# -eq 0 ]]; then
  d=.
  if [[ ! -e ./build.gradle ]]; then
    echo "ERROR: Cannot detect project in this location."
    exit 1
  fi
  theDir=`basename $PWD`
else
  echo "Setting up Project: $1"
  if [[ -e $1/build.gradle ]]; then
    d=$1
  else
    if [[ -e ../$1/build.gradle ]]; then
      d=../$1
    else
      echo "ERROR: Could not detect location of project."
      exit 2
    fi
  fi
  theDir=`basename $d`
fi

echo "Actual project location is $d"
bg=$d/build.gradle
# Code-workspace file is here...
cw=$d/$theDir.code-workspace

# modify build.gradle if it has not previously been modified
if grep jankyLib $bg >/dev/null ; then
  echo "Skipping build.gradle modification as it appears complete already."
else
#  sed '1r ../JanksterNewProjectSetup/build.gradle_Addition' $bg >build.gradle.tmp
  lineNum="$(grep -m 1 -n "def ROBOT_MAIN_CLASS" $bg | cut -d: -f1)"
# echo found on line $lineNum
echo Modifying $bg to include common folders
cat <<EOF >build.gradle.input.tmp
sourceSets {
    main {
        java {
            srcDir "../../jankyLib"
            srcDir "../CommonClassesThisYear"
        }
    }
}
EOF

  sed $lineNum'r build.gradle.input.tmp' $bg >build.gradle.tmp
  rm build.gradle.input.tmp
  cp build.gradle.tmp $bg
  rm build.gradle.tmp
fi

# CREATE project .code-workspace if it has not previously been created
#   Any existing file will cause us to NOT do work here.
if [[ -e $cw ]]; then
  if grep jankyLib $cw >/dev/null ; then
    echo "INFO: No changes made. Existing $cw file seems already prepped."
  else
    echo "ERROR: Cannot modify existing $cw file in this location."
  fi
  exit 1
fi
# Now do the file creation.
echo Creating code workspace file $cw
cat <<EOF >$cw
{
  "folders": [
    {
      "path": "."
    },
    {
      "path": "../CommonClassesThisYear"
    },
    {
      "path": "../../jankyLib"
    }
  ],
  "settings": {
    "java.configuration.updateBuildConfiguration": "automatic",
    "java.server.launchMode": "Standard"
  }
}
EOF

echo "Project setup complete."
