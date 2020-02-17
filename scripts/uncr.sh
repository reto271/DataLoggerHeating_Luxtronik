#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

uncrustify -c uncr.cfg src/*.cpp src/*.hpp --replace

mkdir -p tmp
mv -v src/*.unc-backup~ tmp
mv -v src/*.unc-backup.md5~ tmp


# Back to the original location
popd > /dev/null

exit ${feedback}
