#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Contact
{
        string name;
        string contact;
        string email;
        string address;
        bool favourite;
};

struct Node
{
        Contact contact;
        Node *left;
        Node *right;
};

Node *createNode(string name , string contact , string email,string address)
{
        Node *newNode = new Node;
        newNode->contact.name = name;
        newNode->contact.contact = contact;
        newNode->contact.email = email;
        newNode->contact.address = address;
        newNode->contact.favourite = false;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
}

Node* insertNode(Node *root , string name , string contact , string email,string address)
{
        if (root == nullptr)
        {  
            return createNode(name ,contact ,email,address);
        }
        if (name < root->contact.name)
        {
            root->left = insertNode(root->left, name , contact , email,address);
        }
        else if(name > root->contact.name)
        {
            root->right = insertNode(root->right, name , contact ,email,address);
        }
        return root;
}

void loadContact(Node *&root)
{
    ifstream infile("contacts.txt");

    if(!infile.is_open())
    {
        return;
    }
    string line;
    while(getline(infile,line))
    {
        string name, contact, email, address;

        stringstream ss(line);

        getline(ss, name, ',');
        getline(ss, contact, ',');
        getline(ss, email, ',');
        getline(ss, address, ',');

        root = insertNode(root, name, contact, email, address);
    }

    infile.close();
}

void InorderDisplay(Node *root)
{
        if ( root == nullptr )
        {
            return;
        }
        InorderDisplay(root->left);
        cout << "-----------------------------" << endl;
        cout << "Name    : " << root->contact.name << endl;
        cout << "Contact : " << root->contact.contact << endl;
        cout << "Email   : " << root->contact.email << endl;
        cout << "Address : " << root->contact.address << endl;
        cout << "-----------------------------" << endl;
        InorderDisplay(root->right);
}

Node *searchContactbyName ( Node *root , string name)
{
        if (root == nullptr || root->contact.name == name)
        {
           return root;
        }
        if(root->contact.name < name)
        {
            return searchContactbyName(root->right , name);
        }
        else 
        {
            return searchContactbyName(root->left , name);
        }

}

Node *searchContactbyContact ( Node *root , string contact)
{
        if (root == nullptr || root->contact.contact == contact)
        {
            return root;
        }

        Node *result = searchContactbyContact(root->right , contact);
        if(result != nullptr)
        {
           return result;  
        }
        return searchContactbyContact(root->left , contact);
}

Node *deleteContact(Node *root , string name)
{
        if (root == nullptr)
        {
            return root;
        }
        if (name < root->contact.name)
        {
            root->left = deleteContact(root->left , name);
        }
        else if(name > root->contact.name)
        {
            root->right = deleteContact(root->right , name);
        }
        else
        {
           if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
               return temp;
            }
            else if(root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                return temp;
            }
            Node *temp = root->right;
            while(temp && temp->left != nullptr)
            {
                temp = temp->left;
            }
            root->contact = temp->contact;
            root->right = deleteContact(root->right , temp->contact.name);

        }
    return root;
}

void setFavourite(Node *root , string name)
{
    Node *result = searchContactbyName(root,name);
    if(result != nullptr)
    {
            cout << "-----------------------------" << endl;
            cout << "Name    : " << result->contact.name << endl;
            cout << "Contact : " << result->contact.contact << endl;
            cout << "Favoutite : (1) Yes or (2) No" << endl;
            int choice;
            cin >> choice;
            if(choice == 1)
            {
                result->contact.favourite = true;
                cout << "Contact marked as favourite.\n";
            }
            else if(choice == 2)
            {
                result->contact.favourite = false;
                cout << "Contact unmarked as favourite.\n";
            }
            cout << "-----------------------------" << endl;
    }
    else
    {
        cout << "Contact not Found.\n";
    }
}

void InorderDisplayFavourite(Node *root)
{
        if ( root == nullptr )
        {
            return;
        }
        InorderDisplayFavourite(root->left);
        if(root->contact.favourite)
        {
            cout << "-----------------------------" << endl;
            cout << "Name    : " << root->contact.name << endl;
            cout << "Contact : " << root->contact.contact << endl;
            cout << "Email   : " << root->contact.email << endl;
            cout << "Address : " << root->contact.address << endl;
            cout << "-----------------------------" << endl;
        }
        InorderDisplayFavourite(root->right);
}

void saveContactsToFile(Node *root,ofstream &outfile)
{

    if(root == nullptr)
    {
        return;
    }
    saveContactsToFile(root->left,outfile);
    outfile << root->contact.name << ","<< root->contact.contact << ","<< root->contact.email << ","<< root->contact.address << endl;
    saveContactsToFile(root->right,outfile);
    
}

void saveContacts(Node *root)
{
    ofstream outfile("contacts.txt");

    if(!outfile.is_open())
    {
        return;
    }

    saveContactsToFile(root, outfile);

    outfile.close();
}

void editContact(Node *root, string name)
{
    Node *result = searchContactbyName(root, name);

    if(result == nullptr)
    {
        cout << "Contact not found.\n";
        return;
    }
    cout << "\nWhat do you want to edit?\n";
    cout << "1. Contact Number\n";
    cout << "2. Email\n";
    cout << "3. Address\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    switch(choice)
    {
        case 1:
            {
                string contact;
                cout << "Enter the new contact Number : ";
                cin >> contact;
                if(searchContactbyContact(root,contact) != nullptr)
                {
                    cout << "Contact number already exists. Please enter a unique number.\n";
                    break;
                }
                result->contact.contact = contact;
                break;
            }

        case 2:
            {
                string email;

                while(true)
                {
                    cout << "Enter the new email: ";
                    cin >> email;

                    try
                    {
                        if(email.find("@gmail.com") == string::npos)
                         {
                            throw "Invalid email address!";
                        }

                         break;
                    }
                    catch(const char* msg)
                    {
                        cout << msg << " Please try again.\n";
                     }
                }

                result->contact.email = email;

                cout << "Email updated successfully!\n";
                break;
            }
        
        case 3:
            {
                string address;

                cout << "Enter the new address: ";
                cin.ignore();
                getline(cin, address);

                result->contact.address = address;

                cout << "Address updated successfully!\n";
                break;
            }
    }
    
}

int main()
{
    Node *root = nullptr;
    int choice;
    loadContact(root);
    do
    {
        cout << "\n========== CONTACT MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Contact\n";
        cout << "2. Search Contact\n";
        cout << "3. Delete Contact\n";
        cout << "4. Display Contacts\n";
        cout << "5. Mark / Unmark Favourite\n";
        cout << "6. Display Favourite Contacts\n";
        cout << "7. Edit Contact\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            {
                string name, contact, email,address;

                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter contact number: ";
                cin >> contact;

                if(searchContactbyContact(root,contact) != nullptr)
                {
                    cout << "Contact number already exists. Please enter a unique contact number.\n";
                    break;
                }               
                
                while(true)
                {
                    cout << "Enter email: ";
                    cin >> email;
                    try
                    {
                        if(email.find("@gmail.com") == string::npos)
                        {
                            throw "Invalid email Address!. " ;
                        }
                        break;
                    }
                    catch(const char* msg)
                    {
                        cout << msg << " Please try again.\n";
                    }
                }

                cout << "Enter address: ";
                cin.ignore();
                getline(cin, address);

                root = insertNode(root, name, contact, email,address);

                cout << "Contact added successfully!\n";
                break;
            }

            case 2:
            {
                string name, contact;
                int choice2;

                cout << "Search by Name (1) or Contact Number (2): ";
                cin >> choice2;

                switch(choice2)
                {
                        case 1 :
                        {
                            cout << "Enter name to search: ";
                            cin >> name;

                            Node *result = searchContactbyName(root, name);

                            if(result != nullptr)
                            {
                                cout << "Contact found:\n";
                                cout << "Name    : " << result->contact.name << endl;
                                cout << "Contact : " << result->contact.contact << endl;
                                cout << "Email   : " << result->contact.email << endl;
                                cout << "Address : " << result->contact.address << endl;
                            }
                            else
                            {
                                cout << "Contact not found.\n";
                            }

                            break;
                        }

                        case 2:
                        {
                            cout << "Enter contact number to search: ";
                            cin >> contact;

                            Node *result = searchContactbyContact(root, contact);

                            if(result != nullptr)
                            {
                                cout << "Contact found:\n";
                                cout << "Name    : " << result->contact.name << endl;
                                cout << "Contact : " << result->contact.contact << endl;
                                cout << "Email   : " << result->contact.email << endl;
                                cout << "Address : " << result->contact.address << endl;
                            }
                            else
                            {
                                cout << "Contact not found.\n";
                            }

                            break;
                        }

                        default:
                            cout << "Invalid choice! Please try again.\n";
                }


                break;
            }

            case 3:
            {
                string name;

                cout << "Enter name to delete: ";
                cin >> name;

                Node *result = searchContactbyName(root, name);

                if(result != nullptr)
                {
                    root = deleteContact(root, name);
                    cout << "Contact deleted successfully!\n";
                }
                else
                {
                    cout << "Contact not found.\n";
                }

                break;
            }

            case 4:
            {
                if(root == nullptr)
                {
                    cout << "No contacts available.\n";
                }
                else
                {
                    cout << "\n========== CONTACTS ==========\n";
                    InorderDisplay(root);
                }

                break;
            }

            case 5:
            {
                cout << "Enter name to mark/unmark favourite: ";
                string name;
                cin >> name;
                setFavourite(root, name);
                break;
            }

            case 6:
            {
                if(root == nullptr)
                {
                    cout << "No contacts available.\n";
                }
                else
                {
                    cout << "\n========== FAVOURITE CONTACTS ==========\n";
                    InorderDisplayFavourite(root);
                }

                break;
            }
            case 7:
            {
                string name;
                cout << "Enter the name of contact ";
                cin >> name;
                editContact(root,name);
                break;
            }

            case 8:
                saveContacts(root);
                cout << "Exiting Contact Management System...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }

    } while(choice != 8 );

    return 0;
}