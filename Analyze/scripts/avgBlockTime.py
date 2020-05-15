#!/usr/bin/env python

import os
import csv
from datetime import datetime

DATA_FILE_LOCATION = '../../HeatingData/'


def getLocatTime0ForFile(fileName):
    dateInfo = fileName.split('_')
    year = int(dateInfo[0])
    month = int(dateInfo[1])
    day = int(dateInfo[2].split('.')[0])
    time0 = datetime(year,month,day,0,0).strftime('%s')
    return int(time0)


def getDateFromFile(fileName):
    dateInfo = fileName.split('_')
    year = int(dateInfo[0])
    month = int(dateInfo[1])
    day = int(dateInfo[2].split('.')[0])
    return str(year) + '-' + str(month) + '-' + str(day)


def readDataFromFile(fileName) :

    time0 = getLocatTime0ForFile(fileName)

    with open(DATA_FILE_LOCATION + fileName) as csvDataFile:
        csvReader = csv.reader(csvDataFile)
        legend = next(csvReader)
        units = next(csvReader)
        #print('legend: ' + str(legend) + ' : ' + str(units))

    cnt = 0
    with open(DATA_FILE_LOCATION + fileName) as csvDataFile:
        csvReader = csv.DictReader(csvDataFile)
        operationState = [-0.1] * 1441    # (24h*60min) + header
        operationState[0] =  fileName.split('.')[0].replace('_', '-')
        for row in csvReader:
            # Remove the unit
            if (cnt > 0):
                timeVal = int(row['Time'])
                timeDiff = timeVal - time0
                idx = int(timeDiff / 60) + 1  # +1 -> do not overwrite header row
                value = int(row['EVU Sperre'])
                # print('time: ' + str(timeVal) + ', timediff: ' + str(timeDiff) + ', idx: ' + str(idx) +  ', value = ' + str(value))
                # Invert the data
                if (1 == value):
                    operationState[idx] = 0
                else:
                    operationState[idx] = 1
#            if (cnt ==1):
#                ts = int(row['Time'])
#                print('from file: ' + str(ts) + ' : ' + datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S'))

            cnt = cnt + 1

    return operationState


# --------------------------------------------------
dateVec = []
blockTimeVec = []
minValue = (24 * 60) + 1
maxValue = 0

dateVec.append('Date')
blockTimeVec.append('Block Time')

for fileName in sorted(os.listdir(DATA_FILE_LOCATION)):
    blockTimePerDay = 0
    if fileName.endswith(".dat.csv") :
        operationState = readDataFromFile(fileName)
        nrData = len(operationState)

        # Use the data
        pos = 0
        for val in operationState:
            if (0 != pos):
                if (1 == val):
                    blockTimePerDay =  blockTimePerDay +1
            pos = pos + 1

        # Append values of the day
        if ((0 == len(dateVec)) or (getDateFromFile(fileName) != dateVec[-1])):
            dateVec.append(getDateFromFile(fileName))
            blockTimeVec.append(blockTimePerDay)
        else:
            blockTimeVec[-1] = blockTimeVec[-1] + blockTimePerDay

        # Update min/max valuesd
        print('block time: ' +str(blockTimePerDay))
        if (maxValue < blockTimePerDay):
            maxValue = blockTimePerDay
        if (minValue > blockTimePerDay):
            minValue = blockTimePerDay

# Create data structure for probability of blocking
blockTimeSumData = []
blockTimeSumData.append(dateVec)
blockTimeSumData.append(blockTimeVec)

# Write file with all data sets
sumFile = open(DATA_FILE_LOCATION + 'BlockingTimePerDay.csv', 'w')
with sumFile:
    writer = csv.writer(sumFile)
    writer.writerows(zip(*blockTimeSumData))

print('  max block time: ' + str(maxValue))
print('  min block time: ' + str(minValue))
print("Writing complete")
