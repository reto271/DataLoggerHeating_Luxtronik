#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

../Common/scripts/showSW_Version.sh

echo "::: Build DataDecoder"
../DataDecoder/scripts/build.sh "$0"
feedback=$?

echo "::: Start Conversion"
if [ 0 -eq ${feedback} ] ; then
    cd ../
    for dataFile in $(find HeatingData/ -name "*.dat" | sort) ; do
        ./DataDecoder/bin/DataDecoder ${dataFile}
        conversionFeedback=$?
        if  [ 0 -ne  ${conversionFeedback} ] ; then
            echo "ERROR: conversion file: '${dataFile}' failed"
            feedback=${conversionFeedback}
            errorFileList+=( "${dataFile}" )
        fi
    done
fi

# Overall feedback
echo ""
if [ 0 -eq ${feedback} ] ; then
    echo "All files successfuly converted."
else
    echo "ERROR: Some Files not succesfully converted"
    printf '  - %s\n' "${errorFileList[@]}"
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
