# About the Author of This Repo
* Name: Keyvan Noury
* Email address: noury@usc.edu

# Table of Contents
1. [Problem](README.md#problem)
2. [Task](README.md#task)
3. [Input Dataset](README.md#input-dataset)
4. [Instructions](README.md#instructions)
5. [Output](README.md#output)


# Problem
A newspaper editor was researching immigration data trends on H1B(H-1B, H-1B1, E-3) visa application processing over the past years, 
trying to identify the occupations and states with the most number of approved H1B visas. She has found statistics available from 
the US Department of Labor and its [Office of Foreign Labor Certification Performance Data](https://www.foreignlaborcert.doleta.gov/performancedata.cfm#dis). 
But while there are ready-made reports for [2018](https://www.foreignlaborcert.doleta.gov/pdf/PerformanceData/2018/H-1B_Selected_Statistics_FY2018_Q4.pdf) 
and [2017](https://www.foreignlaborcert.doleta.gov/pdf/PerformanceData/2017/H-1B_Selected_Statistics_FY2017.pdf), 
the site doesn’t have them for past years. 

# Task
As a data engineer, we are asked to create a mechanism to analyze past years data, 
specificially calculate two metrics: **Top 10 Occupations** and **Top 10 States** for **certified** visa applications.

# Input Dataset
**Note:** Each year of data can have different columns. So I analyzed the column names prior to make decisions on the counts.

# Instructions
***The objective is to analyze the input file of h1b applications and create 2 output files

# Output 
The program creates 2 output files in the folder of ../output:
* `top_10_occupations.txt`: Top 10 occupations for certified visa applications
* `top_10_states.txt`: Top 10 states for certified visa applications

Each line holds one record and each field on each line is separated by a semicolon (;).

Each line of the `top_10_occupations.txt` file contains these fields in this order:
1. __`TOP_OCCUPATIONS`__: Use the occupation name associated with an application's Standard Occupational Classification (SOC) code
2. __`NUMBER_CERTIFIED_APPLICATIONS`__: Number of applications that have been certified for that occupation. An application is considered certified if it has a case status of `Certified`
3. __`PERCENTAGE`__: % of applications that have been certified for that occupation compared to total number of certified applications regardless of occupation. 

The records in the file are sorted by __`NUMBER_CERTIFIED_APPLICATIONS`__, and in case of a tie, alphabetically by __`TOP_OCCUPATIONS`__.

Each line of the `top_10_states.txt` file contains these fields in this order:
1. __`TOP_STATES`__: State where the work will take place
2. __`NUMBER_CERTIFIED_APPLICATIONS`__: Number of applications that have been certified for work in that state. An application is considered certified if it has a case status of `Certified`
3. __`PERCENTAGE`__: % of applications that have been certified in that state compared to total number of certified applications regardless of state.

The records in this file are sorted by __`NUMBER_CERTIFIED_APPLICATIONS`__ field, and in case of a tie, alphabetically by __`TOP_STATES`__. 

Depending on the input (e.g., see the example below), there may be fewer than 10 lines in each file. There, however, is not more than 10 lines in each file. 
In case of ties, only list the top 10 based are sorted based on the instructions.

Percentages are rounded off to 1 decimal place. For instance, 1.05% is rounded to 1.1%.

Solution safisfies the following requirements:
* Repo follows the required repo directory structure
* `run.sh` script works as is in our environment and correct results are generated.
* The code is well-commented
* `README.md` contains Problem, Approach and Run instructions sections

I used C++ to code for the given problem and task.
a link of my Github or Bitbucket repo with my source code was sent to the insight website.


## Repo directory structure
The directory structure for my repo looks like this:
```
      ├── README.md 
      ├── run.sh
      ├── src
      │   └──h1b_counting.cpp
      ├── input
      │   └──h1b_input.csv
      ├── output
      |   └── top_10_occupations.txt
      |   └── top_10_states.txt
      ├── insight_testsuite
          └── run_tests.sh
          └── tests
              └── test_1
                  ├── input
                  │   └── h1b_input.csv
                  └── output
                      └── top_10_occupations.txt
                      └── top_10_states.txt
              
```
The content of `src` does not need to be a single file called `h1b-counting.cpp`. But here I used that as my program's name.

## Testing directory structure and output format

To make sure that the code has the correct directory structure and the format of the output files are correct, there is a test script called `run_tests.sh` in the `insight_testsuite` folder.

The tests files are stored in `.csv` format under the `insight_testsuite/tests` folder. Each test should have a separate folder with an `input` folder and `h1b_input.csv` file and an `output` folder with the two requested output files.

You can run the test with the following command from within the `insight_testsuite` folder:

    insight_testsuite~$ ./run_tests.sh 

On a failed test, the output of `run_tests.sh` should look like:

    [FAIL]: test_1
    [Thu Mar 30 16:28:01 PDT 2017] 0 of 1 tests passed

On success:

    [PASS]: test_1
    [Thu Mar 30 16:25:57 PDT 2017] 1 of 1 tests passed

