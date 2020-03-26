#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "Copy data ..."
scp root@192.168.1.100:/volume1/buildingData/heating/* HeatingData/

# Back to the original location
popd > /dev/null
