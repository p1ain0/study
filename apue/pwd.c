#include <pwd.h>
#include<string.h>
#include<stdlib.h>
int main()
{
    struct passwd* c = getpwuid(0);
    c = getpwnam("root");
    while(c = getpwent())
    {
        if(strcmp(c->pw_name, "dh" ) == 0)
            break;
        
    }
    endpwent();
    endpwent();
    return 0;
}