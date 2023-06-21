#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LENGTH 100


FILE* file;

char line[MAX_LINE_LENGTH];

char* read_cooja_log();



char* read_cooja_log(){
    
    if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        printf("Okunan satir: %s", line);
    }
    
    int a;
    while (fscanf(file,"%d",&a) != EOF){
    printf("%d\n",a);
    }
    
    printf("%d\n", a);

    return line;
}

int main(){

    file = fopen("outputs/id1.txt", "r");
    if (file == NULL) {
        printf("Dosya bulunamadi.");
        return 1;
    }

    read_cooja_log();

    fclose(file);
}