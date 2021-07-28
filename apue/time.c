#include <time.h>

int main()
{
    time_t cal;
    struct tm* tmp;
    cal = time(NULL);
    tmp = gmtime(&cal);
    tmp = localtime(&cal);
    time_t now = mktime(tmp);
    return 0;
}