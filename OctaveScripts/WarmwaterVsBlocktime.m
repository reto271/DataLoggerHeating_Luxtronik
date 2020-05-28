## Copyright (C) 2020 reto271
##
## This program is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see
## <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{retval} =} ReadAndPlotCSV (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: reto <reto@reto-XPS>
## Created: 2020-02-27

#function retval = ReadAndPlotCSV (input1, input2)
#
#endfunction


clear all;
#close all;
clear figure;
clc;

##------------------------------------------------------------
analyzeDate;

%Prepare files and output strings
fileDateStr=strrep(fileDate, "_", "-")
fileName = ["../HeatingData/" fileDate ".dat.csv"];

data = csvread (fileName, 2, 0);
timeInMinutes = (data(:,1)-data(1,1))/60;
timeInHours = timeInMinutes / 60;

fileID = fopen(fileName);
textLine = textscan(fileID,'%s', 72,'Delimiter',';');
header = textLine{1,1}(1,:);
[headerText, nrHeaderTexts] = strsplit(header{1}, ",");
clear header;
clear textLine;

##------------------------------------------------------------
indexAmbientTemp=findIndexFromName(headerText, "Aussentemperatur")
indexCurrentWaterTemp=findIndexFromName(headerText, "Warmwasser Ist-Temperatur")
indexEVU_blocking=findIndexFromName(headerText, "EVU Sperre")
indexRunsSince=findIndexFromName(headerText, "Wärmepumpe läuft seit")
indexOperatingState=findIndexFromName(headerText, "Betriebszustand (0:heat 1:wather 3:blocked 5:idle)")

##------------------------------------------------------------
figure(1);
hdl4(1) = subplot(2,1,1);
plot(timeInHours, data(:,indexAmbientTemp), ...
     timeInHours, data(:,indexCurrentWaterTemp));
grid on;
title(["Temperatures - " fileDateStr]);
xlabel("time in hours");
ylabel("Temperature [dec C]");
legend([headerText(indexAmbientTemp), ...
        headerText(indexCurrentWaterTemp) ...
        ], "location", "northoutside");

hdl4(2) = subplot(2,1,2);
plot(timeInHours, -1-data(:,indexEVU_blocking), ...
     timeInHours, data(:,indexOperatingState), ...
     [0],[6],[0],[-3]);
grid on;
title(["Operating State - " fileDateStr]);
xlabel("time in minutes");
legend([headerText(indexEVU_blocking), ...
        headerText(indexOperatingState) ...
        ], "location", "southoutside");
linkaxes(hdl4, 'x');


##------------------------------------------------------------
figure(2);
hdl4(1) = subplot(2,1,1);
plot(
     timeInHours, data(:,indexRunsSince)/60);
grid on;
title(["Wather Temperatures - " fileDateStr]);
xlabel("time in hours");
ylabel("time in minutes");
legend([headerText(indexRunsSince) ...
        ], "location", "northoutside");

totalEVU_Sperre = cumsum(1-data(:,indexEVU_blocking));
EVU_Sperre = (1-data(:,indexEVU_blocking))*max(totalEVU_Sperre);
hdl4(2) = subplot(2,1,2);
plot(timeInHours, EVU_Sperre, ...
     timeInHours, totalEVU_Sperre);
grid on;
title(["Total Sperrzeit - " fileDateStr]);
xlabel("time in hours");
ylabel("time in minutes");
legend([headerText(indexEVU_blocking), ...
        "Total EVU Speerzeit" ...
        ], "location", "southoutside");
linkaxes(hdl4, 'x');
