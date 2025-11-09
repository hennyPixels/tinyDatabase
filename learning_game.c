#include "db_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Spaced repetition intervals (in days)
#define INTERVAL_1 1
#define INTERVAL_2 3
#define INTERVAL_3 7
#define INTERVAL_4 14
#define INTERVAL_5 30

typedef struct {
    int level;
    const char *title;
    const char *description;
    const char *code_example;
    const char *challenge;
    const char *solution;
} GameLesson;

GameLesson game_lessons[] = {
    {
        1,
        "Welcome to C Programming!",
        "C is a powerful systems programming language. Let's start with the basics.\n\n"
        "Every C program needs a main() function - this is where execution begins.\n"
        "printf() is used to print text to the screen.\n\n"
        "BIG PICTURE: C gives you direct control over memory and hardware, making it\n"
        "perfect for operating systems, databases, and embedded systems.",
        "// Your first C program\n"
        "#include <stdio.h>\n\n"
        "int main() {\n"
        "    printf(\"Hello, World!\\n\");\n"
        "    return 0;\n"
        "}",
        "Modify the program to print your name instead of 'World'.",
        "#include <stdio.h>\n\n"
        "int main() {\n"
        "    printf(\"Hello, [Your Name]!\\n\");\n"
        "    return 0;\n"
        "}"
    },
    {
        2,
        "Variables and Data Types",
        "Variables store data. C has several basic types:\n"
        "- int: whole numbers (4 bytes, -2 billion to +2 billion)\n"
        "- float: decimal numbers (4 bytes)\n"
        "- double: larger decimals (8 bytes, more precision)\n"
        "- char: single character (1 byte)\n\n"
        "LEARNING TIP: Think of variables as labeled boxes. Each box has a type that\n"
        "determines what kind of data it can hold and how much space it takes.",
        "int age = 25;\n"
        "float pi = 3.14159;\n"
        "char grade = 'A';\n"
        "double precise = 3.141592653589793;\n\n"
        "printf(\"Age: %d\\n\", age);\n"
        "printf(\"Pi: %.2f\\n\", pi);\n"
        "printf(\"Grade: %c\\n\", grade);\n"
        "printf(\"Precise: %.15f\\n\", precise);",
        "Create variables for your height (in cm), weight (in kg), and initial.\n"
        "Print them with appropriate format specifiers.",
        "int height = 175;\n"
        "float weight = 70.5;\n"
        "char initial = 'J';\n\n"
        "printf(\"Height: %d cm\\n\", height);\n"
        "printf(\"Weight: %.1f kg\\n\", weight);\n"
        "printf(\"Initial: %c\\n\", initial);"
    },
    {
        3,
        "Pointers: The Heart of C",
        "Pointers store memory addresses. They're what make C powerful!\n\n"
        "- & gets the address of a variable\n"
        "- * dereferences a pointer (accesses the value)\n\n"
        "BIG PICTURE: Pointers let you pass data efficiently (by reference, not copy),\n"
        "create dynamic data structures, and interact directly with hardware.\n\n"
        "MEMORY MODEL: Think of memory as a huge array of bytes. Each byte has an\n"
        "address. A pointer is just a variable that holds one of these addresses.",
        "int x = 42;\n"
        "int *ptr = &x;  // ptr holds the address of x\n\n"
        "printf(\"Value of x: %d\\n\", x);\n"
        "printf(\"Address of x: %p\\n\", (void*)&x);\n"
        "printf(\"Value of ptr: %p\\n\", (void*)ptr);\n"
        "printf(\"Value pointed to by ptr: %d\\n\", *ptr);\n\n"
        "*ptr = 100;  // Change x through pointer\n"
        "printf(\"New value of x: %d\\n\", x);",
        "Create two integer variables. Use pointers to swap their values without\n"
        "using a temporary variable directly in main.",
        "int a = 10, b = 20;\n"
        "int *pa = &a, *pb = &b;\n"
        "int temp = *pa;\n"
        "*pa = *pb;\n"
        "*pb = temp;\n"
        "printf(\"a=%d, b=%d\\n\", a, b);  // Output: a=20, b=10"
    },
    {
        4,
        "Arrays and Memory Layout",
        "Arrays store multiple values of the same type in contiguous memory.\n\n"
        "Key concepts:\n"
        "- Array name is a pointer to first element\n"
        "- Elements stored sequentially in memory\n"
        "- No bounds checking (your responsibility!)\n\n"
        "MEMORY INSIGHT: If arr[0] is at address 1000, and int is 4 bytes,\n"
        "then arr[1] is at 1004, arr[2] at 1008, etc.\n\n"
        "BIG PICTURE: Understanding arrays is crucial for database design,\n"
        "where data is often stored in contiguous blocks for performance.",
        "int arr[5] = {10, 20, 30, 40, 50};\n\n"
        "printf(\"First element: %d\\n\", arr[0]);\n"
        "printf(\"Array address: %p\\n\", (void*)arr);\n"
        "printf(\"First element address: %p\\n\", (void*)&arr[0]);\n\n"
        "// Array indexing is pointer arithmetic!\n"
        "printf(\"arr[2] = %d\\n\", arr[2]);\n"
        "printf(\"*(arr+2) = %d\\n\", *(arr + 2));  // Same thing!",
        "Create an array of 10 integers. Write a loop to calculate and print\n"
        "the sum of all elements.",
        "int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};\n"
        "int sum = 0;\n\n"
        "for (int i = 0; i < 10; i++) {\n"
        "    sum += arr[i];\n"
        "}\n\n"
        "printf(\"Sum: %d\\n\", sum);  // Output: 55"
    },
    {
        5,
        "Dynamic Memory Allocation",
        "malloc() and free() let you allocate memory at runtime.\n\n"
        "Key functions:\n"
        "- malloc(size): Allocate 'size' bytes, return pointer\n"
        "- free(ptr): Deallocate memory\n"
        "- calloc(n, size): Allocate and zero-initialize\n"
        "- realloc(ptr, size): Resize allocation\n\n"
        "GOLDEN RULE: Every malloc needs a matching free!\n\n"
        "BIG PICTURE: This is the foundation of dynamic data structures\n"
        "(linked lists, trees, hash tables) and modern memory management.",
        "#include <stdlib.h>\n\n"
        "int *arr = malloc(5 * sizeof(int));\n"
        "if (arr == NULL) {\n"
        "    printf(\"Allocation failed!\\n\");\n"
        "    return 1;\n"
        "}\n\n"
        "for (int i = 0; i < 5; i++) {\n"
        "    arr[i] = i * 10;\n"
        "}\n\n"
        "for (int i = 0; i < 5; i++) {\n"
        "    printf(\"%d \", arr[i]);\n"
        "}\n\n"
        "free(arr);  // Don't forget!",
        "Implement a function that takes an integer n and returns a dynamically\n"
        "allocated array of the first n Fibonacci numbers.",
        "int* fibonacci(int n) {\n"
        "    int *fib = malloc(n * sizeof(int));\n"
        "    if (fib == NULL) return NULL;\n"
        "    \n"
        "    if (n > 0) fib[0] = 0;\n"
        "    if (n > 1) fib[1] = 1;\n"
        "    \n"
        "    for (int i = 2; i < n; i++) {\n"
        "        fib[i] = fib[i-1] + fib[i-2];\n"
        "    }\n"
        "    \n"
        "    return fib;\n"
        "}"
    },
    {
        6,
        "Structures: Custom Data Types",
        "Structures let you group related data together.\n\n"
        "Uses:\n"
        "- Represent complex entities (Person, Product, Record)\n"
        "- Database rows\n"
        "- Network packets\n"
        "- File formats\n\n"
        "BIG PICTURE: Structures are the foundation of object-oriented concepts\n"
        "and data modeling. They're crucial for database design.",
        "typedef struct {\n"
        "    char name[50];\n"
        "    int age;\n"
        "    float gpa;\n"
        "} Student;\n\n"
        "Student s1 = {\"Alice\", 20, 3.8};\n"
        "printf(\"Name: %s\\n\", s1.name);\n"
        "printf(\"Age: %d\\n\", s1.age);\n"
        "printf(\"GPA: %.2f\\n\", s1.gpa);\n\n"
        "// Access via pointer\n"
        "Student *ptr = &s1;\n"
        "printf(\"Name: %s\\n\", ptr->name);",
        "Create a Book structure with title, author, pages, and price.\n"
        "Write a function that takes an array of books and returns the most expensive one.",
        "typedef struct {\n"
        "    char title[100];\n"
        "    char author[50];\n"
        "    int pages;\n"
        "    float price;\n"
        "} Book;\n\n"
        "Book* most_expensive(Book books[], int n) {\n"
        "    Book *max = &books[0];\n"
        "    for (int i = 1; i < n; i++) {\n"
        "        if (books[i].price > max->price) {\n"
        "            max = &books[i];\n"
        "        }\n"
        "    }\n"
        "    return max;\n"
        "}"
    },
    {
        7,
        "File I/O: Persistence",
        "Reading and writing files is essential for data persistence.\n\n"
        "Key functions:\n"
        "- fopen(name, mode): Open file (\"r\", \"w\", \"a\", \"rb\", \"wb\")\n"
        "- fclose(file): Close file\n"
        "- fprintf(file, format, ...): Write formatted\n"
        "- fscanf(file, format, ...): Read formatted\n"
        "- fread/fwrite: Binary I/O\n\n"
        "BIG PICTURE: This is the foundation of database storage.\n"
        "Modern databases use sophisticated file formats, but the principles are the same.",
        "FILE *fp = fopen(\"data.txt\", \"w\");\n"
        "if (fp == NULL) {\n"
        "    perror(\"Error opening file\");\n"
        "    return 1;\n"
        "}\n\n"
        "fprintf(fp, \"Hello, File!\\n\");\n"
        "fprintf(fp, \"Number: %d\\n\", 42);\n"
        "fclose(fp);\n\n"
        "// Read back\n"
        "fp = fopen(\"data.txt\", \"r\");\n"
        "char buffer[100];\n"
        "while (fgets(buffer, sizeof(buffer), fp)) {\n"
        "    printf(\"%s\", buffer);\n"
        "}\n"
        "fclose(fp);",
        "Write a program that saves student records to a binary file and reads them back.\n"
        "Use the Student structure from the previous lesson.",
        "// Write\n"
        "FILE *fp = fopen(\"students.bin\", \"wb\");\n"
        "Student students[3] = {...};\n"
        "fwrite(students, sizeof(Student), 3, fp);\n"
        "fclose(fp);\n\n"
        "// Read\n"
        "fp = fopen(\"students.bin\", \"rb\");\n"
        "Student loaded[3];\n"
        "fread(loaded, sizeof(Student), 3, fp);\n"
        "fclose(fp);\n\n"
        "for (int i = 0; i < 3; i++) {\n"
        "    printf(\"%s: %.2f\\n\", loaded[i].name, loaded[i].gpa);\n"
        "}"
    },
    {
        8,
        "Building a Simple Database",
        "Let's combine everything to build a simple in-memory database!\n\n"
        "Components:\n"
        "- Data structure (array of records)\n"
        "- CRUD operations (Create, Read, Update, Delete)\n"
        "- Persistence (save/load from file)\n\n"
        "BIG PICTURE: This is a simplified version of what SQLite does.\n"
        "Real databases add indexes, query optimization, transactions, and concurrency.",
        "typedef struct {\n"
        "    int id;\n"
        "    char name[50];\n"
        "    int age;\n"
        "} Person;\n\n"
        "Person db[100];\n"
        "int count = 0;\n\n"
        "void insert(const char *name, int age) {\n"
        "    db[count].id = count + 1;\n"
        "    strncpy(db[count].name, name, 50);\n"
        "    db[count].age = age;\n"
        "    count++;\n"
        "}\n\n"
        "Person* find_by_id(int id) {\n"
        "    for (int i = 0; i < count; i++) {\n"
        "        if (db[i].id == id) return &db[i];\n"
        "    }\n"
        "    return NULL;\n"
        "}",
        "Extend the database with:\n"
        "1. A delete function that removes a person by ID\n"
        "2. A search function that finds people by name (partial match)\n"
        "3. Save/load functions for persistence",
        "void delete(int id) {\n"
        "    for (int i = 0; i < count; i++) {\n"
        "        if (db[i].id == id) {\n"
        "            // Shift remaining elements\n"
        "            for (int j = i; j < count - 1; j++) {\n"
        "                db[j] = db[j + 1];\n"
        "            }\n"
        "            count--;\n"
        "            break;\n"
        "        }\n"
        "    }\n"
        "}\n\n"
        "void save() {\n"
        "    FILE *fp = fopen(\"db.bin\", \"wb\");\n"
        "    fwrite(&count, sizeof(int), 1, fp);\n"
        "    fwrite(db, sizeof(Person), count, fp);\n"
        "    fclose(fp);\n"
        "}"
    },
    {
        9,
        "Linked Lists: Dynamic Data",
        "Linked lists store data in nodes that point to each other.\n\n"
        "Advantages:\n"
        "- Dynamic size (grow/shrink easily)\n"
        "- Efficient insertion/deletion at any position\n\n"
        "Disadvantages:\n"
        "- No random access (must traverse from head)\n"
        "- Extra memory for pointers\n\n"
        "BIG PICTURE: Understanding linked lists is crucial for database internals\n"
        "(hash table chaining, LRU caches, transaction logs).",
        "typedef struct Node {\n"
        "    int data;\n"
        "    struct Node *next;\n"
        "} Node;\n\n"
        "Node *head = NULL;\n\n"
        "void insert_front(int value) {\n"
        "    Node *new = malloc(sizeof(Node));\n"
        "    new->data = value;\n"
        "    new->next = head;\n"
        "    head = new;\n"
        "}\n\n"
        "void print_list() {\n"
        "    Node *curr = head;\n"
        "    while (curr != NULL) {\n"
        "        printf(\"%d -> \", curr->data);\n"
        "        curr = curr->next;\n"
        "    }\n"
        "    printf(\"NULL\\n\");\n"
        "}",
        "Implement:\n"
        "1. A function to insert at the end of the list\n"
        "2. A function to delete a node with a specific value\n"
        "3. A function to reverse the list",
        "void insert_end(int value) {\n"
        "    Node *new = malloc(sizeof(Node));\n"
        "    new->data = value;\n"
        "    new->next = NULL;\n"
        "    \n"
        "    if (head == NULL) {\n"
        "        head = new;\n"
        "        return;\n"
        "    }\n"
        "    \n"
        "    Node *curr = head;\n"
        "    while (curr->next != NULL) curr = curr->next;\n"
        "    curr->next = new;\n"
        "}\n\n"
        "Node* reverse() {\n"
        "    Node *prev = NULL, *curr = head, *next;\n"
        "    while (curr) {\n"
        "        next = curr->next;\n"
        "        curr->next = prev;\n"
        "        prev = curr;\n"
        "        curr = next;\n"
        "    }\n"
        "    return prev;\n"
        "}"
    },
    {
        10,
        "Hash Tables: Fast Lookup",
        "Hash tables provide O(1) average-case lookup using a hash function.\n\n"
        "Key concepts:\n"
        "- Hash function: key -> array index\n"
        "- Collision handling: chaining or open addressing\n"
        "- Load factor: # items / # buckets\n\n"
        "BIG PICTURE: Hash tables are everywhere in databases:\n"
        "- In-memory indexes\n"
        "- Query optimization\n"
        "- Join algorithms\n\n"
        "REAL WORLD: PostgreSQL uses hash tables for in-memory hash joins.\n"
        "Redis is essentially a giant hash table.",
        "typedef struct Entry {\n"
        "    char key[50];\n"
        "    int value;\n"
        "    struct Entry *next;  // For chaining\n"
        "} Entry;\n\n"
        "Entry *hash_table[100];\n\n"
        "unsigned int hash(const char *key) {\n"
        "    unsigned int h = 0;\n"
        "    while (*key) h = h * 31 + *key++;\n"
        "    return h % 100;\n"
        "}\n\n"
        "void insert(const char *key, int value) {\n"
        "    unsigned int idx = hash(key);\n"
        "    Entry *new = malloc(sizeof(Entry));\n"
        "    strcpy(new->key, key);\n"
        "    new->value = value;\n"
        "    new->next = hash_table[idx];\n"
        "    hash_table[idx] = new;\n"
        "}",
        "Implement:\n"
        "1. A get() function that retrieves a value by key\n"
        "2. A delete() function that removes a key-value pair\n"
        "3. Test with at least 20 insertions to observe collision handling",
        "int* get(const char *key) {\n"
        "    unsigned int idx = hash(key);\n"
        "    Entry *curr = hash_table[idx];\n"
        "    \n"
        "    while (curr) {\n"
        "        if (strcmp(curr->key, key) == 0) {\n"
        "            return &curr->value;\n"
        "        }\n"
        "        curr = curr->next;\n"
        "    }\n"
        "    \n"
        "    return NULL;\n"
        "}\n\n"
        "void delete(const char *key) {\n"
        "    unsigned int idx = hash(key);\n"
        "    Entry *curr = hash_table[idx];\n"
        "    Entry *prev = NULL;\n"
        "    \n"
        "    while (curr) {\n"
        "        if (strcmp(curr->key, key) == 0) {\n"
        "            if (prev) prev->next = curr->next;\n"
        "            else hash_table[idx] = curr->next;\n"
        "            free(curr);\n"
        "            return;\n"
        "        }\n"
        "        prev = curr;\n"
        "        curr = curr->next;\n"
        "    }\n"
        "}"
    }
};

int seed_game_lessons(sqlite3 *db) {
    const char *sql = "INSERT INTO game_lessons (level, title, description, code_example, challenge, solution, timestamp) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    for (size_t i = 0; i < sizeof(game_lessons) / sizeof(game_lessons[0]); i++) {
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) continue;

        time_t now = time(NULL);
        GameLesson *lesson = &game_lessons[i];

        sqlite3_bind_int(stmt, 1, lesson->level);
        sqlite3_bind_text(stmt, 2, lesson->title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, lesson->description, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, lesson->code_example, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, lesson->challenge, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, lesson->solution, -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 7, now);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    return SQLITE_OK;
}

void print_lesson(sqlite3_stmt *stmt) {
    int level = sqlite3_column_int(stmt, 1);
    const unsigned char *title = sqlite3_column_text(stmt, 2);
    const unsigned char *description = sqlite3_column_text(stmt, 3);
    const unsigned char *code_example = sqlite3_column_text(stmt, 4);
    const unsigned char *challenge = sqlite3_column_text(stmt, 5);

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           LEVEL %d: %-45s║\n", level, title);
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n\n");

    printf("%s\n\n", description);

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("CODE EXAMPLE:\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("%s\n\n", code_example);

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("YOUR CHALLENGE:\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("%s\n", challenge);
}

int get_next_review_interval(int review_count) {
    switch (review_count) {
        case 0: return INTERVAL_1;
        case 1: return INTERVAL_2;
        case 2: return INTERVAL_3;
        case 3: return INTERVAL_4;
        default: return INTERVAL_5;
    }
}

void update_progress(sqlite3 *db, int lesson_id, int confidence) {
    // Check if progress exists
    const char *check_sql = "SELECT review_count FROM learning_progress WHERE lesson_id = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, lesson_id);

    int review_count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        review_count = sqlite3_column_int(stmt, 0) + 1;
        sqlite3_finalize(stmt);

        // Update existing
        const char *update_sql = "UPDATE learning_progress SET last_reviewed = ?, review_count = ?, "
                                 "confidence_level = ?, next_review = ? WHERE lesson_id = ?;";
        sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);

        time_t now = time(NULL);
        int interval = get_next_review_interval(review_count);
        time_t next_review = now + (interval * 24 * 60 * 60);

        sqlite3_bind_int64(stmt, 1, now);
        sqlite3_bind_int(stmt, 2, review_count);
        sqlite3_bind_int(stmt, 3, confidence);
        sqlite3_bind_int64(stmt, 4, next_review);
        sqlite3_bind_int(stmt, 5, lesson_id);
    } else {
        sqlite3_finalize(stmt);

        // Insert new
        const char *insert_sql = "INSERT INTO learning_progress (lesson_id, last_reviewed, review_count, "
                                 "confidence_level, next_review) VALUES (?, ?, 1, ?, ?);";
        sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

        time_t now = time(NULL);
        int interval = get_next_review_interval(0);
        time_t next_review = now + (interval * 24 * 60 * 60);

        sqlite3_bind_int(stmt, 1, lesson_id);
        sqlite3_bind_int64(stmt, 2, now);
        sqlite3_bind_int(stmt, 3, confidence);
        sqlite3_bind_int64(stmt, 4, next_review);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void show_progress_stats(sqlite3 *db) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            YOUR PROGRESS                                   ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n\n");

    const char *sql = "SELECT gl.level, gl.title, lp.review_count, lp.confidence_level, lp.next_review "
                      "FROM game_lessons gl "
                      "LEFT JOIN learning_progress lp ON gl.id = lp.lesson_id "
                      "ORDER BY gl.level;";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int level = sqlite3_column_int(stmt, 0);
        const unsigned char *title = sqlite3_column_text(stmt, 1);

        if (sqlite3_column_type(stmt, 2) == SQLITE_NULL) {
            printf("Level %2d: %-40s [ NOT STARTED ]\n", level, title);
        } else {
            int review_count = sqlite3_column_int(stmt, 2);
            int confidence = sqlite3_column_int(stmt, 3);
            time_t next_review = sqlite3_column_int64(stmt, 4);

            char status[20];
            if (confidence >= 4) strcpy(status, "MASTERED");
            else if (confidence >= 3) strcpy(status, "CONFIDENT");
            else if (confidence >= 2) strcpy(status, "LEARNING");
            else strcpy(status, "BEGINNER");

            time_t now = time(NULL);
            int days_until = (next_review - now) / (24 * 60 * 60);

            printf("Level %2d: %-40s [%-10s] Reviews: %d, Next: %dd\n",
                   level, title, status, review_count, days_until);
        }
    }

    sqlite3_finalize(stmt);
    printf("\n");
}

void play_game(sqlite3 *db) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                  WELCOME TO C PROGRAMMING ADVENTURE!                       ║\n");
    printf("║                                                                            ║\n");
    printf("║  This interactive game will teach you C programming from scratch through  ║\n");
    printf("║  hands-on challenges and spaced repetition learning techniques.           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n");

    while (1) {
        printf("\n");
        printf("MENU:\n");
        printf("1. Start next lesson\n");
        printf("2. Review lessons due today\n");
        printf("3. View progress\n");
        printf("4. View solution for current lesson\n");
        printf("0. Exit\n");
        printf("\nChoice: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 0) {
            printf("\nGreat work! Keep practicing! 🚀\n");
            break;
        }

        switch (choice) {
            case 1: {
                // Get next unstarted or lowest confidence lesson
                const char *sql = "SELECT gl.id, gl.level, gl.title, gl.description, gl.code_example, gl.challenge "
                                  "FROM game_lessons gl "
                                  "LEFT JOIN learning_progress lp ON gl.id = lp.lesson_id "
                                  "WHERE lp.lesson_id IS NULL OR lp.confidence_level < 4 "
                                  "ORDER BY gl.level LIMIT 1;";

                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    int lesson_id = sqlite3_column_int(stmt, 0);
                    print_lesson(stmt);

                    printf("\n\nHow confident are you with this material?\n");
                    printf("1 - Need more practice\n");
                    printf("2 - Getting there\n");
                    printf("3 - Confident\n");
                    printf("4 - Mastered\n");
                    printf("Confidence: ");

                    int confidence;
                    scanf("%d", &confidence);
                    getchar();

                    if (confidence >= 1 && confidence <= 4) {
                        update_progress(db, lesson_id, confidence);
                        printf("\n✓ Progress saved! ");

                        int days = get_next_review_interval(0);
                        if (confidence < 4) {
                            printf("Review again in %d day(s).\n", days);
                        } else {
                            printf("Excellent! You've mastered this lesson!\n");
                        }
                    }
                } else {
                    printf("\n🎉 Congratulations! You've completed all lessons!\n");
                }

                sqlite3_finalize(stmt);
                break;
            }

            case 2: {
                time_t now = time(NULL);
                const char *sql = "SELECT gl.id, gl.level, gl.title, gl.description, gl.code_example, gl.challenge "
                                  "FROM game_lessons gl "
                                  "JOIN learning_progress lp ON gl.id = lp.lesson_id "
                                  "WHERE lp.next_review <= ? AND lp.confidence_level < 4 "
                                  "ORDER BY lp.next_review LIMIT 1;";

                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                sqlite3_bind_int64(stmt, 1, now);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    int lesson_id = sqlite3_column_int(stmt, 0);
                    print_lesson(stmt);

                    printf("\n\nHow confident are you now?\n");
                    printf("1 - Need more practice\n");
                    printf("2 - Getting there\n");
                    printf("3 - Confident\n");
                    printf("4 - Mastered\n");
                    printf("Confidence: ");

                    int confidence;
                    scanf("%d", &confidence);
                    getchar();

                    if (confidence >= 1 && confidence <= 4) {
                        update_progress(db, lesson_id, confidence);
                        printf("\n✓ Progress updated!\n");
                    }
                } else {
                    printf("\n✓ No lessons due for review today. Great job!\n");
                }

                sqlite3_finalize(stmt);
                break;
            }

            case 3:
                show_progress_stats(db);
                break;

            case 4: {
                // Show solution for most recent lesson
                printf("\nEnter lesson level (1-10): ");
                int level;
                scanf("%d", &level);
                getchar();

                const char *sql = "SELECT solution FROM game_lessons WHERE level = ?;";
                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, level);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    const unsigned char *solution = sqlite3_column_text(stmt, 0);
                    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
                    printf("SOLUTION:\n");
                    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
                    printf("%s\n", solution);
                } else {
                    printf("\nLesson not found.\n");
                }

                sqlite3_finalize(stmt);
                break;
            }

            default:
                printf("Invalid choice.\n");
        }
    }
}

int main() {
    sqlite3 *db;
    int rc = init_database(&db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database.\n");
        return 1;
    }

    // Check if game lessons are already seeded
    const char *check_sql = "SELECT COUNT(*) FROM game_lessons;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    if (count == 0) {
        printf("Initializing game lessons...\n");
        seed_game_lessons(db);
        printf("✓ Game ready!\n");
    }

    play_game(db);

    close_database(db);
    return 0;
}
