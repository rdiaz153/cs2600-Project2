#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "address_book_fops.h"

Status load_file(AddressBook *address_book)
{
	int ret;
	address_book->count = 0; // Initialize count to 0 before loading contacts
	int capacity = 10; // Initial capacity for contacts
	address_book->list = malloc(sizeof(ContactInfo) * capacity); // Allocating memory for 10 contacts initially		
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
			 * token 1 = name
			 * token 2 - 6 = phone numbers
			 * token 7 - 11 = email addresses
			 * token 12 = si_no
			 * Add the data to address_book->list
			 * Increment the count
			 * If count reaches the allocated size, reallocate the memory to accomodate more contacts
			*/
			memset(address_book->list[address_book->count].name, 0, sizeof(address_book->list[address_book->count].name));
			memset(address_book->list[address_book->count].phone_numbers, 0, sizeof(address_book->list[address_book->count].phone_numbers));
			memset(address_book->list[address_book->count].email_addresses, 0, sizeof(address_book->list[address_book->count].email_addresses));
			address_book->list[address_book->count].si_no = 0;
			buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character if present
			char *start = buffer;
			for(int i = 0; i < 12; i++)
			{
				char *end = strchr(start, FIELD_DELIMITER);
				if((end != NULL && i == 11) || (end == NULL && i != 11)) 
				// If it's not the last field and end is NULL, or if it's the last field and end is not NULL, it means the line is malformed
				{
					printf("Line %d is malformed: %s\n", address_book->count + 1, buffer);
					fclose(address_book->fp);
					return e_fail;
				}
				char *field;
				if(i == 11)
				{					
					field = start; // Last field, take the rest of the line
					if(isdigit(field[0]))
						address_book->list[address_book->count].si_no = atoi(field);
					else
					{
						printf("Invalid si_no: %s\n", buffer);
						fclose(address_book->fp);
						return e_fail;
					}
				}
				else
				{
					int length = end - start;
					if(i == 0)
					{
						length_check(length, NAME_LEN);
						strncpy(address_book->list[address_book->count].name[0], start, length);
					}
					else if(i >= 1 && i <= 5)
					{					
						length_check(length, NUMBER_LEN);
						strncpy(address_book->list[address_book->count].phone_numbers[i - 1], start, length);
					}
					else if(i >= 6 && i <= 10)
					{
						length_check(length, EMAIL_ID_LEN);
						strncpy(address_book->list[address_book->count].email_addresses[i - 6], start, length);
					}
					else
					{
						printf("Unexpected field index: %d\n", i);
						fclose(address_book->fp);
						return e_fail;
					}
					start = end + 1;

				}
			}
			address_book->count++;
			if (address_book->count >= capacity)
			{
				ContactInfo *temp = realloc(address_book->list, sizeof(ContactInfo) * (address_book->count + capacity));  
				if (temp == NULL)
				{
					printf("Memory reallocation failed: %s\n", strerror(errno));
					free(address_book->list);
					return e_fail;
				}
				address_book->list = temp;
				capacity *= 2;
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

void length_check(int len, int max_len)
{
	if(len >= max_len)
	{
		printf("Input exceeds maximum length of %d characters.\n", max_len - 1);
		exit(EXIT_FAILURE);
	}
}