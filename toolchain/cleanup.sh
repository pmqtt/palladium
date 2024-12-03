#!/bin/bash

rm -rf CMakeCache.txt *.cmake Makefile CMakeFiles palladium

for dir in src; do
  rm -rf "$dir"/CMakeCache.txt "$dir"/*.cmake "$dir"/Makefile "$dir"/CMakeFiles
done

for dir in tests; do
  rm -rf "$dir"/CMakeCache.txt "$dir"/*.cmake "$dir"/Makefile "$dir"/CMakeFiles
done
