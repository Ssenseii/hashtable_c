#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P 53
#define M 1000000007

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct{
    int size;
    int count;
    ht_item** items;
} ht_table;

static ht_item* ht_new_item(char* k, char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

ht_table* ht_new(){
    ht_table* ht = malloc(sizeof(ht_table));

    ht->count = 0;
    ht->size = 53;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
}

static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

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

int compute_hash(const char *s){
    int hash_value = 0;
    int p_pow = 1;

    for (int i = 0; s[i] != '\0'; i++)
    {
        hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow) % M;
        p_pow = (p_pow * P) % M;
    }

    return hash_value;
}


/// nice ///

int main()
{
    ht_table *table = ht_new();

    // Adding some key-value pairs
    ht_item *item1 = ht_new_item("name", "Alice");
    ht_item *item2 = ht_new_item("age", "30");

    // Add them to the hash table
    int index1 = compute_hash(item1->key) % table->size;
    int index2 = compute_hash(item2->key) % table->size;

    table->items[index1] = item1;
    table->items[index2] = item2;

    // Print added items
    printf("Added item: key = %s, value = %s\n", table->items[index1]->key, table->items[index1]->value);
    printf("Added item: key = %s, value = %s\n", table->items[index2]->key, table->items[index2]->value);

    // Clean up
    ht_del_hash_table(table);

    return 0;
}