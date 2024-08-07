/*
 * MIT License
 * 
 * Copyright (c) [2024] [Fars abdraouf]
 * 
 * يُسمح باستخدام هذا الملف وتعديله وتوزيعه وفقًا لشروط ترخيص MIT.
 * انظر ملف LICENSE للحصول على تفاصيل.
 */


// Libraries needed to run the code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>   

// Define the contact structure
typedef struct {
    char firstName[50];
    char lastName[50];
    char phoneNumber[20];
    char email[100];
} Contact;

// Function prototypes
void addContact();
void searchContact();
void modifyContact();
void displayContact(const Contact *contact);
void displayAllContacts();
void deleteContact();
void deleteAllContacts();
bool isValidPhoneNumber(const char *phoneNumber);
bool isValidEmail(const char *email);

int main() {
    int choice;
    do {
        printf("\n=============================\n");
        printf("  Contact Management System\n");
        printf("=============================\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Modify Contact\n");
        printf("4. Display All Contacts\n");
        printf("5. Delete Contact\n");
        printf("6. Delete All Contacts\n");
        printf("7. Exit\n");
        printf("=============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                searchContact();
                break;
            case 3:
                modifyContact();
                break;
            case 4:
                displayAllContacts();
                break;
            case 5:
                deleteContact();
                break;
            case 6:
                deleteAllContacts();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 7);

    return 0;
}

// Function to add a new contact
void addContact() {
    Contact newContact;
    printf("Enter first name: ");
    scanf("%s", newContact.firstName);
    printf("Enter last name: ");
    scanf("%s", newContact.lastName);

    // Validate phone number
    do {
        printf("Enter phone number: ");
        scanf("%s", newContact.phoneNumber);
    } while (!isValidPhoneNumber(newContact.phoneNumber));

    // Validate email
    do {
        printf("Enter email: ");
        scanf("%s", newContact.email);
    } while (!isValidEmail(newContact.email));

    FILE *file = fopen("contacts.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s;%s;%s;%s\n", newContact.firstName, newContact.lastName, newContact.phoneNumber, newContact.email);
    fclose(file);

    printf("Contact added successfully!\n");
}

// Function to validate phone number
bool isValidPhoneNumber(const char *phoneNumber) {
    // Check length
    if (strlen(phoneNumber) != 10) {
        printf("Phone number must be 10 digits.\n");
        return false;
    }

    // Check prefix
    if (phoneNumber[0] != '0' || (phoneNumber[1] != '5' && phoneNumber[1] != '6' && phoneNumber[1] != '7')) {
        printf("Phone number must start with 05, 06, or 07.\n");
        return false;
    }

    // Check if all characters are digits
    for (int i = 0; i < 10; ++i) {
        if (!isdigit(phoneNumber[i])) {
            printf("Phone number must contain only digits.\n");
            return false;
        }
    }

    return true;
}

// Function to validate email
bool isValidEmail(const char *email) {
    // Check if '@' and '.' exist
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');
    if (at == NULL || dot == NULL) {
        printf("Email must contain '@' and '.'.\n");
        return false;
    }

    // Check if '@' comes before '.'
    if (at > dot) {
        printf("Invalid email format.\n");
        return false;
    }

    return true;
}

// Function to search for a contact
void searchContact() {
    char searchStr[50];
    printf("Enter the starting characters of the contact's name or phone number: ");
    scanf("%s", searchStr);

    int len = strlen(searchStr);
    if (len == 0) {
        printf("Invalid input!\n");
        return;
    }

    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Contact contact;
    int found = 0;
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email) != EOF) {
        // Check if the first name or phone number starts with the search string
        if (strncmp(contact.firstName, searchStr, len) == 0 || strncmp(contact.phoneNumber, searchStr, len) == 0) {
            displayContact(&contact);
            found = 1;
        }
    }
    fclose(file);

    if (!found) {
        printf("No contacts found starting with '%s'.\n", searchStr);
    }
}

// Function to modify an existing contact
void modifyContact() {
    char searchStr[50];
    printf("Enter first name, last name, or phone number of contact to modify: ");
    scanf("%s", searchStr);

    FILE *file = fopen("contacts.txt", "r+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Contact contact;
    int found = 0;
    FILE *tempFile = fopen("temp_contacts.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email) != EOF) {
        if (strcmp(contact.firstName, searchStr) == 0 || strcmp(contact.lastName, searchStr) == 0 || strcmp(contact.phoneNumber, searchStr) == 0) {
            found = 1;

            int choice;
            printf("What would you like to modify?\n");
            printf("1.  Modify the entire contact\n");
            printf("2.  Modify the first name\n");
            printf("3.  Modify the last name\n");
            printf("4.  Modify the phone number\n");
            printf("5.  Modify the email\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new first name: ");
                    scanf("%s", contact.firstName);
                    printf("Enter new last name: ");
                    scanf("%s", contact.lastName);
                    do {
                        printf("Enter new phone number: ");
                        scanf("%s", contact.phoneNumber);
                    } while (!isValidPhoneNumber(contact.phoneNumber));
                    do {
                        printf("Enter new email: ");
                        scanf("%s", contact.email);
                    } while (!isValidEmail(contact.email));
                    break;
                case 2:
                    printf("Enter new first name: ");
                    scanf("%s", contact.firstName);
                    break;
                case 3:
                    printf("Enter new last name: ");
                    scanf("%s", contact.lastName);
                    break;
                case 4:
                    do {
                        printf("Enter new phone number: ");
                        scanf("%s", contact.phoneNumber);
                    } while (!isValidPhoneNumber(contact.phoneNumber));
                    break;
                case 5:
                    do {
                        printf("Enter new email: ");
                        scanf("%s", contact.email);
                    } while (!isValidEmail(contact.email));
                    break;
                default:
                    printf("Invalid choice!\n");
                    break;
            }
        }
        fprintf(tempFile, "%s;%s;%s;%s\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("contacts.txt");
        rename("temp_contacts.txt", "contacts.txt");
        printf("Contact modified successfully!\n");
    } else {
        printf("Contact not found!\n");
        remove("temp_contacts.txt");
    }
}

// Function to display all contacts
void displayAllContacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("--------------------------------------------------------------------------\n");
    printf("| %-12s | %-12s | %-12s | %-25s |\n", "First Name", "Last Name", "Phone Number", "Email");
    printf("--------------------------------------------------------------------------\n");

    Contact contact;
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email) != EOF) {
        printf("| %-12s | %-12s | %-12s | %-25s |\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email);
    }
    printf("--------------------------------------------------------------------------\n");
    fclose(file);
}

// Function to delete a specific contact
void deleteContact() {
    char searchStr[50];
    printf("Enter first name, last name, or phone number of contact to delete: ");
    scanf("%s", searchStr);

    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Contact contact;
    int found = 0;
    FILE *tempFile = fopen("temp_contacts.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email) != EOF) {
        if (strcmp(contact.firstName, searchStr) == 0 || strcmp(contact.lastName, searchStr) == 0 || strcmp(contact.phoneNumber, searchStr) == 0) {
            found = 1;
            continue; // Skip writing this contact to temporary file (effectively deleting it)
        }
        fprintf(tempFile, "%s;%s;%s;%s\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("contacts.txt");
        rename("temp_contacts.txt", "contacts.txt");
        printf("Contact deleted successfully!\n");
    } else {
        printf("Contact not found!\n");
        remove("temp_contacts.txt");
    }
}

// Function to delete all contacts
void deleteAllContacts() {
    char confirmation;
    printf("Are you sure you want to delete all contacts? (Enter 'y' or 'Y' to confirm, 'n' or 'N' to cancel): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        FILE *file = fopen("contacts.txt", "w");
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }
        fclose(file);
        printf("All contacts deleted successfully!\n");
    } else if (confirmation == 'n' || confirmation == 'N') {
        printf("Operation canceled. No contacts were deleted.\n");
    } else {
        printf("Invalid input. Operation canceled. No contacts were deleted.\n");
    }
}

// Function to display contact details
void displayContact(const Contact *contact) {
    printf("First Name: %s\n", contact->firstName);
    printf("Last Name: %s\n", contact->lastName);
    printf("Phone Number: %s\n", contact->phoneNumber);
    printf("Email: %s\n", contact->email);
    printf("\n");
}
