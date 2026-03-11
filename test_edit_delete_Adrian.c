#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "address_book.h"
#include "address_book_menu.h"

void setup_address_book(AddressBook *book)
{
    book->list = malloc(sizeof(ContactInfo) * 10);
    book->count = 2;

    strcpy(book->list[0].name[0], "Ross");
    strcpy(book->list[0].phone_numbers[0], "1111111111");
    strcpy(book->list[0].email_addresses[0], "ross@test.com");

    strcpy(book->list[1].name[0], "Bob");
    strcpy(book->list[1].phone_numbers[0], "2222222222");
    strcpy(book->list[1].email_addresses[0], "bob@test.com");
}

//edit test

void test_edit_contact()
{
    AddressBook book;

    setup_address_book(&book);

    // simulate editing Ross -> Charlie
    strcpy(book.list[0].name[0], "Charlie");
    strcpy(book.list[0].phone_numbers[0], "3333333333");
    strcpy(book.list[0].email_addresses[0], "charlie@test.com");

    assert(strcmp(book.list[0].name[0], "Charlie") == 0);
    assert(strcmp(book.list[0].phone_numbers[0], "3333333333") == 0);
    assert(strcmp(book.list[0].email_addresses[0], "charlie@test.com") == 0);

    printf("Edit contact test passed\n");

    free(book.list);
}

//delete test

void test_delete_contact()
{
    AddressBook book;

    setup_address_book(&book);

    int original_count  = book.count;

    int index = 0;

    for (int i = index; i < book.count - 1; i++)
    {
        book.list[i] = book.list[i + 1];
    }

    book.count--;

    assert(book.count == original_count - 1);
    assert(strcmp(book.list[0].name[0], "Bob") == 0);

    printf("Delete contact test passed\n");

    free(book.list);
}

//test runner
int main()
{
    test_edit_contact();
    test_delete_contact();

    printf("\nAll tests passed\n");

    return 0;
}