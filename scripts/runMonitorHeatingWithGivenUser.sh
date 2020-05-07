#!/bin/bash

# ----------------------------------------------------------------------------------------------
# Run this script from /etc/rc.local
# Append at the end the following line at the end of /etc/rc.local
#   /home/openhabian/git/DataLoggerHeating_Luxtronik/scripts/monitorHeating.sh
# Keep 'exit 0' at the end of the /etc/rc.local
# ----------------------------------------------------------------------------------------------
LOG_FILE=./HeatingData/boot1.log

# Change into the project root directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

echo "Starting up..." > ${LOG_FILE}
echo "Process working directory" >> ${LOG_FILE}
pwd >> ${LOG_FILE}
echo "---------------------------" >> ${LOG_FILE}

echo "Run monitor heating as user openhabian" >> ${LOG_FILE}
su -s /bin/bash -c '/home/openhabian/git/DataLoggerHeating_Luxtronik/scripts/monitorHeating_internal.sh &' openhabian
feedback=$?
echo "Run user cmd feedback: ${feedback}" >> ${LOG_FILE}

if [ 0 -ne ${feedback} ] ; then
    echo "Did not work, run it as root" >> ${LOG_FILE}
    ./script/monitorHeating_internal.sh
    feedback=$?
    echo "Run as root feedback: ${feedback}" >> ${LOG_FILE}
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
