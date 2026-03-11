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
		
		//If the file is empty, treat it as 0 contacts.
		int contactCount;
		if(fscanf(address_book->fp, "%d\n", &contactCount) != 1) {
			fclose(address_book->fp);
			address_book->fp = NULL;
			address_book->count = 0;
			return e_success;
		}

		address_book->count = 0;

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
				} // changing si_no to be the first index (to help later functions)
				if (i == 0) {
					if(isdigit(start[0])) {
						address_book->list[address_book->count].si_no = atoi(start);
					} else {
						printf("Invalid serial number: %s\n", buffer);
						fclose(address_book->fp);
						return e_fail;
					}
					start = end + 1;
				}
				else if (i == 1)
				{
					int length = end - start;
					if(!length_check(length, NAME_LEN))
					{
						fclose(address_book->fp);
						return e_fail;
					}

					if (length == 1 && start[0] == ' ') {
						address_book->list[address_book->count].name[0][0] = '\0';
					} else {
						strncpy(address_book->list[address_book->count].name[0], start, length);
					}
					
					start = end + 1;
				} else if(i >= 2 && i <= 6)
					{					
						int length = end - start;
						if(!length_check(length, NUMBER_LEN))
						{
							fclose(address_book->fp);
							return e_fail;
						}
						if (length == 1 && start[0] == ' ')  { // if empty string, enter a space
						address_book->list[address_book->count].phone_numbers[i - 2][0] = '\0';
						} else {
							strncpy(address_book->list[address_book->count].phone_numbers[i - 2], start, length);
						}
						start = end + 1;
					}
					else if(i >= 7 && i <= 11)
					{
						int length;
						if (i == 1) {
							length = (int)strlen(start);
						} else {
							length = (int)(end - start);
						}
						if(!length_check(length, EMAIL_ID_LEN))
						{
							fclose(address_book->fp);
							return e_fail;
						}
						if (length == 1 && start[0] == ' ') {
							address_book->list[address_book->count].email_addresses[i - 7][0] = '\0';
						} else {
							strncpy(address_book->list[address_book->count].email_addresses[i - 7], start, length);
						}
						if (i < 11)
						start = end + 1;
					}
					else
					{
						printf("Unexpected field index: %d\n", i);
						fclose(address_book->fp);
						return e_fail;
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
		fclose(fp);
		address_book->fp = NULL;
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
		printf("Error opening file for writing: %s\n", strerror(errno));
		return e_fail;
	}

	int ret = fprintf(address_book->fp, "%d\n", address_book->count);
	if(!write_error_check(ret))
	{
		fclose(address_book->fp);
		return e_fail;
	}

	for(int i = 0; i < address_book->count; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			int ret;
			if(j == 0)
			{
				
				ret = fprintf(address_book->fp, "%d,", address_book->list[i].si_no);
				if(!write_error_check(ret))
				{
					fclose(address_book->fp);
					return e_fail;
				}
			}
			else if(j == 1)
			{
				ret = fprintf(address_book->fp, "%s,", strlen(address_book->list[i].name[0]) > 0 ? address_book->list[i].name[0] : " ");
				if(!write_error_check(ret))
				{
					fclose(address_book->fp);
					return e_fail;
				}
			}
			else if(j >= 2 && j <= 6)
			{
				ret = fprintf(address_book->fp, "%s,", strlen(address_book->list[i].phone_numbers[j - 2])> 0 ? address_book->list[i].phone_numbers[j - 2] : " ");
				if(!write_error_check(ret))
				{
					fclose(address_book->fp);
					return e_fail;
				}
			}
			else if(j >= 7 && j <= 11)
			{
				if (j < 11) {
					ret = fprintf(address_book->fp, "%s,", strlen(address_book->list[i].email_addresses[j - 7])> 0 ? address_book->list[i].email_addresses[j - 7] : " ");
				} else {
					ret = fprintf(address_book->fp, "%s\n", strlen(address_book->list[i].email_addresses[j - 7])> 0 ? address_book->list[i].email_addresses[j - 7] : " ");
				}

				if(!write_error_check(ret))
				{
					fclose(address_book->fp);
					return e_fail;
				}
			}
			else 
			{
				printf("Unexpected field index: %d\n", j);
				fclose(address_book->fp);
				return e_fail;
			}
		}
	}

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 
	if(fclose(address_book->fp) < 0)
	{
		printf("Error closing file: %s\n", strerror(errno));
		return e_fail;
	}
	address_book->fp = NULL;

	return e_success;
}

bool length_check(int len, int max_len)
{
	if(len >= max_len)
	{
		printf("Input exceeds maximum length of %d characters.\n", max_len - 1);
		return false;
	}
	return true;
}

bool write_error_check(int ret)
{
	if(ret < 0)
	{
		printf("Error writing to file: %s\n", strerror(errno));
		return false;
	}
	return true;
}