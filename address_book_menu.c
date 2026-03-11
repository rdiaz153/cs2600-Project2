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

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

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
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
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
