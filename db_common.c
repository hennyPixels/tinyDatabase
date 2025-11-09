#include "db_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_database(sqlite3 **db) {
    int rc = sqlite3_open(DB_FILE, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    // Create lessons table
    const char *sql_create_lessons =
        "CREATE TABLE IF NOT EXISTS lessons ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "topic TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "difficulty INTEGER NOT NULL CHECK(difficulty >= 1 AND difficulty <= 4),"
        "content TEXT NOT NULL,"
        "timestamp INTEGER NOT NULL"
        ");";

    // Create learning_progress table for the game
    const char *sql_create_progress =
        "CREATE TABLE IF NOT EXISTS learning_progress ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "lesson_id INTEGER NOT NULL,"
        "last_reviewed INTEGER NOT NULL,"
        "review_count INTEGER DEFAULT 0,"
        "confidence_level INTEGER DEFAULT 1,"
        "next_review INTEGER,"
        "FOREIGN KEY(lesson_id) REFERENCES lessons(id)"
        ");";

    // Create game_lessons table for intro game content
    const char *sql_create_game_lessons =
        "CREATE TABLE IF NOT EXISTS game_lessons ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "level INTEGER NOT NULL,"
        "title TEXT NOT NULL,"
        "description TEXT NOT NULL,"
        "code_example TEXT,"
        "challenge TEXT,"
        "solution TEXT,"
        "completed INTEGER DEFAULT 0,"
        "timestamp INTEGER NOT NULL"
        ");";

    char *err_msg = NULL;

    rc = sqlite3_exec(*db, sql_create_lessons, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (lessons): %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    rc = sqlite3_exec(*db, sql_create_progress, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (progress): %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    rc = sqlite3_exec(*db, sql_create_game_lessons, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (game_lessons): %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    return SQLITE_OK;
}

void close_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

const char* get_difficulty_string(int level) {
    switch (level) {
        case DIFFICULTY_BEGINNER: return "Beginner";
        case DIFFICULTY_INTERMEDIATE: return "Intermediate";
        case DIFFICULTY_ADVANCED: return "Advanced";
        case DIFFICULTY_EXPERT: return "Expert";
        default: return "Unknown";
    }
}
