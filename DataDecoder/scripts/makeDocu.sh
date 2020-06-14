#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

rm -rf docu
mkdir -p docu
mkdir -p tmp

cp ../Common/doxyfile .
sed -i 's#__THE_PROJECT_NAME__#Data Decoder - Converts Binary Data to CSV#g' doxyfile
doxygen doxyfile
rm -f doxyfile

#cd docu/latex
#make 2>&1 >> ../../tmp/latex.log

# Back to the original location
popd > /dev/null
