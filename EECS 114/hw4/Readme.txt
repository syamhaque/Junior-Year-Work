By: Mohammed Haque	ID: 62655407

editdist.c is the file that is meant to implement Edit Distance with dynamic programming and performs 4 operations: right, replace, delete, and insert with differing costs between them. This file will print the edit distance between two strings given to us in part 1 and 2, with part 1 printing a table as well that initially looks like this: 

Operation | Cost | Total | z
initial   |    0 |    0  | electrical engineering

This table will print each operation done, the cost of each corresponding operation, the total cost, and the string z as it is being transformed dynamically from string x to y. My program is well documented with comments along with some unused code that I believe would help in displaying my thought process as most of it is my initial attempt at designing a suitable data structure to perform all 4 operations in O(1) time.

The final cost I attained for each string conversion is printed after the table and is shown below: 
part 1 cost = 54
input 1 cost = 1816
input 2 cost = 1824
input 3 cost = 1829

In order to run my program you must type these commands into the shell in the following order:
1. make editdist
2. ./editdist

To access the typescript for this program, run the file labeled editdist_script. This script includes the above commands.