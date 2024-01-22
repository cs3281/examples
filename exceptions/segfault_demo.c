#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    /* show a segmentation fault occuring by not allocating memory to a pointer */

    /* declare a pointer to char */
    char *str;

    /* define some garbage literal string */
    char *literal = "adioofsdfffkfsdfsdlf;lflsdfdsfjfjklfjdfjdsfjdlfjdsl;f"; 

    printf ("string contents of the literal string = %s\n", literal);

    /* now try copying the contents of the literal into the pointer without
       allocating any memory thereby causing segmentation fault */
    strcpy (str, literal);

    printf ("string contents pointed to by the pointer = %s\n", str);
}
