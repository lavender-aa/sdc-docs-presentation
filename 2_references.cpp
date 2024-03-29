/*
    Now, we arrive at reference guides. These
    guides are all about information; you tell
    the user what: functions, fields, methods,
    and how they are all used. 

    This is where you get a slight look at the underlying
    logic of the program, though still not going into details
    about each function's specific implementation. In this example
    code, the implementation of each function is shown, but they
    are not a part of the reference documentation.  

    Most technical documentation is reference documentation.
    If you're going through the official docs of your programming
    language, for example, it will almost certainly be the case.

    Unlike before, I'll be writing my examples of reference
    documentation only on the Equation and Data structs, though
    these types of comments would be around anything the user
    would come into contact with (in this case I'm writing 
    documentation for any future developer who may want to modify
    or extend this code).
*/

#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// Representation of a linear equation.
/*
    Equation types represent linear equations of the 
    form y=Ax+B, where A and B are its public members. 
*/
struct Equation {

    // Constant A in the form Ax+B.
    double a;

    // Constant B in the form Ax+B.
    double b;

    // Gets a formatted string representing the Equation.
    /*
        return string takes form of "Ax+B" or "Ax-B"
    */
    string as_string() {
        string plus = (b >= 0) ? "+" : "";
        string equation = to_string(a) + "x" + plus + to_string(b);
        return equation;
    }
};

// A type that contains all the relevant information to the program.
struct Data {

    // The history of all past Equations used
    /*
        Oldest functions at the head, newest at the tail
    */
    vector<Equation> history;

    // A list of options available in the program
    /*
        Used for matching user input from program_loop().
        
        No option with matching value "-1" may be appended,
        as "-1" is a magic number associated with unfound
        options. 
    */
    vector<string> options;

    // All the x values at which the program will evaluate each function.
    vector<double> domain_values;

    // The current equation to be used in calculations
    Equation current_equation;

    // Updates public member history.
    void update_history(Equation eq) {
        history.push_back(eq);
    }

    // updates all the information contained in this struct at once 
    /* 
        mainly used for initialization
    */
    void update(vector<Equation> hist, vector<string> opt, vector<double> dom, Equation curr) {
        history = hist;
        options = opt;
        domain_values = dom;
        current_equation = curr;
    }

    // Sets a new current equation.
    /*
        Updates the history with the previous current equation.
        Accepts the two constants associated with linear equations.
    */
    void set_current_equation(double a, double b) {
        current_equation = Equation {a, b};
        update_history(current_equation);
    }

    // Sets a new current equation.
    /*
        Updates the history with the previous current equation.
        Accepts an Equation. 
    */
    void set_current_equation(Equation eq) {
        current_equation = eq;
        update_history(current_equation);
    }
} calc_data;


double calculate(Equation eq, double x) {
    return (eq.a * x) + eq.b;
}
void make_table() {
    cout << "\n\nEvaluating current equation (f(x)=" << calc_data.current_equation.as_string() << ") at default domain points:" << endl;

    for(int i = 0; i < calc_data.domain_values.size(); i++) {
        double x_value = calc_data.domain_values[i];
        cout <<  "\tf(" << x_value << ") = " << calculate(calc_data.current_equation, x_value) << endl;
    }

    cout << endl << "Press any key to return to the home screen. ";
    getch();
    cout << "\n\n\n----------------\n";
}
void adjust_domain_points() {
    cout << "\n\nThe current values are: \n\t";
    for(int i = 0; i < calc_data.domain_values.size(); i++) {
        if(i == calc_data.domain_values.size() - 1) {
            cout << "x=" << calc_data.domain_values[i];
            continue;
        }
        cout << "x=" << calc_data.domain_values[i] << ", ";
    }
    cout << "\n\nNew domain values must be separated by commas and contain no spaces (ex. \"1,2.4,3\") \nEnter values here: ";
    string input;
    cin >> input;
    cout << "\n\n\n----------------\n";
    input += ",";
    size_t pos = 0;
    string token;
    vector<double> new_values;
    string delimiter = ",";
    while((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        new_values.push_back(stod(token));
        input.erase(0, pos + delimiter.length());
    }

    calc_data.domain_values = new_values;
}
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
    calc_data.set_current_equation(rand_eq);
    make_table();
}
void choose_from_history() {
    if(calc_data.history.size() == 0) {
        cout << "\nNo functions have been used.\nPress any key to return to the home screen.";
        getch();
        cout << "\n\n\n----------------\n";
        return;
    }
    cout << "\nThere are " << calc_data.history.size() << " functions to choose from: \n";
    for(int i = 0; i < calc_data.history.size(); i++) {
        cout << "\t[" << i << "] f(x)=" << calc_data.history[i].as_string() << endl;
    }
    cout << "Select one, or press [c] to cancel: ";
    string choice;
    int number;
    cin >> choice;
    if(choice == "c") {
        cout << "\n\n\n----------------\n";
        return;
    }
    std:istringstream(choice) >> number;
    calc_data.set_current_equation(calc_data.history[number]);
    make_table();
}
void input_function() {
    double A = 0;
    double B = 0;
    cout << endl << "Select double precision floating point constants A and B for the linear function f(x)=Ax+B: " << endl;
    cout << "\tA: ";
    cin >> A;
    cout << "\tB: ";
    cin >> B;
    calc_data.set_current_equation(A, B);
    make_table();
}
int choice_to_index(string choice) {
    for(int i = 0; i < calc_data.options.size(); i++) {
        if(choice == calc_data.options[i]){
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
void program_loop() {
    string choice = "";
    while (true) {
        print_options();
        cin >> choice;
        switch(choice_to_index(choice)) {
            case -1: 
                cout << "choice '" << choice << "' not found.\n\n\n----------------\n";
                break;
            case 0: 
                input_function();
                break;
            case 1: 
                choose_from_history();
                break;
            case 2: 
                generate_random();
                break;
            case 3: 
                adjust_domain_points();
                break;
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
    vector<Equation> history = {};
    vector<string> options = {"0", "1", "2", "3", "q"};
    vector<double> domain = {1, 2, 3, 4, 5};
    Equation equation = {0, 0};
    calc_data.update(history, options, domain, equation);
    program_loop();
}