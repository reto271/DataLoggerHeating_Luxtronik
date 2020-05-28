## Copyright (C) 2020 reto
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
## @deftypefn {} {@var{retval} =} findIndexFromName (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author: reto <reto@reto-XPS>
## Created: 2020-05-28

function signalIndex = findIndexFromName (signalLegend, signalName)
  signalIndex = -1;  
  for i = 1:length(signalLegend)
    if (1 == strcmp(signalName,signalLegend(1,i){1}))
      signalIndex=i;
    endif
  endfor
endfunction
