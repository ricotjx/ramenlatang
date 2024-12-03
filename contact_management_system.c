#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONENO_LEN 15
#define EMAIL_LEN 50
#define ADDRESS_LEN 50

// define a structure to store individual contact details
typedef struct Contact
{
	char name[NAME_LEN];
	char phoneNo[PHONENO_LEN];
	char email[EMAIL_LEN];
	char address[ADDRESS_LEN];
    int isFavorite; // 1 for favorite, 0 for non-favorite
} Contact;

Contact contacts[MAX_CONTACTS]; // global variable for a contact

// functions prototypes
void removeNewLine(char *str);
void toLowercase(char *str);
int isValidName(const char *name);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
void addName(Contact contacts[], int *contact_count);
void addPhone(Contact contacts[], int *contact_count);
void addEmail(Contact contacts[], int *contact_count);
void addAddress(Contact contacts[], int *contact_count);
void addContact(Contact contacts[], int *contact_count);
void editContact(Contact contacts[], int contact_count);
void deleteContact(Contact contacts[], int *contact_count);
void searchContact(Contact contacts[], int *contact_count);
int searchByName(Contact contacts[], int *contact_count, int matchedIndices[]);
int searchByPhoneNo(Contact contacts[], int *contact_count, int matchedIndices[]);
int searchByEmail(Contact contacts[], int *contact_count, int matchedIndices[]);
void sortContacts(Contact contacts[], int contact_count);
void sortByName(Contact contacts[], int contact_count);
void sortByPhoneNumber(Contact contacts[], int n);
void sortByEmail(Contact contacts[], int n);
void displayContacts(Contact contacts[], int contact_count);
void saveContacts(Contact contacts[], int contact_count);
void loadContacts(Contact contacts[], int *contact_count);
void displayContactNames(Contact contacts[], int contact_count);
void displaySortedContacts(Contact contacts[], int contact_count);
void favorites(Contact contacts[], int contact_count, int *favorite_count);
void markAsFavorite(Contact contacts[], int contact_count, int *favorite_count);
void removeFavorite(Contact contacts[], int contact_count, int *favorite_count);
void displayFavorites(Contact contacts[], int contact_count, int *favorite_count);
int partialSearch(Contact contacts[], int contact_count, const char *searchTerm, int matchedIndices[]);
int isDuplicate(Contact contacts[], int contact_count, Contact newContact, int *duplicateIndex);

int main()
{
    int contact_count = 0;
    int favorite_count = 0;
    int i = 0;
    int j = 0;

    loadContacts(contacts, &contact_count); // load contacts when application starts
    displayContactNames(contacts, contact_count);   // display contact names when application starts

    int choice;
    char exitChoice;

    // check if contact list is full
    if (contact_count >= MAX_CONTACTS) {
        printf("CONTACT LIST IS FULL! PLEASE DELETE SOME CONTACTS FIRST.\n");
        deleteContact(contacts, &contact_count);
    }
    else
    {
        do
        {
            // MAIN MENU
            printf("\nCONTACT MANAGEMENT SYSTEM\n");
            printf("-------------------------\n");
            printf("1. ADD CONTACT\n");
            printf("2. EDIT CONTACT\n");
            printf("3. DELETE CONTACT\n");
            printf("4. SEARCH CONTACT\n");
            printf("5. SORT CONTACTS\n");
            printf("6. DISPLAY CONTACTS\n");
            printf("7. MANAGE FAVORITE CONTACTS\n");
            printf("8. EXIT\n\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            while (getchar() != '\n');  // clear input buffer after scanf

            // allow user to choose what they would like to do using switch case
            switch (choice)
            {
                case 1:
                    addContact(contacts, &contact_count); // allow user to add new contact
                    break;
                case 2:
                    editContact(contacts, contact_count); // allow user to edit contact
                    break;
                case 3:
                    deleteContact(contacts, &contact_count); // allow user to delete contact
                    break;
                case 4:
                    searchContact(contacts, &contact_count); // allow user to search contact
                    break;
                case 5:
                    sortContacts(contacts, contact_count); // allow user to sort contacts
                    displaySortedContacts(contacts, contact_count); // display sorted contacts
                    break;
                case 6:
                    displayContacts(contacts, contact_count); // allow user to display contacts
                    break;
                case 7:
                    favorites(contacts, contact_count, &favorite_count); // allow user to manage favorite contacts
                    break;
                case 8:
                    printf("\nEXITED SUCCESSFULLY\n"); // allow user to exit application
                    return 0;
                default:
                    printf("\nINVALID CHOICE\n");
            }

            printf("\nEnter 'E' to exit or 'R' to return to the main menu: "); // allow user to choose whether to exit or return to main menu after completing each task
            exitChoice = getchar();
            while (getchar() != '\n');

            if (exitChoice != 'R' && exitChoice != 'r')
            {
                printf("\nEXITED SUCCESSFULLY\n");
                break;
            }
        } while (1);
    }

    return 0;

}

// function to remove new line
void removeNewLine(char *str) 
{
	str[strcspn(str, "\n")] = 0;
}

// function to convert user input to lowercase
void toLowercase(char *str)
{
    for (int i=0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// function to validate name
int isValidName(const char *name) 
{
	for (size_t i = 0; i < strlen(name); i++) 
    {
		if (!isalpha(name[i]) && name[i] != ' ') return false; // check if the name only has alphabets and space
	}
	return true;
}

// function to validate phone number
int isValidPhone(const char *phone) 
{
    // ensure phone number starts with "+60"
    if (strncmp(phone, "+60", 3) != 0)
    {
        printf("\nPHONE NUMBER MUST START WITH +60\n");
        return false;
    }

	size_t length = strlen(phone); // get the length of the phone string

	if (length != 12 && length != 13) // check if phone number is between 9 and 10 digits after +60
    {
        printf("\nPHONE NUMBER MUST HAVE ONLY 9 TO 10 DIGITS AFTER +60\n");
        
        return false;
    }

	for (int i = 3; i < length; i++) //check if phone number after +60 only contains digits
    {
	    if (!isdigit(phone[i])) 
        {
            printf("\nPHONE NUMBER MUST ONLY CONTAIN DIGITS\n");
            return false;
	    }
    }

    return true;
}

// function to validate email
int isValidEmail(const char *email) 
{
	int atFound = 0; // check if "@" was found
	int dotFound = 0; // check if "." was found

	for (int i = 0; i < strlen(email); i++) 
    {
		if (email[i] == '@') 
        {
			atFound = 1; // mark that "@" is found
		}
		if (atFound && email[i] == '.') 
        {
			dotFound = 1; // mark that "." is found after "@"
		}
	}
	return atFound && dotFound;
}

// function to input name
void addName(Contact contacts[], int *contact_count)    
{
    printf("\nNAME: "); 
    fgets(contacts[*contact_count].name, sizeof(contacts[*contact_count].name), stdin);
    removeNewLine(contacts[*contact_count].name);

    toLowercase(contacts[*contact_count].name);     // convert name to lowercase

    while (strlen(contacts[*contact_count].name) == 0) 
    { // check if there is input
        printf("\nNAME CANNOT BE EMPTY\n\n");
        printf("Please enter again: ");
        fgets(contacts[*contact_count].name, sizeof(contacts[*contact_count].name), stdin);
        removeNewLine(contacts[*contact_count].name);
    }

    while (!isValidName(contacts[*contact_count].name)) 
    { // check if the name is valid
        printf("\nINVALID INPUT\n\n");
        printf("Please enter again: ");
        fgets(contacts[*contact_count].name, sizeof(contacts[*contact_count].name), stdin);
        removeNewLine(contacts[*contact_count].name);
    }
}

// function to input phone number
void addPhone(Contact contacts[], int *contact_count)
{
	printf("PHONE NUMBER: ");
	fgets(contacts[*contact_count].phoneNo, sizeof(contacts[*contact_count].phoneNo), stdin);
    removeNewLine(contacts[*contact_count].phoneNo);

	while (strlen(contacts[*contact_count].phoneNo) == 0) 
    { // check if there is input
		printf("\nPHONE NUMBER CANNOT BE EMPTY\n\n");
		printf("Please enter again: ");
		fgets(contacts[*contact_count].phoneNo, sizeof(contacts[*contact_count].phoneNo), stdin);
		removeNewLine(contacts[*contact_count].phoneNo);
	}
	while (!isValidPhone(contacts[*contact_count].phoneNo)) 
    { // check if the phone number is valid
		printf("\nINVALID PHONE NUMBER\n\n");
		printf("Please enter a valid phone number (+6012345678): ");
		fgets(contacts[*contact_count].phoneNo, sizeof(contacts[*contact_count].phoneNo), stdin);
		removeNewLine(contacts[*contact_count].phoneNo);
	}
}

// function to input email
void addEmail(Contact contacts[], int *contact_count)
{
    printf("EMAIL: ");
	fgets(contacts[*contact_count].email, sizeof(contacts[*contact_count].email), stdin);
	removeNewLine(contacts[*contact_count].email);

    toLowercase(contacts[*contact_count].email);        // convert email to lowercase

	while (strlen(contacts[*contact_count].email) == 0) 
    { // check if there is input
		printf("\nEMAIL CANNOT BE EMPTY\n\n");
		printf("Please enter again: ");
		fgets(contacts[*contact_count].email, sizeof(contacts[*contact_count].email), stdin);
		removeNewLine(contacts[*contact_count].email);
	}

	while (!isValidEmail(contacts[*contact_count].email)) 
    { // check if the email is valid
		printf("\nINVALID EMAIL\n\n");
		printf("Please enter a valid email (example@domain.com): ");
		fgets(contacts[*contact_count].email, sizeof(contacts[*contact_count].email), stdin);
		removeNewLine(contacts[*contact_count].email);
	}
}

// function to input address
void addAddress(Contact contacts[], int *contact_count)
{
	printf("ADDRESS ('-' if no address): ");
	fgets(contacts[*contact_count].address, sizeof(contacts[*contact_count].address), stdin);
	removeNewLine(contacts[*contact_count].address);

    // check if there is input
    while (strlen(contacts[*contact_count].address) == 0)
    {
        printf("\nADDRESS CANNOT BE EMPTY\n\n");
        printf("Please enter again:");
        fgets(contacts[*contact_count].address, sizeof(contacts[*contact_count].address), stdin);
        removeNewLine(contacts[*contact_count].address);
    }
}

// function to add contact
void addContact(Contact contacts[], int *contact_count) 
{
    Contact newContact;

    addName(contacts, contact_count);   // call addName function to add name
    addPhone(contacts, contact_count);  // call addPhone function to add phone number
    addEmail(contacts, contact_count);  // call addEmail function to add email
    addAddress(contacts, contact_count);    // call addAddress function to add address

    newContact = contacts[*contact_count];
    int duplicateIndex;

    if (isDuplicate(contacts, *contact_count, newContact, &duplicateIndex))
    {
        printf("FAILED TO ADD CONTACT\n\n");
        // display duplicate contact details
        printf("DUPLICATE CONTACT DETAILS\n");
        printf("-------------------------\n");
        printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[duplicateIndex].name, contacts[duplicateIndex].phoneNo, contacts[duplicateIndex].email, contacts[duplicateIndex].address, contacts[duplicateIndex].isFavorite? "Yes" : "No");
    }
    else
    {
        (*contact_count)++;
        saveContacts(contacts, *contact_count);     // save new contact to file
        printf("\nCONTACT ADDED SUCCESSFULLY\n");
    }
}

// function to edit contact
void editContact(Contact contacts[], int contact_count) 
{	
    // check if there are existing contacts
    if (contact_count == 0)
    {
        printf("\nNO EXISTING CONTACTS\n");     // if there are no existing contacts
    }
    else
    {
        int matchedIndices[MAX_CONTACTS];
        int matchedCount = 0;

        char searchName[NAME_LEN];
        
        // ask user to enter name of contact to edit
        printf("\nEnter name of contact to edit: ");
        fgets(searchName, sizeof(searchName), stdin);
        removeNewLine(searchName);

        toLowercase(searchName);        // convert searchName to lowercase

        // call partialSearch function to search for contact to edit
        matchedCount = partialSearch(contacts, contact_count, searchName, matchedIndices);

        if (matchedCount == 0)  // no matching contact found
        {
            printf("\nCONTACT NOT FOUND\n");    // if contact not found
            return;
        }
        else if(matchedCount == 1)  // one matching contact found
        {
            int editChoice = 0;

            // display contact details of matching contact
            printf("\nCONTACT FOUND\n");
            printf("-------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[matchedIndices[0]].name, contacts[matchedIndices[0]].phoneNo, contacts[matchedIndices[0]].email, contacts[matchedIndices[0]].address, contacts[matchedIndices[0]].isFavorite? "Yes" : "No");
            
            // allow user to choose what contact detail they would like to edit
            printf("EDIT:\n");
            printf("-----\n");
            printf("1. NAME\n");
            printf("2. PHONE NUMBER\n");
            printf("3. EMAIL\n");
            printf("4. ADDRESS\n");
            printf("5. ALL CONTACT DETAILS\n\n");
            printf("Enter your choice: ");
            scanf("%d", &editChoice);
            while (getchar() != '\n');  // clear input buffer after scanf

            switch (editChoice)
            {
                case 1:
                    addName(contacts, &matchedIndices[0]);      // call addName function to edit name
                    break;
                case 2:
                    addPhone(contacts, &matchedIndices[0]);     // call addPhone function to edit phone number
                    break;
                case 3:
                    addEmail(contacts, &matchedIndices[0]);     // call addEmail function to edit email
                    break;
                case 4:
                    addAddress(contacts, &matchedIndices[0]);   // call addAddress function to edit address
                    break;
                case 5:
                    addName(contacts, &matchedIndices[0]);
                    addPhone(contacts, &matchedIndices[0]);
                    addEmail(contacts, &matchedIndices[0]);
                    addAddress(contacts, &matchedIndices[0]);
                    break;
                default:
                    printf("\nINVALID CHOICE\n");
            }
            
            printf("\nCONTACT EDITED SUCCESSFULLY\n");
            saveContacts(contacts, contact_count);  // save updated contact details to file
        } 
        else    // multiple matching contacts found
        {
            int contactToEdit = 0;

            // display names of matching contacts
            printf("\n%d MATCHES FOUND\n", matchedCount);
            printf("-----------------\n");

            for (int i=0; i<matchedCount; i++)
            {
                printf("%d. %s\n", i+1, contacts[matchedIndices[i]].name);
            }

            printf("\nPlease enter number of the contact to edit: ");    // ask user to choose contact to edit
            scanf("%d", &contactToEdit);
            while (getchar() != '\n');  // clear input buffer after scanf
            
            // validate choice
            if (contactToEdit < 1 || contactToEdit > matchedCount)
            {
                printf("\nINVALID CHOICE\n");
                return;
            }

            int editIndex = matchedIndices[contactToEdit - 1]; // index of chosen contact

            // display contact details of chosen contact
            int editChoice = 0;
            printf("\nCONTACT FOUND\n");
            printf("-------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[editIndex].name, contacts[editIndex].phoneNo, contacts[editIndex].email, contacts[editIndex].address, contacts[editIndex].isFavorite? "Yes" : "No");
            
            // allow user to choose what contact detail they would like to edit
            printf("EDIT:\n");
            printf("-----\n");
            printf("1. NAME\n");
            printf("2. PHONE NUMBER\n");
            printf("3. EMAIL\n");
            printf("4. ADDRESS\n");
            printf("5. ALL CONTACT DETAILS\n\n");
            printf("Enter your choice: ");
            scanf("%d", &editChoice);
            while (getchar() != '\n');  // clear input buffer after scanf

            switch (editChoice)
            {
                case 1:
                    addName(contacts, &editIndex);      // call addName function to edit name
                    break;
                case 2:
                    addPhone(contacts, &editIndex);     // call addPhone function to edit phone number
                    break;
                case 3:
                    addEmail(contacts, &editIndex);     // call addEmail function to edit email
                    break;
                case 4:
                    addAddress(contacts, &editIndex);   // call addAddress function to edit address
                    break;
                case 5:
                    addName(contacts, &editIndex);
                    addPhone(contacts, &editIndex);
                    addEmail(contacts, &editIndex);
                    addAddress(contacts, &editIndex);
                    break;
                default:
                    printf("\nINVALID CHOICE\n");
            }
            
            printf("\nCONTACT EDITED SUCCESSFULLY\n");
            saveContacts(contacts, contact_count);  // save updated contact details to file
        }
    }
}

// function to delete contact
void deleteContact(Contact contacts[], int *contact_count) 
{
    // check if there are existing contacts
    if (*contact_count == 0)
    {
        printf("\nNO EXISTING CONTACTS\n");     // if there are no existing contacts
        return;
    }
    else
    {
        char searchName[NAME_LEN];
        char confirm;
        int matchedIndices[MAX_CONTACTS];
        int matchedCount = 0;
        
        // ask user to enter name of contact to delete
        printf("\nEnter name of contact to delete: ");
        fgets(searchName, sizeof(searchName), stdin);
        removeNewLine(searchName);

        toLowercase(searchName);        // convert searchName to lowercase

        // call partialSearch function to search for contact to delete
        matchedCount = partialSearch(contacts, *contact_count, searchName, matchedIndices);

        if (matchedCount == 0)  // no matching contact found
        {
            printf("\nCONTACT NOT FOUND\n");    // if no matching contact found
        }
        else if (matchedCount == 1) // one matching contact found
        {
            // display contact details before deletion
            printf("\nCONTACT FOUND\n");
            printf("-------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[matchedIndices[0]].name, contacts[matchedIndices[0]].phoneNo, contacts[matchedIndices[0]].email, contacts[matchedIndices[0]].address, contacts[matchedIndices[0]].isFavorite? "Yes" : "No");
            
            // prompt user for confirmation of deletion
            printf("Are you sure you want to delete this contact? (Y/N) : ");
            confirm = getchar();
            while ((getchar() != '\n'));    // clear input buffer

            if (confirm == 'Y' || confirm == 'y') 
            {
                // shift elements to fill the gap
                for (int j = matchedIndices[0]; j < *contact_count - 1; j++) 
                {
                    contacts[j] = contacts[j + 1];
                }
                (*contact_count)--; // decrement contact count
                printf("\nCONTACT DELETED SUCCESSFULLY\n");
                saveContacts(contacts, *contact_count); // save updates to file
            } 
            else 
            {
                printf("\nDELETION CANCELLED\n");
            }
            return; // exit after processing deletion
        }
        else    // multiple matching contacts found
        {
            int contactToDelete = 0;

            // display names of matching contacts
            printf("\n%d MATCHES FOUND\n", matchedCount);
            printf("-----------------\n");

            for (int i=0; i<matchedCount; i++)
            {
                printf("%d. %s\n", i+1, contacts[matchedIndices[i]].name);
            }

            printf("\nPlease enter number of the contact to delete: ");    // ask user to choose contact to delete
            scanf("%d", &contactToDelete);
            while (getchar() != '\n');  // clear input buffer after scanf
            
            // validate choice
            if (contactToDelete < 1 || contactToDelete > matchedCount)
            {
                printf("\nINVALID CHOICE\n");
                return;
            }

            int deleteIndex = matchedIndices[contactToDelete - 1]; // index of chosen contact

            // display contact details of chosen contact before deletion
            printf("\nCONTACT DETAILS\n");
            printf("---------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[deleteIndex].name, contacts[deleteIndex].phoneNo, contacts[deleteIndex].email, contacts[deleteIndex].address, contacts[deleteIndex].isFavorite? "Yes" : "No");

            // prompt user for confirmation of deletion
            printf("Are you sure you want to delete this contact? (Y/N) : ");
            confirm = getchar();
            while ((getchar() != '\n'));    // clear input buffer

            if (confirm == 'Y' || confirm == 'y') 
            {
                // shift elements to fill the gap
                for (int j = deleteIndex; j < *contact_count - 1; j++) 
                {
                    contacts[j] = contacts[j + 1];
                }
                (*contact_count)--; // decrement contact count
                printf("\nCONTACT DELETED SUCCESSFULLY\n");
                saveContacts(contacts, *contact_count); // save updates to file
            } 
            else
            {
                printf("\nDELETION CANCELLED\n");
            }
            return; // exit after processing deletion
        }
    }
}

// function to search contact
void searchContact(Contact contacts[], int *contact_count) 
{
    // check if there are existing contacts
    if (*contact_count == 0)
    {
        printf("\nNO EXISTING CONTACTS\n");     // if there are no existing contacts
        return;
    }
    else
    {
        int matchedIndices[MAX_CONTACTS];
        int matchedCount = 0;
        int searchChoice = 0;

        // allow user to choose how they want to search contact
        printf("\nSEARCH CONTACT BY:\n");
        printf("1. NAME\n");
        printf("2. PHONE NUMBER\n");
        printf("3. EMAIL\n");
        printf("\nEnter your choice: ");
        scanf("%d", &searchChoice);
        while ((getchar() != '\n')); // Clear the input buffer

        switch (searchChoice)
        {
            case 1:
                matchedCount = searchByName(contacts, contact_count, matchedIndices);       // call searchByName function to search contact by name
                break;
            case 2:
                matchedCount = searchByPhoneNo(contacts, contact_count, matchedIndices);    // call searchByPhoneNo function to search contact by phone number
                break;
            case 3:
                matchedCount = searchByEmail(contacts, contact_count, matchedIndices);      // call searchByEmail function to search contact by email
                break;
            default:
                printf("INVALID CHOICE.\n");
                return;
        }

        // output searched contact details based on matchedCount
        if (matchedCount == 0)  // no matching contact found
        {
            printf("\nCONTACT NOT FOUND\n");    // if no matching contact found
        }
        else if (matchedCount == 1) // one matching contact found
        {
            // display contact details of matching contact
            printf("\nCONTACT DETAILS\n");
            printf("---------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[matchedIndices[0]].name, contacts[matchedIndices[0]].phoneNo, contacts[matchedIndices[0]].email, contacts[matchedIndices[0]].address, contacts[matchedIndices[0]].isFavorite? "Yes" : "No");
        }
        else    // multiple matching contacts found
        {
            int contactToSearch = 0;

            // display names of matching contacts
            printf("\n%d MATCHES FOUND\n", matchedCount);
            printf("-----------------\n");

            for (int i=0; i<matchedCount; i++)
            {
                printf("%d. %s\n", i+1, contacts[matchedIndices[i]].name);
            }

            printf("\nPlease enter number of contact to view contact details: ");    // ask user to choose contact to view contact details
            scanf("%d", &contactToSearch);
            while (getchar() != '\n');  // clear input buffer after scanf

            int searchIndex = matchedIndices[contactToSearch - 1]; // index of chosen contact
            
            // validate choice
            if (contactToSearch < 1 || contactToSearch > matchedCount)
            {
                printf("\nINVALID CHOICE\n");
                return;
            }
            else
            {
                // display details of chosen contact
                printf("\nCONTACT DETAILS\n");
                printf("---------------\n\n");
                printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[searchIndex].name, contacts[searchIndex].phoneNo, contacts[searchIndex].email, contacts[searchIndex].address, contacts[searchIndex].isFavorite? "Yes" : "No");
            }
        }
    }
}

// function to search contact by name
int searchByName(Contact contacts[], int *contact_count, int matchedIndices[])
{
    char searchName[NAME_LEN];

    // ask user to enter name of contact to search
    printf("\nEnter name of contact to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    removeNewLine(searchName);

    toLowercase(searchName);        // convert searchName to lowercase

    // call partialSearch function to search for contact to view contact details
    return partialSearch(contacts, *contact_count, searchName, matchedIndices);
}

// function to search contact by phone number
int searchByPhoneNo(Contact contacts[], int *contact_count, int matchedIndices[])
{
    char searchPhoneNo[PHONENO_LEN];
    
    // ask user to enter phone number of contact to search
    printf("Enter phone number of contact to search: ");
    fgets(searchPhoneNo, sizeof(searchPhoneNo), stdin);
    removeNewLine(searchPhoneNo);

    // call partialSearch function to search for contact to view contact details
    return partialSearch(contacts, *contact_count, searchPhoneNo, matchedIndices);
}

// function to search contact by email
int searchByEmail(Contact contacts[], int *contact_count, int matchedIndices[])
{
    char searchEmail[EMAIL_LEN];

    // ask user to enter email of contact to search
    printf("\nEnter email of contact to search: ");
    fgets(searchEmail, sizeof(searchEmail), stdin);
    removeNewLine(searchEmail);

    toLowercase(searchEmail);       // convert searchEmail to lowercase

    // call partialSearch function to search for contact to view contact details
    return partialSearch(contacts, *contact_count, searchEmail, matchedIndices);
}

// function to sort contacts
void sortContacts(Contact contacts[], int contact_count) 
{
    // check if there are existing contacts
    if (contact_count == 0)
    {
        printf("\nNO CONTACTS TO SORT\n");  // if no existing contacts
    }
    else
    {
        // allow user to choose how they want the contact list to be sorted using switch..case
        int sortChoice = 0;

        printf("\nSORT CONTACTS BY:\n");
        printf("---------------\n");
        printf("1. NAME\n");
        printf("2. PHONE NUMBER\n");
        printf("3. EMAIL\n\n");
        printf("Enter your choice: ");
        scanf("%d", &sortChoice);
        while (getchar() != '\n');  // clear input buffer after scanf
        
        switch (sortChoice)
        {
            case 1:
                sortByName(contacts, contact_count);            // call sortByName function to sort contacts by name
                break;
            case 2:
                sortByPhoneNumber(contacts, contact_count);     // call sortByPhoneNumber function to sort contacts by phone number
                break;
            case 3:
                sortByEmail(contacts, contact_count);           // call sortByEmail function to sort contacts by email
                break;
            default:
                printf("INVALID CHOICE\n");
                printf("PLEASE TRY AGAIN\n");
        }
    }
}

// function to sort contacts by name
void sortByName(Contact contacts[], int contact_count) 
{
    for (int i = 0; i < contact_count-1; i++)
    { 
        // iterates from the first element to the second-last element in the array
        for (int j = i+1; j < contact_count; j++) 
        { // compares contacts[i] with each subsequent contact contacts[j] 
            if (strcasecmp(contacts[i].name, contacts[j].name) > 0) 
            { //compare names alphabetically
                Contact temp = contacts[i]; // swapping contacts
                contacts[i] = contacts[j];
                contacts[j] = temp; // swap names
            }
        }
    }
    printf("\nCONTACTS SORTED BY NAME\n");    // indicating that sorting is complete
}

// function to sort contacts by phone number
void sortByPhoneNumber(Contact contacts[], int contact_count) 
{
    for (int i = 0; i < contact_count-1; i++)
    {
        for (int j = i+1; j < contact_count; j++) 
        {
            if (strcmp(contacts[i].phoneNo + 1, contacts[j].phoneNo + 1) > 0) 
            { // compare phone numbers 
                Contact temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp; // swap numbers
            }
        }
    }
    printf("\nCONTACTS SORTED BY PHONE NUMBER\n");    // indicating that sorting is complete
}

// function to sort contacts by email
void sortByEmail(Contact contacts[], int contact_count) 
{
    for (int i = 0; i < contact_count-1; i++)
    {
        for (int j = i+1; j < contact_count; j++) 
        {
            if (strcmp(contacts[i].email, contacts[j].email) > 0) 
            { // compare email
                Contact temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp; // swap numbers
            }
        }
    }
    printf("\nCONTACTS SORTED BY EMAIL\n");   // indicating that sorting is complete
}

// function to display sorted contacts
void displaySortedContacts(Contact contacts[], int contact_count)
{
    // check if there are existing contacts
    if (contact_count == 0)
    {
        return;
    }
    else
    {
        printf("\nSORTED CONTACTS\n");
        printf("---------------\n\n");
        for (int i = 0; i < contact_count; i++) 
        {
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[i].name, contacts[i].phoneNo, contacts[i].email, contacts[i].address, contacts[i].isFavorite? "Yes" : "No");
        }
    }
}

// function to display contacts
void displayContacts(Contact contacts[], int contact_count) 
{
    // check if there are existing contacts
    if (contact_count == 0)
    {
        printf("\nNO EXISTING CONTACTS\n");     // if there are no existing contacts
    }
    else
    {
        printf("\nCONTACT LIST\n");
        printf("------------\n\n");

        // use for loop to display contacts
        for (int i=0; i<contact_count; i++) 
        {
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\nFAVORITE: %s\n\n", contacts[i].name, contacts[i].phoneNo, contacts[i].email, contacts[i].address, contacts[i].isFavorite? "Yes" : "No");
        }
    }
}

// function to save contacts
void saveContacts(Contact contacts[], int contact_count)
{
    // open "contacts.txt" file for write
    FILE *file = fopen("contacts.txt", "w");
    //check if file was opened successfully
    if (file == NULL)
    {
        printf("\nERROR OPENING FILE\n");
        return;
    }
    // use for loop to write contact details to contacts.txt file
    for (int i=0; i<contact_count; i++)
    {
        fprintf(file, "%s, %s, %s, %s, %d\n", contacts[i].name, contacts[i].phoneNo, contacts[i].email, contacts[i].address, contacts[i].isFavorite);
    }

    //close "contacts.txt" file
    fclose(file);
}

// function to load contacts
void loadContacts(Contact contacts[], int *contact_count)
{
    // open "contacts.txt" file for read
    FILE *file = fopen("contacts.txt", "r");
    // check if there is existing file with saved contacts
    if (file == NULL) 
    {
        printf("\nNO EXISTING CONTACTS\n");
        return;
    }
    
    *contact_count = 0;
    char buffer[200];

    // use while loop to load contacts from file
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        sscanf(buffer, "%[^,], %[^,], %[^,], %[^,], %d", contacts[*contact_count].name, contacts[*contact_count].phoneNo, contacts[*contact_count].email, contacts[*contact_count].address, &contacts[*contact_count].isFavorite);
        
        // remove any newlines from contact fields
        removeNewLine(contacts[*contact_count].name);
        removeNewLine(contacts[*contact_count].phoneNo);
        removeNewLine(contacts[*contact_count].email);
        removeNewLine(contacts[*contact_count].address);
        
        (*contact_count)++; // increment contact count
    }

    // close "contacts.txt" file
    fclose(file);
    printf("CONTACTS LOADED FROM FILE\n");
    printf("-------------------------\n\n");
}

// function to display contact names
void displayContactNames(Contact contacts[], int contact_count)
{ 
    // display contact names
    for (int i=0; i<contact_count; i++)
    {
        printf("%d. %s\n", i+1, contacts[i].name);
    }
}

// function to manage favorite contacts
void favorites(Contact contacts[], int contact_count, int *favorite_count)
{
    // check if there are existing contacts
    if (contact_count == 0)
    {
        printf("\nNO EXISTING CONTACTS\n");
    }
    else
    {
        int favoriteChoice = 0; // allow user to choose to add/remove/display favorites

        printf("\nFAVORITES MENU\n");
        printf("--------------\n");
        printf("1. MARK AS FAVORITE\n");
        printf("2. REMOVE FROM FAVORITES\n");
        printf("3. DISPLAY FAVORITES\n\n");
        printf("Enter your choice: ");
        scanf("%d", &favoriteChoice);
        while (getchar() != '\n');  // clear input buffer after scanf
        
        switch (favoriteChoice)
        {
            case 1:
                markAsFavorite(contacts, contact_count, favorite_count);    // call markAsFavorite function to mark contact as favorite
                break;
            case 2:
                removeFavorite(contacts, contact_count, favorite_count);    // call removerFavorite function to remove contact from favorites
                break;
            case 3:
                displayFavorites(contacts, contact_count, favorite_count);  // call displayFavorites function to display favorites
                break;
            default:
                printf("INVALID CHOICE\n");
                printf("PLEASE TRY AGAIN\n");
        }
    }
}

// function to mark a contact as favorite
void markAsFavorite(Contact contacts[], int contact_count, int *favorite_count) 
{
    char searchName[NAME_LEN];
    int matchedIndices[MAX_CONTACTS];
    int matchedCount;

    // ask user to enter name of contact to mark as favorite
    printf("\nEnter name of contact to mark as favorite: ");
    fgets(searchName, sizeof(searchName), stdin);
    removeNewLine(searchName);

    toLowercase(searchName);        // convert searchName to lowercase

    // call partialSearch function to search for contact
    matchedCount = partialSearch(contacts, contact_count, searchName, matchedIndices);

    if (matchedCount == 0)  // no matching contact found
    {
        printf("\nCONTACT NOT FOUND\n");    // if no matching contact found
    }
    else if (matchedCount == 1) // one matching contact found
    {
        // display contact details of matching contact
        printf("\nCONTACT DETAILS\n");
        printf("---------------\n\n");
        printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\n\n", contacts[matchedIndices[0]].name, contacts[matchedIndices[0]].phoneNo, contacts[matchedIndices[0]].email, contacts[matchedIndices[0]].address);
        
        // check if contact has already been marked as favorite
        if (contacts[matchedIndices[0]].isFavorite)
        {
            printf("\nCONTACT HAS ALREADY BEEN MARKED AS FAVORITE\n");
        }
        else
        {
            contacts[matchedIndices[0]].isFavorite = 1;     // mark contact as favorite
            printf("\nCONTACT HAS BEEN MARKED AS FAVORITE SUCCESSFULLY\n");
            (*favorite_count)++;    // increment favorite_count
            saveContacts(contacts, contact_count);      // save updated contact
        }
    }
    else    // multiple matching contacts found
    {
        int contactToFav = 0;

        // display names of matching contacts
        printf("\n%d MATCHES FOUND\n", matchedCount);
        printf("-----------------\n");

        for (int i=0; i<matchedCount; i++)
        {
            printf("%d. %s\n", i+1, contacts[matchedIndices[i]].name);
        }

        printf("\nPlease enter number of contact to mark as favorite: ");    // ask user to choose contact to view contact details
        scanf("%d", &contactToFav);
        while (getchar() != '\n');  // clear input buffer after scanf

        int favIndex = matchedIndices[contactToFav - 1]; // index of chosen contact
        
        // validate choice
        if (contactToFav < 1 || contactToFav > matchedCount)
        {
            printf("\nINVALID CHOICE\n");
            return;
        }
        else
        {
            // display details of chosen contact
            printf("\nCONTACT DETAILS\n");
            printf("---------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\n\n", contacts[favIndex].name, contacts[favIndex].phoneNo, contacts[favIndex].email, contacts[favIndex].address);
        }

        // check if contact has already been marked as favorite
        if (contacts[favIndex].isFavorite)
        {
            printf("\nCONTACT HAS ALREADY BEEN MARKED AS FAVORITE\n");
        }
        else
        {
            contacts[favIndex].isFavorite = 1;     // mark contact as favorite
            printf("\nCONTACT HAS BEEN MARKED AS FAVORITE SUCCESSFULLY\n");
            (*favorite_count)++;    // increment favorite_count
            saveContacts(contacts, contact_count);      // save updated contact
        }
    }   
}

// function to remove a contact from favorites
void removeFavorite(Contact contacts[], int contact_count, int *favorite_count)
{
    char searchName[NAME_LEN];
    int matchedIndices[MAX_CONTACTS];
    int matchedCount;
    char confirm;

    if (*favorite_count == 0)
    {
        printf("\nNO EXISTING FAVORITES\n");  // if there are no existing favorites
        return;
    }
    else
    {
        // ask user to enter name of contact to remove from favorites
        printf("\nEnter name of contact to remove from favorites: ");
        fgets(searchName, sizeof(searchName), stdin);
        removeNewLine(searchName);

        toLowercase(searchName);        // convert searchName to lowercase

        // call partialSearch function to search for contact
        matchedCount = partialSearch(contacts, contact_count, searchName, matchedIndices);

        if (matchedCount == 0)  // no matching contact found
        {
            printf("\nCONTACT NOT FOUND\n");    // if no matching contact found
        }
        else if (matchedCount == 1) // one matching contact found
        {
            // display contact details of matching contact
            printf("\nCONTACT DETAILS\n");
            printf("---------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\n\n", contacts[matchedIndices[0]].name, contacts[matchedIndices[0]].phoneNo, contacts[matchedIndices[0]].email, contacts[matchedIndices[0]].address);

            // check if contact is marked as favorite
            if (!contacts[matchedIndices[0]].isFavorite)
            {
                printf("\nCONTACT IS NOT MARKED AS FAVORITE\n");    // if contact is not marked as favorite
            }
            else
            {
                // prompt user for confirmation of removal
                printf("Are you sure you want to remove this contact from favorites? (Y/N) : ");
                confirm = getchar();
                while ((getchar() != '\n'));

                if (confirm == 'Y' || confirm == 'y') 
                {
                    contacts[matchedIndices[0]].isFavorite = 0;     // remove contact from favorites
                    printf("\nCONTACT HAS BEEN REMOVED FROM FAVORITES SUCCESSFULLY\n");
                    (*favorite_count)--;        // decrement favorite_count
                    saveContacts(contacts, contact_count);  // save updated contact
                } 
                else 
                {
                    printf("\nREMOVAL CANCELLED\n");
                }
                return;     // exit after processing removal
            }
        }
        else    // multiple matching contacts
        {
            int removeFav = 0;

            // display names of matching contacts
            printf("\n%d MATCHES FOUND\n", matchedCount);
            printf("-----------------\n");

            for (int i=0; i<matchedCount; i++)
            {
                printf("%d. %s\n", i+1, contacts[matchedIndices[i]].name);
            }

            printf("\nPlease enter number of the contact to remove from favorites: ");    // ask user to choose contact to remove from favorites
            scanf("%d", &removeFav);
            while (getchar() != '\n');  // clear input buffer after scanf
            
            // validate choice
            if (removeFav < 1 || removeFav > matchedCount)
            {
                printf("\nINVALID CHOICE\n");
                return;
            }

            int removeIndex = matchedIndices[removeFav - 1]; // index of chosen contact

            // display contact details of chosen contact before removal
            printf("\nCONTACT DETAILS\n");
            printf("---------------\n\n");
            printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\n\n", contacts[removeIndex].name, contacts[removeIndex].phoneNo, contacts[removeIndex].email, contacts[removeIndex].address);

            // check if contact is marked as favorite
            if (!contacts[removeIndex].isFavorite)
            {
                printf("\nCONTACT IS NOT MARKED AS FAVORITE\n");    // if contact is not marked as favorite
            }
            else
            {
                // prompt user for confirmation of removal
                printf("Are you sure you want to remove this contact from favorites? (Y/N) : ");
                confirm = getchar();
                while ((getchar() != '\n'));    // clear input buffer

                if (confirm == 'Y' || confirm == 'y') 
                {
                    contacts[removeIndex].isFavorite = 0;     // remove contact from favorites
                    printf("\nCONTACT HAS BEEN REMOVED FROM FAVORITES SUCCESSFULLY\n");
                    (*favorite_count)--;        // decrement favorite_count
                    saveContacts(contacts, contact_count);  // save updated contact
                } 
                else 
                {
                    printf("\nREMOVAL CANCELLED\n");
                }
                return;     // exit after processing removal
            }
        }
    }
}

// function to display only favorite contacts
void displayFavorites(Contact contacts[], int contact_count, int *favorite_count) 
{
    int found = 0; // to check if there are any favorites
    
    if (*favorite_count == 0)
    {
        printf("\nNO EXISTING FAVORITES\n");  // if there are no existing favorites
        return;
    }
    else
    {
        // display favorite contacts
        printf("\nFAVORITE CONTACTS\n");
        printf("------------------\n\n");

        for (int i = 0; i < contact_count; i++) 
        {
            if (contacts[i].isFavorite == 1) 
            {
                found = 1;  // mark that favorite contact was found
                printf("NAME : %s\nPHONE NO : %s\nEMAIL : %s\nADDRESS : %s\n\n", contacts[i].name, contacts[i].phoneNo, contacts[i].email, contacts[i].address);
            }
        }
        
        if (!found)
        {
            printf("\nNO FAVORITE CONTACTS FOUND\n"); // if there are no existing favorite contacts
        }
    }
}

int partialSearch(Contact contacts[], int contact_count, const char *searchTerm, int matchedIndices[])
{
    int matchedCount = 0;       // number of matching contacts

    for (int i=0; i<contact_count; i++)
    {        
        if (strstr(contacts[i].name, searchTerm))
        {
            matchedIndices[matchedCount] = i; // store index of matching contact
            matchedCount++;
        }
        else if (strstr(contacts[i].phoneNo, searchTerm))
        {
            matchedIndices[matchedCount] = i;   // store index of matching contact
            matchedCount++;
        }
        else if (strstr(contacts[i].email, searchTerm))
        {
            matchedIndices[matchedCount] = i;   // store index of matching contact
            matchedCount++;
        }
    }

    return matchedCount;
}

// function to check for duplicate contact
int isDuplicate(Contact contacts[], int contact_count, Contact newContact, int *duplicateIndex)
{
    for (int i = 0; i < contact_count; i++) 
    {
        // check if there is duplicate phone number 
        if (strcasecmp(contacts[i].phoneNo, newContact.phoneNo) == 0)
        {
            printf("\nDUPLICATE FOUND : THERE IS AN EXISTING CONTACT WITH THE SAME PHONE NUMBER\n");
            
            *duplicateIndex = i;    // get index of existing duplicate contact

            return 1;
        }

        // check if there is duplicate email
        if (strcasecmp(contacts[i].email, newContact.email) == 0)
        {
            printf("\nDUPLICATE FOUND : THERE IS AN EXISTING CONTACT WITH THE SAME EMAIL\n");

            *duplicateIndex = i;    // get index of existing duplicate contact

            return 1;
        }
    }
    return 0;   // no duplicate found
}