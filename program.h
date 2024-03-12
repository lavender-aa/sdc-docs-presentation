#ifndef PROGRAM_H
#define PROGRAM_H

#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class program {
    public:
    void run() {
        print_greeting();
        vector<Equation> history = {};
        vector<string> options = {"0", "1", "2", "3", "q"};
        vector<double> domain = {1, 2, 3, 4, 5};
        Equation equation = {0, 0};
        data.update(history, options, domain, equation);
        program_loop();
    }

    private:
    struct Equation {
        double a;
        double b;
        string as_string() {
            string plus = (b >= 0) ? "+" : "";
            string equation = to_string(a) + "x" + plus + to_string(b);
            return equation;
        }
    };

    struct Data {
        vector<Equation> history;
        vector<string> options;
        vector<double> domain_values;
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

        void set_current_equation(double a, double b) {
            current_equation = Equation {a, b};
            update_history(current_equation);
        }

        void set_current_equation(Equation eq) {
            current_equation = eq;
            update_history(current_equation);
        }
    } data;

    double calculate(Equation eq, double x) {
        return (eq.a * x) + eq.b;
    }

    void make_table() {
        cout << "\n\nEvaluating current equation (f(x)=" << data.current_equation.as_string() << ") at default domain points:" << endl;

        for(int i = 0; i < data.domain_values.size(); i++) {
            double x_value = data.domain_values[i];
            cout <<  "\tf(" << x_value << ") = " << calculate(data.current_equation, x_value) << endl;
        }

        cout << endl << "Press any key to return to the home screen. ";
        getch();
        cout << "\n\n\n----------------\n";
    }

    void adjust_domain_points() {
        cout << "\n\nThe current values are: \n\t";
        for(int i = 0; i < data.domain_values.size(); i++) {
            if(i == data.domain_values.size() - 1) {
                cout << "x=" << data.domain_values[i];
                continue;
            }
            cout << "x=" << data.domain_values[i] << ", ";
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

        data.domain_values = new_values;
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

        data.set_current_equation(rand_eq);
        make_table();
    }

    void choose_from_history() {
        if(data.history.size() == 0) {
            cout << "\nNo functions have been used.\nPress any key to return to the home screen.";
            getch();
            cout << "\n\n\n----------------\n";
            return;
        }

        cout << "\nThere are " << data.history.size() << " functions to choose from: \n";
        for(int i = 0; i < data.history.size(); i++) {
            cout << "\t[" << i << "] f(x)=" << data.history[i].as_string() << endl;
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
        data.set_current_equation(data.history[number]);

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

        data.set_current_equation(A, B);
        make_table();
    }

    int choice_to_index(string choice) {
        for(int i = 0; i < data.options.size(); i++) {
            if(choice == data.options[i]){
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
};
#endif