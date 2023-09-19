#include <stdio.h>
#include <string.h>

int main(){
    char string[256];
    gets(string);
    char newString[strlen(string)];
    for(int i = 0; i < strlen(string); i++){
        if(string[i] == '\0'){
            newString[i] = '\0';
            break;
        }else if(string[i] != '.'){
            newString[i] = string[i];
        }else{
            newString[i] = '\0';
            break;
        }
    }
    for(int i = strlen(string) - 1; i >= 0; i--){
        printf("%c", newString[i]);
    }
    printf("\n");
    return 0;
}