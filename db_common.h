#ifndef DB_COMMON_H
#define DB_COMMON_H

#include <sqlite3.h>
#include <time.h>

// Database file name
#define DB_FILE "lessons.db"

// Difficulty levels
typedef enum {
    DIFFICULTY_BEGINNER = 1,
    DIFFICULTY_INTERMEDIATE = 2,
    DIFFICULTY_ADVANCED = 3,
    DIFFICULTY_EXPERT = 4
} DifficultyLevel;

// Lesson structure
typedef struct {
    int id;
    char topic[256];
    char category[128];
    int difficulty;
    char content[4096];
    time_t timestamp;
} Lesson;

// Initialize database and create tables if they don't exist
int init_database(sqlite3 **db);

// Close database connection
void close_database(sqlite3 *db);

// Get difficulty level string
const char* get_difficulty_string(int level);

#endif // DB_COMMON_H
