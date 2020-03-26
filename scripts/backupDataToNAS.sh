#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null


echo "Copy data ..."
scp HeatingData/*.dat root@192.168.1.100:/volume1/buildingData/heating
scp HeatingData/*.log root@192.168.1.100:/volume1/buildingData/heating

# Back to the original location
popd > /dev/null
