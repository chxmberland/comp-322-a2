#include <iostream>
#include <fstream>
#include <sstream> // String stream
#include <string>
#include <cmath> // Log function

using namespace std;


/* ---------- UTILITY FUNCTIONS ---------- */

bool isNumber(string input) {

    // Using float conversion function to check is input is a number
    try {
        float num = stof(input);

        // Able to parse to number
        return true;

    // Unable to convert to a float
    } catch (const std::invalid_argument& ia) {
        
        return false;
    }
} // end isNumber

/* CLASS AND STRUCT DECLARATIONS */
struct UserInfo { 
    int age;
    double weight;
    double waist;
    double neck;
    double height;
    double hips;
    double carbs;
    double protein;
    double fat;
    double calories;
    double bfp;
    std::string name;
    std::string gender;
    std::string lifestyle;
    UserInfo* next;
}; // end UserInfo

void displaySingleUser(UserInfo* u) {

    // Printing all of the values
    cout << "Name: " << u->name << endl;
    cout << "Gender: " << u->gender << endl;
    cout << "Lifestyle: " << u->lifestyle << endl;
    cout << "Age: " << u->age << endl;
    cout << "Weight (kg): " << u->weight << endl;
    cout << "Waist (cm): " << u->waist << endl;
    cout << "Neck (cm): " << u->neck << endl;
    cout << "Height (cm): " << u->age << endl;
    cout << "Carbohydrates: " << u->carbs << endl;
    cout << "Protien: " << u->protein << endl;
    cout << "Fat: " << u->fat << endl;
    cout << "Required calories: " << u->calories << endl;
    cout << "BFP: " << u->bfp << endl;
    cout << "\n";
} // end displaySingleUser

class UserInfoManager {
public:
    UserInfoManager(); // constructor. initializes linked list 
    ~UserInfoManager(); // destructor. delete allocated memory 
    void addUserInfo(); // adds info to list
    void deleteUser(string username); // removes a user
    void readFromFile(string filename); // read and populate list void writeToFile(string filename);
    void display(string username);
}; // end UserInfoManager

class HealthAssistant {
public:
    void getUserDetail(); // Wrapper method that calls addUserInfo in class UserInfoManager
    void getBfp(string username);
    void getDailyCalories(string username);
    void getMealPrep(string username);
    void display(string username); // Wrapper method
    void serialize(string filename);
    void readFromFile(string filename); // Wrapper method
    void deleteUser(string username); // Wrapper method

    void massLoadAndCompute(string filename); // Add-on function
private:
    UserInfoManager manager;
}; // end HealthAssistant

class UserListSingleton {
public:
    static UserListSingleton& getInstance() {
        static UserListSingleton instance;
        return instance;
    }
    UserInfo* getListInstance() {
        return listInstance;
    }
    void appendToList(UserInfo* usr) {

        // Checking to see if the list is empty
        UserInfo** head = &listInstance;
        if (!(*head)) {
            *head = usr;

        // Appending user to end of the list
        } else {

            // Creating a new pointer to track the end of the list
            UserInfo* tail = *head;
            while (tail->next != nullptr) {
                tail = tail->next;
            }
            tail->next = usr; 
        }
    }
    UserInfo* getUser(string username) {
        UserInfo* head = listInstance; // Making a copy of the pointer
    
        // Input validation
        if (username == "" || isNumber(username)) {
            cout << "Invalid name. Ensure that the name is not blank and contains only alphabetical characters.";
            exit(1);
        }

        // Ensuring the input is all lowercase
        for (int i = 0; i < username.size(); i++) {
            username[i] = tolower(username[i]);
        }

        // Ensuring the list is not empty
        if (!head) {
            cout << "List is empty." << endl;
            return nullptr;
        }

        // If not empty
        while (head != nullptr) {
            
            // Checking to see if username matches
            if (head->name == username) {
                return head;
            }

            head = head->next;
        }

        // Name is not in the list
        cout << "Unnable to find user." << endl;
        return nullptr;
    }
    void deleteFromList(string username) {
        UserInfo** head = &listInstance;

        // If the list is empty
        if (!(*head)) {
            cout << "Attempted to delete a user from an empty list." << endl;
            return;
        }

        // Checking to see if the user is even in the list 
        UserInfo* u = getUser(username);

        // Ensuring function returned correct value
        if (u == nullptr) {
            cout << "Unnable to retrieve user. Ensure the list is not empty and the username is correct.";
            exit(1);
        }

        // Ensuring the input is all lowercase
        for (int i = 0; i < username.size(); i++) {
            username[i] = tolower(username[i]);
        }

        // If the first user is the one to delete
        if ((*head)->name == username) {

            // If there's only one element in the list
            if ((*head)->next == nullptr) {
                delete *head;
                return;

            // Moving to the next element
            } else {
                UserInfo* temp = *head;
                (*head) = (*head)->next;
                delete temp;
            }

        // Appending user to end of the list
        } else {

            // Creating a new pointer to track the end of the list
            UserInfo* tail = *head;
            while (tail->next != nullptr) {

                // Checking to see if the next name matches
                if (tail->next->name == username) {

                    // If the next object is the last object in the list
                    if (tail->next->next == nullptr) {
                        UserInfo* temp = tail->next;
                        tail->next = nullptr;
                        delete temp;
                        return;

                    // The object to delete is neither the first or last in the list
                    } else {
                        UserInfo* temp = tail->next;
                        tail->next = tail->next->next;
                        delete temp;
                        return;
                    }
                }

                // Moving to the next object
                tail = tail->next;
            } 
        }
    }
    void printList() {
        UserInfo* head = listInstance; // Copy of the pointer
        while (head) {
            displaySingleUser(head);
            head = head->next;
        }
    } // end printList

private:
    UserListSingleton() {} // Initializing to a null pointer
    ~UserListSingleton() {}
    UserInfo* listInstance; // Pointer to first element in linked list
};


/* ---------- METHOD IMPLEMENTATIONS ---------- */

UserInfoManager::UserInfoManager() {}

UserInfoManager::~UserInfoManager() {}

void UserInfoManager::deleteUser(string username) {
    UserListSingleton::getInstance().deleteFromList(username);
}

void UserInfoManager::readFromFile(string filename) {

    // Opening the file in reading mode
    ifstream in_file(filename);

    // Ensure the file actually exists
    if (!in_file.is_open()) {
        cout << "Error opening input file.";
        exit(1);
    }

    // Looping through each line of the file until the end is reached
    string line;
    while(getline(in_file, line)) {
        UserInfo* u = new UserInfo;

        // Get stream object from row
        stringstream s(line);
        string data;

        // Getting each piece of data from a row
        getline(s, data, ',');
        u->age = stof(data); // age
        getline(s, data, ',');
        u->weight = stof(data); // weight
        getline(s, data, ',');
        u->waist = stof(data); // waist
        getline(s, data, ',');
        u->neck = stof(data); // neck
        getline(s, data, ',');
        u->height = stof(data); // height
        getline(s, data, ',');
        u->hips = stof(data); // hips
        getline(s, data, ',');
        u->carbs = stof(data); // carbs
        getline(s, data, ',');
        u->protein = stof(data); // protein
        getline(s, data, ',');
        u->fat = stof(data); // fat
        getline(s, data, ',');
        u->calories = stof(data); // calories
        getline(s, data, ',');
        u->bfp = stof(data); // bfp
        getline(s, data, ',');
        u->name = data; // name
        getline(s, data, ',');
        u->gender = data; // gender
        getline(s, data, ',');
        u->lifestyle = data; // lifestyle

        // Adding the row to the global user data vector
        UserListSingleton::getInstance().appendToList(u);
    }
}

void UserInfoManager::addUserInfo() {
    UserInfo* user = new UserInfo;

    // Name
    string name;
    cout << "Enter your name: ";
    cin >> name;

    // Input validation
    if (name == "" || isNumber(name)) {
        cout << "Bad input.";
        exit(1);
    }

    // Putting name in all lowercase
    for (int i = 0; i < name.size(); i++) {
        name[i] = tolower(name[i]);
    }
    user->name = name;

    // Gender
    string gender;
    cout << "Enter your gender [Male or Female]: ";
    cin >> gender;

    // Input validation
    if (gender == "" || isNumber(gender)) {
        cout << "Bad input.";
        exit(1);
    }

    // Ensuring gender input is lowercase for comparison
    for (int i = 0; i < gender.size(); i++) {
        gender[i] = tolower(gender[i]);
    }

    // Checking to see if is valid
    if (gender != "male" && gender != "female") {
        cout << "Please enter either male of female.";
        exit(1);
    }

    // Adding to UserInfo struct
    user->gender = gender;

    // Age
    string age_input;
    cout << "Enter your age: ";
    cin >> age_input;
    
    // Input validation
    if (age_input == "" || !isNumber(age_input)) {
        cout << "Bad input.";
        exit(1);
    }
   double age = stof(age_input);

    // Second age input validation
    if (age < 0 || age > 120) {
        cout << "Bad input, ensure age is a realistic positive number.";
        exit(1);
    }
    user->age = age;

    // Weight
    string weight_input;
    cout << "Enter your weight (kg): ";
    cin >> weight_input;

    // Input validation
    if (weight_input == "" || !isNumber(weight_input)) {
        cout << "Bad input.";
        exit(1);
    }
    user->weight = stof(weight_input); 

    // Waist
    string waist_input;
    cout << "Enter your waist size (cm): ";
    cin >> waist_input;

    // Input validation
    if (waist_input == "" || !isNumber(waist_input)) {
        cout << "Bad input.";
        exit(1);
    }
    user->waist = stof(waist_input);

    // Neck
    string neck_input;
    cout << "Enter your neck size (cm): ";
    cin >> neck_input;

    // Input validation
    if (neck_input == "" || !isNumber(neck_input)) {
        cout << "Bad input.";
        exit(1);
    }
    user->neck = stof(neck_input);

    // Height
    string height_input;
    cout << "Enter your height (cm): ";
    cin >> height_input;

    // Input validation
    if (height_input == "" || !isNumber(height_input)) {
        cout << "Bad input.";
        exit(1);
    }
    user->height = stof(height_input);

    // Lifestyle
    string lifestyle;
    cout << "Enter your lifestyle [sedentary, moderate, moderately active, active]: ";
    cin >> lifestyle;

    // Input validation
    if (lifestyle == "" || isNumber(lifestyle)) {
        cout << "Bad input.";
        exit(1);
    }

    // Ensuring lifestyle input is lowercase for comparison
    for (int i = 0; i < lifestyle.size(); i++) {
        lifestyle[i] = tolower(lifestyle[i]);
    }

    // Secondary input validation
    if (lifestyle != "sedentary" && lifestyle != "moderate" && lifestyle != "moderately active" && lifestyle != "active") {
        cout << "Please enter either sedentary, moderate, moderately active or active.";
        exit(1);
    }
    user->lifestyle = lifestyle;

    // If user is female
    if (user->gender == "female") {
        string hip_size;
        cout << "Enter your hip size (cm): ";
        cin >> hip_size;

        if (hip_size == "" || !isNumber(hip_size)) {
            cout << "Bad input.";
            exit(1);
        }
        user->hips = stof(hip_size);
    } else {
        user->hips = 0;
    }

    // Creating default values to indicate emptiness
    user->carbs = 0;
    user->protein = 0;
    user->fat = 0;
    user->calories = 0;
    user->bfp = 0;
    user->next = nullptr;

    // Appending the new user to the list
    UserListSingleton::getInstance().appendToList(user);

    cout << "\n";
} // end addUserInfo

void UserInfoManager::display(string username) {

    // Ensuring the input is all lowercase
    for (int i = 0; i < username.size(); i++) {
        username[i] = tolower(username[i]);
    }
    
    // Checking if we want to print all of the users
    if (username == "all") {
        UserListSingleton::getInstance().printList();

    // Printing a single name
    } else {

        // Getting the user from the list
        UserInfo* u = UserListSingleton::getInstance().getUser(username);

        // Ensuring function returned correct value
        if (u == nullptr) {
            cout << "Unnable to retrieve user. Ensure the list is not empty and the username is correct.";
            exit(1);
        }

        // If user found
        displaySingleUser(u);
    }
} // end display

void HealthAssistant::getUserDetail() {
    manager.addUserInfo();
}

void HealthAssistant::getBfp(string username) { 

    // Getting the user from the list
    UserInfo* u = UserListSingleton::getInstance().getUser(username);

    // Ensuring function returned correct value
    if (u == nullptr) {
        cout << "Unnable to retrieve user. Ensure the list is not empty and the username is correct.";
        exit(1);
    }

    // Calculating body fat percentage
    int bfp;
    string group;

    // Male
    if (u->gender == "male") {
        bfp = static_cast<int>(495 / (1.0324 - 0.19077 * log10(u->waist - u->neck) + 0.15456 * log10(u->height)) - 450);

    // Female
    } else {
        bfp = static_cast<int>(495 / (1.29579 - 0.35004 * log10(u->waist + u->hips - u->neck) + 0.22100 *log10(u->height)) - 450);
    }

    // Summarizing findings
    cout << "BFP: " << bfp << "\n" << endl;

    // Storing the bfp in the user variable
    UserInfo* ptr = UserListSingleton::getInstance().getUser(username);
    ptr->bfp = bfp;
}

void HealthAssistant::getDailyCalories(string username) {

    // Getting the user from the list
    UserInfo* u = UserListSingleton::getInstance().getUser(username);

    // Ensuring function returned correct value
    if (u == nullptr) {
        cout << "Unnable to retrieve user. Ensure the list is not empty and the username is correct.";
        exit(1);
    }

    int calories = 0;

    // Male
    if (u->gender == "male") {

        // 19-30
        if (19 <= u->age && u->age < 31) {

            if (u->lifestyle == "sedentary") {
                calories = 2400;
            } else if (u->lifestyle == "moderately active") {
                calories = 2600;
            } else {
                calories = 3000;
            }

        // 31-51
        } else if (31 <= u->age && u->age < 51) {

            if (u->lifestyle == "sedentary") {
                calories = 2200;
            } else if (u->lifestyle == "moderately active") {
                calories = 2400;
            } else {
                calories = 2800;
            }

        // 51+
        } else {
            
            if (u->lifestyle == "sedentary") {
                calories = 2000;
            } else if (u->lifestyle == "moderately active") {
                calories = 2200;
            } else {
                calories = 2400;
            }
        }

    // Female
    } else {

        // 19-30
        if (19 <= u->age && u->age < 31) {

            if (u->lifestyle == "sedentary") {
                calories = 2000;
            } else if (u->lifestyle == "moderately active") {
                calories = 2000;
            } else {
                calories = 2400;
            }

        // 31-51
        } else if (31 <= u->age && u->age < 51) {

            if (u->lifestyle == "sedentary") {
                calories = 1800;
            } else if (u->lifestyle == "moderately active") {
                calories = 2000;
            } else {
                calories = 2200;
            }

        // 51+
        } else {
            
            if (u->lifestyle == "sedentary") {
                calories = 1600;
            } else if (u->lifestyle == "moderately active") {
                calories = 1800;
            } else {
                calories = 2000;
            }
        }
    }

    // Storing the bfp in the user variable
    UserInfo* ptr = UserListSingleton::getInstance().getUser(username);
    ptr->calories = calories;
}

void HealthAssistant::getMealPrep(string username) {

    // Getting the user from the list
    UserInfo* u = UserListSingleton::getInstance().getUser(username);

    // Ensuring function returned correct value
    if (u == nullptr) {
        cout << "Unnable to retrieve user. Ensure the list is not empty and the username is correct.";
        exit(1);
    }

    // Ensuring calories have been calculated
    if (u->calories == 0) {
        cout << "Calories have not been calculated. Cannot create a meal plan." << endl;
        return;
    }

    u->carbs = (u->calories * 0.5) / 4;
    u->protein = (u->calories * 0.3) / 4;
    u->fat  = (u->calories * 0.2) / 9;
}

void HealthAssistant::display(string username) {
    manager.display(username);
}

void HealthAssistant::serialize(string filename) {

    // Constructing file in append mode
    ofstream out_file(filename, ios::app);

    // Check if the file is open
    if (!out_file.is_open()) {
        cout << "Failed to open output file" << endl;
        exit(1);
    }

    // Looping through each user in the linked list
    UserInfo* head = UserListSingleton::getInstance().getListInstance();

    while (head != NULL && head != nullptr) {
        
        // Data will be seralized in the order it appears in the UserInfo struct
        vector<string> data;
        data.push_back(to_string(head->age));
        data.push_back(to_string(head->weight));
        data.push_back(to_string(head->waist));
        data.push_back(to_string(head->neck));
        data.push_back(to_string(head->height));
        data.push_back(to_string(head->hips));
        data.push_back(to_string(head->carbs));
        data.push_back(to_string(head->protein));
        data.push_back(to_string(head->fat));
        data.push_back(to_string(head->calories));
        data.push_back(to_string(head->bfp));
        data.push_back(head->name);
        data.push_back(head->gender);
        data.push_back(head->lifestyle);

        // Writing the data for the user to the file
        for (int i = 0; i < data.size(); i++) {

            if (i == data.size() - 1) {
                out_file << data[i] << "\n";

            // Adding comma to seperate terms
            } else {
                out_file << data[i] << ",";
            }
        }

        // Moving to the next user
        head = head->next;
    }

    out_file.close();
}

void HealthAssistant::readFromFile(string filename) {
    manager.readFromFile(filename);
}

void HealthAssistant::deleteUser(string username) {
    manager.deleteUser(username);
}

void HealthAssistant::massLoadAndCompute(string filename) {
    
    // Reading in all of the users
    readFromFile(filename);

    // Looping through each user
    UserInfo* ptr = UserListSingleton::getInstance().getListInstance();
    while (ptr != nullptr) {

        // Computes statistics based on NAME ALONE
        // If there are duplicates, only the first one is calculated
        getBfp(ptr->name);
        getDailyCalories(ptr->name);
        getMealPrep(ptr->name);

        // Moving to next user
        ptr = ptr->next;
    }
}

/* MAIN */

int main() {
    HealthAssistant ha;

    ha.getUserDetail();

    cout << "Displaying all users" << endl;
    ha.display("all");

    // Computing bodyfat
    cout << "Computing" << endl;
    ha.getBfp("john");
    ha.getDailyCalories("john");
    ha.getMealPrep("john");

    cout << "Displaying all again" << endl;
    ha.display("all");

    // // Get user details from console input for 1st user 
    // ha.getUserDetail();

    // // Get user details from console input for 2nd user 
    // ha.getUserDetail();

    // // Display information for john (assuming that john is the name of the first user)
    // cout << "Displaying John only.\n" << endl;
    // ha.display("john");

    // // Display information for all the users
    // cout << "Displaying all.\n" << endl;
    // ha.display("all");

    // // Get bfp for the 1st user
    // cout << "Calculating metrics for John.\n" << endl;
    // ha.getBfp("john");
    // ha.getDailyCalories("john");
    // ha.getMealPrep("john");

    // // Write all the data for all the users to file 
    // cout << "Serializing data from HA." << endl;
    // ha.serialize("user_data.csv");

    // // Now let's get the data from file using a new instance of HealthAssistant
    // HealthAssistant ha2;
    // cout << "Reading from file into HA2.\n" << endl;
    // ha2.readFromFile("user_data.csv");

    // // Deleting Jack
    // cout << "Displaying all in HA2.\n" << endl;
    // ha2.display("all");

    // cout << "Deleting Jack in HA2\n" << endl;
    // ha2.deleteUser("jack"); // Assuming 2nd user is jack

    // cout << "Displaying all in HA2 (confirming deletion).\n" << endl;
    // ha2.display("all"); // Should only display for john at this point

    // // // Now use the first object to display all
    // cout << "Displaying all (FIRST OBJECT AGAIN)\n" << endl;
    // ha.display("all"); // Both john and jack should be present
 } // end main