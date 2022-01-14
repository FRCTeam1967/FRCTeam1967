#!/bin/bash
echo "Checking all projects in ../ to make sure:"
echo "* Appear to have appropriate gradlew/gradlew.bat modifications."
echo "* Each has a .gitignore file."
echo "============================="
for d in ../*; do
  if [[ -d "$d" ]]; then
    # Will not run if no directories are available
    if [[ -f "$d/gradlew" ]]; then
      echo "Checking project in $d/"
      # Now we know this folder must be a project. So let's check our checklist items.
      # 1. Let's make sure gradle contains at least a reference to JanksterNewProjectSetup
      if grep JanksterNewProjectSetup $d/gradlew >/dev/null ; then
        echo "  Passed gradlew check."
        if grep JanksterNewProjectSetup $d/gradlew.bat >/dev/null ; then
          echo "  Passed gradlew.bat check."
          if [[ -e $d/src/main/java/.gitignore ]]; then
            echo "  Passed .gitignore check."
          else
            echo "  ERROR: Project NOT updated. Failed check for $d/.gitignore"
          fi
        else
          echo "  ERROR: Project NOT updated. Failed check in $d/gradlew.bat"
        fi
      else
        echo "  ERROR: Project NOT updated. Failed check in $d/gradlew"
      fi
    fi
  fi
done

