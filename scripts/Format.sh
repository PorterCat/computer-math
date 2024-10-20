#!/usr/bin/sh

root=$(dirname $0)/..
find ${root} -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i