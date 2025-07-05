#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
using namespace std;

const int NUM_SUBJECTS = 5;

struct Student {
    int id;
    string name;
    vector<int> grades;
    double average;

    Student(int studentId, const string& studentName) 
        : id(studentId), name(studentName), grades(NUM_SUBJECTS, 0), average(0.0) {}
    Student() : id(0), name(""), grades(NUM_SUBJECTS, 0), average(0.0) {}
};

class GradeManager {
private:
    vector<Student> students;

    int findStudentById(int id) {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].id == id) return static_cast<int>(i);
        }
        return -1;
    }

    double calculateAverage(const vector<int>& grades) {
        return grades.empty() ? 0.0 : 
               static_cast<double>(accumulate(grades.begin(), grades.end(), 0)) / grades.size();
    }

public:
    void addStudent() {
        int id;
        string name;
        cout << "Enter student ID: ";
        cin >> id;

        if (findStudentById(id) != -1) {
            cout << "Student with ID " << id << " already exists!" << endl;
            return;
        }

        cin.ignore();
        cout << "Enter student name: ";
        getline(cin, name);

        Student newStudent(id, name);
        cout << "Enter grades for " << NUM_SUBJECTS << " subjects:" << endl;
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            int grade;
            cout << "Subject " << (i + 1) << ": ";
            cin >> grade;
            while (grade < 0 || grade > 100) {
                cout << "Invalid grade (0-100): ";
                cin >> grade;
            }
            newStudent.grades[i] = grade;
        }

        newStudent.average = calculateAverage(newStudent.grades);
        students.push_back(newStudent);
        cout << "Student added successfully!" << endl;
    }

    void displayAllStudents() {
        if (students.empty()) {
            cout << "No students found!" << endl;
            return;
        }

        cout << "\n" << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Grades" << setw(10) << "Average" << endl;
        cout << string(60, '-') << endl;

        for (const auto& student : students) {
            cout << setw(5) << student.id << setw(20) << student.name << setw(3) << "[";
            for (size_t i = 0; i < student.grades.size(); i++) {
                cout << student.grades[i];
                if (i < student.grades.size() - 1) cout << ",";
            }
            cout << "]" << setw(10) << fixed << setprecision(2) << student.average << endl;
        }
    }

    void searchStudent() {
        int id;
        cout << "Enter student ID to search: ";
        cin >> id;

        int index = findStudentById(id);
        if (index == -1) {
            cout << "Student not found!" << endl;
            return;
        }

        const Student& student = students[index];
        cout << "\nStudent Found: " << student.name << " (ID: " << student.id << ")" << endl;
        cout << "Grades: ";
        for (size_t i = 0; i < student.grades.size(); i++) {
            cout << student.grades[i];
            if (i < student.grades.size() - 1) cout << ", ";
        }
        cout << "\nAverage: " << fixed << setprecision(2) << student.average << endl;
    }

    void sortStudents() {
        if (students.empty()) {
            cout << "No students to sort!" << endl;
            return;
        }

        sort(students.begin(), students.end(), 
             [](const Student& a, const Student& b) {
                 return a.average > b.average;
             });
        cout << "Students sorted by average grade!" << endl;
    }

    void calculateClassStats() {
        if (students.empty()) {
            cout << "No students found!" << endl;
            return;
        }

        double totalAverage = 0.0;
        for (const auto& student : students) {
            totalAverage += student.average;
        }
        double classAverage = totalAverage / students.size();

        auto maxElement = max_element(students.begin(), students.end(),
                                     [](const Student& a, const Student& b) {
                                         return a.average < b.average;
                                     });

        auto minElement = min_element(students.begin(), students.end(),
                                     [](const Student& a, const Student& b) {
                                         return a.average < b.average;
                                     });

        cout << "\n=== CLASS STATISTICS ===" << endl;
        cout << "Total Students: " << students.size() << endl;
        cout << "Class Average: " << fixed << setprecision(2) << classAverage << endl;
        cout << "Highest Average: " << maxElement->average << " (" << maxElement->name << ")" << endl;
        cout << "Lowest Average: " << minElement->average << " (" << minElement->name << ")" << endl;
    }

    void removeStudent() {
        int id;
        cout << "Enter student ID to remove: ";
        cin >> id;

        int index = findStudentById(id);
        if (index == -1) {
            cout << "Student not found!" << endl;
            return;
        }

        cout << "Removing student: " << students[index].name << endl;
        students.erase(students.begin() + index);
        cout << "Student removed successfully!" << endl;
    }

    void updateGrades() {
        int id;
        cout << "Enter student ID to update: ";
        cin >> id;

        int index = findStudentById(id);
        if (index == -1) {
            cout << "Student not found!" << endl;
            return;
        }

        cout << "Enter new grades for " << students[index].name << ":" << endl;
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            int grade;
            cout << "Subject " << (i + 1) << ": ";
            cin >> grade;
            while (grade < 0 || grade > 100) {
                cout << "Invalid grade (0-100): ";
                cin >> grade;
            }
            students[index].grades[i] = grade;
        }

        students[index].average = calculateAverage(students[index].grades);
        cout << "Grades updated successfully!" << endl;
    }

    size_t getStudentCount() const { return students.size(); }
};

void displayMenu() {
    cout << "\n=== STUDENT GRADE MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Add Student\n2. Display All Students\n3. Search Student\n4. Sort Students by Average" << endl;
    cout << "5. Calculate Class Statistics\n6. Remove Student\n7. Update Student Grades\n8. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    GradeManager manager;
    int choice;

    cout << "Welcome to the Student Grade Management System!" << endl;
    cout << "This system uses STL vectors for dynamic data management." << endl;

    do {
        displayMenu();
        cin >> choice;

        while (cin.fail() || choice < 1 || choice > 8) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice! Enter 1-8: ";
            cin >> choice;
        }

        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.displayAllStudents(); break;
            case 3: manager.searchStudent(); break;
            case 4: manager.sortStudents(); break;
            case 5: manager.calculateClassStats(); break;
            case 6: manager.removeStudent(); break;
            case 7: manager.updateGrades(); break;
            case 8: 
                cout << "Thank you for using the system!" << endl;
                cout << "Total students managed: " << manager.getStudentCount() << endl;
                break;
        }

    } while (choice != 8);

    return 0;
}