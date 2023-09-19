#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count(char string[], char character){
    int charCount = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (tolower(string[i]) == tolower(character))
        {
            charCount++;
        }
    }
    return charCount;
}

void countAll(char string[]){
    for (int i = 0; i < strlen(string) - 1; i++)
    {
        printf("%c:%d, ", tolower(string[i]), count(string, string[i]));
    }
    printf("%c:%d\n", tolower(string[strlen(string) - 1]), count(string, string[strlen(string) - 1]));
}

int main(){
    char string[256];
    printf("Enter a string: ");
    scanf("%s", string);
    countAll(string);
    return 0;
}