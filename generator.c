#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *plik;
    int i, j;

    plik = fopen("liczby.txt", "w");
    if (plik == NULL)
    {
        printf("Blad otwarcia pliku \"liczby.txt\"\n");
        exit(0);
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)

            fprintf(plik, "%6.1f", (float)i + j);
        fprintf(plik, "\n");
    }
    fclose(plik);
    return 0;
}