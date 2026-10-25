//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jeremy Schultz
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : 7-1 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Course object
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// BST Node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);

            cout << node->course.courseNumber
                << ", "
                << node->course.courseTitle
                << endl;

            inOrder(node->right);
        }
    }

    Course searchNode(Node* node, string courseNumber) {

        while (node != nullptr) {

            if (courseNumber == node->course.courseNumber) {
                return node->course;
            }

            if (courseNumber < node->course.courseNumber) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        Course emptyCourse;
        return emptyCourse;
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course) {

        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    void PrintCourseList() {
        inOrder(root);
    }

    Course Search(string courseNumber) {
        return searchNode(root, courseNumber);
    }
};

// Remove extra spaces
string trim(string str) {

    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, (last - first + 1));
}

// Load courses from CSV file
void LoadCourses(string fileName, BinarySearchTree& bst) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string token;
        vector<string> fields;

        while (getline(ss, token, ',')) {
            fields.push_back(trim(token));
        }

        if (fields.size() < 2) {
            cout << "Invalid line skipped." << endl;
            continue;
        }

        Course course;

        course.courseNumber = fields[0];
        course.courseTitle = fields[1];

        for (size_t i = 2; i < fields.size(); i++) {

            if (!fields[i].empty()) {
                course.prerequisites.push_back(fields[i]);
            }
        }

        bst.Insert(course);
    }

    file.close();

    cout << "Courses loaded successfully." << endl;
}

// Print information for one course
void PrintCourseInfo(BinarySearchTree& bst, string courseNumber) {

    transform(courseNumber.begin(),
        courseNumber.end(),
        courseNumber.begin(),
        ::toupper);

    Course course = bst.Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << endl;
    cout << course.courseNumber
        << ", "
        << course.courseTitle
        << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {

        cout << "None";
    }
    else {

        for (size_t i = 0; i < course.prerequisites.size(); i++) {

            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

int main() {

    BinarySearchTree bst;

    bool dataLoaded = false;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {

        case 1:

            cout << "Enter file name: ";
            cin >> fileName;

            LoadCourses(fileName, bst);
            dataLoaded = true;

            break;

        case 2:

            if (!dataLoaded) {

                cout << "Please load the file first." << endl;
            }
            else {

                cout << endl;
                cout << "Here is a sample schedule:" << endl;

                bst.PrintCourseList();
            }

            break;

        case 3:

            if (!dataLoaded) {

                cout << "Please load the file first." << endl;
            }
            else {

                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                PrintCourseInfo(bst, courseNumber);
            }

            break;

        case 9:

            cout << "Good bye." << endl;
            break;

        default:

            cout << choice
                << " is not a valid option."
                << endl;
        }
    }

    return 0;
}