#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

echo "Copy data ..."
scp -P 8022 root@192.168.1.100:/volume1/buildingData/heating/* HeatingData/

# chmod 644 HeatingData/*.dat
# chmod 644 HeatingData/*.log

# Back to the original location
popd > /dev/null
