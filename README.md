# Aviation data as a graph

## Overview

The purpose of this project is to provide a flexible way to access aviation data within a graph, as well as performing certain operations. We have implemented three algorithms:
- A shortest path calculator that takes into account flight schedules, departure time, and connection time. This algorithm is based on BFS/ Dijkstra SSSP.

- A ranking for airports that takes into account flight traffic. We use a Markov chain and an algorithm similar to PageRank for ranking all the airports within a dataset.

- A "furthest airport" algorithm based on BFS. This algorithm takes a starting point and returns the airports that require the most stopovers to get to.

Our analysis is based on airline route data from the US Bureau of Transportation Statistics. It is available in monthly csv files, with data stretching as far back as 1987.

## File requirements

The file must be a .csv file with the following as its first line:
"DAY_OF_WEEK","FL_DATE","OP_UNIQUE_CARRIER","ORIGIN_AIRPORT_ID","ORIGIN","DEST_AIRPORT_ID","DEST","CRS_DEP_TIME","CRS_ARR_TIME","AIR_TIME","DISTANCE",

To obtain files not included in the repo, visit:
https://www.transtats.bts.gov/Fields.asp?Table_ID=236

When downloading the data, choose the following columns ONLY:
- DayOfWeek
- FlightDate
- Reporting_Airline
- OriginAirportID
- Origin
- DestAirportID
- Dest
- CRSDepTime
- CRSArrTime
- AirTime
- Distance


The selection order of the columns should not matter, and the data should have the columns in the correct order when you press download. The program should work with any correctly formatted file from this source, regardless of the date of the file.

## Compiling and running

For optimal performance, the code should be compiled on EWS machines using the following steps:

1. Open a terminal.
2. Navigate to the root folder of the repo (this is also the folder that README.md is in by default).
3. Type the following instructions:
    - module load llvm/6.0.1
    - make

The code may compile on other systems and using other methods. However, this has not been tested, so EWS with the steps above is recommended.

To run the program, simply type the following command into the terminal. You must be in the root directory of the repo for this to work.
- ./main

Sample instructions:

[Mate Terminal: home] cd [repoPath] 

[Mate Terminal: nahmad31-ksodhi2-jli173-tommaso3] module load llvm/6.0.1

[Mate Terminal: nahmad31-ksodhi2-jli173-tommaso3] make

[Mate Terminal: nahmad31-ksodhi2-jli173-tommaso3] ./main


Note: [repoPath] is the path to the repo, i.e "Documents/nahmad31-ksodhi2-jli173-tommaso3/"

## How to use the program

There are two ways of running the program: automatic mode and manual mode. In automatic mode, the user passes in an instruction as command line arguments, and the program runs with no further input. In manual mode, the program takes input from the terminal and executes instructions.

### How to use the command line arguments

The first command line argument passed is required to be the data file that you wish to analyze. This parameter is also available in manual mode, in which case the program will not ask for a file from terminal.

Example:

[Terminal] ./main

This will print "File Name: " to the console, at which point you should enter the desired filepath and file (including file extension.)

[Terminal] ./main data/july-2019-data.csv

This will automatically select the data/july-2019-data.csv file and run in manual mode.

Note: If you fail to provide a valid filepath, the program will quit automatically.

When running in manual mode, you may not provide any other arguments, but the filepath parameter is optional.

When running in automatic mode, the filepath parameter is required. The second parameter must be "-a", and the third and final parameter must be a valid command from the list below.

Example:

[Terminal] ./main file.csv -a rank

This will automatically run the rank command on the file "file.csv".

[Terminal] ./main file.csv -a

This is an invalid input, and the program will exit.

### How to use commands

The commands are the same in automatic and manual mode. In automatic mode, you should provide a single command in the command line arguments. In manual mode, you can provide as many commands as you wish, but you must enter them one at a time.

Here is a complete list of commands:
- quit: quits the program.
- help: Displays a list of commands and how to use them.
- rank: Ranks airports and prints the results to the console..
- shortestpath [X] [Y] [start] [connection]: returns shortest path from X to Y. Arguments X and Y are required.
  Start and connection are optional. Connection is the minimum connection time (in minutes, must be an integer)
  Start is the starting time given as an hhmm string. (ie 1340 is 13:40 or 1:40 PM).
  If one of [start, connection] is provided, both must be provided. You cannot provide just one.
  The result will be printed to the console.
 
- furthest [X]: return the furthest airports from X. This algorithm counts using number of stopovers.

Example:

[Terminal] ./main

Enter filepath: file.csv

Enter command: furthest ABC

The furthest airports from ABC are: XYZ. The number of flights required is 3.

Enter command: quit

[Terminal]

In manual mode, the console will keep asking for commands until "quit" is called, at which point it terminates.
