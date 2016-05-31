#pragma once

#define BOLD(x) "\033[1m" << x << "\033[0m"

#define RED(x) "\033[31m" << x << "\033[0m"
#define GREEN(x) "\033[32m" << x << "\033[0m"
#define YELLOW(x) "\033[33m" << x << "\033[0m"
#define BLUE(x) "\033[34m" << x << "\033[0m"
#define MAGENTA(x) "\033[35m" << x << "\033[0m"
#define CYAN(x) "\033[36m" << x << "\033[0m"
#define WHITE(x) "\033[37m" << x << "\033[0m"

#define REDB(x) "\033[31m" << BOLD(x)
#define GREENB(x) "\033[32m" << BOLD(x)
#define YELLOWB(x) "\033[33m" << BOLD(x)
#define BLUEB(x) "\033[34m" << BOLD(x)
#define MAGENTAB(x) "\033[35m" << BOLD(x)
#define CYANB(x) "\033[36m" << BOLD(x)
#define WHITEB(x) "\033[37m" << BOLD(x)