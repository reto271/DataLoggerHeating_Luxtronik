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

fileDate="2020_05_14"

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

##------------------------------------------------------------
figure(10);
hdl4(1) = subplot(2,1,1);
plot(timeInHours, data(:,6), ...
     timeInHours, data(:,10), ...
     timeInHours, data(:,11), ...
     timeInHours, data(:,10) - data(:,11));
grid on;
title(["Temperatures - " fileDateStr]);
xlabel("time in hours");
ylabel("Temperature [dec C]");
legend([headerText(6),...
        headerText(10), ...
        headerText(11), ...
        "Temperatur Differenz" ...
        ], "location", "northoutside");

hdl4(2) = subplot(2,1,2);
plot(timeInHours, data(:,35)/60, ...
     timeInHours, data(:,51));
grid on;
title(["Operating Time - " fileDateStr]);
xlabel("time in hours");
ylabel("time in minutes");
legend([headerText(35), headerText(51) ...
        ], "location", "southoutside");

linkaxes(hdl4, 'x');
