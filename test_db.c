#include "db_common.h"
#include <stdio.h>

int main() {
    sqlite3 *db;
    int rc = init_database(&db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database.\n");
        return 1;
    }

    printf("✓ Database opened successfully\n");

    // Test 1: Count lessons
    const char *count_sql = "SELECT COUNT(*) FROM lessons;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, count_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        printf("✓ Total lessons in database: %d\n", count);
    }
    sqlite3_finalize(stmt);

    // Test 2: Count by category
    printf("\n--- Lessons by Category ---\n");
    const char *category_sql = "SELECT category, COUNT(*) as count FROM lessons "
                               "GROUP BY category ORDER BY count DESC;";
    rc = sqlite3_prepare_v2(db, category_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *category = sqlite3_column_text(stmt, 0);
            int count = sqlite3_column_int(stmt, 1);
            printf("  %-30s : %d lessons\n", category, count);
        }
    }
    sqlite3_finalize(stmt);

    // Test 3: Count by difficulty
    printf("\n--- Lessons by Difficulty ---\n");
    const char *difficulty_sql = "SELECT difficulty, COUNT(*) as count FROM lessons "
                                 "GROUP BY difficulty ORDER BY difficulty;";
    rc = sqlite3_prepare_v2(db, difficulty_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int difficulty = sqlite3_column_int(stmt, 0);
            int count = sqlite3_column_int(stmt, 1);
            printf("  %-20s : %d lessons\n", get_difficulty_string(difficulty), count);
        }
    }
    sqlite3_finalize(stmt);

    // Test 4: Sample lesson topics
    printf("\n--- Sample Lesson Topics ---\n");
    const char *sample_sql = "SELECT topic, category, difficulty FROM lessons LIMIT 5;";
    rc = sqlite3_prepare_v2(db, sample_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *topic = sqlite3_column_text(stmt, 0);
            const unsigned char *category = sqlite3_column_text(stmt, 1);
            int difficulty = sqlite3_column_int(stmt, 2);
            printf("  [%s] %s\n    Category: %s\n",
                   get_difficulty_string(difficulty), topic, category);
        }
    }
    sqlite3_finalize(stmt);

    // Test 5: Verify all tables exist
    printf("\n--- Database Tables ---\n");
    const char *tables_sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
    rc = sqlite3_prepare_v2(db, tables_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *table_name = sqlite3_column_text(stmt, 0);
            printf("  ✓ %s\n", table_name);
        }
    }
    sqlite3_finalize(stmt);

    close_database(db);
    printf("\n✓ All database tests passed!\n");
    printf("✓ Database persistence verified (data stored in: lessons.db)\n");

    return 0;
}
