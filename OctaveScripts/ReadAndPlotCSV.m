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
close all;
clc;

data = csvread ("../test.csv", 1, 0);
timeInMinutes = (data(:,1)-data(1,1))/60;

fileID = fopen("../test.csv");
textLine = textscan(fileID,'%s', 72,'Delimiter',',');
header = textLine{1,1};

figure(1);
plot(timeInMinutes, data(:,2)/10, timeInMinutes, data(:,3)/10, ...
     timeInMinutes, data(:,8)/10, timeInMinutes, data(:,9)/10);
grid on;
title("Manual - first data set");
xlabel("time in minutes");
ylabel("Temperature in deg Celcius");
# legend(header{2,1}, header{3,1}, header{8,1}, header{9, 1});
legend(header{2,1}, header{3,1}, header{8,1}, header{9, 1}, "location", "southoutside");
