#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

class ListNode {
public:
    int data;
    ListNode* next;
    ListNode* prev;

    ListNode(int data) : data(data), next(nullptr), prev(nullptr) {}
};

class LinkedList {
public:
    ListNode* head;
    ListNode* tail;

    LinkedList() : head(nullptr), tail(nullptr) {}

    void push_back(int data) {
        ListNode* newNode = new ListNode(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    bool all_of(int value) const {
        ListNode* current = head;
        while (current) {
            if (current->data != value) return false;
            current = current->next;
        }
        return true;
    }

    bool any_of(int value) const {
        ListNode* current = head;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
        }
        return false;
    }

    int count(int value) const {
        int count = 0;
        ListNode* current = head;
        while (current) {
            if (current->data == value) count++;
            current = current->next;
        }
        return count;
    }
};

class Student {
public:
    string last_name;
    string initials;
    LinkedList grades;

    Student(const string& ln, const string& ini, const LinkedList& gr)
            : last_name(ln), initials(ini), grades(gr) {}

    string toString() const {
        string result = last_name + " " + initials + ": ";
        ListNode* current = grades.head;
        while (current) {
            result += to_string(current->data) + " ";
            current = current->next;
        }
        return result;
    }
};

class StudentNode {
public:
    Student data;
    StudentNode* next;
    StudentNode* prev;

    StudentNode(const Student& data) : data(data), next(nullptr), prev(nullptr) {}
};

class StudentList {
private:
    StudentNode* head;
    StudentNode* tail;

public:
    StudentList() : head(nullptr), tail(nullptr) {}

    void addStudent(const Student& student) {
        StudentNode* newNode = new StudentNode(student);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void removeStudent(const string& last_name) {
        StudentNode* current = head;
        while (current && current->data.last_name != last_name) {
            current = current->next;
        }
        if (current) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
        }
    }

    void editStudent(const string& last_name, const Student& new_student) {
        StudentNode* current = head;
        while (current && current->data.last_name != last_name) {
            current = current->next;
        }
        if (current) {
            current->data = new_student;
        }
    }

    void listAllStudents() const {
        StudentNode* current = head;
        while (current) {
            cout << current->data.toString() << "\n";
            current = current->next;
        }
    }

    void listExcellentStudents() const {
        StudentNode* current = head;
        while (current) {
            if (current->data.grades.all_of(5)) {
                cout << current->data.toString() << "\n";
            }
            current = current->next;
        }
    }

    void listStudentsWithThrees() const {
        StudentNode* current = head;
        while (current) {
            if (current->data.grades.any_of(3)) {
                cout << current->data.toString() << "\n";
            }
            current = current->next;
        }
    }

    void listStudentsWithTwos() const {
        StudentNode* current = head;
        while (current) {
            if (current->data.grades.count(2) == 1) {
                cout << current->data.toString() << "\n";
            }
            current = current->next;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            StudentNode* current = head;
            while (current) {
                file << current->data.last_name << " " << current->data.initials << " ";
                ListNode* gradeNode = current->data.grades.head;
                while (gradeNode) {
                    file << gradeNode->data << " ";
                    gradeNode = gradeNode->next;
                }
                file << "\n";
                current = current->next;
            }
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            head = tail = nullptr;
            string last_name, initials;
            while (file >> last_name >> initials) {
                LinkedList grades;
                for (int i = 0; i < 5; ++i) {
                    int grade;
                    file >> grade;
                    grades.push_back(grade);
                }
                addStudent(Student(last_name, initials, grades));
            }
        }
    }
};

void printMenu() {
    cout << "1. Додати студента\n";
    cout << "2. Видалити студента\n";
    cout << "3. Редагувати студента\n";
    cout << "4. Показати всіх студентів\n";
    cout << "5. Показати студентів, які склали іспити тільки на 5\n";
    cout << "6. Показати студентів, які мають трійки\n";
    cout << "7. Показати студентів, які мають двійки\n";
    cout << "8. Зберегти дані у файл\n";
    cout << "9. Завантажити дані з файлу\n";
    cout << "0. Вийти\n";
}

bool inputGrade(int& grade) {
    cin >> grade;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

void addStudent(StudentList& list) {
    string last_name, initials;
    LinkedList grades;
    cout << "Введіть прізвище: ";
    cin >> last_name;
    cout << "Введіть ініціали: ";
    cin >> initials;
    cout << "Введіть оцінки з 5 предметів: ";
    for (int i = 0; i < 5; ++i) {
        int grade;
        while (!inputGrade(grade)) {
            cout << "Помилка! Введіть числове значення для оцінки: ";
        }
        grades.push_back(grade);
    }
    list.addStudent(Student(last_name, initials, grades));
}

void removeStudent(StudentList& list) {
    string last_name;
    cout << "Введіть прізвище студента, якого потрібно видалити: ";
    cin >> last_name;
    list.removeStudent(last_name);
}

void editStudent(StudentList& list) {
    string last_name, new_last_name, initials;
    LinkedList grades;
    cout << "Введіть прізвище студента, якого потрібно редагувати: ";
    cin >> last_name;
    cout << "Введіть нове прізвище: ";
    cin >> new_last_name;
    cout << "Введіть нові ініціали: ";
    cin >> initials;
    cout << "Введіть нові оцінки з 5 предметів: ";
    for (int i = 0; i < 5; ++i) {
        int grade;
        while (!inputGrade(grade)) {
            cout << "Помилка! Введіть числове значення для оцінки: ";
        }
        grades.push_back(grade);
    }
    list.editStudent(last_name, Student(new_last_name, initials, grades));
}

void listAllStudents(const StudentList& list) {
    list.listAllStudents();
}

void listExcellentStudents(const StudentList& list) {
    list.listExcellentStudents();
}

void listStudentsWithThrees(const StudentList& list) {
    list.listStudentsWithThrees();
}

void listStudentsWithTwos(const StudentList& list) {
    list.listStudentsWithTwos();
}

void saveToFile(const StudentList& list) {
    string filename;
    cout << "Введіть ім'я файлу для збереження: ";
    cin >> filename;
    list.saveToFile(filename);
}

void loadFromFile(StudentList& list) {
    string filename;
    cout << "Введіть ім'я файлу для завантаження: ";
    cin >> filename;
    list.loadFromFile(filename);
}

int main() {
    StudentList student_list;
    int choice;
    do {
        printMenu();
        cin >> choice;
        switch (choice) {
            case 1: addStudent(student_list); break;
            case 2: removeStudent(student_list); break;
            case 3: editStudent(student_list); break;
            case 4: listAllStudents(student_list); break;
            case 5: listExcellentStudents(student_list); break;
            case 6: listStudentsWithThrees(student_list); break;
            case 7: listStudentsWithTwos(student_list); break;
            case 8: saveToFile(student_list); break;
            case 9: loadFromFile(student_list); break;
        }
    } while (choice != 0);

    return 0;
}
