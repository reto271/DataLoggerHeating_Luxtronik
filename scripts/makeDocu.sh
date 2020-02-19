#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

mkdir -p doc
doxygen doxyfile

# Back to the original location
popd > /dev/null
