# GeneEnterpriseSoftware
Improves the research techniques and provides a single portal to search and update 
employee details.
Note - Command line interface of the application software is made intuitive, user need
not extensively consult README file to run and test the software.

# Compile and run the main program 

```
   $ g++ -std=c++11 main.cpp -o main
   $ ./main
```
========================================================================

# Step-0 -> Provide faculty size initially to run the application (For testing purposes).
## Usage Example (C++)

```
	Hi awesome user! Kindly enter size to test heaps and hashtable functionality of 
	my application software - GENE: More information follows :D

    1. 10 employee structs?
	2. 500 employees structs?
	3. 1000 employees structs?
	4. 1500 employees structs?
	5. 2000 employees structs?
	
	Choice number: ENTER HERE ! (Choose from {1,2..5})
```
NOTE - This is for TESTING Heaps functionality for different faculty sizes. User need not
input the whole path to .txt test file to perform testing. System will automatically
retrieve the file of correct size. This way testing could be performed on different 
heap sizes. Please refer to f2-test/ folder to find these details.
  
========================================================================
  
# STEP-1 -> Provide your name 
## Usage Example (C++)

```
	Hi! My name is GENE.
	Please enter your name: ENTER HERE! 
``` 

==========================================================================

# STEP-2 ->  Authentication
Provide your user name and password to access the system. 

```
Please enter your UserID : ENTER HERE!
Enter password : ENTER HERE!
```
Sample parameters:
* `kaurjas` : UserID
* `1234` : Password

*WARNING* - User ID and Password will ONLY be considered valid iff they are taken from the correct
file. Example - If size selected in STEP-0 (above) is 2000, then refer to `f2-test/faculty2000.txt`
and input any UserID and Password. Similarly, if size selected in STEP-0 is 500 instead, then 
refer to `f2-test/faculty500.txt` and choose any userID and Password. All of them are VALID. 
For testing purposes, try different usernames and passwords from that file to authenticate.

Note - Try adding a wrong USERNAME or PASSWORD to interpret the output.
Note - Attributes in `facultyXXXX.txt` of each employee are in order as follows :-
> Username Pass FirstName LastName Gender PapersPublished ResearchArea NoCitations OverallRanking Experience Salary(in $1000s)

===========================================================================

# STEP-3 -> Choose option based on functionalities currently offered.
## Usage Example 

```
Choose one option:
1. Compare DNA genomes of two species
2. Faculty details
 Choice number: ENTER HERE (1 or 2)!
```

============================================================================

# STEP-4 -> If chosen Option-1. 
If the option is chosen to compare faculty details, input two fasta(.fa) files
separated by a *space*.

## Usage Example

```
Input two .fa files to compare:
ENTER_HERE ENTER_HERE

``` 
Sample parameters:
* `human_100,000.fa`
* `mouse_100,000.fa`
 
NOTE - Choose any two .fa files present in `f1-test/` folder.

=============================================================================

# STEP-5 -> If chosen Option-2

## Usage Example as STEP-3

```
Choose one option:
1. Compare DNA genomes of two species
2. Faculty details
 Choice number: ENTER HERE (1 or 2)!
```
--------------------------------------------------------------------------------
If chosen Option-1, enter the first and last name (starting with capital letters) to search
for. 
Sample parameters:
* `Jasmeen Kaur`

*Warning SAME AS STEP-2* - Kindly input first and last name from the same file name as the 
selected size initially. Example- if initially selected size is 2000 (in step-0), then 
enter any first and last name from `f2-test/faculty2000.txt`. Similarly if size selected 
is equal to 10 (in step-0), then enter any name from `f2-test/faculty10.txt`.

---------------------------------------------------------------------------------
If chosen Option-2, enter the choice number of relevant criteria as follows:-

## Usage Example

```
Based on which criterion, should I perform ranking of scientists? : 
1. Overall ranking in the firm
2. Number of scientific papers published
3. Total number of citations
4. Experience
5. Annual Salary (in $1000)

Choice number: ENTER HERE (Choose from {1,2..5})
``` 

Then enter the number of scientists need to be displayed as follows:-

## Usage Example
```
Done! How many scientists should be streamed on 
terminal output (in decreasing order) ? ENTER HERE (3 or 5 or 10 ...)
``` 
====================================================================================

*IMPORTANT NOTE* - In the last line, time taken to implement HEAP functionality (based on initial 
selected size (in step-0) is shown). Different tests on HEAPS could be performed by 
selecting different faculty sizes initially. This is test for FUNCTIONALITY - HEAPS. 

For FUNCTIONALITY - DYNAMIC PROGRAMMING, the time taken to calculate similarity is outputted
just before the result. Different files present in `f1-test/` folder can be selected
to perform robust testing.

For FUNCTIONALITY - HASH TABLES, for testing different faculty sizes could be selected (as in Step-0) to 
test for different employee hash tables. Note that there are TWO hash tables in GENE, both with 
different keys. One used in STEP-5.1, and other in STEP-2. 

NOTE - A script of how I generated test cases has been provided in `f2-test/` folder.  


=================================================================================

Author
======
Jasmeen Kaur
Student
Australian National University 
UID - 6871990
