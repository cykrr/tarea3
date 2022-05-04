#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>
#include "util.h"

void lower_than(void *k1, void *k2);

int main() {
    List *books = readBooks();
    showList (books);

    return 0;
}
