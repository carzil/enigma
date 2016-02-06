#include "zlib.h"

#include <stdio.h>

void check_external_zlib() {
    puts(zlibVersion());
}