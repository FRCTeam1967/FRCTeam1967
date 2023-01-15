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
g=$d/gradlew 
gb=$d/gradlew.bat
gi=$d/src/main/java/.gitignore

# # modify build.gradle if it has not previously been modified
# if grep jankyLib $bg >/dev/null ; then
#   echo "Skipping build.gradle modification as it appears complete already."
# else
# #  sed '1r ../JanksterNewProjectSetup/build.gradle_Addition' $bg >build.gradle.tmp
#   lineNum="$(grep -m 1 -n "def ROBOT_MAIN_CLASS" $bg | cut -d: -f1)"
# # echo found on line $lineNum
# echo Modifying $bg to include common folders
# cat <<EOF >build.gradle.input.tmp
# sourceSets {
#     main {
#         java {
#             srcDir "../../jankyLib"
#             srcDir "../CommonClassesThisYear"
#         }
#     }
# }
# EOF

#   sed $lineNum'r build.gradle.input.tmp' $bg >build.gradle.tmp
#   rm build.gradle.input.tmp
#   cp build.gradle.tmp $bg
#   rm build.gradle.tmp
# fi

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

# modify gradlew if it has not previously been modified
if grep JanksterNewProjectSetup $g >/dev/null ; then
  echo "Skipping gradlew modification as it appears complete already."
else
  # Create an input file to add to gradlew
  cat <<EOF >gradlew.input.tmp

# Run Janksters java-build script
source ../JanksterNewProjectSetup/MacModsScript

EOF

  sed '1r gradlew.input.tmp' $g >gradlew.tmp
  cp gradlew.tmp $g
  rm gradlew.tmp
  rm gradlew.input.tmp
fi

# modify gradlew.bat if it has not previously been modified
if grep JanksterNewProjectSetup $gb >/dev/null ; then
  echo "Skipping gradlew.bat modification as it appears complete already."
else
# Create an input file to add to gradlew.bat
  cat <<EOF >gradlew.bat.input.tmp

@rem Run Janksters java-build script
call ../JanksterNewProjectSetup/WindowsModsScript.bat

EOF

  sed '1r gradlew.bat.input.tmp' $gb >gradlew.bat.tmp
  cp gradlew.tmp.bat $gb
  rm gradlew.bat.tmp
  rm gradlew.bat.input.tmp
fi

# Put a .gitignore file to ignore changes in the link/copy area
if [[ ! -e $gi ]]; then
  cat <<EOF >$e
org/
EOF
fi

echo "Project setup complete."
