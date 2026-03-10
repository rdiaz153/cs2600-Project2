#ifndef ABK_FILEOPS_H
#define ABK_FILEOPS_H

#include <stdbool.h>
#include "address_book.h"

Status load_file(AddressBook *address_book);
Status save_file(AddressBook *address_book);

bool length_check(int len, int max_len);
bool write_error_check(int ret);

#endif
