#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

git clone https://github.com/google/googletest.git

# Back to the original location
popd > /dev/null
