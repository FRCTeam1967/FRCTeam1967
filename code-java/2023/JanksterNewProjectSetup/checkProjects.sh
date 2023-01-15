#!/bin/bash

janksterPaths="CommonClassesThisYear jankyLib"
janksterMacScript="JanksterNewProjectSetup/MacModsScript"
janksterWinScript="JanksterNewProjectSetup/WindowsModsScript.bat"

printCheckCriteria() {
  echo "Checking project(s) to ensure they:"
  echo "* Contain a workspace file"
  echo "* The workspace file includes Jankster paths"
  echo "* gradlew contains Jankster additions"
  echo "* gradlew.bat contains Jankster additions"
  echo "* Contain a .gitignore file for src/main/java/org"
  echo "============================="
}

checkWorkspaceInDirectory() {
  d="$1"
  wsChecksPassed=0
  workspace=($d/*.code-workspace)
  if [[ -f "$workspace" ]]; then
    # Make sure that the workspace at least references both CommonClassesThisYear and jankyLib
    for path in $janksterPaths; do
      if ! grep -q $path "$workspace"; then
        echo "  ERROR: Workspace does not reference $path"
        wsChecksPassed=1
      fi
    done
  else
    echo "  ERROR: Project is missing a workspace"
    wsChecksPassed=1
  fi

  return $wsChecksPassed
}

checkBuildDotGradleInDirectory() {
  # At one point, we were putting Jankster paths into build.gradle, but 
  # we changed our minds. While that works for building, the RedHat Java
  # support doesn't know about them, causing auto-complete to fail. 
  d="$1"
  gradleChecksPassed=0
  if [[ -f "$d/build.gradle" ]]; then
    for path in $janksterPaths; do
      if grep -q $path "$d/build.gradle"; then
        echo "  ERROR: build.gradle has a reference to $path that should be removed"
        gradleChecksPassed=1
      fi
    done
  else
    echo "  ERROR: Project is missing a build.gradle file"
    gradleChecksPassed=1
  fi

  return $gradleChecksPassed
}

checkForGitIgnoreInDirectory() {
  d="$1"
  # Check that we have a .gitignore in the proper place
  if [[ ! -e "$d/src/main/java/.gitignore" ]]; then
      echo "  ERROR: Failed check for $d/src/main/java/.gitignore"
      return 1
  fi

  return 0
}

checkGradlewInDirectory() {
  d="$1"
  if ! grep -q $janksterMacScript "$d/gradlew"; then
    echo "  ERROR: gradlew file is missing Jankster additions"
    return 1
  fi 

  return 0
}

checkGradlewBatInDirectory() {
  d="$1"
  if ! grep -q $janksterWinScript "$d/gradlew.bat"; then
    echo "  ERROR: gradlew.bat file is missing Jankster additions"
    return 1
  fi 

  return 0
}

reportResult() {
  if [[ $2 -eq 0 ]]; then
    echo "$1 passed."
  else
    echo "$1 FAILED."
  fi
}

checkProjectDirectory() {
  d="$1"
  if [[ -d "$d" ]]; then
    # Will not run if no directories are available
    if [[ -f "$d/gradlew" ]]; then
      echo "Checking project in $d/"
      # Now we know this folder must be a project. So let's check our checklist items.
      # Check that the project has a workspace (we assume at most one workspace is found)
      checkWorkspaceInDirectory "$d"
      reportResult "  Workspace checks....." $?
      checkBuildDotGradleInDirectory "$d"
      reportResult "  Build.grade checks..." $?
      checkForGitIgnoreInDirectory "$d"
      reportResult "  .gitignore checks...." $?
      checkGradlewInDirectory "$d"
      reportResult "  gradlew checks......." $?
      checkGradlewBatInDirectory "$d"
      reportResult "  gradlew.bat checks..." $?
    fi
  fi
}

# See if we look like we're in a project directory or the parent
if [[ -f build.gradle ]]; then
  echo "Checking project in current directory:"
  printCheckCriteria
  checkProjectDirectory .
else 
  echo "Checking all projects in current directory:"
  printCheckCriteria
  for d in *; do
    checkProjectDirectory "$d"
  done
fi

