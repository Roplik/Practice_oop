#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char *argv[]) 
{
    char *username = NULL; 

    if (argc > 1) 
    {
        username = argv[1]; 
    }
    
    if (username == NULL) 
    {
        username = getenv("USER");
        if (username == NULL) 
        {
            username = getenv("USERNAME");
        }
    }


    if (username != NULL) 
    {
        printf("Hello, %s!\n", username);
    } 
    else 
    {
        printf("Hello, USER!\n");
    }

    return 0;
}