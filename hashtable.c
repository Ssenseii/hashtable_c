/*

[ X ] - Handle Collision
[ ] - Dynamic Resizing
[ ] - Key Value Retrieval - Insertion - Deletion
[ ] - Improved Hash Function
[ ] - Load Factor calculation
[ ] - Null Key Handling
[ ] - Memory management
[ ] - Error Handling
[ ] - Debugging Output
[ ] - Iterators
[ ] - Unit Tests
[ ] - Configurable Size
[ ] - String Key Management
[ ] - Handle Collision
[ ] - Long Term Storage
[ ] - Documentation
[ ] - Performance Measurement

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 53
#define P 53
#define M 1000000007
#define LOAD_FACTOR_THRES 0.7

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

// this is for creating an item using a key-value pair
static ht_item *ht_new_item(char *k, char *v)
{
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

// this is for creating a new table.
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


int main()
{
    ht_table *table = ht_new(INIT_SIZE);

    // Adding some key-value pairs
    ht_item *item1 = ht_new_item("name", "Alice");
    ht_item *item2 = ht_new_item("age", "30");

    int index1, index2;
    // Add them to the hash table

    ht_insert(table, item1);
    ht_insert(table, item2);
    ht_remove(table, item1);

    // Print added items

    // Clean up
    ht_del_hash_table(table);

    return 0;
}