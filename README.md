# Hashtable in C

/* 📀 = *Meltdown - Travis Scott\* \*/

If you don't know hashtables, you know nothing babygirl.
C doesn't come with hashtables included like JavaScript, also its a lll. so you're gonna touch on some wild stuff.

This is gonna take you about 2 hours to work through.

# Introduction

Why hashtables?
basically they give you O(1) complexity in insertion, deletion, and lookup.
stores in key-value pairs.

**Hash Functions** are used to map the key to an index in an array where the value will be stored.
that's a complicated sentence:
say you have a phone-book
Alice-123456789 is a key-value pair in it.
the hashing function will take the key _Alice_ and turn it into an index.
an index is a number between 0 and a max value.
so basically alice, will take an index of 5.

No matter how good the hashing function is. there will always be collisions (alice and bob might get the index 5)
we handle those using _chaining_ or _open-addressing_.

**What's chaining?**
each slot in the storage array, has a linked list of key-value pairs.
linked lists are those with two nodes: a value and a pointer to the next value.
e.g bob and alice have 5, the slot 5 in the array will have both of them as a linked list of key-value pairs

**What's open addressing?**
the function just looks for the next available slot using probing methods (we'll talk about them later)

**So why are HashTables Fast?**
Say you have a phone number:
Alice - 123456789.

1. Creating an Entry
   We hash the Key alice by running it through a hashing function and it returns a number: E.G **5**.
   now we store the value in that index.
   [ , , , , ,123456789, , , , ];

2. Retrieving an Entry
   We hash the Key alice by running it again through a hashing function and it returns a number: **5** obviously.
   now we just get our value back using that index.
   storage[5] = 123456789;

**What are we going to do now?**
We'll write one in the C programming language, with a few functions like search, insert and delete.
I'll add more later if I'm bored.

# Hash Table

We'll store the key-value pairs as a struct

```
typedef struct{
    char* key;
    char* value;
} ht_item;
```

we'll also define out hash table as a struct.

```
typedef struct{
    int size;
    int count;
    ht_item** item;
} ht_hash_table;
```

the size is for the size of the struct, the count is for the number of elements inside, and the item is the bucket.
now we need to start filling it.

```
static ht_item* init_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(k);
    return i;
}
```

static cause its only used here.
allocate the size of a ht_item. duplicate our crap inside it, and return it.

```
ht_hash_table* ht_new(){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*))
}
```

The first argument, (size_t)ht->size, specifies how many elements you want to allocate.
Here, it's casting the ht->size (the size of the hash table) to size_t, a type used for representing sizes and counts in memory allocation.

**memory leaks**

C is an amazing language cause it's too close to the metal. 
One of the biggest downsides to that: manual memory management.

static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
    for(int i = 0 ; i < ht->size; i ++){
        ht_item* item = ht->item[i];
        if(item != null){
            ht_del_item(item);
        }
    }

    free(ht-items);
    free(ht);
}

# Let's Talk Hash Functions

*The existence of pathological input sets means there are no perfect hash functions for all inputs. The best we can do is to create a function which performs well for the expected data set.*

[CP_String_Hashing]{https://cp-algorithms.com/string/string-hashing.html}

There's an interesting problem I heard about years ago where the most efficient way to compare two strings isn't to compare them letter to letter.
it's basically to hash them and compare their hashes.
*coincidentally I just learned that if hash(string) = 0 for any given string is a **valid** hashing function, though a stupid and useless one*

Best one we can use is called a **Polynomial Rolling Hash Function**.
```
int compute_hash(string const& s){
    const p = 53;
    const int m = 1e9 + 7;
    int hash_value = 0;
    int p_pow = 1:

    for(char c : s){
        hash_value = (hash_value + (c - "a" + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
``` 

Okay, let me explain:
This is a hashing function that uses two constants: one is a prime number, the other is a big ass number cause the value of collision is 1 / that big ass number.
we give it a string
and returns a large integer that can store very big numbers.
const& means that the string won't be mutated inside the function.
(I was using long long but then I was like... naaaah)

**Why Prime Numbers?**
Prime numbers are chosen in hash functions because they provide better distribution of hash values, reduce the likelihood of collisions, and avoid patterns or biases that can arise with non-prime numbers. This leads to more efficient and reliable hashing, which is especially important when dealing with large datasets or hash tables.

# Handling Collisions

Hash functions map an infinitely large number of inputs to a finite number of outputs.
that ought to create a problem.

we'll be using the open-addressing method + double hashing to calculat eht eindex an items should be stroed at after i collisions.
```
static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
    const int hash_a = compute_hash(s);
    const int hash_b = compute_hash(s);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
```

# Insert, Search and Delete

void ht_insert(ht_hash_table* ht, const char* key, const char* value){
    ht_item* item = ht_new_item(key, value);
    int index = compute_hash(item->key);
    ht_item* cur_item = ht->items[index];
    int i = 0;

    while(cur_item != NULL){
        index = compute_hash  (item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }

    ht->items[index] = item;
    ht->count++;
}

void ht_search(ht_hash_table* ht, const char* key) 