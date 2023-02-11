#!/bin/bash

DIR=$(dirname "$0")
DATADIR="$DIR/data"
CMD="$DIR/../json"
failed_tests=""

color_red="$(tput setaf 1)"
color_green="$(tput setaf 2)"
color_cyan="$(tput setaf 6)"
color_reset="$(tput sgr0)"

# only run valgrind on accept files
for file in "$DATADIR"/y_*.json; do
  testname=$(basename "$file")
  valgrind --error-exitcode=2 --leak-check=full --log-file="$file.valgrind.log" "${CMD}" > /dev/null 2>&1 < "$file"
  if [[ $? -eq 0 ]] ; then
    echo "$testname: ${color_green}OK${color_reset}"
  else
    failed_tests="$failed_tests $testname"
  fi
done

# spit out all the failed tests at the bottom
for test in $failed_tests ; do
  echo "$test: ${color_red}FAIL${color_reset}"
done

if [[ -n $failed_tests ]] ; then
  exit 1
fi
