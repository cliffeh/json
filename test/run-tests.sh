#!/bin/bash

DIR=$(dirname "$0")
CMD="$DIR/../json"  
failed_tests=""

color_red="$(tput setaf 1)"
color_green="$(tput setaf 2)"
color_cyan="$(tput setaf 6)"
color_reset="$(tput sgr0)"

# accept files
for file in $DIR/y_*.json; do
  testname=$(basename "$file")
  ${CMD} <"$file" >"$file.out" 2>"$file.err"
  if [[ $? -eq 0 ]] ; then
    echo "$testname: ${color_green}OK${color_reset}"
  else
    failed_tests="$failed_tests $testname"
  fi
done

# reject files
for file in $DIR/n_*.json; do
  testname=$(basename "$file")
  ${CMD} <"$file" >"$file.out" 2>"$file.err"
  if [[ $? -eq 0 ]] ; then
    failed_tests="$failed_tests $testname"
  else
    echo "$testname: ${color_green}OK${color_reset}"
  fi
done

# # either-or files
# for file in $DIR/i_*.json; do
#   testname=$(basename "$file")
#   ${CMD} <"$file" >"$file.out" 2>"$file.err"
#   echo "$testname ${color_cyan}FAIR ($?)${color_reset}"
# done

# spit out all the failed tests at the bottom
for test in $failed_tests ; do
  echo "$test: ${color_red}FAIL${color_reset}"
done

if [[ -n $failed_tests ]] ; then
  exit 1
fi
