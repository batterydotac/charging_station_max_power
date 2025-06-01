# charging_station_max_power
--- Testing data for Organicmaps charging station maximum power processor ---

This repository contains the data used to test the charging station maximum power processor for Organicmaps as of 2025/05/31.

Compile:

g++ -o parsemax parsemax.cpp

Run:

./parsemax tough.xml > tough.txt

./parsemax all.xml > all.txt

There are two elements to the processor.  The first interprets the maximum power for a given socket type via an OpenStreetMap tag.  The second combines the current result with the previous result to create a displayed maximum.  The constant "MAX_POWER_RESET" defines after how many lines to reset the second algorithm.  During normal operation, a reset will occur for each station.

The XML files were generated using Osmium, where "tough.xml" is a small selection of the world total "all.xml" with some added edge cases.  For example, to filter all charging stations in North America, run:

osmium tags-filter north-america-latest.osm.pbf amenity=charging_station -o north-america-latest.xml

The original eight PBF files - one per continent - were downloaded from Geofabrik.
