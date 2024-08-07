/*
 * MIT License
 * 
 * Copyright (c) [2024] [Fars abdraouf]
 * 
 * يُسمح باستخدام هذا الملف وتعديله وتوزيعه وفقًا لشروط ترخيص MIT.
 * انظر ملف LICENSE للحصول على تفاصيل.
 */


// المكتبات اللازمة لعمل الكود
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// هيكلة جهات الاتصال
typedef struct {
    char firstName[50]; // الاسم الاول 
    char lastName[50]; // الاسم الاخير
    char phoneNumber[20]; // رقم الهاتف
    char email[100]; // البريد الالكتروني
} Contact;

// الدوال الرئيسية 
void addContact(); //اضافة جهة اتصال 
void searchContact(); // بحث عن جهة اتصال
void modifyContact(); // تعديل جهة اتصال
void displayContact(const Contact *contact); // عرض جهة اتصال
void displayAllContacts(); // عرض كل الجهات
void deleteContact(); // حذف جهة اتصال
void deleteAllContacts(); // حذف كل الجهات

// دالة للتحقق من رقم الهاتف
bool isValidPhoneNumber(const char *phoneNumber);

// دالة للتحقق من البريد الالكتروني
bool isValidEmail(const char *email);

// الدالة الرئيسية
int main() {
    int choice;
    do {
        // عرض قائمة الخيارات
        printf("\nContact Management System\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Modify Contact\n");
        printf("4. Display All Contacts\n");
        printf("5. Delete Contact\n");
        printf("6. Delete All Contacts\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        // معالجة اختيار المستخدم
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

// دالة لاضافة جهة الاتصال
void addContact() {
    Contact newContact;
    printf("Enter first name: ");
    scanf("%s", newContact.firstName);
    printf("Enter last name: ");
    scanf("%s", newContact.lastName);

    // التحقق من رقم الهاتف
    do {
        printf("Enter phone number (10 digits starting with 05, 06, or 07): ");
        scanf("%s", newContact.phoneNumber);
    } while (!isValidPhoneNumber(newContact.phoneNumber));

    // التحقق من البريد الالكتروني
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

// دالة للتحقق من رقم الهاتف
bool isValidPhoneNumber(const char *phoneNumber) {
    // التحقق من ان رقم الهاتف 10 ارقام
    if (strlen(phoneNumber) != 10) {
        printf("Phone number must be 10 digits.\n");
        return false;
    }

    // التحقق من ان رقم الهاتف يبدا ب 05, 06, 07
    if (phoneNumber[0] != '0' || (phoneNumber[1] != '5' && phoneNumber[1] != '6' && phoneNumber[1] != '7')) {
        printf("Phone number must start with 05, 06, or 07.\n");
        return false;
    }

    // التحقق من ان كل عناصر ارقام
    for (int i = 0; i < 10; ++i) {
        if (!isdigit(phoneNumber[i])) {
            printf("Phone number must contain only digits.\n");
            return false;
        }
    }

    return true;
}

// دالة للتحقق من البريد الالكتروني
bool isValidEmail(const char *email) {
    // التحقق من ان البريد الالكتروني يتواجد في @ و .
    char *at = strchr(email, '@');
    char *dot = strchr(email, '.');
    if (at == NULL || dot == NULL) {
        printf("Email must contain '@' and '.'.\n");
        return false;
    }

    // التحقق من ان البريد الالكتروني يتواجد قبل @ و .
    if (at > dot) {
        printf("Invalid email format.\n");
        return false;
    }

    return true;
}

// دالة للبحث عن جهات الاتصال
void searchContact() {
    char searchStr[50];
    printf("Enter the starting characters or number of the contact's name: ");
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
        // تحقق من بداية الاسم أو الرقم
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

// دالة لتعديل جهات الاتصال
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
            printf("1. Modify the entire contact\n");
            printf("2. Modify the first name\n");
            printf("3. Modify the last name\n");
            printf("4. Modify the phone number\n");
            printf("5. Modify the email\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new first name: ");
                    scanf("%s", contact.firstName);
                    printf("Enter new last name: ");
                    scanf("%s", contact.lastName);
                    do {
                        printf("Enter new phone number (10 digits starting with 05, 06, or 07): ");
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
                        printf("Enter new phone number (10 digits starting with 05, 06, or 07): ");
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

// دالة لعرض جميع جهات الاتصال
void displayAllContacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Contact contact;
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", contact.firstName, contact.lastName, contact.phoneNumber, contact.email) != EOF) {
        displayContact(&contact);
    }

    fclose(file);
}

// دالة لحذف جهة اتصال محددة
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
            continue; // تخطي كتابة جهة الاتصال هذه في ملف مؤقت (حذفها)
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

// دالة لحذف جميع جهات الاتصال
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


// دالة لعرض تفاصيل جهات الاتصال
void displayContact(const Contact *contact) {
    printf("First Name: %s\n", contact->firstName);
    printf("Last Name: %s\n", contact->lastName);
    printf("Phone Number: %s\n", contact->phoneNumber);
    printf("Email: %s\n", contact->email);
    printf("\n");
}
