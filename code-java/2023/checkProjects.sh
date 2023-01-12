#!/bin/bash

janksterPaths="CommonClassesThisYear jankyLib"

echo "Checking all projects in ../ to make sure:"
echo "* They contain a workspace file"
echo "* The workspace file includes Jankster paths"
echo "* build.gradle includes Jankster source paths"
echo "* Each has a .gitignore file."
echo "============================="
for d in ../*; do
  if [[ -d "$d" ]]; then
    # Will not run if no directories are available
    if [[ -f "$d/gradlew" ]]; then
      echo "Checking project in $d/"
      # Now we know this folder must be a project. So let's check our checklist items.
      # Check that the project has a workspace (we assume at most one workspace is found)
      workspace=($d/*.code-workspace)
      if [[ -f "$workspace" ]]; then
      	wsChecksPassed=1
      	# Make sure that the workspace at least references both CommonClassesThisYear and jankyLib
      	for path in $janksterPaths; do
      	  if ! grep -q $path "$workspace"; then
      	  	  wsChecksPassed=0
    	  	  echo "  ERROR: Workspace does not reference $path"
      	  fi
      	done
      	if [[ $wsChecksPassed -eq 1 ]]; then
      	  echo "  Passed workspace checks."
      	fi
      else
      	echo "  ERROR: Project is missing a workspace"
      fi
      
      # Check that build.gradle references the proper Jankster paths
      if [[ -f "$d/build.gradle" ]]; then
        gradleChecksPassed=1
        for path in $janksterPaths; do
          if ! grep -q $path "$d/build.gradle"; then
            echo "  ERROR: build.gradle does not reference $path"
            gradleChecksPassed=0
    	  fi
        done
        if [[ $gradleChecksPassed -eq 1 ]]; then
          echo "  Build.gradle checks passed"
        fi
      else
        echo "  ERROR: Project is missing a build.gradle file"
      fi
      
      # Check that we have a .gitignore in the proper place
	  if [[ -e $d/src/main/java/.gitignore ]]; then
        echo "  Passed .gitignore check."
	  else
        echo "  ERROR: Failed check for $d/src/main/java/.gitignore"
      fi

    fi
  fi
done

