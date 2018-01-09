#!/bin/bash
#
# Update sources in Makefile.am

set -eu

cd ./src
SOURCES=()
for f in $(git ls-files -- '*.c' '*.cc' '*.h' '*.hpp'); do
  SOURCES+=("$f")
done

IFS=$'\n' SORTED=($(sort <<<"${SOURCES[*]}"))
unset IFS

echo "cryptopals_SOURCES = ${SORTED[*]}"
sed -i "s|^cryptopals_SOURCES = .*|cryptopals_SOURCES = ${SORTED[*]}|g" Makefile.am
