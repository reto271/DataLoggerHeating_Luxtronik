#!/usr/bin/env python

import os
import csv
from datetime import datetime

DATA_FILE_LOCATION = '../../HeatingData/'

class Histogram:
    m_histo = []
    m_size = 0
    def init(self, sizeHisto):
        self.m_histo = [0] * sizeHisto
        self.m_size = sizeHisto

    def addValue(self, value):
        if (value < self.m_size):
            self.m_histo[value] = self.m_histo[value] + 1

    def dumpHistogram(self):
        print('\nHistogram (only non-zero values)')
        pos = 0
        for nrOccur in self.m_histo:
            if (0 < nrOccur):
                print(str(pos) + ' : ' + str(nrOccur))
            pos = pos + 1
# --- end class Histogram --------------------------------------------


def getLocatTime0ForFile(fileName):
    dateInfo = fileName.split('_')
    year = int(dateInfo[0])
    month = int(dateInfo[1])
    day = int(dateInfo[2].split('.')[0])
    time0 = datetime(year,month,day,0,0).strftime('%s')
    return int(time0)

def generateTimeVector():
    timeVector = [0] * 1441
    timeVector[0] = 'Time'
    minute=0
    hour=0
    for cnt in range(1440):
        timeVector[cnt+1] = str(hour) + ':' + str(minute)
        minute = minute + 1
        if (60 == minute) :
            minute = 0
            hour = hour + 1
    return timeVector


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

#histo = Histogram()
#histo.init(1500)


# Generate data structures and add time vector
operatingStateSum = []
operatingStateCumSum = [0] * 1441
timeVector = generateTimeVector()
operatingStateSum.append(timeVector)
operatingStateCumSum[0] = 'Sum'

for fileName in sorted(os.listdir(DATA_FILE_LOCATION)):
    if fileName.endswith(".dat.csv") :
        operationState = readDataFromFile(fileName)
        nrData = len(operationState)
        #histo.addValue(nrData)
        # Use the data
        operatingStateSum.append(operationState)
        pos = 0
        for val in operationState:
            if (0 != pos):
                if (1 == val):
                    operatingStateCumSum[pos] = operatingStateCumSum[pos] + 1
            pos = pos + 1

#histo.dumpHistogram()

# Scale to 100%
maxVal = max(operatingStateCumSum[1:])
operatingStateCumSum[0] = 'Sum of '+ str(maxVal) + ' data sets'
pos = 1
for val in operatingStateCumSum[1:]:
    operatingStateCumSum[pos] = 100 * operatingStateCumSum[pos] / maxVal
    pos = pos + 1

# Create data structure for probability of blocking
cumSumData = []
cumSumData.append(timeVector)
cumSumData.append(operatingStateCumSum)

# Write file with all data sets
sumFile = open(DATA_FILE_LOCATION + 'BlockingTimeSummary.csv', 'w')
with sumFile:
    writer = csv.writer(sumFile)
    writer.writerows(zip(*operatingStateSum))

# Write file with probability info
cumSumFile = open(DATA_FILE_LOCATION + 'BlockingTimeCumSum.csv', 'w')
with cumSumFile:
    writer = csv.writer(cumSumFile)
    writer.writerows(zip(*cumSumData))

print("Writing complete")
