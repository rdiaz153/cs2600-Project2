#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book_fops.h"

Status load_file(AddressBook *address_book)
{
	int ret;
	address_book->count = 0; // Initialize count to 0 before loading contacts
	address_book->list = malloc(sizeof(ContactInfo) * 10); // Allocating memory for 10 contacts initially		
	if(address_book->list == NULL)
	{
		printf("Memory allocation failed: %s\n", strerror(errno));
		free(address_book->list);
		return e_fail;
	}

	printf("Loading the file...\n");
	/* 
	 * Check for file existance
	 */
	ret = access(DEFAULT_FILE, F_OK);
	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */
		address_book->fp = fopen(DEFAULT_FILE, "r");
		if(address_book->fp == NULL)
		{
			printf("Error opening file: %s\n", strerror(errno));
			free(address_book->list);
			return e_fail;
		}

		char buffer[420];
		while (fgets(buffer, sizeof(buffer), address_book->fp) != NULL)
		{
			/* 
			 * Add the logic to read the file and populate the address_book->list
			 * Make sure to do error handling
			 */ 
			/*
			 * read line
			 * parse the line using strtok
			 * token 1 = name
			 * token 2 - 6 = phone numbers
			 * token 7 - 11 = email addresses
			 * token 12 = si_no
			 * Add the parsed data to address_book->list
			 * Increment the count
			 * If count reaches the allocated size, reallocate the memory to accomodate more contacts
			*/
			buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character if present
			char *token = strtok(buffer, FIELD_DELIMITER);
			for(int i = 0; i < 12; i++)
			{
				if (token == NULL)
				{
					printf("Error parsing file: %s\n", strerror(errno));
					free(address_book->list);
					return e_fail;
				}
				if(i == 0)
					strcpy(address_book->list[address_book->count].name[0], token);
				else if(i >= 1 && i <= 5)
					strcpy(address_book->list[address_book->count].phone_numbers[i-1], token);
				else if(i >= 6 && i <= 10)
					strcpy(address_book->list[address_book->count].email_addresses[i-6], token);
				else
					address_book->list[address_book->count].si_no = atoi(token);
			}
			address_book->count++;
			if (address_book->count >= 10)
			{
				ContactInfo *temp = realloc(address_book->list, sizeof(ContactInfo) * (address_book->count + 10));
				if (temp == NULL)
				{
					printf("Memory reallocation failed: %s\n", strerror(errno));
					free(address_book->list);
					return e_fail;
				}
				address_book->list = temp;
			}
		}

		fclose(address_book->fp);
	}
	else
	{
		/* Create a file for adding entries */
		printf("File not found. Creating a new file...\n");
		FILE *fp = fopen(DEFAULT_FILE, "w");
		if (fp == NULL)
		{
			printf("Error creating file: %s\n", strerror(errno));
			free(address_book->list);
			return e_fail;
		}
		address_book->fp = fp;
		fclose(address_book->fp);
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 
	fclose(address_book->fp);

	return e_success;
}
