#include <sys/utsname.h>
#include <unistd.h>
int main()
{
    //系统标识
    struct utsname utsname_data;
    int state;
    state = uname(&utsname_data);

    char *data = utsname_data.machine;
    data = utsname_data.nodename;
    data = utsname_data.release;
    data = utsname_data.sysname;
    data = utsname_data.version;

    char hostname[256];
    state = gethostname(hostname, 256);

    return 0;
}