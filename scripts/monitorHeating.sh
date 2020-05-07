#!/bin/bash

# ----------------------------------------------------------------------------------------------
# Run this script from /etc/rc.local
# Append at the end the following line at the end of /etc/rc.local
#   /home/openhabian/git/DataLoggerHeating_Luxtronik/scripts/monitorHeating.sh
# Keep 'exit 0' at the end of the /etc/rc.local
# ----------------------------------------------------------------------------------------------

# Change into the project root directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

echo "Starting up..." > ./HeatingData/boot.log
pwd >> ./HeatingData/boot.log
runuser -l openhabian '/home/openhabian/git/DataLoggerHeating_Luxtronik/MonitorHeating/bin/MonitorHeating 192.168.1.144 | tee -a ./HeatingData/trace.log'
feedback=$?
echo "Run user cmd feedback: ${feedback}" >> ./HeatingData/boot.log

if [ 0 -ne ${feedback} ] ; then
    echo "Run it as root" >> ./HeatingData/boot.log
    ./MonitorHeating/bin/MonitorHeating 192.168.1.144 | tee -a ./HeatingData/trace.log
else
    echo "It did work!!!" >> ./HeatingData/boot.log
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
