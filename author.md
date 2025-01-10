# Introduction

The Barchart Race is a program that emulates an animation of a Bar Chart over time, printing the chart for each period on
the standard output in a set interval, giving it the impression of being animated. 

The data source for the graph is given by the User, and options such as the number of bars and framerate can either 
be set by the User or, if it's not provided, set to a standard value.

# Author(s)

**Lucas Apolonio de Amorim (lucasapolonio@hotmail.com)**

**Moisés Ferreira de Lima (moisesaoaoao@gmail.com)**

# Problems found or limitations

# Grading



| Item                                                                                                       | Valor máximo | Valor esperado |
|------------------------------------------------------------------------------------------------------------|:------------:|:--------------:|
| Read, validate, and process command line arguments                                                         |      5       |       5        |
| Program runs with default values, if no options are provided                                               |      5       |       5        |
| Read the input data file and store the information in memory                                               |      10      |       10       |
| Display a summary of the data read from the input file _before_ the animation starts                       |      5       |       5        |
| Display a summary of the running options (set by the user or default values) _before_ the animation starts |      5       |       5        |
| The project contains at least two classes                                                                  |      10      |       10       |
| Create individual bar charts containing all data items for a single date                                   |      10      |       10       |
| Run the bar chart animation at the correct speed                                                           |      10      |       10       |
| The bar chart anatomy is complete and correct                                                              |      30      |       30       |
| Program handles errors properly                                                                            |      10      |       10       |

# Compiling and Running

```
cmake -S source -B build
cd ./build
make bcr
./bcr [options] <filepath>
```
or
```
cd ./source
g++ -lstdc++fs -std=c++17 -o ../build/bcr main.cpp bcr_am.cpp barchart.cpp libs/coms.cpp Database.cpp -lstdc++fs
cd ../build
./bcr [options] <filepath>
```