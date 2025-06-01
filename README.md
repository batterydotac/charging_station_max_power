# charging_station_max_power
--- Testing data for Organicmaps charging station maximum power processor ---

This repository contains the data used to test the charging station maximum power processor for Organicmaps as of 2025/05/31.

Compile:

g++ -o parsemax parsemax.cpp

Run:

./parsemax tough.xml > tough.txt

./parsemax all.xml > all.txt

There are two elements to the processor.  The first interprets the maximum power for a given socket type via an OpenStreetMap tag.  The second combines the previous result with the present result to create an accumulated maximum.  The constant "MAX_POWER_RESET" defines after how many lines to reset the second algorithm.  During normal operation, a reset will occur for each station, but stations may contain two or more socket types in some cases.  DC and AC values are treated separately.

The XML files were generated using Osmium, where "tough.xml" is a small selection of the world total "all.xml" with some added edge cases.  For example, to filter all charging stations in North America, run:

osmium tags-filter north-america-latest.osm.pbf amenity=charging_station -o north-america-latest.xml

Next, to filter the ":output" tags, run:

cat north-america-latest.xml | grep ":output" > north-america-latest-output.xml

Then all of these results can be combined:

cat *output.xml >> all.xml

The original eight PBF files - one per continent - were downloaded from Geofabrik.  Antarctica did not have any charging stations, so it was excluded.

The output files have six columns:
1. Tag
2. Value
3. Cleaned value
4. Extracted numeric value
5. DC or AC detection type
6. Accumulated result
