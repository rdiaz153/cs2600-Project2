#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int type, const char *msg)
{
	char input[50];
	int option;

	while(1){
		if(msg){
			printf("%s", msg);
		}
		fgets(input, sizeof(input), stdin);

		if(type == 0){
			return 0;
		}
		else if(type == 1){
			if(sscanf(input, "%d", &option)==1){
				return option;
			}
			else{
				printf("Try again\n");
			}
		}
		else if(type ==2){
			return toupper(input[0]);
		}

		
	}
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
	if (str != '\0')
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
	int index = address_book->count;
	
	int capacity = 10; //temp variable for testing

	while(1){
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
			printf("%s\n", address_book->list[index].name[0]);
			address_book->list[index].si_no = index + 1; // creates serial id for contact added 
			address_book->count++; //increments on every added contact 
			
		}
		else if(option == 2){ // ask user to input phone no
			for(int i = 0; i < PHONE_NUMBER_COUNT; i++){

				if(address_book->list[index].phone_numbers[i][0] == '\0'){ //checks the total amount of phone no (5) to see if thers space
					printf("Enter the Phone Number %d: ", i + 1);
					fgets(address_book->list[index].phone_numbers[i], NUMBER_LEN, stdin);
					break; //will only allow one contact per question
				}
			}
		}
		else if(option == 3){ //ask user to input email address
			for(int i = 0; i < EMAIL_ID_COUNT ; i++){ //same loop, checks counter to see the is enough space for emails.
				if(address_book->list[index].email_addresses[i][0] == '\0'){ 
					printf("Enter the Email Address %d:  ", i+1);
					fgets(address_book->list[index].email_addresses[i], EMAIL_ID_LEN, stdin);
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

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
