/*
    Finally, we get to explanations.

    Explanations are the "why" of your code. Not
    every line/method/whatever in your code needs
    an explanation, so you should only focus on making
    comments for any unusual or complicated choices. 

    Explanations can also be more discussion-like, 
    describing differing points of views on specific
    topics. 

    This is the original file I wrote for the lab;
    most of these comments are explanation comments, 
    but there are a few reference-y comments hanging 
    around as well. 
*/


#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// a representation of an equation of the form f(x)=Ax+B,
// where A and B are constants
struct Equation {
    double a;
    double b;

    // returns a string representation of the function
    // ex. Equation{10, 5}.as_string() returns "10x+5"
    // 
    // the `plus` value adds a plus sign before b if it is positive,
    // since otherwise it would not have any sign in front of it (eg. "10x5")
    string as_string() {
        string plus = (b >= 0) ? "+" : "";
        string equation = to_string(a) + "x" + plus + to_string(b);
        return equation;
    }
};

// stores all the important information to keep the flow going
//
// i don't know if there's a better way to use and change the
// fields of a struct without passing references to it
// around all over the place, but i feel like there's
// probably a better solution out there somewhere
struct Data {
    // a list of all the previously used equations
    vector<Equation> history;

    // a list of all the actions the user can take
    vector<string> options;

    // a list of all the x-values that make_table will 
    // calculate the results of
    vector<double> domain_values;

    // the current function to use to calculate results
    Equation current_equation;

    void update_history(Equation eq) {
        history.push_back(eq);
    }

    void update(vector<Equation> hist, vector<string> opt, vector<double> dom, Equation curr) {
        history = hist;
        options = opt;
        domain_values = dom;
        current_equation = curr;
    }

    // sets the current equation while adding it to the history
    void set_current_equation(double a, double b) {
        current_equation = Equation {a, b};
        update_history(current_equation);
    }

    void set_current_equation(Equation eq) {
        current_equation = eq;
        update_history(current_equation);
    }
} game_data;

double calculate(Equation eq, double x) {
    return (eq.a * x) + eq.b;
}

// gets and prints out the result of the current function for each of the x-values
void make_table() {
    cout << "\n\nEvaluating current equation (f(x)=" << game_data.current_equation.as_string() << ") at default domain points:" << endl;

    for(int i = 0; i < game_data.domain_values.size(); i++) {
        double x_value = game_data.domain_values[i];
        cout <<  "\tf(" << x_value << ") = " << calculate(game_data.current_equation, x_value) << endl;
    }

    cout << endl << "Press any key to return to the home screen. ";
    getch(); // <- this stops execution until the next input from the user is received
             // (it's really annoying to have to scroll back up to see the values)
    cout << "\n\n\n----------------\n";
}

// lets the user set their own x-values where they want each 
// function to be evaluated
//
//         !!! input must be separated by commas, contain no spaces !!!
//             (ex. "1,2,3,4,5")
void adjust_domain_points() {

    // show current x-values
    cout << "\n\nThe current values are: \n\t";
    for(int i = 0; i < game_data.domain_values.size(); i++) {
        if(i == game_data.domain_values.size() - 1) {
            cout << "x=" << game_data.domain_values[i];
            continue;
        }
        cout << "x=" << game_data.domain_values[i] << ", ";
    }

    // 
    cout << "\n\nNew domain values must be separated by commas and contain no spaces (ex. \"1,2.4,3\") \nEnter values here: ";
    string input;
    cin >> input;
    cout << "\n\n\n----------------\n";
    input += ","; // does not capture the last value without an extra delimiter
    
    // splits the input into a vector (new_values), casting to doubles in the process
    size_t pos = 0;
    string token;
    vector<double> new_values;
    string delimiter = ",";
    while((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        new_values.push_back(stod(token));
        input.erase(0, pos + delimiter.length());
    }

    game_data.domain_values = new_values;
}

// generates random doubles given double typed bounds 
double random_bounds(double start, double end) {
    return (rand() / (double)RAND_MAX) * (end - start) + start;
}

void generate_random() {
    cout << "\n\nPick integer start and end values for the range of random values: \n";
    double start, end;
    
    cout << "\n\tStart: ";
    cin >> start;

    cout << "\n\tEnd: ";
    cin >> end;

    cout << "\nGenerating random linear equation: \n";
    Equation rand_eq = Equation {random_bounds(start, end), random_bounds(start, end)};
    cout << "\tf(x)=" << rand_eq.as_string();

    game_data.set_current_equation(rand_eq);
    make_table();
}

// shows the history of all the functions the user has used
// (can contains repeated and consecutive elements)
void choose_from_history() {

    // in case it's the first option the user chose
    if(game_data.history.size() == 0) {
        cout << "\nNo functions have been used.\nPress any key to return to the home screen.";
        getch();
        cout << "\n\n\n----------------\n";
        return;
    }

    // when there is at least 1 function in the history list to view
    cout << "\nThere are " << game_data.history.size() << " functions to choose from: \n";
    for(int i = 0; i < game_data.history.size(); i++) {

        // list them all in a nice way
        cout << "\t[" << i << "] f(x)=" << game_data.history[i].as_string() << endl;
    }


    cout << "Select one, or press [c] to cancel: ";
    string choice;
    int number;
    cin >> choice;

    // return to main menu
    if(choice == "c") {
        cout << "\n\n\n----------------\n";
        return;
    }

    // cast choice (string) to int to use as an index for history
    std:istringstream(choice) >> number;
    game_data.set_current_equation(game_data.history[number]);

    make_table();
}

// the first option; prompts the user to make a new function of their choosing
void input_function() {

    // constants init ( f(x) = Ax + B )
    double A = 0;
    double B = 0;

    // prompt user for each constant
    //
    // some funky stuff happens here if you
    // input stuff other than doubles
    //
    // i'm sure i could fix it if i messed around with 
    // stringstreams more but i'm already scarred to be honest
    // 
    // maybe someday. definitely not today though
    cout << endl << "Select double precision floating point constants A and B for the linear function f(x)=Ax+B: " << endl;
    cout << "\tA: ";
    cin >> A;

    cout << "\tB: ";
    cin >> B;

    game_data.set_current_equation(A, B);
    make_table();
}

// converts a user choice to the index its matching option in Data
// 
// since you can't match strings in c++, this is what i came up with
int choice_to_index(string choice) {
    for(int i = 0; i < game_data.options.size(); i++) {
        if(choice == game_data.options[i]){
            return i;
        }
    }
    return -1;
}

void print_options() {
    cout << "Options:" << endl
         << "\t[0]: Input a function" << endl
         << "\t[1]: Select a previous function" << endl
         << "\t[2]: Generate a random function"  << endl
         << "\t[3]: Adjust domain evaluation points" << endl
         << "\t[q]: Quit the program" << endl
         << "Select an option: ";
}

// essentially the main menu for the program
// gets input from user and passes off work to the proper functions
void program_loop() {
    string choice = "";

    // (main control loop)
    while (true) {
        print_options();
        cin >> choice;

        // can't switch string in c++ :'(
        switch(choice_to_index(choice)) {

            // 'no index found' case (any invalid options, eg: "aoeunsth", "a", "nathsx9420yh", etc.)
            case -1: 
                cout << "choice '" << choice << "' not found.\n\n\n----------------\n";
                break;

            // option [0]: input a function
            case 0: 
                input_function();
                break;

            // option [1]: view the history
            case 1: 
                choose_from_history();
                break;
            
            // option [2]: generate a random function
            case 2: 
                generate_random();
                break;

            // option [3]: change default points of evaluation
            // (segfaults my behated) ((i fought for like 2 hours))
            case 3: 
                adjust_domain_points();
                break;

            // option [q]: quit
            default: 
                cout << "Quitting the program.\n\n";
                return;
        }
    }
}

void print_greeting() {
    cout << "\nThis program calculates a table of values for any given linear function.\n----------------\n";
}

int main() {
    print_greeting();

    // setting up the initial game_data for the Data struct in a readable way
    vector<Equation> history = {};
    vector<string> options = {"0", "1", "2", "3", "q"};
    vector<double> domain = {1, 2, 3, 4, 5};
    Equation equation = {0, 0};

    // Data essentially just stores all the important information that needs to 
    // be referenced to actually use the program continuously
    game_data.update(history, options, domain, equation);

    // start the program
    program_loop();
}

/*
    i don't know how to write unit tests in c++,
    so i can only hope that the testing i've done
    manually is enough 
    (not that this program is particularly robust 
    or anything, just that it should at least behave 
    properly when the directions are followed without
    malicious intent)
*/