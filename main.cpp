// CRM for an insurance company in C++
// Author: Rosa Santelia


#include <bits/stdc++.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <limits>

using namespace std;

// Maximum number of customers and interactions using a constant -> It is possible to expand it based on the customer's specifications
const int max_customers = 100;
const int max_interactions = 500;

// I create the Base record_CRM Class
class record_CRM {
public:
    virtual void display() const = 0; // Pure virtual function
    virtual void writeToFile(ofstream& file) const = 0; // Pure virtual function for CSV
    virtual ~record_CRM() {} // Virtual destructor
};

// I create the Customer Class that contains all contact info of each customer
class Customer : public record_CRM {
public:
    int customer_ID;
    char name[50];
    char surname[50];
    char company_name[200];
    char company_mail[50];
    int mobile_phone;
    char country[50];
    char address[100];

    // Constructor to initialize customer details
    Customer(int _customer_ID, char _name[], char _surname[], char _company_name[], char _company_mail[], int _mobile_phone, char _country[], char _address[])
        : customer_ID(_customer_ID), mobile_phone(_mobile_phone) {
        strcpy(name, _name);
        strcpy(surname, _surname);
        strcpy(company_name, _company_name);
        strcpy(company_mail, _company_mail);
        strcpy(country, _country);
        strcpy(address, _address);
    }

    // Copy Constructor
    Customer(const Customer& other) : customer_ID(other.customer_ID), mobile_phone(other.mobile_phone) {
        strcpy(name, other.name);
        strcpy(surname, other.surname);
        strcpy(company_name, other.company_name);
        strcpy(company_mail, other.company_mail);
        strcpy(country, other.country);
        strcpy(address, other.address);
    }

    // Function to display customer details
    void display() const override {
        cout << "Customer ID: " << customer_ID << endl;
        cout << "Name: " << name << " " << surname << endl;
        cout << "Company: " << company_name << endl;
        cout << "Email: " << company_mail << endl;
        cout << "Phone: " << mobile_phone << endl;
        cout << "Country: " << country << endl;
        cout << "Address: " << address << endl;
    }

    // Function to write customer data to a CSV file
    void writeToFile(ofstream& file) const override {
        file << customer_ID << "," << name << "," << surname << "," << company_name << "," << company_mail << "," << mobile_phone << "," << country << "," << address << "\n";
    }

    // Friend function to read customer data from a CSV file
    friend Customer readCustomerFromFile(ifstream& file);
};

// Friend function definition
Customer readCustomerFromFile(ifstream& file) {
    int id, mobile;
    char name[50], surname[50], company_name[200], company_mail[50], country[50], address[100];
    if (file.good()) {
        file >> id;
        file.ignore();
        file.getline(name, 50, ',');
        file.getline(surname, 50, ',');
        file.getline(company_name, 200, ',');
        file.getline(company_mail, 50, ',');
        file >> mobile;
        file.ignore();
        file.getline(country, 50, ',');
        file.getline(address, 100);
        return Customer(id, name, surname, company_name, company_mail, mobile, country, address);
    }
    return Customer(0, "", "", "", "", 0, "", "");
}

// I create the Customer_Interaction Class that contains all info about appointments, calls and contracts with each customer
class Customer_Interaction : public record_CRM {
public:
    int customer_interaction_ID;
    int customer_ID;
    char type[50];
    char date[15];
    char time[15];
    char place[250];
    char description[200];
    char contract[100];

    // Constructor to initialize customer interactions details
    Customer_Interaction(int _customer_interaction_ID, int _customer_ID, char _type[], char _date[], char _time[], char _place[], char _description[], char _contract[])
        : customer_interaction_ID(_customer_interaction_ID), customer_ID(_customer_ID) {
        strcpy(type, _type);
        strcpy(date, _date);
        strcpy(time, _time);
        strcpy(place, _place);
        strcpy(description, _description);
        strcpy(contract, _contract);
    }

    // Default Constructor
    Customer_Interaction() : customer_interaction_ID(0), customer_ID(0) {
        strcpy(type, "");
        strcpy(date, "");
        strcpy(time, "");
        strcpy(place, "");
        strcpy(description, "");
        strcpy(contract, "");
    }

    //Copy constructor
    Customer_Interaction(const Customer_Interaction& other) : customer_interaction_ID(other.customer_interaction_ID), customer_ID(other.customer_ID) {
        strcpy(type, other.type);
        strcpy(date, other.date);
        strcpy(time, other.time);
        strcpy(place, other.place);
        strcpy(description, other.description);
        strcpy(contract, other.contract);
    }

    // Function to display interaction details
    void display() const override {
        cout << "Interaction ID: " << customer_interaction_ID << endl;
        cout << "Customer ID: " << customer_ID << endl;
        cout << "Type: " << type << endl;
        cout << "Date: " << date << endl;
        cout << "Time: " << time << endl;
        cout << "Place: " << place << endl;
        cout << "Description: " << description << endl;
        cout << "Contract: " << contract << endl;
    }

    // Function to write interaction data to a CSV file
    void writeToFile(ofstream& file) const override {
        file << customer_interaction_ID << "," << customer_ID << "," << type << "," << date << "," << time << "," << place << "," << description << "," << contract << "\n";
    }

    // Friend function to read interaction data from a CSV file
    friend Customer_Interaction readInteractionFromFile(ifstream& file);
};

// Friend function definition
Customer_Interaction readInteractionFromFile(ifstream& file) {
    int id, customerId;
    char type[50], date[15], time[15], place[250], description[200], contract[100];
    if (file.good()) {
        file >> id;
        file.ignore();
        file >> customerId;
        file.ignore();
        file.getline(type, 50, ',');
        file.getline(date, 15, ',');
        file.getline(time, 15, ',');
        file.getline(place, 250, ',');
        file.getline(description, 200, ',');
        file.getline(contract, 100);
        return Customer_Interaction(id, customerId, type, date, time, place, description, contract);
    }
    return Customer_Interaction();
}

// I create the CRM System Class to manage both customers and interactions
class CRM_System {
private:
    record_CRM* entities[max_customers + max_interactions]; // Array to store both customers and interactions
    char company_name[200];
    char operator_name[50];
    int next_customer_id;
    int next_interaction_id;
    int entity_count;

public:
    // Constructor to initialize CRM records
    CRM_System(char _company_name[], char _operator_name[])
        : next_customer_id(1), next_interaction_id(1), entity_count(0) {
        strcpy(company_name, _company_name);
        strcpy(operator_name, _operator_name);
    }

    // Add a new customer
    void addCustomer(char name[], char surname[], char company_name[], char company_mail[], int mobile_phone, char country[], char address[]) {
        if (entity_count < max_customers + max_interactions) {
            entities[entity_count++] = new Customer(next_customer_id++, name, surname, company_name, company_mail, mobile_phone, country, address);
            cout << "Customer added successfully" << endl;
        } else {
            cout << "Maximum number of entities reached" << endl;
        }
    }

    // Add a new interaction
    void addInteraction(int customer_id, char type[], char date[], char time[], char place[], char description[], char contract[]) {
        if (entity_count < max_customers + max_interactions) {
            entities[entity_count++] = new Customer_Interaction(next_interaction_id++, customer_id, type, date, time, place, description, contract);
            cout << "Interaction added successfully" << endl;
        } else {
            cout << "Maximum number of entities reached" << endl;
        }
    }

    // Modify a customer by surname
    void modifyCustomer(const char* surname) {
        for (int i = 0; i < entity_count; ++i) {
            Customer* customer = dynamic_cast<Customer*>(entities[i]);
            if (customer && strcmp(customer->surname, surname) == 0) {
                cout << "Modify details for customer: " << surname << endl;
                cin.ignore(); // Clear buffer
                cout << "New Name: ";
                cin.getline(customer->name, 50);
                cout << "New Surname: ";
                cin.getline(customer->surname, 50);
                cout << "New Company Name: ";
                cin.getline(customer->company_name, 200);
                cout << "New Company Email: ";
                cin.getline(customer->company_mail, 50);

                string mobile_phone_str;
                while (true) {
                    cout << "New Mobile Phone: ";
                    getline(cin, mobile_phone_str);
                    try {
                        customer->mobile_phone = stoi(mobile_phone_str);
                        break; // Input valido, esci dal ciclo
                    } catch (const invalid_argument& e) {
                        cout << "Invalid input. Please enter a valid number.\n";
                    } catch (const out_of_range& e) {
                        cout << "Number out of range. Please enter a valid number.\n";
                    }
                }

                cout << "New Country: ";
                cin.getline(customer->country, 50);
                cout << "New Address: ";
                cin.getline(customer->address, 100);

                cout << "Customer details updated successfully!" << endl;
                return;
            }
        }
        cout << "Customer not found!" << endl;
    }

    // Modify interaction details
    void modifyInteraction(int interaction_id) {
        for (int i = 0; i < entity_count; ++i) {
            Customer_Interaction* interaction = dynamic_cast<Customer_Interaction*>(entities[i]);
            if (interaction && interaction->customer_interaction_ID == interaction_id) {
                cout << "Modify details for interaction ID: " << interaction_id << endl;
                cin.ignore();
                cout << "New Type: ";
                cin.getline(interaction->type, 50);
                cout << "New Date: ";
                cin.getline(interaction->date, 15);
                cout << "New Time: ";
                cin.getline(interaction->time, 15);
                cout << "New Place: ";
                cin.getline(interaction->place, 250);
                cout << "New Description: ";
                cin.getline(interaction->description, 200);
                cout << "New Contract: ";
                cin.getline(interaction->contract, 100);

                cout << "Interaction details updated successfully!" << endl;
                return;
            }
        }
        cout << "Interaction not found!" << endl;
    }

    // Delete a customer (by ID)
    void deleteCustomer(int customer_id) {
        for (int i = 0; i < entity_count; ++i) {
            Customer* customer = dynamic_cast<Customer*>(entities[i]);
            if (customer && customer->customer_ID == customer_id) {
                delete entities[i]; // Deallocate memory for the customer
                for (int j = i; j < entity_count - 1; ++j) {
                    entities[j] = entities[j + 1]; // Shift the remaining entities
                }
                entity_count--; // Decrease the count
                cout << "Customer deleted successfully" << endl;
                return;
            }
        }
        cout << "Customer not found!" << endl;
    }

    // Delete an interaction
    void deleteInteraction(int interaction_id) {
        for (int i = 0; i < entity_count; ++i) {
            Customer_Interaction* interaction = dynamic_cast<Customer_Interaction*>(entities[i]);
            if (interaction && interaction->customer_interaction_ID == interaction_id) {
                delete entities[i]; // Deallocate memory for the interaction
                for (int j = i; j < entity_count - 1; ++j) {
                    entities[j] = entities[j + 1]; // Shift the remaining entities
                }
                entity_count--; // Decrease the count
                cout << "Interaction deleted successfully" << endl;
                return;
            }
        }
        cout << "Interaction not found!" << endl;
    }

    // Display all entities
    void displayEntities() const {
        if (entity_count == 0) {
            cout << "No entities found" << endl;
            return;
        }
        for (int i = 0; i < entity_count; ++i) {
            entities[i]->display();
            cout << "------------------------" << endl;
        }
    }

    // Save all records to a CSV file
    void saveToFile(const char* filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
        for (int i = 0; i < entity_count; ++i) {
            entities[i]->writeToFile(file);
        }
        file.close(); // Explicitly closing the file
        cout << "Data saved successfully!" << endl;
    }
};

// Main Function for the User Interaction
int main() {
    string assurance_name;
    string operator_name;

    printf("InsuraPro Solutions welcomes you to your CRM\n");
    printf("Enter the company name of your insurance company: ");
    getline(cin, assurance_name);
    printf("Enter your first name: ");
    getline(cin, operator_name);

    cout << "Hello " << operator_name << " of " << assurance_name << endl;

    CRM_System crm(assurance_name.data(), operator_name.data());

    bool running = true;
    while (running) {
        string choice_str;
        int choice;

        cout << "\nCRM Menu: 📲\n";
        cout << "🧔 1. Add Customer\n";
        cout << "➕ 2. Add Interaction\n";
        cout << "☑️ 3. Display All Entities\n";
        cout << "✏️ 4. Modify Customer\n";
        cout << "✒️ 5. Modify Interaction\n";
        cout << "✖️ 6. Delete Customer\n";
        cout << "❌ 7. Delete Interaction\n";
        cout << "💾 8. Save Data to File\n";
        cout << "⛔ 9. Exit\n";
        cout << "Enter your choice: ";

        getline(cin, choice_str);

        try {
            choice = stoi(choice_str);
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        } catch (const out_of_range& e) {
            cout << "Number out of range. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                char name[50], surname[50], company_name[200], company_mail[50], country[50], address[100];
                string mobile_phone_str;
                int mobile_phone;
                cout << "Enter Customer Details\n";
                cout << "Name: ";
                cin.getline(name, 50);
                cout << "Surname: ";
                cin.getline(surname, 50);
                cout << "Company Name: ";
                cin.getline(company_name, 200);
                cout << "Company Email: ";
                cin.getline(company_mail, 50);

                while (true) {
                    cout << "Mobile Phone: ";
                    getline(cin, mobile_phone_str);
                    try {
                        mobile_phone = stoi(mobile_phone_str);
                        break;
                    } catch (const invalid_argument& e) {
                        cout << "Invalid input. Please enter a valid number.\n";
                    } catch (const out_of_range& e) {
                        cout << "Number out of range. Please enter a valid number.\n";
                    }
                }

                cout << "Country: ";
                cin.getline(country, 50);
                cout << "Address: ";
                cin.getline(address, 100);
                crm.addCustomer(name, surname, company_name, company_mail, mobile_phone, country, address);
                break;
            }
            case 2: {
                int customer_id;
                char type[50], date[15], time[15], place[250], description[200], contract[100];
                cout << "Enter Interaction Details\n";
                cout << "Customer ID: ";
                cin >> customer_id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Type (Appointment, Mail or Call): ";
                cin.getline(type, 50);
                cout << "Date (YYYY/MM/DD): ";
                cin.getline(date, 15);
                cout << "Time (HH:MM): ";
                cin.getline(time, 15);
                cout << "Place (Address, Town): ";
                cin.getline(place, 250);
                cout << "Description: ";
                cin.getline(description, 200);
                cout << "Contract: (Number) ";
                cin.getline(contract, 100);
                crm.addInteraction(customer_id, type, date, time, place, description, contract);
                break;
            }
            case 3: {
                crm.displayEntities();
                break;
            }
            case 4: {
                char surname[50];
                cout << "Enter Surname to modify: ";
                cin.getline(surname, 50);
                crm.modifyCustomer(surname);
                break;
            }
            case 5: {
                int interaction_id;
                cout << "Enter Interaction ID to modify: ";
                cin >> interaction_id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                crm.modifyInteraction(interaction_id);
                break;
            }
            case 6: {
                int customer_id;
                cout << "Enter Customer ID to delete: ";
                cin >> customer_id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                crm.deleteCustomer(customer_id);
                break;
            }
            case 7: {
                int interaction_id;
                cout << "Enter Interaction ID to delete: ";
                cin >> interaction_id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                crm.deleteInteraction(interaction_id);
                break;
            }
        case 8: {
            char filename[100];
            cout << "Enter filename to save data: ";
            cin.getline(filename, 100);
            crm.saveToFile(filename);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "choice: " << choice << endl;
            break;
        }
            case 9: {
                running = false;
                cout << "See you soon 👋\n";
                break;
            }
            default: {
                cout << "Invalid choice, please try again.\n";
            }
        }
    }

    return 0;
}