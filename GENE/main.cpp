#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include <tuple>
#include <math.h>

using namespace std;
using namespace std::chrono;

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

// Create a hash table for checking access of the user. 
const int init_size_hash_table = 5000;   
value_struct faculty_hash_table [init_size_hash_table]; 
value_struct table_for_search [init_size_hash_table]; 

// Create a heap to search the scientists by ranking 
const int faculty_size = 5000;    
value_struct heap_structs[faculty_size];
int heap_size = 0; 

string faculty_filename;   

// Variables used in generate_hash() function.
const int large_prime = 1046527;  
const int a = rand() % large_prime;
const int b = rand() % large_prime;

//Times taken for 3 functionalities
// HEAP
double time_heap = 0.0;
// DYNAMIC PROGRAMMING - shown to user in the output 
// HASHTABLE - Access time is O(1), computation is mainly in generate_hash() function
//             which can vary on different computer systems.

// -----------------------------------------------------------------------

/**
 * This function is extracting genome from given filename
 * and returning a string appended with the whole genome. 
 * @param : input file
 * @return : Genome string
 */
string get_genome (string filename) {
    //Put the filename in the terminal object
    ifstream FileObj (filename);
    if (!FileObj.good()) {    //checks the flags for i/o errors 
        cerr << "Error opening '" << filename << "'. Bailing out." << endl;
        return "";
    }
    
    string line, name, content; 
    while (getline(FileObj, line).good()) {
        if (line.empty() || line[0] == '>') {   // identifier marker
            if (!name.empty()) {
                name.clear();
            }
            if (!line.empty()) {
                name = line.substr(1);
            }
            content.clear();
        } else if (!name.empty()) {
            if (line.find (' ') != string::npos) {   // invalid sequence no spaces allowed
               name.clear();
               content.clear(); 
            } else {
                content += line;
            }
        }
    }
    return content; 
}

/**
 * Generate a UNIVERSAL HASH with given key and probe_no in OPEN ADDRESSING. 
 * For generating UNIVERSAL HASH, I have choosen the universal hash family - [(a*key + b) mod p] mod m.
 * 
 * @param key : User_ID of the employee
 * @param probe_no : Counter for number of probes made in Open-addressing hashing 
 * @return : hash index 
 */
int generate_hash (string key, int probe_no) {
    int final_sum = 0;
    // iterate over the string key characters
    for (int i = 0; i < key.length(); i++) {
        char x = key.at(i);
        final_sum += int(x);
    }
    final_sum += probe_no;

    // generate a universal hash of the form : [(a*key + b) mod p] mod m
    int final_hash = ((a*final_sum + b) % large_prime) % init_size_hash_table;
    return final_hash;
}


/**
 * Find the longest common subsequence between given two strings 
 * @param genome1 : Input DNA string
 * @param genome2 : Input DNA string
 * 
 * @return : Length of Longest Common Subsequence (LCS)
 */

double find_longest_common_Subsequence (string genome1, int genome1_size, string genome2, int genome2_size, int gap) {
    // Create a 2-d array of size (2, size of smaller string)
    const int smaller_genome_size = min(genome1_size, genome2_size);
    int *first_row{new int[smaller_genome_size+1]{}};      // make an array with size 1 larger than the smaller one.  
    int *second_row{new int[smaller_genome_size+1]{}};      // similarly make the second array initialised with 0. 

    // determine the string with which comparison is to be made.
    string comparison_string, smaller_string = "";
    if (genome1_size < genome2_size) {
        comparison_string = genome2;
        smaller_string = genome1; 
    }
    else {
        comparison_string = genome1; 
        smaller_string = genome2;
    } 

    for (int i = 0; i < comparison_string.length(); i+=gap) {     // iterate over larger string. 
        int copy_pointer = 0; 
        for (int j = gap; j < smaller_genome_size+1; j+=gap) {   // iterate over the second row 
            // if characters are equal, then add 1 to the diagonally previous element
            if (j >= 3) {
                first_row[copy_pointer] = second_row[copy_pointer];
                copy_pointer+=gap;
            }
            if (comparison_string.at(i) == smaller_string.at(j-gap)) {    // compare the two strings
                second_row[j] = first_row[j-gap] + 1;      // fill the second row with the help of first row
            } else {             // else choose the maximum of above and left element
                second_row[j] = max (first_row[j], second_row[j-gap]);      
            }
        }
        // when one iteration is over, copy the rest of the remaining elements in the array
        for (int k = copy_pointer-gap; k <= smaller_genome_size; k+=gap) {
            first_row[k] = second_row[k];
        }                    
    }   
    // finally the result will be the last element. 
    int ans = second_row[smaller_genome_size];

    // delete the dyamically allocated heap memory 
    delete[] first_row;
    delete[] second_row; 
    return ans; 
}

/**
 * This will calculate similarity between two different genes by finding the 
 * longest common subsequence using dynamic programming.
 * @param genome1 : genome string of first species
 * @param genome2 : genome string of second species
 * 
 * @return : % similarity of genes among two species.
 */
tuple<double, int> find_similarity (string genome1, int genome1_size, string genome2, int genome2_size, int gap) {  
    auto start = high_resolution_clock::now();
    double longest_len = find_longest_common_Subsequence (genome1, genome1_size, genome2, genome2_size, gap);
    double similarity = ((double(longest_len)*gap) / double((genome1_size+genome2_size)/2)) * 100;   
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end-start);
    cout << "Time taken to calculate similarity: " << duration.count() << " seconds" << endl;  
    return make_tuple(similarity, longest_len);     
}

/**
 * If user selected the first option of comparing genomes.
 * 
 * @input : Take two .fa files as input
 * @output : % similarity and accompanying results.
 */

void compare_genomes_option1() {
    // extract the genome string from fasta file
    cout << endl << "Input two .fa files to compare: " << endl;
    string genome1, genome2; 
    cin >> genome1 >> genome2 ;

    string genome_mouse = get_genome ("f1-test/" + genome1);
    string genome_human = get_genome ("f1-test/" + genome2);

    //Calculate size of genome_mouse
    int genome_size_mouse = 0; 
    for (auto e : genome_mouse) {
        genome_size_mouse++; 
    }

    //Calculate size of genome_human
    int genome_size_human = 0; 
    for (auto e : genome_human) {
        genome_size_human++; 
    }
    cout << endl;
    cout << "*************************************" << endl << endl;
    cout << "Total number of nucleotides in first genome subsequence: " << genome_size_mouse << endl;
    cout << "Total number of nucleotides in second genome subsequence: " << genome_size_human<< endl << endl;  
    cout << "*************************************" << endl;

    // changing the gap value with increase in sizes, this will lead to decrease in precision
    // but is also scaled up almost proportionally - to have the results in around 30 seconds.
    int gap =  0;
    if (max (genome_size_human, genome_size_mouse) <= 10000) gap = 1;
    else if (max (genome_size_human, genome_size_mouse) <= 100000) gap = 5;
    else if (max (genome_size_human, genome_size_mouse) <= 1000000) {
        // Print the similarity of the genomes using Longest Common Subsequence 
        cout << endl;
        cout << "Take a stretch while I am working on it... " << '\n'; 
        cout << "Hope to give answer back to you in approx. 30 seconds :D" << '\n';
        cout << endl;   
        gap = 25; 
    } 
    else if (max (genome_size_human, genome_size_mouse) > 1000000) {
        cout << endl;
        cout << "Take a stretch while I am working on it... " << '\n'; 
        cout << "Hope to give answer back to you in approx. 30 seconds :D" << '\n';
        cout << endl;  
        gap = 50;
    } 

    // print the results properly
    tuple<double,int> ans = find_similarity (genome_human, genome_size_human, genome_mouse, genome_size_mouse, gap);
    int different_dna_size = ((genome_size_human + genome_size_mouse)/2) / gap;
    cout << "--------------------------------------------" << endl << endl;   
    cout << "Length of Longest Common subsequence found: " << get<1>(ans) << " out of " << different_dna_size << " different nucleotide sequences."<< endl;
    cout << "Calculated similarity: "<< get<0>(ans) << "%" << endl << endl;
    cout << "--------------------------------------------" << endl;
}

/**
 * Calculate the parent of the current node at index i in the HEAP.
 * 
 * @param i : current node/index.
 * 
 * @return : parent of the given node/index. 
 */
int parent(int i) {
    return floor(i/2);
}

/**
 * Insert a employee struct into the heap. After insertion, perfrom HEAPIFY.
 * 
 * @param rec : Employee struct which needs to be inserted into the heap followed by heapification. 
 * @param ranking_choice : key on which comparison in the heap will be made. 
 * 
 * @return : void
*/
void heap_insert(value_struct rec, int ranking_choice) {
    heap_structs[heap_size] = rec;   // insert the new record
    bool compare_parent = false;
    int i = heap_size; 
    // if just one element then insert it!
    if (i == 0) {
        heap_size += 1;
    }

    if (ranking_choice == 5) compare_parent = heap_structs[parent(i)].papers_published < heap_structs[i].papers_published;
    else if (ranking_choice == 7) compare_parent = heap_structs[parent(i)].no_of_citations < heap_structs[i].no_of_citations;
    else if (ranking_choice == 8) compare_parent = heap_structs[parent(i)].overall_ranking < heap_structs[i].overall_ranking;
    else if (ranking_choice == 9) compare_parent = heap_structs[parent(i)].experience < heap_structs[i].experience;
    else if (ranking_choice == 10) compare_parent = heap_structs[parent(i)].salary_in_k < heap_structs[i].salary_in_k;

    while ((i >= 0) && (compare_parent) && !(heap_structs[i].empty)) {
        swap(heap_structs[parent(i)], heap_structs[i]);
        i = parent(i);
        if (ranking_choice == 5) compare_parent = heap_structs[parent(i)].papers_published < heap_structs[i].papers_published;
        else if (ranking_choice == 7) compare_parent = heap_structs[parent(i)].no_of_citations < heap_structs[i].no_of_citations;
        else if (ranking_choice == 8) compare_parent = heap_structs[parent(i)].overall_ranking < heap_structs[i].overall_ranking;
        else if (ranking_choice == 9) compare_parent = heap_structs[parent(i)].experience < heap_structs[i].experience;
        else if (ranking_choice == 10) compare_parent = heap_structs[parent(i)].salary_in_k < heap_structs[i].salary_in_k;
    }
    heap_size += 1;    // increase the heap size by 1.
}

/**
 * If user selected the second option of accessing faculty details. 
 * 
 */
void faculty_details_option2() {
    char search_again = 'y'; 
    while (search_again == 'y') {    
        cout << "Choose one of the options below:- " << endl;
        cout << "1. Search details of any employee across whole database" << endl;
        cout << "2. Search employees by a ranking criteria" << endl; 
        int chosen_option = 0; 
        cout << endl << "Choice number: "; 
        cin >> chosen_option; 

        if (chosen_option == 1) {
            // take the initials 
            cout << endl << "Kindly enter full name of the employee to search for:-" << endl<< endl;
            cout << "==========================================================" << endl;
            cout << "WARNING - FIRST AND LAST NAME START WITH UPPERCASE LETTERS"  << endl;
            cout << "==========================================================" << endl;
            cout << endl << "Search: "; 
            string firstname, lastname = ""; 
            cin >> firstname >> lastname; 

            // search in the hash table 
            int probe_no = 0; 
            bool found_flag = false;
            int potential_index = generate_hash (firstname+lastname, probe_no);    // This is O(1) complexity if not too much crowding.

            while (!(table_for_search [potential_index].empty) && 
                    (probe_no != init_size_hash_table)) {     // until non-empty structs
                if ((table_for_search [potential_index].first_name == firstname) && 
                    (table_for_search [potential_index].last_name == lastname)) {
                    // print details of employee
                    value_struct result = table_for_search [potential_index]; 
                    cout << endl << "=============Employee details=============" << endl << endl;
                    cout << "First Name: " << result.first_name << endl;
                    cout << "Last Name: " << result.last_name << endl;
                    cout << "Overall Ranking in the firm: " << result.overall_ranking << endl;
                    cout << "Gender: " << result.gender << endl;
                    cout << "Number of Papers published: " << result.papers_published << endl;
                    cout << "Major Research Area: " << result.research_area << endl;
                    cout << "Total number of citations: " << result.no_of_citations << endl;
                    cout << "Research Experience: " << result.experience << " years" << endl;
                    cout << "Annual salary (in $1000): " << result.salary_in_k << endl << endl;
                    cout << "============================================" << endl << endl;
                    found_flag = true; 
                    break;
                } else probe_no++;      // otherwise increment the index
                potential_index = generate_hash (firstname+lastname, probe_no);
            }
            if (!found_flag) cout << "Err.... sorry! Details of given employee does not exist." << endl;
        }

        value_struct init_user_data;
        init_user_data.empty = true;
        init_user_data.user_id = "a"; 
        init_user_data.pass = "a"; 
        init_user_data.first_name = "a"; 
        init_user_data.last_name = "a"; 
        init_user_data.gender = 'a';         
        init_user_data.papers_published = 0;      
        init_user_data.research_area = "a"; 
        init_user_data.no_of_citations = 0; 
        init_user_data.overall_ranking = 0; 
        init_user_data.experience = 0; 
        init_user_data.salary_in_k = 0; 

        // MAX-HEAP
        if (chosen_option == 2) {        
            cout << endl << "Based on which criterion, should I perform ranking of scientists? : " << endl << endl;
            cout << "1. Overall ranking in the firm" << endl;
            cout << "2. Number of scientific papers published" << endl;
            cout << "3. Total number of citations" << endl;
            cout << "4. Experience" << endl;
            cout << "5. Annual Salary (in $1000)" << endl << endl;
            int ranking_choice = 0; 
            cout << endl << "Choice number: "; 
            cin >> ranking_choice; 

            // now find the index in the vector based on ranking and change the current ranking choice. 
            if (ranking_choice == 1) ranking_choice = 8; 
            else if (ranking_choice == 2) ranking_choice = 5;
            else if (ranking_choice == 3) ranking_choice = 7;
            else if (ranking_choice == 4) ranking_choice = 9;
            else if (ranking_choice == 5) ranking_choice = 10;
 
            ifstream FileObj(faculty_filename);
            if (!FileObj.good()) {    //checks the flags for i/o errors 
                cerr << "Error opening private file'" << faculty_filename << "'. Bailing out." << endl;
            }

            // take input from the text file
            string line;    // receive input from the text file here. 
            while (getline(FileObj, line).good()) {
                if (line.empty() || line[0] == '>') {
                    line.clear();       // clear the line and do not store anything 
                } else {
                    istringstream iss(line);
                    string buf;    // buffer string
                    vector<string> users;    

                    while (iss >> buf) {
                        users.push_back(buf);    // push the attribute to the users vector
                    }

                    value_struct curr_rec; 
                    curr_rec.empty = false;
                    curr_rec.user_id = users.at(0); 
                    curr_rec.pass = users.at(1); 
                    curr_rec.first_name = users.at(2); 
                    curr_rec.last_name = users.at(3); 
                    curr_rec.gender = users.at(4)[0];                             
                    curr_rec.papers_published = stoi(users.at(5));      
                    curr_rec.research_area = users.at(6); 
                    curr_rec.no_of_citations = stoi(users.at(7)); 
                    curr_rec.overall_ranking = stoi(users.at(8)); 
                    curr_rec.experience = stoi(users.at(9)); 
                    curr_rec.salary_in_k = stoi(users.at(10)); 

                    // now store this record using heap insert in the heap one by one
                    heap_insert(curr_rec, ranking_choice); 
                } 
            }

            // now after heap completion, ask user how many scientists to show 
            cout << endl << "Done! How many scientists should be streamed on terminal output (in decreasing order) ?"; 
            int k = 0; cin >> k; 
            
            cout << endl;
            cout << "******** EMPLOYEES ARE RANKED AS PER CHOOSEN CRITERIA IN DECREASING ORDER *******" << endl;

            // print the top k scientists details in O(1) time.
            for (int i = 0; (i < k) && (i < heap_size); i++) {
                cout << endl << "=============Employee details-" << i+1 << "=============" << endl << endl;
                cout << "First Name: " << heap_structs[i].first_name << endl;
                cout << "Last Name: " << heap_structs[i].last_name << endl;
                cout << "Overall Ranking in the firm: " << heap_structs[i].overall_ranking << endl;
                cout << "Gender: " << heap_structs[i].gender << endl;
                cout << "Number of Papers published: " << heap_structs[i].papers_published << endl;
                cout << "Major Research Area: " << heap_structs[i].research_area << endl;
                cout << "Total number of citations: " << heap_structs[i].no_of_citations << endl;
                cout << "Research Experience: " << heap_structs[i].experience << " years" << endl;
                cout << "Annual salary (in $1000): " << heap_structs[i].salary_in_k << endl << endl;
                cout << "============================================" << endl << endl;
            }
            break;            
        }
        cout << endl << "--------------------------------------------------------------" << endl;         
        cout << "Total time taken to implement search by ranking functionality: " << time_heap << " milliseconds" << endl;   
        cout << "--------------------------------------------------------------" << endl << endl;
        cout << "Search Again? PRESS y or n: ";
        cin >> search_again;
        cout << endl << endl;
    }
    cout << endl << "--------------------------------------------------------------" << endl;         
    cout << "Total time taken to implement search by ranking functionality: " << time_heap << " milliseconds" << endl;   
    cout << "--------------------------------------------------------------" << endl << endl;
    cout << "Search Again? PRESS y or n: ";
    cin >> search_again;
    cout << "Exiting..." << endl;
}

/**
 * Check if current user is allowed to access the system. 
 * 
 * @param userid : userID of current user
 * @param pass : password to match in hash table
 * @return : True if granted_access; else False
 */
bool check_access (string input_userid, string input_pass) {
    // create a hashtable of all the allowed scientists, user names, passwords and their details. 
    // now we need to check if the inputted password is matching the one in the hashtable
    int probe_no = 0; 
    int potential_index = generate_hash (input_userid, probe_no);    // This is O(1) complexity if not too much crowding.

    while (!(faculty_hash_table [potential_index].empty) && 
            (probe_no != init_size_hash_table)) {     // until non-empty structs
        if ((faculty_hash_table [potential_index].user_id == input_userid) 
                && (faculty_hash_table [potential_index].pass == input_pass)) {
            return true;     
        } else probe_no++;      // otherwise increment the index
        potential_index = generate_hash (input_userid, probe_no);
    }
    return false;        // as we come out of the while loop return false.
}

/**
 * Make a hash table above with the provided input text file and
 * insert all the structs/record one by one. After completion this hashtable 
 * could be searched inside. NOTE - Also need to implement deletion and 
 * search of the hash table.
 * 
 * @param filename : .txt file containing employee details
 * @return : Total number of lines/users in the text file.  
 */
void initialise_setup(string filename) {
    ifstream FileObj(filename);
    if (!FileObj.good()) {    //checks the flags for i/o errors 
        cerr << "Error opening private file'" << filename << "'. Bailing out." << endl;
    }

    //initialise the hash table created globally with empty=true initially 
    value_struct init_user_data;
    init_user_data.empty = true;
    init_user_data.user_id = "a"; 
    init_user_data.pass = "a"; 
    init_user_data.first_name = "a"; 
    init_user_data.last_name = "a"; 
    init_user_data.gender = 'a';         // this is [] overloading - getting the first char of a string
    init_user_data.papers_published = 0;      // converting string to integer
    init_user_data.research_area = "a"; 
    init_user_data.no_of_citations = 0; 
    init_user_data.overall_ranking = 0; 
    init_user_data.experience = 0; 
    init_user_data.salary_in_k = 0; 

    // Initialise faculty hash table 
    for (int i = 0; i < init_size_hash_table; i++) {
        faculty_hash_table[i] = init_user_data;   
    }

    // Initialise search hash table 
    for (int i = 0; i < init_size_hash_table; i++) {
        table_for_search[i] = init_user_data;   
    }

    string line;
    int counter_lines = 0;      // count the total number of lines/users in the txt file

    while (getline(FileObj, line).good()) {
        counter_lines++;     // increment the counter by 1 each time the line is encoutered.
        if (line.empty() || line[0] == '>') {
            line.clear();       // clear the line and do not store anything 
        }
        else {
            // Store the splitted words of a single user in the vector - users 
            istringstream iss(line);
            string buf;    
            vector<string> users;    
            while (iss >> buf) {
                users.push_back(buf);    // push the attribute to the users vector
            }
            string key = users.at(0);   // username is the key

            value_struct curr_user_satellite_data;

            // FIRST HASHTABLE FOR CHECKING ACCESS. -----------------------------
            int probe_no_1 = 0;     // probes are 0 initially
            while (probe_no_1 != init_size_hash_table) {    // no of probes reach the size of the hash table 
                int hash_uname = generate_hash (key, probe_no_1);
                if (faculty_hash_table [hash_uname].empty == true) {    
                    curr_user_satellite_data.empty = false;
                    curr_user_satellite_data.user_id = users.at(0); 
                    curr_user_satellite_data.pass = users.at(1); 
                    curr_user_satellite_data.first_name = users.at(2); 
                    curr_user_satellite_data.last_name = users.at(3); 
                    curr_user_satellite_data.gender = users.at(4)[0];                          
                    curr_user_satellite_data.papers_published = stoi(users.at(5));      
                    curr_user_satellite_data.research_area = users.at(6); 
                    curr_user_satellite_data.no_of_citations = stoi(users.at(7)); 
                    curr_user_satellite_data.overall_ranking = stoi(users.at(8)); 
                    curr_user_satellite_data.experience = stoi(users.at(9)); 
                    curr_user_satellite_data.salary_in_k = stoi(users.at(10)); 

                    // assign the above struct to that index in the hash table
                    faculty_hash_table [hash_uname] = curr_user_satellite_data;     
                    break;
                } else {      // it not empty then do linear probing and try insert in the next position. 
                    probe_no_1++;   
                }
            }
            // fill the search table - SECOND HASHTABLE FOR SEARCHING. -------------------------
            int probe_no_2 = 0; 
            while (probe_no_2 != init_size_hash_table) {
                int name_hash = generate_hash(curr_user_satellite_data.first_name + curr_user_satellite_data.last_name, probe_no_2);
               
                if (table_for_search[name_hash].empty == true) {
                    table_for_search [name_hash] = curr_user_satellite_data;
                    break; 
                } else {
                    probe_no_2++;
                }
            }
        }
    }
}

/**
 * Greet users by checking access
 * 
 * @return : True, if user was greeted and had access of system; Else False.
 */
bool greet_user() {
    //GREET USER
    cout << "Hi! My name is GENE.\nPlease enter your name: ";
    string user, userID, pass; 
    getline(cin,user); 
    cout << endl << "Nice to meet you " << user << "!" << endl;
    cout << "I am an enterprise application software for undertaking research practices by Ribo Biotechnology company, Australia." << endl;
    cout << "Since I hold confidential details of working scientists and undergoing research in the company," << endl;
    cout << "kindly confirm if you've been provided with access regarding accessing this system. " << endl;
    cout << endl << "Please enter your UserID : "; 
    cin >> userID; 
    cout << "Enter password : ";
    cin >> pass;

    // ACCESS DENIED
    if (!check_access(userID, pass)) {
        cout << "Errr.. Sorry your UserID or password does not match. "<< endl;
        return false; 
    }
    //ACCESS GRANTED
    cout << endl << "Welcome to GENE " << user << "!" << endl;
    cout << "Choose one option:\n1. Compare DNA genomes of two species\n2. Faculty details\n ";
    cout << endl << "Choice number: ";
    int options=0; 
    while (options != 1 && options != 2) {
        cin >> options;
        if (options == 1) {
            compare_genomes_option1();
            return true;  
        } 
        else if (options == 2) {
            cout << endl;
            faculty_details_option2();
            return true;
        }
        // loop until user enters correct option
        else cout << "Oops... Not a valid option. Try again!"<< endl;
    }    
    return true;
}

/**
 * This is the main function. 
 */
int main(int argc, char **argv) {
    cout << endl;
    cout << "Hi awesome user! Kindly enter size to test heaps and hashtable functionality of my application software - GENE: " << endl;
    cout << "More information follows :D" << endl;
    cout << endl;
    cout << "1. 10 employee structs?" << endl << "2. 500 employees structs?" << endl;
    cout << "3. 1000 employees structs?" << endl << "4. 1500 employees structs?" << endl;
    cout << "5. 2000 employees structs?" << endl << endl;
    cout << "Choice number: "; 

    string input_filename;
    int choice_chosen; 
    while (choice_chosen <= 0 || choice_chosen > 5) {
        cin >> choice_chosen; 
        cin.ignore();
        if (choice_chosen == 1) {
            input_filename = "faculty10.txt";
            break;
        }
        else if (choice_chosen == 2) {
            input_filename = "faculty500.txt";
            break;
        }
        else if (choice_chosen == 3) {
            input_filename = "faculty1000.txt";
            break;
        }
        else if (choice_chosen == 4) {
            input_filename = "faculty1500.txt";
            break;
        }
        else if (choice_chosen == 5) {
            input_filename = "faculty2000.txt";
            break;
        }
        else {
            cout << "Oops... Not a valid option. Exiting..." << endl;
        }
    }
    
    faculty_filename = "f2-test/" + input_filename;    // input file of which hash table needs to be made. 
    cout << endl << endl<< "Let's begin!" << endl;

    auto start = high_resolution_clock::now();    
    initialise_setup(faculty_filename);    // set the global total_users to be equal to the value returned. 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop-start);
    time_heap = duration.count(); 

    // All the options are handled by greet_user() function.
    if (!greet_user()) {    // if no access exit; passing pointer to the array   --CHECK not sure of *
        cout << "Exiting... " << endl; 
        return 0;
    }
    
    return 0;
}