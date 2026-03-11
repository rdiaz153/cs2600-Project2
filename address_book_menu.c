#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
#include "address_book_fops.h"

#include "address_book_fops.h"
//#include "abk_log.h"
#include "address_book_menu.h"
#include "address_book.h"

int get_option(int type, const char *msg)
{
	int option; 

	if (msg && *msg)
	{
		printf("%s", msg);
	}

	scanf("%d", &option);

	return option;

}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}
// Viknesh
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
    if (address_book == NULL || address_book->list == NULL || address_book->count == 0)
    {
        printf("No contacts to display.\n");
        return e_fail;
    }

    char option;

    do
    {
        menu_header(title);

        if (msg != NULL && *msg != '\0')
            printf("%s\n", msg);

        // figure out which contacts to show for this page
        int start = *index;
        int end = start + WINDOW_SIZE;
        if (end > address_book->count)
            end = address_book->count;

        // printing table header
        printf("%-5s %-20s %-15s %-25s\n", "S.No", "Name", "Phone", "Email");
        printf("--------------------------------------------------------------\n");

        // print each contact in the current page
        for (int i = start; i < end; i++)
        {
            ContactInfo *c = &address_book->list[i];
            printf("%-5d %-20s %-15s %-25s\n",
                c->si_no,
                c->name[0],
                strlen(c->phone_numbers[0]) > 0 ? c->phone_numbers[0] : "-",
                strlen(c->email_addresses[0]) > 0 ? c->email_addresses[0] : "-");
        }

        // show page info and navigation options
        int total_pages = (address_book->count + WINDOW_SIZE - 1) / WINDOW_SIZE;
        int current_page = (*index / WINDOW_SIZE) + 1;
        printf("\n--- Page %d of %d ---\n", current_page, total_pages);

        if (*index + WINDOW_SIZE < address_book->count)
            printf("N. Next\n");
        if (*index > 0)
            printf("P. Previous\n");
        printf("Q. Quit\n");

        // get user input
        char input[8];
        printf("Select: ");
        fgets(input, sizeof(input), stdin);
        option = toupper((unsigned char)input[0]);

        if (option == 'N' && *index + WINDOW_SIZE < address_book->count)
            *index += WINDOW_SIZE;
        else if (option == 'P' && *index > 0)
            *index -= WINDOW_SIZE;

    } while (option != 'Q');

    return e_success;
}


void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != NULL)
	{
		printf("#######  %s\n", str);
	}
}


Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				{
					int idx = 0;
					list_contacts(address_book, "List Contacts" ,&idx, NULL, e_list);
				
				}
				break;		
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}


void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}


Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	// error handling, making sure our address book actually exists
	if (address_book == NULL || address_book->list == NULL || address_book->count == 0) {
		printf("No contacts found.\n");
		return e_fail;
	}
	
	//print menu heading for searching
	menu_header("Search Contact to Delete by:\n\n");

	//Menu
	printf("Search by: \n");
	printf("1: Name \n");
	printf("2: Phone Number \n");
	printf("3: Email \n");
	printf("4: Session ID \n");

	//Ask the user to give an option and receive 
	int fieldOption = get_option(NUM, "Please select an option: ");

	// field will be fed into search function
	//prompt will be shown to user so that they can input their search query
	//SEARCH_LEN will be the length cap of query
	int field;
	const char *prompt;
	int SEARCH_LEN;

	// switch case to prepare SEARCH_LEN, prompt, and field for the user.
	switch(fieldOption) {
		case 1:
			field = 0;
			prompt = "Please enter a name to search: ";
			SEARCH_LEN = NAME_LEN;
			break;
		case 2:
			field = 1;
			prompt = "Please enter a phone number to search: ";
			SEARCH_LEN = 10;
			break;
		case 3:
			field = 2;
			prompt = "Please enter an email address to search: ";
			SEARCH_LEN = EMAIL_ID_LEN;
			break;
		case 4:
			field = 3;
			prompt = "Please enter a session ID to search: ";
			SEARCH_LEN = NAME_LEN;
			break;
		default:
			prompt = "Invalid option.\n";
			return e_fail;
	}

	// removing newline and replacing with null terminator
	char query[SEARCH_LEN];
	printf("%s", prompt);
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	//error handling is empty query
	if (strlen(query) == 0) {
		printf("Search query cannot be empty.\n");
		return e_fail;
	}

	Status result = search(query, address_book, address_book->count, field, NULL, e_search);

	if (result == e_no_match) {
		printf("No contacts found matching \"%s\".\n", query);
	}

	get_option(NONE, "\nPress Enter to continue...");

	return result;
}

Status edit_contact(AddressBook *address_book)
{	

	int index;

	//Check if there are contacts available
	if (address_book->count == 0) 
	{
		printf("No contacts available to edit.\n");
		return e_fail;
	}

	//Ask user which contact to edit
	printf("Enter the contact serial number to edit: ");
	scanf("%d", &index);

	//Validate index
	if (index < 0 || index >= address_book->count)
	{
		printf("Invalid contact index. \n");
		return e_fail;
	}

	//Display current contact info
	printf("\nCurrent Contact Details:\n");
	printf("Name: %s\n", address_book->list[index].name[0]);
	printf("Phone: %s\n", address_book->list[index].phone_numbers[0]);
	printf("Email: %s\n", address_book->list[index].email_addresses[0]);

	//Prompt for new values
	printf("\nEnter new name: ");
	scanf("%s", address_book->list[index].name[0]);

	printf("Enter new phone number: ");
	scanf("%s", address_book->list[index].phone_numbers[0]);

	printf("Enter new email address: ");
	scanf("%s", address_book->list[index].email_addresses[0]);

	//Confirm edit
	printf("Contact updated successfully.\n");
}

Status delete_contact(AddressBook *address_book)
{
	int index;

	//Check if there are contacts available
	if (address_book->count == 0)
	{
		printf("No contacts available to delete.\n");
		scanf("%d", &index);
	}

	//Ask which contact to delete
	printf("Enter the contact serial number to delete: ");
	scanf("%d", &index);

	//Validate index
	if (index < 0 || index >= address_book->count)
	{
		printf("Invalid contact index.\n");
		return e_fail;
	}

	//Shift contacts up in the array
	for (int i = index; i < address_book->count - 1; i++)
	{
		address_book->list[i] = address_book->list[i + 1];
	}

	//Reduce total contact count
	address_book->count--;

	printf("Contact deleted successfully.\n");

	return e_success;
}
