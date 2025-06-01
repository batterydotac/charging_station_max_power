# charging_station_max_power
Testing data for Organicmaps charging station maximum power processor

This repository contains the data used to test the charging station maximum power processor for Organicmaps as of 2025/05/31.

Compile with:
g++ -o parsemax parsemax.cpp

Run by:
./parsemax tough.xml > tough.txt
./parsemax all.xml > all.txt

There are two elements to the processor.  The first interprets the maximum power for a given socket type via a tag.  The second combines the current result with the previous result to create a displayed maximum.  The constant "MAX_POWER_RESET" defines after how many lines to reset the second algorithm.  During normal operation, a reset will occur for each station.
