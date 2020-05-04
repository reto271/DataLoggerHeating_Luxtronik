# Data Logger, Converter and Analysis Tool for Luxtronik Heating Control

The project consists of three major components and three minor.

Component      | Classification | Comment
-------------- | ------------- | -------
MonitorHeating | Major         | Reads the data from the Luxtronik control and stores them in binary format.
DataDecoder    | Major         | Converts the binary data to CSV
Analyze        | Major         | Does some basic analysis on the CSV files
OctaveScripts  | Minor         | Prints graphs based on the CSV files
Tests          | Minor         | Runs some tests on the MonitorHeating and DataDecoder
Utils          | Minor         | Local time to unix-time converter util


# MonitorHeating
Uses the command 3004 to read the current data from the Luxtronik heating controller. It takes the IP address as input parameter and reads the data every minute from the controller. The data is stored in binary format. The number of used bits is optimized for each value to minimize the file size. The Common/src/ValueTable.cpp lists the stored values and the corresponding bit-sizes.

The binary files are suitable to backup, since the consume little space.

# DataDecoder
The data decoder converts the binary data files into CSV files. The CSV files consume more space than the binary files but are more suitable for further processing.

# Analyze
A few python scripts to analyze the data. As input the CSV data is used. You may adapt these scripts for your own purpose.

# Minor Components

## OctaveScripts
An octave script generating a few charts. It displays the warm water and outside temperature. A second chart shows the blocking time and the operating mode of the heating controller.

## Tests
Runs a few google tests on the sources of MonitorHeating and DataDecoder.

## Utils
A util to convert local time to Unix time.

# Acknowledgments
Thanks to Bouni and his documentation on https://github.com/Bouni/Home-Assistant-Luxtronik
