#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
  
int main() {
    regex_t re;
  
    if (regcomp(&re, "^ *[A-Z]+ *: *(.*)", REG_EXTENDED) != 0) {
        perror("regcomp");
        exit(EXIT_FAILURE);
    }
  
    char str[] = "  HELLO  :  there world";
  
    regmatch_t m[2];
    if (regexec(&re, str, 2, m, 0) == 0) {
        int start = m[1].rm_so, end = m[1].rm_eo;
        printf("match: %d %d\n", start, end);
        char ch = str[end];
        str[end] = '\0';
        printf("substring: |%s|\n", str + start);
        str[end] = ch;
    }
    else
        printf("no match\n");
  
    return 0;
}