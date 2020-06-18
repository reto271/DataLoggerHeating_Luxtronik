#!/bin/bash

# Change into the script directory
ScriptDirectory=$(readlink -f $(dirname "$0"))
pushd "${ScriptDirectory}" > /dev/null

# Due source, must change into the scripts directory. Afterwards go to the desired directory.
cd ../../Common/scripts/
source showSW_Version.sh
#echo "DocuVersion: ${DATA_LOGGER_LUXTRONIK_SW_VERSION}"
cd "${ScriptDirectory}/../"

# Remove the old docu
rm -rf docu
mkdir -p docu
mkdir -p tmp

cp ../Common/doxyfile .
sed -i "s#__THE_PROJECT_NAME__#Monitor Heating - Read Data from Luxtronik Controller (${DATA_LOGGER_LUXTRONIK_SW_VERSION})#g" doxyfile
doxygen doxyfile

#cd docu/latex
#make 2>&1 >> ../../tmp/latex.log

# Back to the original location
popd > /dev/null
