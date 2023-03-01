#define SIZE 20
#define ARRAY_SIZE (SIZE*SIZE)

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define COLOR_BLACK  "\033[0;30m"
#define COLOR_WHITE "\033[0;37m"

static const char charset[] = ". 123456789*F";
static const char* colours[] = {
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    ANSI_COLOR_BLUE,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_RED,
    ANSI_COLOR_MAGENTA,
    ANSI_COLOR_YELLOW,
    ANSI_COLOR_CYAN,
    COLOR_BLACK,
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    COLOR_WHITE,
    ANSI_COLOR_RED
};