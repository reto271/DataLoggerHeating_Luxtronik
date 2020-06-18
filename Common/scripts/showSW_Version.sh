#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

MajorMinorVersion=$(cat ../src/ProgramVersion.hpp | \
                        grep "std::string SoftwareVersion" | \
                        awk -F'"' '{ print $2 }')
BetaVersion=$(cat ../src/ProgramVersion.hpp | \
                  grep "std::string MinorVersion" | \
                  awk -F'"' '{ print $2 }')

#echo ""
echo "SW Version: ${MajorMinorVersion} ${BetaVersion}"
#echo "---"

# Back to the original location
popd > /dev/null
