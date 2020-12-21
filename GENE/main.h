// Warning : This code hasn't been implemented fully yet.

#ifndef RIBO_MAIN_FUNCTIONALITIES_
#define RIBO_MAIN_FUNCTIONALITIES_

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>

// GLOBAL DECLARATIONS   -------------------------------------------------
// declaring a global hash table for storing the faculty details
struct value_struct {
    bool empty = true;      // structs are empty by default
    string user_id;         // 0
    string pass;            // 1
    string first_name;      // 2 
    string last_name;       // 3
    char gender;            // 4
    int papers_published;   // 5
    string research_area;   // 6
    int no_of_citations;    // 7
    int overall_ranking;    // 8
    int experience;         // 9
    int salary_in_k;        // 10
};

// Create a global hash table array. 
const int init_size_hash_table = 30;
value_struct faculty_hash_table [init_size_hash_table];     
//---------------------------------------------------------------------------

// Check appropriate access of the user
bool greet_user();
bool check_access (string input_userid, string input_pass);

// Make a hash table above from provided input text file of employee details
void initialise_setup(string filename)

// Use a Universal hash function  with given key(UserID) and probe_no(in open addressing).
int generate_hash (string key, int probe_no);

// If user selected faculty option2, provide relevant functionalities.
void faculty_details_option2();

// If user selected genome option1, provide relevant functionalities.
void compare_genomes_option1();

// Iterating over whole DNA from fasta(.fa) file.
string get_genome (string filename);

// calculate similarity between two genes.
double find_similarity (string genome1, string genome2);