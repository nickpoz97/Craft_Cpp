#!/bin/bash

bin/catch_test
gcov cmake-build-coverage/CMakeFiles/catch_test.dir/src/*/*.gcno
lcov --no-external  -c --directory . -o coverage.info
lcov --remove ./coverage.info "*/test/*" "*/deps/*" -o ./coverage.info
rm -r ./*gcov
genhtml coverage.info --output-directory doc/coverage --prefix "$(pwd)"
