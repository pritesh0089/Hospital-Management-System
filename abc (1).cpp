#include <iostream>
#include <random>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <queue>
#include <stack>
#include <sstream>
#include <map>
#include <sstream>
#include <regex>

#define hash_size 50
#define MAX 2000

using namespace std;

struct Bill
{
    double amount;
    string description;

    Bill(double amt, string desc) : amount(amt), description(desc) {}
};

typedef struct patient_details
{
    string name, email, DOB, address, emergency_contact_name, relation_with_emergency_contact, email_of_emergency_contact, mobile_emergency_contact, appointment, sex;
    long int mobile_number;
} p_d;

typedef struct medications
{
    queue<string> meds;
} med;

typedef struct patient_history
{
    stack<string> his;
} p_h;

typedef struct patient_information
{
    p_d details;
    med meds;
    p_h his;
    vector<Bill> bills;
    long long int mobile_number;
} info;

struct node
{
    info patient;
    int height;
    node *left;
    node *right;

    // Constructor for ease of node creation
    node(info p) : patient(p), height(1), left(nullptr), right(nullptr) {}
};

int Height(node *n)
{
    if (n == nullptr)
    {
        return 0;
    }
    return n->height;
}

node *new_node(info patient)
{
    return new (nothrow) node(patient); // Using nothrow for basic error handling
}

node *Search(node *root, long long int x)
{
    if (root == nullptr)
    {
        // If the root is null, there's nothing to search, so return null.
        return nullptr;
    }

    if (x < root->patient.details.mobile_number)
    {
        // If the search key is less than the root's key, search in the left subtree.
        return Search(root->left, x);
    }
    else if (x > root->patient.details.mobile_number)
    {
        // If the search key is greater than the root's key, search in the right subtree.
        return Search(root->right, x);
    }
    else
    {
        // Found the node with the matching key.
        return root;
    }
}

node *Insert(node *root, info patient, int *count)
{
    if (root == nullptr)
    {
        root = new_node(patient);
        if (root == nullptr)
        {
            cerr << "Memory allocation failed" << endl;
            return nullptr;
        }
        (*count)++;
        return root;
    }

    if (patient.mobile_number < root->patient.mobile_number)
    {
        root->left = Insert(root->left, patient, count);
    }
    else if (patient.mobile_number > root->patient.mobile_number)
    {
        root->right = Insert(root->right, patient, count);
    }
    // Duplicate values are currently ignored. You can add handling here if needed.

    root->height = 1 + max(Height(root->left), Height(root->right));

    return root;
}

node *Successor(node *root, int *count)
{
    if (root == nullptr)
    {
        return root;
    }

    if (root->left)
    {
        root = Successor(root->left, count);
    }

    return root;
}

int hashIndex(long long number)
{
    long long f_n = (3 * number) + 4;
    f_n = f_n % hash_size;
    f_n += hash_size;
    return f_n % hash_size;
}

void new_user_enter_personal_info(info &patient)
{
    cout << endl;
    cout << "*********************************" << endl;

    cout << "PATIENT DETAILS" << endl
         << endl;

    cout << "Name of Patient: ";
    cin.ignore(); // Clear the input buffer to safely use getline after cin
    getline(cin, patient.details.name);

    cout << "Email Address of Patient: ";
    getline(cin, patient.details.email);

    cout << "Date of Birth: ";
    getline(cin, patient.details.DOB);

    cout << "Biological Sex of Patient (M/F/O): ";
    getline(cin, patient.details.sex);

    cout << "Address of Patient: ";
    getline(cin, patient.details.address);

    cout << endl;
    cout << "*********************************" << endl;
    cout << "EMERGENCY CONTACT" << endl
         << endl;

    cout << "Name of Contact: ";
    getline(cin, patient.details.emergency_contact_name);

    cout << "Relation with Contact: ";
    getline(cin, patient.details.relation_with_emergency_contact);

    cout << "Email Address of Contact: ";
    getline(cin, patient.details.email_of_emergency_contact);

    cout << "Phone Number of contact: ";
    getline(cin, patient.details.mobile_emergency_contact);
}

void already_registered_user_patient_details(node *patient_node)
{
    if (patient_node == nullptr)
    {
        cout << "Patient not found in the database." << endl;
        return;
    }

    cout << "*********************************" << endl;
    cout << "PATIENT DETAILS" << endl
         << endl;

    // Displaying patient information
    cout << "User Information:" << endl;
    cout << "-----------------" << endl;
    cout << "Name: " << patient_node->patient.details.name << endl;
    cout << "Mobile Number: " << patient_node->patient.details.mobile_number << endl;
    cout << "Email: " << patient_node->patient.details.email << endl;
    cout << "Date of Birth: " << patient_node->patient.details.DOB << endl;
    cout << "Biological Sex of Patient: " << patient_node->patient.details.sex << endl;
    cout << "Address of Patient: " << patient_node->patient.details.address << endl;

    cout << endl;
    cout << "*********************************" << endl;
    cout << "EMERGENCY CONTACT DETAILS" << endl
         << endl;

    // Displaying emergency contact information
    cout << "Name of Contact: " << patient_node->patient.details.emergency_contact_name << endl;
    cout << "Relation with Contact: " << patient_node->patient.details.relation_with_emergency_contact << endl;
    cout << "Email Address of Contact: " << patient_node->patient.details.email_of_emergency_contact << endl;
    // cout << "Phone number of Contact: " << patient_node->patient.details.mobile_emergency_contact << endl;
}

void mobile_number_verification(node *patients_list[], long long number, int &count)
{
    int hash_index = hashIndex(number);

    node *patient_node = Search(patients_list[hash_index], number);

    if (patient_node != nullptr)
    {
        cout << "Here!";
        // Patient exists, display details
        already_registered_user_patient_details(patient_node);
    }
    else
    {
        // Patient does not exist, proceed with OTP verification
        while (1)
        {
            srand(time(NULL));
            int generated_otp, input_otp;
            generated_otp = (rand() % (899999 + 1)) + 100000;

            cout << "(your OTP is: " << generated_otp << ")" << endl;
            cout << "Kindly Enter the OTP: ";
            cin >> input_otp;
            cout << endl;

            if (generated_otp == input_otp)
            {
                info new_patient_info; // Create new info object
                new_patient_info.details.mobile_number = number;
                new_user_enter_personal_info(new_patient_info);

                // Insert the new patient info into the AVL tree
                patients_list[hash_index] = Insert(patients_list[hash_index], new_patient_info, &count);
                break;
            }
            else
            {
                cout << "ERROR!! Entered OTP is Wrong!! Please enter the OTP again." << endl;
            }
        }
    }
}

class Appointment
{
public:
    string time;
    string description;

    Appointment(const string &time, const string &description)
        : time(time), description(description) {}
};

class Calendar
{
private:
    map<string, vector<Appointment>> appointments;

public:
    void addAppointment(const string &date, const Appointment &appointment)
    {
        appointments[date].push_back(appointment);
    }

    void viewAppointments(const string &date)
    {
        if (appointments.find(date) == appointments.end())
        {
            cout << "No appointments for " << date << endl;
            return;
        }

        cout << "Appointments for " << date << ":" << endl;
        for (const auto &appointment : appointments[date])
        {
            cout << "Time: " << appointment.time << ", Description: " << appointment.description << endl;
        }
    }
};

string formatInputDate(string input)
{
    // Basic date format validation and transformation (e.g., 5 Dec to 2023-12-05)
    regex datePattern("(\\d{1,2})[ ]?([a-zA-Z]+)");
    smatch matches;

    if (regex_search(input, matches, datePattern))
    {
        string day = matches[1].str();
        string month = matches[2].str();

        // Convert month to number (This can be expanded for all months)
        map<string, string> months = {
            {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"}, {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"}, {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}};

        if (months.find(month) != months.end())
        {
            if (day.length() == 1)
                day = "0" + day; // Ensure day is two digits
            return "2023-" + months[month] + "-" + day;
        }
    }

    return "";
}

struct Point
{
    int x, y;
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool DFS(vector<vector<char>> maze, int x, int y, int destX, int destY, stack<Point> &path)
{
    if (x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || maze[x][y] == '#' || maze[x][y] == '.')
    {
        return false;
    }

    if (x == destX && y == destY)
    {
        path.push({x, y});
        return true;
    }

    maze[x][y] = '.';

    for (int i = 0; i < 4; i++)
    {
        int adjX = x + dx[i];
        int adjY = y + dy[i];

        if (DFS(maze, adjX, adjY, destX, destY, path))
        {
            path.push({x, y});
            return true;
        }
    }

    maze[x][y] = ' ';
    return false;
}

void addBill(node *patient_node, double amount, const string &description)
{
    if (patient_node == nullptr)
    {
        cout << "Patient not found." << endl;
        return;
    }
    patient_node->patient.bills.push_back(Bill(amount, description));
}

void viewBills(node *patient_node)
{
    if (patient_node == nullptr)
    {
        cout << "Patient not found." << endl;
        return;
    }
    cout << "Bills for " << patient_node->patient.details.name << ":" << endl;
    for (const auto &bill : patient_node->patient.bills)
    {
        cout << "Amount: $" << bill.amount << ", Description: " << bill.description << endl;
    }
}

int main()
{
    Calendar myCalendar;
    string date, time, description;
    node *patients_list[hash_size];
    int count = 0;

    vector<vector<char>> maze;
    FILE *mazeFile;
    mazeFile = fopen("hospitalmaze.txt", "r");
    if (mazeFile == NULL)
    {
        printf("Unable to open the maze file.\n");
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    vector<char> row;
    map<Point, Point> parentMaze;
    char inputStart, inputEnd;

    while ((read = getline(&line, &len, mazeFile)) != -1)
    {
        row.clear();
        for (int i = 0; i < read; ++i)
        {
            if (line[i] != '\n')
            {
                row.push_back(line[i]);
            }
        }
        maze.push_back(row);
    }

    fclose(mazeFile);
    free(line);

    map<char, pair<int, int>> pointCoordinates = {
        {'A', {10, 1}},
        {'B', {8, 18}},
        {'C', {16, 5}},
        {'D', {16, 18}},
        {'E', {5, 7}},
        {'F', {11, 2}},
        {'G', {2, 5}},
        {'H', {4, 1}},
        {'I', {17, 7}},
        {'J', {6, 20}},
        {'K', {10, 8}},
        {'L', {17, 12}}};

    stack<Point> path;
    int startX, startY, destX, destY;

    // stack<Point> path;
    // Initialize patients_list to nullptr
    for (int i = 0; i < hash_size; i++)
    {
        patients_list[i] = nullptr;
    }

    int choice;
    long long int mobile;

    while (true)
    {
        cout << "\nHospital Management System\n";
        cout << "1. Register New Patient\n";
        cout << "2. Search Old Patient Records\n";
        cout << "3. Book an Appointment\n";
        cout << "4. Check Appointments\n";
        cout << "5. Find Way in Hospital\n";
        cout << "6. Add Bill to Patient\n";
        cout << "7. View Patient Bills\n";
        cout << "8. Exit System\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "*********************************" << endl;
            cout << "PATIENT LOGIN PORTAL" << endl;
            cout << endl;
            cout << "Please Enter Your Mobile Number: ";
            cin >> mobile;
            cout << endl;
            mobile_number_verification(patients_list, mobile, count);
            break;
        case 2:
            cout << "*********************************" << endl;
            cout << "PATIENT DATA PORTAL" << endl;
            cout << endl;
            cout << "Please Enter Your Mobile Number: ";
            cin >> mobile;
            cout << endl;
            mobile_number_verification(patients_list, mobile, count);
            break;
        case 3:
            cout << "Enter date (e.g., 5 Dec): ";
            cin >> date;
            date = formatInputDate(date);
            if (date.empty())
            {
                cout << "Invalid date format.\n";
                break;
            }
            cout << "Enter time (e.g., 14:30): ";
            cin >> time;
            cout << "Enter description: ";
            cin.ignore();
            getline(cin, description);
            myCalendar.addAppointment(date, Appointment(time, description));
            break;
        case 4:
            cout << "Enter date to view appointments (e.g., 5 Dec): ";
            cin >> date;
            date = formatInputDate(date);
            if (date.empty())
            {
                cout << "Invalid date format.\n";
                break;
            }
            myCalendar.viewAppointments(date);
            break;

        case 5:

            cout << "Enter the letters of the start and destination points (e.g., A B): ";
            cin >> inputStart >> inputEnd;

            if (pointCoordinates.find(inputStart) != pointCoordinates.end() &&
                pointCoordinates.find(inputEnd) != pointCoordinates.end())
            {
                startX = pointCoordinates[inputStart].first;
                startY = pointCoordinates[inputStart].second;
                destX = pointCoordinates[inputEnd].first;
                destY = pointCoordinates[inputEnd].second;
            }
            else
            {
                cout << "Invalid input. Please enter letters between A and L." << endl;
                break;
            }

            if (DFS(maze, startX, startY, destX, destY, path))
            {
                cout << "Path found:\n";

                // Mark the path
                while (!path.empty())
                {
                    Point p = path.top();
                    path.pop();
                    maze[p.x][p.y] = '.';
                }

                // Display the maze with the path
                for (auto &row : maze)
                {
                    for (char c : row)
                    {
                        cout << c;
                    }
                    cout << endl;
                }
            }
            else
            {
                cout << "No path found.\n";
            }

            break;
        case 6:
            cout << "Enter Patient's Mobile Number to Add Bill: ";
            cin >> mobile;
            {
                int hash_index = hashIndex(mobile);
                node *patient_node = Search(patients_list[hash_index], mobile);
                if (patient_node != nullptr)
                {
                    double billAmount;
                    string billDescription;
                    cout << "Enter Bill Amount: ";
                    cin >> billAmount;
                    cout << "Enter Bill Description: ";
                    cin.ignore();
                    getline(cin, billDescription);
                    addBill(patient_node, billAmount, billDescription);
                }
                else
                {
                    cout << "Patient not found." << endl;
                }
            }
            break;

        case 7:
            cout << "Enter Patient's Mobile Number to View Bills: ";
            cin >> mobile;
            {
                int hash_index = hashIndex(mobile);
                node *patient_node = Search(patients_list[hash_index], mobile);
                if (patient_node != nullptr)
                {
                    viewBills(patient_node);
                }
                else
                {
                    cout << "Patient not found." << endl;
                }
            }
            break;
        case 8:
            cout << "Exiting the system...\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    cout << "Exiting program..." << endl;
    return 0;
}