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
	char input[50];
	int index = 0;

	if (msg != NULL && *msg != '\0') { // check if msg is null
		printf("%s", msg);
	}

	if (fgets(input, sizeof(input), stdin) == NULL) {
		return e_no_opt;
	}

	if(type == CHAR) {
		while (input[index] != '\0') {
			if(!isspace((unsigned char)input[index])) {
				return toupper((unsigned char)input[index]);
			}
			index++;
		}
	}

	if (type == NUM) {
		while (input[index] != '\0') {
			if(isdigit((unsigned char)input[index])) {
				return input[index] - '0';
			}
			index++;
		}
		return e_no_opt;
	}

	if (type == NONE) {
		return e_no_opt;
	}

	return e_no_opt;
	
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
        if (end > address_book->count) {
			end = address_book->count;
		}

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
				ret = add_contacts(address_book);
				if (ret == e_success) {
					printf("Contact added successfully.\n");
				} else {
					printf("Failed to add contact.\n");
				}
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
	int index = address_book->count;
	memset(&address_book->list[index], 0, sizeof(ContactInfo));
	
	int capacity = 10; //temp variable for testing

	while(1){
		printf("\n");
		printf("0. Back\n"); //layout when add func gets called go back 
		printf("1. Name    :%s\n", address_book->list[index].name[0][0] != '\0' ? address_book->list[index].name[0] : "" ); //name
		
		printf("2. Phone No(s): "); //phone number
        for(int i = 0; i < PHONE_NUMBER_COUNT; i++) { //loop so it shows the current num of phone added so far
            if(address_book->list[index].phone_numbers[i][0] != '\0') {
                printf("%s ", address_book->list[index].phone_numbers[i]);
            }
        }
        printf("\n");

		 printf("3. Email ID(s): "); //email addresses
        for(int i = 0; i < EMAIL_ID_COUNT; i++) { // for loop so it shows the current num of email addresses added so far
            if(address_book->list[index].email_addresses[i][0] != '\0') {
                printf("%s ", address_book->list[index].email_addresses[i]);
            }
        }
        printf("\n");
		
	
		int option = get_option(NUM, "Please select an option: "); //get the input
	
	
		if(address_book->count >= capacity){ //checks to see if there is enough space for user to add more contacts
			printf("Maximum contacts reached\n");
			return e_fail;
		}
	
		if(option == 0){ //if option 0 returns back to menu
			return e_back;
		}
		else if(option == 1){ //ask user to input name
			printf("Enter the Name: ");
			fgets(address_book->list[index].name[0], NAME_LEN, stdin);
			address_book->list[index].name[0][strcspn(address_book->list[index].name[0], "\n")] = '\0';
			printf("%s\n", address_book->list[index].name[0]);
			address_book->list[index].si_no = index + 1; // creates serial id for contact added 
			address_book->count++; //increments on every added contact 
			
		}
		else if(option == 2){ // ask user to input phone no
			for(int i = 0; i < PHONE_NUMBER_COUNT; i++){

				if(address_book->list[index].phone_numbers[i][0] == '\0'){ //checks the total amount of phone no (5) to see if thers space
					printf("Enter the Phone Number %d: ", i + 1);
					fgets(address_book->list[index].phone_numbers[i], NUMBER_LEN, stdin);
					address_book->list[index].phone_numbers[i][strcspn(address_book->list[index].phone_numbers[i], "\n")] = '\0';
					break; //will only allow one contact per question
				}
			}
		}
		else if(option == 3){ //ask user to input email address
			for(int i = 0; i < EMAIL_ID_COUNT ; i++){ //same loop, checks counter to see the is enough space for emails.
				if(address_book->list[index].email_addresses[i][0] == '\0'){ 
					printf("Enter the Email Address %d:  ", i + 1);
					fgets(address_book->list[index].email_addresses[i], EMAIL_ID_LEN, stdin);
					address_book->list[index].email_addresses[i][strcspn(address_book->list[index].email_addresses[i], "\n")] = '\0';
					break;
				}
				
			}
	
		}
		else{ // in case user puts other option than 0-3
			printf("Please enter a valid option  (0-3) :");
		}
	

	
	
}
	
	return e_success;

	

}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, SearchModes mode)
{	
	if(address_book->count == 0){ //if statement to see if address_book > 0 else return fail
		printf("No contacts avaliable.\n");
		return e_fail;
	}

    int found = 0;

    
    printf("\n===============================================================\n"); // ascii art for table 
    printf(": S.No : Name                 : Phones             : Emails       :\n");
    printf("===============================================================\n");

    for(int i = 0; i < address_book->count; i++){
        int match = 0;

        
        if(field == 1){ // if statement to see what user chooses to search by, name
            if((mode == EXACT && strcasecmp(address_book->list[i].name[0], str) == 0) ||
               (mode == PARTIAL && strcasestr(address_book->list[i].name[0], str) != NULL))
                match = 1;
        }
        else if(field == 2){ // phone
            for(int j = 0; j < PHONE_NUMBER_COUNT; j++){
                if(strlen(address_book->list[i].phone_numbers[j]) > 0 &&
                   ((mode == EXACT && strcmp(address_book->list[i].phone_numbers[j], str) == 0) ||
                    (mode == PARTIAL && strstr(address_book->list[i].phone_numbers[j], str) != NULL))){
                    match = 1;
                    break;
                }
            }
        }
        else if(field == 3){ // email
            for(int j = 0; j < EMAIL_ID_COUNT; j++){
                if(strlen(address_book->list[i].email_addresses[j]) > 0 &&
                   ((mode == EXACT && strcmp(address_book->list[i].email_addresses[j], str) == 0) ||
                    (mode == PARTIAL && strstr(address_book->list[i].email_addresses[j], str) != NULL))){
                    match = 1;
                    break;
                }
            }
        }
        else if(field == 4){ // serial number
            if(address_book->list[i].si_no == atoi(str))
                match = 1;
        }

        
        if(match){  // prints out info if match is found
            found = 1;

            
            char phones[128] = ""; // gets all contacts phone nums
            for(int j = 0; j < PHONE_NUMBER_COUNT; j++){
                if(strlen(address_book->list[i].phone_numbers[j]) > 0){
                    if(strlen(phones) > 0) strcat(phones, ", ");
                    strcat(phones, address_book->list[i].phone_numbers[j]);
                }
            }

           
            char emails[256] = ""; //gtes all contacts email addy
            for(int j = 0; j < EMAIL_ID_COUNT; j++){
                if(strlen(address_book->list[i].email_addresses[j]) > 0){
                    if(strlen(emails) > 0) strcat(emails, ", ");
                    strcat(emails, address_book->list[i].email_addresses[j]);
                }
            }

            printf(": %-4d : %-20s : %-18s : %-30s :\n", //prints all info avaliable for said contact
                   address_book->list[i].si_no,
                   address_book->list[i].name[0],
                   phones,
                   emails);
        }
    }

    printf("===============================================================\n"); // last ascii art bit

    if(!found) {//if statement in case contact is not found
        printf("No matching contact found.\n");
		return e_no_match;
	}
    return e_success;
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

	//Ask the user to give an option and receive 
	int fieldOption = get_option(NUM, "Please select an option (1 for name, 2 for phone number, 3 for email address, 4 for serial number): ");

	// field will be fed into search function
	//prompt will be shown to user so that they can input their search query
	//SEARCH_LEN will be the length cap of query
	int field;
	const char *prompt;
	int SEARCH_LEN;

	// switch case to prepare SEARCH_LEN, prompt, and field for the user.
	switch(fieldOption) {
		case 1:
			field = 1;
			prompt = "Please enter a name to search: ";
			SEARCH_LEN = NAME_LEN;
			break;
		case 2:
			field = 2;
			prompt = "Please enter a phone number to search: ";
			SEARCH_LEN = 10;
			break;
		case 3:
			field = 3;
			prompt = "Please enter an email address to search: ";
			SEARCH_LEN = EMAIL_ID_LEN;
			break;
		case 4:
			field = 4;
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

	int c;
	while ((c = getchar()) != '\n' && c != EOF);

	get_option(NONE, "\nPress Enter to continue...");

	return result;
}

Status edit_contact(AddressBook *address_book)
{	

	int si_no;

	//Check if there are contacts available
	if (address_book->count == 0) 
	{
		printf("No contacts available to edit.\n");
		return e_fail;
	}



	//Ask user which contact to edit
	printf("Enter the contact serial number to edit: ");
	scanf("%d", &si_no);

	int index = -1;
	for (int i = 0; i < address_book->count; i++) {
		if (address_book->list[i].si_no == si_no){
			index = i;
			break;
		}
	}

	//Validate index
	if (index == -1)
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
	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	int si_no;

	//Check if there are contacts available
	if (address_book->count == 0)
	{
		printf("No contacts available to delete.\n");
	}

	//Ask which contact to delete
	printf("Enter the contact serial number to delete: ");
	scanf("%d", &si_no);
	getchar();

	int index = -1;
	for (int i = 0; i < address_book->count; i++) {
		if (address_book->list[i].si_no == si_no){
			index = i;
			break;
		}
	}
	//Validate index
	if (index == -1)
	{
		printf("No contact found with the serial number %d.\n", si_no);
		return e_fail;
	}

	//Shift contacts up in the array
	for (int i = index; i < address_book->count - 1; i++)
	{
		address_book->list[i] = address_book->list[i + 1];
	}

	//Reduce total contact count
	address_book->count--;

	printf("\nContact deleted successfully.\n");
	get_option(NONE, "\nPress Enter to continue...");

	return e_success;
}
