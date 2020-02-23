#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "MakeDocu MonitorHeating"
../MonitorHeating/scripts/makeDocu.sh "$0"
feedback=$?

echo "MakeDocu DataDecoder"
if [ 0 -eq ${feedback} ] ; then
    ../DataDecoder/scripts/makeDocu.sh "$0"
    feedback=$?
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
