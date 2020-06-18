#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

Common/scripts/showSW_Version.sh

echo "Copy data ..."
scp -P 8022 HeatingData/*.dat root@192.168.1.100:/volume1/buildingData/heating
scp -P 8022 HeatingData/*.log root@192.168.1.100:/volume1/buildingData/heating

# Back to the original location
popd > /dev/null
