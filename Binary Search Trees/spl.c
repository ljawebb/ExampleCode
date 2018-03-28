/*
* Text File Spellchecker
* Checks a text file against a given dictionary text file and finds the mispelt words.
* Laurence Webb 2017 <lw17773@my.bristol.ac.uk>
*/

#include "bst.h"

#define MAXWORDLENGTH 50

bst* create_dictionary(const char* dict);
bst* create_mispelt(const char* check, bst* dictionary);
int mystrcmp(const void* a, const void* b);
char* myprintstr(const void* v);

int main(int argc, const char *argv[]) {

    bst *dictionary, *mispelt;

    if(argc == 3){
        printf("Spellchecking %s against %s...\n", argv[2], argv[1]);
    }
    else {
        printf("ERROR: Incorrect usage, please provide a dictionary file and file to check "
        "e.g. %s dictionary.txt words.txt\n", argv[0]);
        exit(1);
    }

    dictionary = create_dictionary(argv[1]);
    mispelt    = create_mispelt(argv[2], dictionary);

    bst_free(&dictionary);
    bst_free(&mispelt);

    return 0;
}

bst* create_dictionary(const char* dict)
{
    FILE *dictionary;
    char word[MAXWORDLENGTH];
    bst* d;

    d = bst_init(MAXWORDLENGTH, mystrcmp, myprintstr);
    dictionary = fopen(dict, "r");

    while (fscanf(dictionary, "%s", word) != EOF) {
        bst_insert(d, word);
    }

    fclose(dictionary);
    return d;
}

bst* create_mispelt(const char* check, bst* dictionary)
{
    FILE *mispelt;
    char word[MAXWORDLENGTH];
    bst* m;

    m = bst_init(MAXWORDLENGTH, mystrcmp, myprintstr);
    mispelt = fopen(check, "r");

    while (fscanf(mispelt, "%s", word) != EOF) {
        if (bst_isin(dictionary, word) == false) {
            if (bst_isin(m, word) == false) {
                printf("%s\n", word);
                bst_insert(m, word);
            }
        }
    }

    fclose(mispelt);
    return m;
}

int mystrcmp(const void* a, const void* b)
{
    return strcmp((char*)a, (char*)b);
}

char* myprintstr(const void* v)
{
    return (char*)v;
}
