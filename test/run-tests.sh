#!/bin/bash

DIR=$(dirname "$0")
DATADIR="$DIR/data"
CMD="$DIR/../json"
failed_tests=""

color_red="$(tput setaf 1)"
color_green="$(tput setaf 2)"
color_reset="$(tput sgr0)"

# accept files
for file in "$DATADIR"/y_*.json; do
  ${CMD} -I0 <"$file" >"$file.out" 2>"$file.err"
  if [[ $? -eq 0 ]]; then
    echo "ACCEPT:$file: ${color_green}OK${color_reset}"

    # test for correct output
    tr -d '\n' <"$file" | diff -w "$file.out" - >"$file.diff"
    if [[ $? -ne 0 ]]; then
      failed_tests="$failed_tests DIFF:$file"
    fi
  else
    failed_tests="$failed_tests ACCEPT:$file"
  fi
done

# reject files
for file in "$DATADIR"/n_*.json; do
  ${CMD} <"$file" >"$file.out" 2>"$file.err"
  if [[ $? -eq 0 ]]; then
    failed_tests="$failed_tests REJECT:$file"
  else
    echo "REJECT:$file: ${color_green}OK${color_reset}"
  fi
done

# jq files (pretty-print tests)
for file in "$DATADIR"/jq_*.json; do
  ${CMD} -I2 <"$file" >"$file.pretty" 2>"$file.err"
  diff "$file" "$file.pretty" >& "$file.pretty.err"
  if [[ $? -ne 0 ]]; then
    failed_tests="$failed_tests PRETTY:$file"
  else
    echo "PRETTY:$file: ${color_green}OK${color_reset}"
  fi
done

if [[ $1 = "--all" ]]; then
  # either-or files
  for file in "$DATADIR"/i_*.json; do
    ${CMD} <"$file" >"$file.out" 2>"$file.err"
    if [[ $? -eq 0 ]]; then
      echo "EITHER:$file: ${color_green}OK${color_reset}"
    else
      failed_tests="$failed_tests EITHER:$file"
    fi
  done
fi

# spit out all the failed tests at the bottom
for file in $failed_tests; do
  echo "$file: ${color_red}FAIL${color_reset}"
done

if [[ -n $failed_tests ]]; then
  exit 1
fi
