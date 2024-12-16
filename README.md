# ExpenseTrackerCLI

How to use:
  1. Download the repository
  2. Move it to desired folder 
  3. Run "make" command and wait for compilation
  4. Use all the commands below

ATTENTION! DO NOT CHANGE ANYTHING INSIDE MAKEFILE
 
A simple expense tracker CLI which saves and prints the expenses

The list of commands and their expected output is shown below:

$ ./exptracker add --description "Lunch" --amount 20


$ ./exptracker add --description "Dinner" --amount 10


$ ./exptracker list


$ ./exptracker update --id 1 --description "lunch" --amount 40


$ ./exptracker summary


$ ./exptracker delete --id 2


$ ./exptracker summary


$ ./exptracker summary --month 8


https://roadmap.sh/projects/expense-tracker
