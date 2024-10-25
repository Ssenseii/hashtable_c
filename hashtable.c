/*

[ X ] - Handle Collision
[ X ] - Dynamic Resizing
[ X ] - Key Value Retrieval - Insertion - Deletion
[ ] - Improved Hash Function
[ X ] - Load Factor calculation
[ ] - Null Key Handling
[ ] - Memory management
[ ] - Error Handling
[ ] - Debugging Output
[ ] - Iterators
[ ] - Unit Tests
[ ] - Configurable Size
[ ] - String Key Management
[ X ] - Handle Collision
[ X ] - Long Term Storage
[ ] - Documentation
[ ] - Performance Measurement
[ X ] - Better CLI Interface

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_SIZE 53
#define P 53
#define M 1000000007
#define LOAD_FACTOR_THRES 0.7

#define MAX_KEY_SIZE 64
#define MAX_VALUE_SIZE 256

// this is an item in the Hash table
typedef struct
{
    char *key;
    char *value;
} ht_item;

// this is the hash table
typedef struct
{
    int size;
    int count;
    ht_item **items;
} ht_table;

// Helper Functions
void trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] > '\0';
    }
}

// check if the key is non-null and non-empty
int is_valid_key(const char *key)
{
    if (key == NULL || strlen(key) == 0)
    {
        printf("Error: Key Cannot Be Empty");
        return 0;
    }

    for (size_t i = 0; i < strlen(key); i++)
    {
        if (!isalnum(key[i]))
        {
            printf("Error: Key can only contain alphanumeric characters.\n");
            return 0;
        }
    }

    return 1;
}

/*
 * @brief Creates a new hashtable item with the given key value pair.
 * @param k pointer to a nul-terminated string representing the key
 * @param v point to a null-terminated string representing the value
 * @return pointer to the newly allocated ht_item struct.
 */
static ht_item *ht_new_item(char *k, char *v)
{
    ht_item *i = malloc(sizeof(ht_item));
    if (i == NULL)
    {
        return NULL; // Handle memory allocation failure
    }
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

/*
 * @brief Creates a new hashtable with a specified size in the main function.
 * @param size integer representing the initial size of the hashtable
 * @return hashtable allocated within the memory with a count of 0 items.
 */
ht_table *ht_new(int size)
{
    ht_table *ht = malloc(sizeof(ht_table));

    ht->count = 0;
    ht->size = size;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));

    return ht;
}

/// deleting an freeing memory space from a item
static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

/// garbage collection for the entire table.
void ht_del_hash_table(ht_table *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

/// @brief  Hash function
/// @param s  String that is to be hashed: the key
/// @return the hash value of the key

int compute_hash(const char *s)
{
    int hash_value = 0;
    int p_pow = 1;

    for (int i = 0; s[i] != '\0'; i++)
    {
        hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow) % M;
        p_pow = (p_pow * P) % M;
    }

    return hash_value;
}

/// @brief collision checking function using double hashing
/// @param s the string that is to be hashed.

static int ht_get_hash(
    const char *s, const int num_buckets, const int attempt)
{
    const int hash_a = compute_hash(s);
    const int hash_b = 1 + (compute_hash(s) % (num_buckets - 1));
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_remove(ht_table *ht, ht_item *item)
{
    int index = ht_get_hash(item->key, ht->size, 0);
    if (ht->items[index] != NULL)
    {
        index = ht_get_hash(item->key, ht->size, 1);
        if (ht->items[index] != NULL)
        {
            printf("Key Doesn't Exist");
        }
    }

    ht_del_item(item);
    ht->count--;

    printf("Removed item\n");
    printf("Current Count = %d\n", ht->count);
}

void ht_insert(ht_table *ht, ht_item *item)
{
    int index = ht_get_hash(item->key, ht->size, 0);
    if (ht->items[index] != NULL)
    {
        index = ht_get_hash(item->key, ht->size, 1);
    }

    ht->items[index] = item;
    ht->count++;

    printf("Added item: key = %s, value = %s\n", ht->items[index]->key, ht->items[index]->value);
    printf("Current Count = %d\n", ht->count);
}

void ht_retrieve(ht_table *ht, char *key)
{
    int index = ht_get_hash(key, ht->size, 0);
    if (ht->items[index] != NULL && strcmp(ht->items[index]->key, key) == 0)
    {
        printf("Item Found: \n Key = %s \n Value = %s", ht->items[index]->key, ht->items[index]->value);
    }
    else
    {
        printf("Item Not Found\n");
    };
}

void ht_resize(ht_table *ht, int new_size)
{
    ht_table *new_ht = ht_new(new_size);

    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_insert(new_ht, item); /// rehashing the items from start.
        }
    }

    free(ht->items);
    ht->size = new_ht->size;
    ht->count = new_ht->count;
    free(new_ht);
}

void ht_resize_if_needed(ht_table *ht)
{
    float load_factor = (float)ht->count / ht->size;
    if (load_factor > LOAD_FACTOR_THRES)
    {
        ht_resize(ht, ht->size * 2);
    }
}

/*
    Long Term Storage
*/

void save_table(ht_table *ht, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->items[i] != NULL)
        {
            fprintf(file, "%s->%s\n", ht->items[i]->key, ht->items[i]->value);
        }
    }

    fclose(file);
    printf("Table Saved Succesfully to %s", filename);
}

void load_table(ht_table *ht, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, "->");
        char *value = strtok(NULL, "\n");

        if (key && value)
        {
            ht_insert(ht, ht_new_item(key, value));
        }
    }

    fclose(file);
    printf("Table Loaded Succesfully to %s", filename);
}

int main()
{
    // ht_table *table = ht_new(INIT_SIZE);

    // // Adding some key-value pairs
    // ht_item *item1 = ht_new_item("name", "Alice");
    // ht_item *item2 = ht_new_item("age", "30");

    // int index1, index2;
    // // Add them to the hash table

    // ht_insert(table, item1);
    // ht_insert(table, item2);
    // ht_remove(table, item1);
    // ht_retrieve(table, "age");

    // // Print added items

    // // Clean up
    // ht_del_hash_table(table);

    // return 0;

    ht_table *table = ht_new(INIT_SIZE);
    int choice;
    char key[MAX_KEY_SIZE], value[MAX_VALUE_SIZE];
    const char *filename = "hashtable_data.csv";

    printf("=== Hash Table Interface ===\n");
    while (1)
    {
        printf("\nSelect an operation:\n");
        printf("1. Add Item\n");
        printf("2. Remove Item\n");
        printf("3. Search for Item\n");
        printf("4. Print Hash Table\n");
        printf("5. Display Stats\n");
        printf("6. Save Table to File\n");
        printf("7. Load Table from File\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        // Add Item
        case 1:
            printf("Adding Item...\n");
            printf("Enter Key: ");
            scanf("%s", key);
            printf("Enter Value: ");
            scanf("%s", value);
            ht_insert(table, ht_new_item(key, value));
            break;

        // Remove Item
        case 2:
            printf("Removing Item...\n ");
            printf("Enter Key: ");
            scanf("%s", key);
            ht_item *item_to_remove = ht_new_item(key, "");
            ht_remove(table, item_to_remove);
            free(item_to_remove);
            break;

        // Retrieve an Item
        case 3:
            printf("Retrieving Item...\n ");
            printf("Enter Key: ");
            scanf("%s", key);
            ht_retrieve(table, key);
            break;

        // Print Hashtable
        case 4:
            printf("=== Hash Table ===\n");
            for (int i = 0; i < table->size; i++)
            {
                if (table->items[i] != NULL)
                {
                    printf("[%d]: %s -> %s\n", i, table->items[i]->key, table->items[i]->value);
                }
                else
                {
                    printf("[%d]: NULL\n", i);
                }
            }
            break;

        // Display Stats
        case 5:
            printf("=== Hash Table Stats ===\n");
            printf("Size: %d\n", table->size);
            printf("Count: %d\n", table->count);
            printf("Load Factor: %.2f\n", (float)table->count / table->size);
            break;

        case 6: // Save Table to File
            save_table(table, filename);
            break;

        case 7: // Load Table from File
            load_table(table, filename);
            break;

        // Exit
        case 0:
            printf("Exiting...\n");
            ht_del_hash_table(table);
            return 0;

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }
}