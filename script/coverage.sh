#!/bin/bash

if [ $# -lt 2 ]; then
  echo "You must pass \"coverage data root dir\" and \"output root dir\" as script arguments "
  exit
fi

COVERAGE_SRC=$1
OUTPUT_ROOT=$2

echo "searching coverage files in $COVERAGE_ROOT"
echo "Output dir: $OUTPUT_ROOT/doc/coverage"

# generating coverage info file and removing useless data
lcov -c -b "$OUTPUT_ROOT" --no-external \
  --directory "$COVERAGE_SRC/Geometry" \
  --directory "$COVERAGE_SRC/Interaction" \
  --directory "$COVERAGE_SRC/Rendering" \
  -o ./coverage.info

lcov --remove ./coverage.info "*/test/*" \
  "*/deps/*" \
  "*/src/Geometry/Sphere.cpp" \
  "*/src/Geometry/Item.cpp" \
  "*/src/App.cpp" \
  -o ./coverage.info

# generating html documentation
genhtml ./coverage.info --output-directory "$OUTPUT_ROOT"/doc/coverage --prefix "$OUTPUT_ROOT"
