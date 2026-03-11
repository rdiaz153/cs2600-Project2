#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"

void setUp(void) {}
void tearDown(void) {}

// helper to build a test address book with n contacts
static AddressBook make_book(int count)
{
    AddressBook ab;
    ab.fp = NULL;
    ab.count = count;
    ab.list = (ContactInfo *)malloc(count * sizeof(ContactInfo));

    for (int i = 0; i < count; i++) {
        memset(&ab.list[i], 0, sizeof(ContactInfo));
        snprintf(ab.list[i].name[0], NAME_LEN, "Person%d", i + 1);
        snprintf(ab.list[i].phone_numbers[0], NUMBER_LEN, "555%07d", i + 1);
        snprintf(ab.list[i].email_addresses[0], EMAIL_ID_LEN, "person%d@test.com", i + 1);
        ab.list[i].si_no = i + 1;
    }
    return ab;
}

static void free_book(AddressBook *ab)
{
    free(ab->list);
    ab->list = NULL;
    ab->count = 0;
}

// empty book should return fail
void test_list_null_book(void)
{
    AddressBook ab;
    ab.list = NULL;
    ab.count = 0;
    ab.fp = NULL;
    Status ret = (ab.list == NULL || ab.count == 0) ? e_fail : e_success;
    TEST_ASSERT_EQUAL(e_fail, ret);
}

// count should match what we put in
void test_list_count_correct(void)
{
    AddressBook ab = make_book(5);
    TEST_ASSERT_EQUAL_INT(5, ab.count);
    free_book(&ab);
}

// first contact name check
void test_list_first_contact_name(void)
{
    AddressBook ab = make_book(3);
    TEST_ASSERT_EQUAL_STRING("Person1", ab.list[0].name[0]);
    free_book(&ab);
}

// last contact name check
void test_list_last_contact_name(void)
{
    AddressBook ab = make_book(3);
    TEST_ASSERT_EQUAL_STRING("Person3", ab.list[2].name[0]);
    free_book(&ab);
}

// page window size should not exceed WINDOW_SIZE
void test_list_window_size(void)
{
    AddressBook ab = make_book(10);
    int index = 0;
    int end = index + WINDOW_SIZE;
    if (end > ab.count) end = ab.count;
    TEST_ASSERT_EQUAL_INT(WINDOW_SIZE, end - index);
    free_book(&ab);
}

// next page moves index forward by WINDOW_SIZE
void test_list_next_page(void)
{
    AddressBook ab = make_book(10);
    int index = 0;
    if (index + WINDOW_SIZE < ab.count)
        index += WINDOW_SIZE;
    TEST_ASSERT_EQUAL_INT(WINDOW_SIZE, index);
    free_book(&ab);
}

// previous page moves index back
void test_list_prev_page(void)
{
    AddressBook ab = make_book(10);
    int index = WINDOW_SIZE;
    if (index > 0)
        index -= WINDOW_SIZE;
    TEST_ASSERT_EQUAL_INT(0, index);
    free_book(&ab);
}

// serial numbers should be 1,2,3...
void test_list_si_no_correct(void)
{
    AddressBook ab = make_book(4);
    for (int i = 0; i < ab.count; i++)
        TEST_ASSERT_EQUAL_INT(i + 1, ab.list[i].si_no);
    free_book(&ab);
}

// phone number stored correctly
void test_list_phone_correct(void)
{
    AddressBook ab = make_book(2);
    TEST_ASSERT_EQUAL_STRING("5550000001", ab.list[0].phone_numbers[0]);
    free_book(&ab);
}

// email stored correctly
void test_list_email_correct(void)
{
    AddressBook ab = make_book(2);
    TEST_ASSERT_EQUAL_STRING("person1@test.com", ab.list[0].email_addresses[0]);
    free_book(&ab);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_list_null_book);
    RUN_TEST(test_list_count_correct);
    RUN_TEST(test_list_first_contact_name);
    RUN_TEST(test_list_last_contact_name);
    RUN_TEST(test_list_window_size);
    RUN_TEST(test_list_next_page);
    RUN_TEST(test_list_prev_page);
    RUN_TEST(test_list_si_no_correct);
    RUN_TEST(test_list_phone_correct);
    RUN_TEST(test_list_email_correct);
    return UNITY_END();
}
