#!/usr/bin/env python

import os
import csv
from datetime import datetime
#import datetime

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
    print(dateInfo)
    year = int(dateInfo[0])
    month = int(dateInfo[1])
    day = int(dateInfo[2].split('.')[0])

    time0 = datetime(year,month,day,0,0).strftime('%s')
    print('from file name: ' + time0)
    return int(time0)



def readDataFromFile(fileName) :

    time0 = getLocatTime0ForFile(fileName)

    #print('process: ' + fileName)
    with open(DATA_FILE_LOCATION + fileName) as csvDataFile:
        csvReader = csv.reader(csvDataFile)
        legend = next(csvReader)
        units = next(csvReader)
        #print('legend: ' + str(legend) + ' : ' + str(units))

    cnt = 0
    with open(DATA_FILE_LOCATION + fileName) as csvDataFile:
        csvReader = csv.DictReader(csvDataFile)
        operationState = [0] * 1440    # (24h*60min)
        for row in csvReader:
            # Remove the unit
            if (cnt > 0):
                timeVal = int(row['Time'])
                timeDiff = timeVal - time0
                idx = int(timeDiff / 60)
                value = int(row['Betriebszustand (0:heat 1:wather 3:blocked 5:idle)'])
                # print('time: ' + str(timeVal) + ', timediff: ' + str(timeDiff) + ', idx: ' + str(idx) +  ', value = ' + str(value))
                operationState[idx] = value

#            if (cnt ==1):
#                ts = int(row['Time'])
#                print('from file: ' + str(ts) + ' : ' + datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S'))

            cnt = cnt + 1

    return operationState

# --------------------------------------------------

histo = Histogram()
histo.init(1500)
operatingStateSum = []

for fileName in os.listdir(DATA_FILE_LOCATION):
    if fileName.endswith(".dat.csv") :
        print('---')
        operationState = readDataFromFile(fileName)
        #print('op: ' + operationState[0])
        #print('op: ' + operationState[1])
        nrData = len(operationState)
        print('len: ' + str(nrData))
        histo.addValue(nrData)
        # Use the data
        operatingStateSum.append(operationState)

print(operatingStateSum)
histo.dumpHistogram()





myFile = open('example2.csv', 'w')
with myFile:
    writer = csv.writer(myFile)
    writer.writerows(zip(*operatingStateSum))

print("Writing complete")
