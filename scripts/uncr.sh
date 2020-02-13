#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

uncrustify -c uncr.cfg *.cpp *.hpp --replace

# Back to the original location
popd > /dev/null

exit ${feedback}
