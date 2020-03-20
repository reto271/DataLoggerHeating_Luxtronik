#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null


echo "Uncrustify MonitorHeating"
../MonitorHeating/scripts/uncr.sh
feedback=$?

echo "Uncrustify DataDecoder"
if [ 0 -eq ${feedback} ] ; then
    ../DataDecoder/scripts/uncr.sh
    feedback=$?
fi

echo "Uncrustify Common"
if [ 0 -eq ${feedback} ] ; then
    ../Common/scripts/uncr.sh
    feedback=$?
fi

echo "Uncrustify Test"
if [ 0 -eq ${feedback} ] ; then
    ../Test/scripts/uncr.sh
    feedback=$?
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
