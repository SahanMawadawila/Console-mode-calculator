#include<iostream>
#include<fstream>
#include<sstream>
#include<stack>
#include<string>
#include<iomanip>
#include<cmath>
using namespace std;

class Node {
public:
    char data;
    Node* left; // pointer to next node
    Node* right; // pointer to previous node
public:
    Node() {
        data = 0;
        left = NULL;
        right = NULL;
    }
};


// Doubly linked list class define
class DoublyLinkedList {
public:
    Node* start = NULL;
public:
    void insertBeg(int X); //Insert a new node at the beginning
    void deleteBeg(); //Delete a node from the beginning

};


// Insert a new node at the beginning
void DoublyLinkedList::insertBeg(int X) {
    Node* temp = new Node();
    temp->data = X;

    //empty linked list
    if (start == NULL) {
        start = temp;
    }

    //not empty linked list
    else {
        temp->right = start;
        start->left = temp;
        start = temp;
    }
}


// Delete a node from the beginning 
void DoublyLinkedList::deleteBeg() {
    Node* ptr = start;
    //empty linked list
    if (start == NULL) {
        cout << "There is no element to delete." << endl;
    }

    // if the list is of one node 
    else if (start->right == NULL) {
        start = NULL;
    }

    // if there are more than one nodes in the list
    else {
        start = start->right;
        start->left = NULL; // there are no node to the left of the start node
    }
    //free the space by deleting the node
    delete ptr;
}
class Stack : public DoublyLinkedList {
public:
    Node* top = NULL;
    int size = 0;

public:

    void push(int data); //push a value into stack
    auto pop(); //delete the top most value from stack
    int count(); //get the no of elements of the stack
    bool isEmpty() {
        return start == NULL;
    }

};

void Stack::push(int data) {
    DoublyLinkedList::insertBeg(data);
    top = start; // Update the top to the newly inserted element
    size++;
}

auto Stack::pop()
{
    if (size != 0) {
        auto x = top->data;
        DoublyLinkedList::deleteBeg();
        top = start; // Update the top to the new top element
        size--;
        return x;
    }
    }
    
int Stack::count()
{
    return size;
}

//
bool isOperator(char character) {
    return (character == '+' || character == '-' || character == '*' || character == '/');
}

int getOperatorsPrecision(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b != 0) return a / b;
        else throw runtime_error("Error:Division by zero!");
    }
    return 0;
}


double calculateResult(const string& expression) {
    stack<double> numericalValues;
    stack<char> operators;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (isdigit(c)) {
            double number = 0;
            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                number = number * 10 + (expression[i] - '0');
                i++;
            }
            i--;

            numericalValues.push(number);
        }
        else if (c == '(' || c == '[' || c == '{') {
            operators.push(c);
        }
        else if (c == ')' || c == ']' || c == '}') {
            while (!operators.empty() && operators.top() != '(' && operators.top() != '[' && operators.top() != '{') {
                char op = operators.top();
                operators.pop();

                if (numericalValues.size() < 2) throw runtime_error("Invalid formula!");
                double b = numericalValues.top();
                numericalValues.pop();
                double a = numericalValues.top();
                numericalValues.pop();

                numericalValues.push(applyOperation(a, b, op));
            }

            if (!operators.empty() && ((c == ')' && operators.top() == '(') || (c == ']' && operators.top() == '[') || (c == '}' && operators.top() == '{'))) {
                operators.pop();
            }
            else {
                throw runtime_error("Error!");
            }
        }
        else if (isOperator(c)) {
            while (!operators.empty() && getOperatorsPrecision(c) <= getOperatorsPrecision(operators.top())) {
                char op = operators.top();
                operators.pop();

                if (numericalValues.size() < 2) throw runtime_error("Error. Two operators are next to each other. !");
                double b = numericalValues.top();
                numericalValues.pop();
                double a = numericalValues.top();
                numericalValues.pop();

                numericalValues.push(applyOperation(a, b, op));
            }

            operators.push(c);
        }
        else {
            throw runtime_error("Invalid character in formula!");
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();

        if (numericalValues.size() < 2) throw runtime_error("Invalid formula!");
        double b = numericalValues.top();
        numericalValues.pop();
        double a = numericalValues.top();
        numericalValues.pop();

        numericalValues.push(applyOperation(a, b, op));
    }

    if (numericalValues.size() != 1) throw runtime_error("Error. A operator missing!");

    return numericalValues.top();
}


//return false bracekts are not balanced
bool are_brackets_balanced(const string& expression) {
    //creating a stack object
    Stack stack;

    //using a range based for loop to iterate each element in the expression.
    //each element assign to bracket char variable
    for (char bracket : expression) {

        // if brackets are opening brackets, then push into the stack
        if (bracket == '{' || bracket == '[' || bracket == '(') {
            stack.push(bracket);
        }
        //if bracekts are closing brackets,
        else if (bracket == '}' || bracket == ']' || bracket == ')' ) {
            //if stack is empty, bracekts are unbalanced. closing brcakets < opening brackets
            if (stack.count() == 0) {
                return false;
            }

            //store top bracket to the top_bracket variable
            char top_bracket = stack.pop();

            if ((bracket == '}' && top_bracket != '{') ||
                (bracket == ']' && top_bracket != '[') ||
                (bracket == ')' && top_bracket != '(')) {
                return false;
            }
        }
    }
    //checking finally stack is empty or not.. not empty means opening brackets > closing brcakets
    return stack.count() == 0;
}



int main(int argc, char* argv[]) {
    ifstream File;
    string fileLine;
    const int size = 100;
    string arr[size];

    //to output file
    ofstream outputFile("result.txt");


    if (argc < 2) {
        cerr << "Error: No input file name is provided" << endl;
        return -1;
    }

    File.open(argv[1], ios::in);
    while (getline(File, fileLine)) {
        cout << fileLine << ":";
        outputFile << fileLine << ":";
        if (fileLine.back() != '=') {
            cout << "Error: Equal sign missing at the end." << endl;
            outputFile << "Error: Equal sign missing at the end." << endl;
        }

        else if (!are_brackets_balanced(fileLine)) {
            cout << "Error: Unbalanced brackets." << endl;
            outputFile<< "Error: Unbalanced brackets." << endl;
        }

        else {
            try {
                // removes the '=' sign
                string formulaWithoutEqualSign = fileLine.substr(0, fileLine.size() - 1);

                // Calculate the result of the formula
                double result = calculateResult(formulaWithoutEqualSign);

                // Write the result to the console
                cout << fixed<<setprecision(2) << result << endl;
                outputFile<< fixed << setprecision(2) << result << endl;
            }
            catch (const exception& e) {
                cout << e.what() << endl; // Error occurred in calculate Result function
                outputFile << e.what() << endl;
            }
        }
    }
    outputFile.close();

    return 0;
}