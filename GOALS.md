# Project: Looking at airline/flight data

Data source: https://www.transtats.bts.gov/Fields.asp?Table_ID=236

Columns used: Day of week, flight date, carrier code, origin/destination airport ID + 3 letter code, CRS departure/arrival time, airtime, distance

Month chosen for initial analysis: Jul 2019

Potential applications:
- Traversal (BFS)
- shortest path (given a function to minimize, like number of stopovers, total airtime, etc.)
- Airport importance (use a Page Rank like algorithm)
- Stretch goal: Detecting unmet demand (potentially using airport importance, distance, and number of direct flights)

Advantages: 
- Can run this analysis on any series from 1987 on the same website
- Highly flexible (over 100 columns that can be used for analysis, with options to only use a subset of columns)
- Dataset is in CSV format, which is easy to parse without use of external libraries.

Potential issues:
- If we use flights as edges, must handle multi-edges. We will be storing a single flight edge (depart airport, arrive airport, departure time, frequency) and we will have some multi-edges.
- Very large (several MB, several hundred thousand flights) dataset. Passing this into a database could take a lot of time,and search would require a large amount of time. We will store flight frequency instead of individual flights.
- The data is not 100% clean. While most important columns (flight time, etc.) have over 98% of fields filled in, the dataset is large enough that we would still have to handle incorrectly formatted data. Potential workarounds include more expensive parsing and potentially storing "cleaned" files for easier analysis. 

