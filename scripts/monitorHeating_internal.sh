#!/bin/bash

# ----------------------------------------------------------------------------------------------
# Run this script the monitorHeating application and logs the trace into a file
# ----------------------------------------------------------------------------------------------

# Change into the project root directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

./MonitorHeating/bin/MonitorHeating 192.168.1.144 2>&1 | tee -a ./HeatingData/trace.log
feedback=${PIPESTATUS[1]}

# Back to the original location
popd > /dev/null

exit ${feedback}
