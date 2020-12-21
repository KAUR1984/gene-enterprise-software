#include <iostream>
#include <fstream>

using namespace std;

// Username Pass FirstName LastName Gender PapersPublished ResearchArea NoCitations OverallRanking Experience Salary(in $1000s)
int main() {
    string filename = "faculty500.txt";
    int total_employees = 500;
    ofstream Fileobj(filename);
    string chars[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    string my_ints[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    string genders[] = {"m", "f"};
    string research_areas[] = {"Structural_Biology_of_infectious_diseases", "Plant_Biotechnology", "Pharmacogenetics", "Forensic_DNA", "Bioinformatics", "Proteomics"};
    string rand_usernames[44] = {"dartfalter", 
                                "tomorrowheel"
                                "highmarried",
                                "carpushistorian",
                                "genderfobbing",
                                "noseawesome",
                                "lepejump",
                                "complainmonstrous",
                                "stalehornet",
                                "nodeswill",
                                "dugoutaction",
                                "oblonglemons",
                                "deformityscrape",
                                "quivercold",
                                "ourselvesstraight",
                                "whiphello",
                                "chingersround",
                                "scootwoodchuck",
                                "philtrumchallenge",
                                "childboth",
                                "faltercrackit",
                                "frickedswiftstick",
                                "linnetpatsy",
                                "stridentgrey",
                                "squawktimkins",
                                "tetalreality",
                                "whompingsettlement",
                                "delightnosy",
                                "hypothesisobtain",
                                "pluserecto",
                                "cleansweepshit",
                                "tooclack",
                                "protectiveportus",
                                "harpistwretched",
                                "heldgrindy",
                                "gatevoracious",
                                "rejectcloistered",
                                "scenetoothsome",
                                "decimalturgid",
                                "limpkinsstatement",
                                "dobblefeather",
                                "poetgroon",
                                "morbiditydecanter",
                                "fludsvisit"};
    string first_names[10] = {"Michael", "Christopher","Jessica", "Matthew","Ashley","Jennifer","Joshua", "Amanda","Daniel","David"}; 
    string last_names[10] = {"James", "Robert", "John", "Joseph", "Andrew", "Ryan", "Brandon", "Jason", "Justin", "Sarah"};

    int attributes = 11;   // total number of attributes
    // write to the file
    for (int i = 0; i < total_employees; i++) {
        // first print the user name
        int usernames_index = rand()%43; 
        Fileobj << rand_usernames[usernames_index] << " ";
        
        // print password
        int pass_index = rand()%43; 
        Fileobj << rand_usernames[pass_index] << " ";

        // print firstname and last name 
        Fileobj << first_names[rand() % 10] << " " << last_names[rand() % 10] << " "; 

        // print gender 
        Fileobj << genders[rand()%2] << " ";

        // print papers published
        Fileobj << rand() % 1000 << " "; 

        // print research area 
        Fileobj << research_areas[rand()%6] << " "; 

        // print no of citations
        Fileobj << rand() % 1000 << " " << (rand() % total_employees) + 1 << " " << rand() % 60 << " " << rand() % 20 << " "; 

        Fileobj << endl;

    }
    Fileobj.close();

    return 0; 
}