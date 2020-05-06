#!/bin/bash

# Change into the project root directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

./MonitorHeating/bin/MonitorHeating 192.168.1.144 | tee -a ../HeatingData/trace.log

# Back to the original location
popd > /dev/null

exit ${feedback}
