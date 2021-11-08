#!/bin/bash

if [ $# -lt 2 ]; then
  echo "You must pass cmake build dir and output root as script arguments "
  exit
fi

COVERAGE_ROOT=$1
OUTPUT_ROOT=$2

echo "searching coverage files in $COVERAGE_ROOT"
echo "output statistics files in $OUTPUT_ROOT"

# generating coverage files
gcov ./CMakeFiles/catch_test.dir/src/*/*.gcno
# generating coverage info file and removing useless data
lcov -c -b "$OUTPUT_ROOT" --no-external --directory . -o ./coverage.info
lcov --remove ./coverage.info "*/test/*" "*/deps/*" -o ./coverage.info
# generating html documentation
genhtml ./coverage.info --output-directory "$OUTPUT_ROOT"/doc/coverage --prefix "$OUTPUT_ROOT"
mkdir -p tmp_gcov_files
# moving tmp files in a separate directory
mv ./*gcov tmp_gcov_files
