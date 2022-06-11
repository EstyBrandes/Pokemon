#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define SIX 6
typedef int Bool;

typedef struct Pokemon
{
    int ID;
    char *number;
    char *name;
    char *type1;
    char *type2;
} Pokemon_t;

typedef struct pokemonCollection
{
    Pokemon_t **collection;
    int logicSize;
    int physicSize;
} PokemonCollection_t;

typedef struct Trainer
{
    char *firstName;
    char *lastName;
    PokemonCollection_t pokemons;
    Pokemon_t *pokemonsHeldByTheTrainer[SIX]; // 6 pokemons in the trainers hand//
    int numOfPokemonsInHand;
} Trainer_t;

// ------ Declerations-----//
void printMenu();
int askMenuChoice();
void askFullName(Trainer_t *user);
char *getString();
void checkMemoryAlloc(void *pointer);
int makeAction(int choice, Trainer_t *user, PokemonCollection_t *filePokemons, int *pokId);
void getPokemonsFromFile(char *fileName, PokemonCollection_t *filePokemons);
void initPokemonCollection(PokemonCollection_t *filePokemons);
Pokemon_t *createPokemon(char *str);
void updatePokemon(Pokemon_t *pok, int i, char *data);
void printPokemon(Pokemon_t *pok);
void printPokemons(PokemonCollection_t *poks);
void freePokCollection(PokemonCollection_t *poks);
void freePokemon(Pokemon_t *pok);
void freeTrainer(Trainer_t *user);
void printPokemonsByType(PokemonCollection_t *pok, char *type);
void catchNewPokemon(Trainer_t *user, PokemonCollection_t *filepokemon, int *pokId);
void addPokemonToTrainer(Trainer_t *user, Pokemon_t *pok, int *pokId);
void printPokemonsInHand(Trainer_t *user);
void printPokemonsInBank(Trainer_t *user);
void initTrainer(Trainer_t *user);
void addPokemonToCollection(PokemonCollection_t *pokCollection, Pokemon_t *pok);
Pokemon_t *clone(Pokemon_t *pok);
void replaceBetweenPokemons(Trainer_t *user);
Pokemon_t **getPokById(int id, Pokemon_t **pokCollection, int sizeCollection);
void releasePokemon(Trainer_t *user);
Bool releasePokFromCollection(int id, Pokemon_t **collection, int *size);
void fixCollection(int i, Pokemon_t **collection, int size);
void writeToFileByType(char *type, PokemonCollection_t *pokCollection);
void writeCollectionToFile(char *fileName, Pokemon_t **collection, int size);
void checkFileOpening(FILE *f);
void sortByColumn(Pokemon_t **pokCollection, int size, char *column);
void swapPokemons(Pokemon_t **p1, Pokemon_t **p2);

void main(int argc, char *argv[])
{
    // /program Pokemon.txt
    PokemonCollection_t filePokemons;
    Trainer_t user;
    initTrainer(&user);
    initPokemonCollection(&filePokemons);
    askFullName(&user);                          // function to check trainers name
    getPokemonsFromFile(argv[1], &filePokemons); // function to open pokemons file

    int pokId = 0;

    while (true)
    {
        printMenu();
        int userChoice = askMenuChoice();
        getchar();
        if (makeAction(userChoice, &user, &filePokemons, &pokId))
            break;
    }
    freePokCollection(&filePokemons);
    freeTrainer(&user);
}

void printMenu()
{
    printf("Choose Your Option :\n");
    printf("0.  Exit\n");
    printf("1.  Change Your Trainers Name:\n");
    printf("2.  Reload pokemons file:\n");
    printf("3.  Print  pokemons by Category:\n");
    printf("4.  Export pokemons by Type:\n");
    printf("5.  Export pokemons list by ABC:\n");
    printf("6.  Add pokemons to your pocket:\n");
    printf("7.  Replace pokemons:\n");
    printf("8.  Release pokemon:\n");
    printf("9.  Print all the pokemons in your pocket:\n");
    printf("10. Print all pokemons in the bank:\n");
}

int askMenuChoice()
{
    printf("\n---Please enter your choice:  \n");
    int choice = 0;
    scanf("%d", &choice);
    return choice;
}

void askFullName(Trainer_t *user)
{
    printf("First Name - ");
    user->firstName = getString();
    printf("\nLast Name - ");
    user->lastName = getString();
}

char *getString()
{
    int lSize = 0, pSize = 1;
    char *res = (char *)malloc(sizeof(char) * pSize);
    checkMemoryAlloc(res);
    char ch = getchar();
    while (ch != '\n')
    {
        if (lSize == pSize)
        {
            pSize *= 2;
            res = (char *)realloc(res, sizeof(char) * pSize);
            checkMemoryAlloc(res);
        }
        res[lSize] = ch;
        lSize++;
        ch = getchar();
    }
    res = (char *)realloc(res, sizeof(char) * (lSize + 1));
    res[lSize] = '\0';

    checkMemoryAlloc(res);
    return res;
}

void checkMemoryAlloc(void *pointer)
{
    if (pointer == NULL)
    {
        printf("Alloc Failed");
        exit(1);
    }
}

int makeAction(int choice, Trainer_t *user, PokemonCollection_t *filePokemons, int *pokId)
{
    int exit = 0;
    switch (choice)
    {
    case 0:
        exit = 1;
        break;
    case 1:
        free(user->firstName);
        free(user->lastName);
        askFullName(user);
        break;
    case 2:
        printf("\nEnter file name to load - ");
        char *fileName = getString();
        getPokemonsFromFile(fileName, filePokemons);
        free(fileName);
        break;
    case 3:
        printf("\nEnter type to print - ");
        char *type = getString();
        printPokemonsByType(filePokemons, type);
        free(type);
        break;
    case 4:
        printf("\nEnter type into a file - ");
        char *typeToWrite = getString();
        writeToFileByType(typeToWrite, filePokemons);
        free(typeToWrite);
        break;
    case 5:
        printf("\nEnter column to sort by - ");
        char *column = getString();
        sortByColumn(filePokemons->collection, filePokemons->logicSize, column);
        writeCollectionToFile("sortedPokemons", filePokemons->collection, filePokemons->logicSize);
        free(column);
        break;
    case 6:
        catchNewPokemon(user, filePokemons, pokId);
        break;
    case 7:
        replaceBetweenPokemons(user);
        break;
    case 8:
        releasePokemon(user);
        break;
    case 9:
        printPokemonsInHand(user);
        break;
    case 10:
        printPokemonsInBank(user);
        break;
    default:
        break;
    }
    return exit;
}

void getPokemonsFromFile(char *fileName, PokemonCollection_t *filePokemons)
{
    char buffer[1024];
    FILE *fp = fopen(fileName, "r");
    checkFileOpening(fp);
    while (fgets(buffer, 1024, fp))
        addPokemonToCollection(filePokemons, createPokemon(buffer));
    fclose(fp);
}

void initPokemonCollection(PokemonCollection_t *filePokemons)
{
    filePokemons->logicSize = 0;
    filePokemons->physicSize = 1;
    filePokemons->collection = (Pokemon_t **)malloc(sizeof(Pokemon_t *));
    checkMemoryAlloc(filePokemons->collection);
}

Pokemon_t *createPokemon(char *str)
{
    Pokemon_t *res = (Pokemon_t *)malloc(sizeof(Pokemon_t));
    checkMemoryAlloc(res);
    char *token = strtok(str, " \n");
    int i = 0;

    while (token != NULL)
    {
        // printf("%d, %s", i, token);
        updatePokemon(res, i, token);
        token = strtok(NULL, " \n");
        i++;
    }
    if (i == 3)
        res->type2 = NULL;
    return res;
}

void updatePokemon(Pokemon_t *pok, int i, char *data)
{
    switch (i)
    {
    case 0:
        pok->number = (char *)malloc(sizeof(char) * strlen(data));
        checkMemoryAlloc(pok->number);
        strcpy(pok->number, data);
        break;
    case 1:
        pok->name = (char *)malloc(sizeof(char) * strlen(data));
        checkMemoryAlloc(pok->name);
        strcpy(pok->name, data);
        break;
    case 2:
        pok->type1 = (char *)malloc(sizeof(char) * strlen(data));
        checkMemoryAlloc(pok->type1);
        strcpy(pok->type1, data);
        break;
    case 3:
        pok->type2 = (char *)malloc(sizeof(char) * strlen(data));
        checkMemoryAlloc(pok->type2);
        strcpy(pok->type2, data);
        break;
    default:
        break;
    }
}

void printPokemon(Pokemon_t *pok)
{
    printf("%s ", pok->number);
    printf("%s ", pok->name);
    printf("%s ", pok->type1);
    if (pok->type2 != NULL)
        printf("%s", pok->type2);
    printf("\n");
}

void printPokemons(PokemonCollection_t *poks)
{
    int i = 0;
    for (i = 0; i < poks->logicSize; i++)
        printPokemon(poks->collection[i]);
}

void freePokCollection(PokemonCollection_t *poks)
{
    int i;
    for (i = 0; i < poks->logicSize; i++)
    {
        freePokemon(poks->collection[i]);
    }
    free(poks->collection);
}

void freePokemon(Pokemon_t *pok)
{
    free(pok->name);
    free(pok->number);
    free(pok->type1);
    free(pok->type2);
}

void freeTrainer(Trainer_t *user)
{
    free(user->firstName);
    free(user->lastName);
    freePokCollection(&user->pokemons);
}

void printPokemonsByType(PokemonCollection_t *pok, char *type)
{
    int i;
    for (size_t i = 0; i < pok->logicSize; i++)
    {
        if (!strcmp(type, pok->collection[i]->type1))
        {
            printPokemon(pok->collection[i]);
        }
        if (pok->collection[i]->type2 && !strcmp(type, pok->collection[i]->type1))
        {
            printPokemon(pok->collection[i]);
        }
    }
}

void catchNewPokemon(Trainer_t *user, PokemonCollection_t *filepokemon, int *pokId)
{
    int i;
    printf("\nEnter the name of the pokemon: ");
    char *newPokemonName = getString();
    Bool found = false;
    for (i = 0; i < filepokemon->logicSize; i++)
    {
        if (strcmp(newPokemonName, filepokemon->collection[i]->name) == 0)
        {
            addPokemonToTrainer(user, filepokemon->collection[i], pokId);
            found = true;
            break;
        }
    }
    if (!found)
        printf("No such pokemon!\n");
    free(newPokemonName);
}

void initTrainer(Trainer_t *user)
{
    user->firstName = NULL;
    user->lastName = NULL;
    user->numOfPokemonsInHand = 0;
    initPokemonCollection(&user->pokemons);
}

void addPokemonToTrainer(Trainer_t *user, Pokemon_t *pok, int *pokId)
{
    int i;
    Pokemon_t *newPok = clone(pok);
    newPok->ID = *pokId;
    (*pokId)++;
    if (user->numOfPokemonsInHand < SIX)
    {
        user->pokemonsHeldByTheTrainer[user->numOfPokemonsInHand] = newPok;
        user->numOfPokemonsInHand++;
    }
    else
    {
        addPokemonToCollection(&user->pokemons, newPok);
    }
}

Pokemon_t *clone(Pokemon_t *pok)
{
    Pokemon_t *res = (Pokemon_t *)malloc(sizeof(Pokemon_t));
    res->name = (char *)malloc(sizeof(char) * (strlen(pok->name) + 1));
    checkMemoryAlloc(res->name);
    strcpy(res->name, pok->name);
    res->number = (char *)malloc(sizeof(char) * (strlen(pok->number) + 1));
    checkMemoryAlloc(res->number);
    strcpy(res->number, pok->number);
    res->type1 = (char *)malloc(sizeof(char) * (strlen(pok->type1) + 1));
    checkMemoryAlloc(res->type1);
    strcpy(res->type1, pok->type1);
    if (pok->type2 != NULL)
    {
        res->type2 = (char *)malloc(sizeof(char) * (strlen(pok->type2) + 1));
        checkMemoryAlloc(res->type2);
        strcpy(res->type2, pok->type2);
    }
    else
    {
        res->type2 = NULL;
    }
    return res;
}

void addPokemonToCollection(PokemonCollection_t *pokCollection, Pokemon_t *pok)
{
    if (pokCollection->logicSize == pokCollection->physicSize)
    {
        pokCollection->physicSize *= 2;
        pokCollection->collection = (Pokemon_t **)realloc(pokCollection->collection, pokCollection->physicSize * sizeof(Pokemon_t *));
        checkMemoryAlloc(pokCollection->collection);
    }

    pokCollection->collection[pokCollection->logicSize] = pok;
    pokCollection->logicSize++;
}

void replaceBetweenPokemons(Trainer_t *user)
{
    int idToSend = -1, idToGet = -1;
    Pokemon_t **pokToSend = NULL, **pokToGet = NULL;
    printf("\nEnter Pokemon to send - ");
    scanf("%d", &idToSend);
    pokToSend = getPokById(idToSend, user->pokemonsHeldByTheTrainer, user->numOfPokemonsInHand);
    if (pokToSend == NULL)
    {
        printf("\nInvalid ID Pokemon to send");
        return;
    }
    printf("\nEnter Pokemon to get - ");
    scanf("%d", &idToGet);
    pokToGet = getPokById(idToGet, user->pokemons.collection, user->pokemons.logicSize);
    if (pokToGet == NULL)
    {
        printf("\nInvalid ID Pokemon to get");
        return;
    }
    swapPokemons(pokToSend, pokToGet);
    printf("\n Replace is done (:");
}

Pokemon_t **getPokById(int id, Pokemon_t **pokCollection, int sizeCollection)
{
    int i;
    Pokemon_t **res = NULL;
    for (i = 0; i < sizeCollection; i++)
    {
        if (pokCollection[i]->ID == id)
        {
            res = pokCollection + i;
            break;
        }
    }
    return res;
}

void printPokemonsInHand(Trainer_t *user)
{
    int i;
    if (user->numOfPokemonsInHand == 0)
    {
        printf("\nNo pokemons in hand! \n");
    }
    for (i = 0; i < user->numOfPokemonsInHand; i++)
    {
        printPokemon(user->pokemonsHeldByTheTrainer[i]);
    }
}

void printPokemonsInBank(Trainer_t *user)
{
    int i;
    if (user->pokemons.logicSize == 0)
    {
        printf("\nNo pokemons in bank! \n");
    }
    for (i = 0; i < user->pokemons.logicSize; i++)
    {
        printPokemon(user->pokemons.collection[i]);
    }
}

void releasePokemon(Trainer_t *user)
{
    int i, idToRelease = -1;
    Bool found = false;
    printf("\nPlease enter pokemon Id to release - ");
    scanf("%d", &idToRelease);
    found = releasePokFromCollection(idToRelease, user->pokemonsHeldByTheTrainer, &user->numOfPokemonsInHand);
    if (!found)
    {
        releasePokFromCollection(idToRelease, user->pokemons.collection, &user->pokemons.logicSize);
    }
}

Bool releasePokFromCollection(int id, Pokemon_t **collection, int *size)
{
    int i;
    Bool found = false;
    for (i = 0; i < *size; i++)
    {
        if (collection[i]->ID == id)
        {
            freePokemon(collection[i]);
            fixCollection(i, collection, *size);
            (*size)--;
            found = true;
            break;
        }
    }
    return found;
}

void fixCollection(int i, Pokemon_t **collection, int size)
{
    int t;
    for (t = i; t < size - 1; i++)
        collection[t] = collection[t + 1];
}

void writeToFileByType(char *type, PokemonCollection_t *pokCollection)
{
    int i;
    Pokemon_t **pokemosToWrite = (Pokemon_t **)malloc(sizeof(Pokemon_t *) * pokCollection->logicSize);
    int logsize = 0;
    for (i = 0; i < pokCollection->logicSize; i++)
    {
        if (!strcmp(pokCollection->collection[i]->type1, type))
        {
            pokemosToWrite[logsize] = pokCollection->collection[i];
            logsize++;
        }
        if (pokCollection->collection[i]->type2 != NULL && !strcmp(pokCollection->collection[i]->type2, type))
        {
            pokemosToWrite[logsize] = pokCollection->collection[i];
            logsize++;
        }
    }
    writeCollectionToFile(type, pokemosToWrite, logsize);
    free(pokemosToWrite);
}

void writeCollectionToFile(char *fileName, Pokemon_t **collection, int size)
{
    char *f = (char *)malloc(sizeof(char) * (strlen(fileName) + 5));
    checkMemoryAlloc(f);
    strcpy(f, fileName);
    strcat(f, ".txt");
    printf("%s", f);
    FILE *fp = fopen(f, "w");
    checkFileOpening(fp);
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(fp, "%s ", collection[i]->number);
        fprintf(fp, "%s ", collection[i]->name);
        fprintf(fp, "%s", collection[i]->type1);
        if (collection[i]->type2 != NULL)
            fprintf(fp, " %s\n", collection[i]->type2);
        else
            fprintf(fp, "\n");
    }
    fclose(fp);
}

void checkFileOpening(FILE *f)
{
    if (f == NULL)
    {
        printf("\nFailed to open file!");
        exit(1);
    }
}

void sortByColumn(Pokemon_t **pokCollection, int size, char *column)
{
    int i, j;
    printf("%s", column);
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - 1 - i; j++)
            if (!strcmp(column, "name"))
            {
                if (0 < strcmp(pokCollection[j]->name, pokCollection[j + 1]->name))
                    swapPokemons(pokCollection + j, pokCollection + j + 1);
            }
            else if (!strcmp(column, "type1"))
            {
                if (0 < strcmp(pokCollection[j]->type1, pokCollection[j + 1]->type1))
                    swapPokemons(pokCollection + j, pokCollection + j + 1);
            }
            else if (!strcmp(column, "type2"))
            {
                if (pokCollection[j]->type2 == NULL)
                    swapPokemons(pokCollection + j, pokCollection + j + 1);
                else if (pokCollection[j + 1]->type2 != NULL && 0 < strcmp(pokCollection[j]->type2, pokCollection[j + 1]->type2))
                    swapPokemons(pokCollection + j, pokCollection + j + 1);
            }
            else
            {
                if (0 < strcmp(pokCollection[j]->number, pokCollection[j + 1]->number))
                    swapPokemons(pokCollection + j, pokCollection + j + 1);
            }
}

void swapPokemons(Pokemon_t **p1, Pokemon_t **p2)
{
    Pokemon_t *tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}