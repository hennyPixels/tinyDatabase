#include "db_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_menu() {
    printf("\n=== Systems Programming Lesson Database ===\n");
    printf("1. Add new lesson\n");
    printf("2. View all lessons\n");
    printf("3. Search lessons\n");
    printf("4. View lesson by ID\n");
    printf("5. Delete lesson\n");
    printf("6. List by category\n");
    printf("7. List by difficulty\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

int add_lesson(sqlite3 *db) {
    char topic[256], category[128], content[4096];
    int difficulty;

    printf("\n--- Add New Lesson ---\n");
    printf("Topic: ");
    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = 0;

    printf("Category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;

    printf("Difficulty (1=Beginner, 2=Intermediate, 3=Advanced, 4=Expert): ");
    scanf("%d", &difficulty);
    getchar(); // consume newline

    if (difficulty < 1 || difficulty > 4) {
        printf("Invalid difficulty level!\n");
        return -1;
    }

    printf("Content (end with a line containing only '.'): \n");
    content[0] = '\0';
    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, ".\n") == 0) break;
        if (strlen(content) + strlen(line) < sizeof(content) - 1) {
            strcat(content, line);
        }
    }

    // Use prepared statement for security
    const char *sql = "INSERT INTO lessons (topic, category, difficulty, content, timestamp) "
                      "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    time_t now = time(NULL);

    sqlite3_bind_text(stmt, 1, topic, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, difficulty);
    sqlite3_bind_text(stmt, 4, content, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 5, now);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    printf("\nLesson added successfully! ID: %lld\n", sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

void print_lesson(sqlite3_stmt *stmt) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char *topic = sqlite3_column_text(stmt, 1);
    const unsigned char *category = sqlite3_column_text(stmt, 2);
    int difficulty = sqlite3_column_int(stmt, 3);
    const unsigned char *content = sqlite3_column_text(stmt, 4);
    time_t timestamp = sqlite3_column_int64(stmt, 5);

    printf("\n--- Lesson ID: %d ---\n", id);
    printf("Topic: %s\n", topic);
    printf("Category: %s\n", category);
    printf("Difficulty: %s\n", get_difficulty_string(difficulty));
    printf("Created: %s", ctime(&timestamp));
    printf("Content:\n%s\n", content);
    printf("-------------------\n");
}

int view_all_lessons(sqlite3 *db) {
    const char *sql = "SELECT id, topic, category, difficulty, content, timestamp "
                      "FROM lessons ORDER BY id;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        print_lesson(stmt);
        count++;
    }

    if (count == 0) {
        printf("\nNo lessons found.\n");
    } else {
        printf("\nTotal lessons: %d\n", count);
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int search_lessons(sqlite3 *db) {
    char search_term[256];
    printf("Enter search term: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = 0;

    const char *sql = "SELECT id, topic, category, difficulty, content, timestamp "
                      "FROM lessons WHERE topic LIKE ? OR category LIKE ? OR content LIKE ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    char pattern[270];
    snprintf(pattern, sizeof(pattern), "%%%s%%", search_term);

    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, pattern, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, pattern, -1, SQLITE_TRANSIENT);

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        print_lesson(stmt);
        count++;
    }

    if (count == 0) {
        printf("\nNo matching lessons found.\n");
    } else {
        printf("\nFound %d lesson(s).\n", count);
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int view_lesson_by_id(sqlite3 *db) {
    int id;
    printf("Enter lesson ID: ");
    scanf("%d", &id);
    getchar();

    const char *sql = "SELECT id, topic, category, difficulty, content, timestamp "
                      "FROM lessons WHERE id = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        print_lesson(stmt);
    } else if (rc == SQLITE_DONE) {
        printf("\nLesson not found.\n");
    } else {
        fprintf(stderr, "Query failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int delete_lesson(sqlite3 *db) {
    int id;
    printf("Enter lesson ID to delete: ");
    scanf("%d", &id);
    getchar();

    // First check if lesson exists
    const char *sql_check = "SELECT id FROM lessons WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_check, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        printf("\nLesson with ID %d not found.\n", id);
        sqlite3_finalize(stmt);
        return SQLITE_OK;
    }
    sqlite3_finalize(stmt);

    // Confirm deletion
    char confirm;
    printf("Are you sure you want to delete lesson %d? (y/n): ", id);
    scanf("%c", &confirm);
    getchar();

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return SQLITE_OK;
    }

    // Delete the lesson
    const char *sql_delete = "DELETE FROM lessons WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, sql_delete, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Deletion failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    printf("\nLesson deleted successfully.\n");
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int list_by_category(sqlite3 *db) {
    char category[128];
    printf("Enter category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;

    const char *sql = "SELECT id, topic, category, difficulty, content, timestamp "
                      "FROM lessons WHERE category = ? ORDER BY difficulty, topic;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, category, -1, SQLITE_STATIC);

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        print_lesson(stmt);
        count++;
    }

    if (count == 0) {
        printf("\nNo lessons found in category '%s'.\n", category);
    } else {
        printf("\nFound %d lesson(s) in category '%s'.\n", count, category);
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int list_by_difficulty(sqlite3 *db) {
    int difficulty;
    printf("Enter difficulty (1=Beginner, 2=Intermediate, 3=Advanced, 4=Expert): ");
    scanf("%d", &difficulty);
    getchar();

    if (difficulty < 1 || difficulty > 4) {
        printf("Invalid difficulty level!\n");
        return -1;
    }

    const char *sql = "SELECT id, topic, category, difficulty, content, timestamp "
                      "FROM lessons WHERE difficulty = ? ORDER BY category, topic;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, difficulty);

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        print_lesson(stmt);
        count++;
    }

    if (count == 0) {
        printf("\nNo lessons found for difficulty '%s'.\n", get_difficulty_string(difficulty));
    } else {
        printf("\nFound %d lesson(s) for difficulty '%s'.\n", count, get_difficulty_string(difficulty));
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int main() {
    sqlite3 *db;
    int rc = init_database(&db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database.\n");
        return 1;
    }

    printf("Database initialized successfully. Using file: %s\n", DB_FILE);

    int choice;
    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                add_lesson(db);
                break;
            case 2:
                view_all_lessons(db);
                break;
            case 3:
                search_lessons(db);
                break;
            case 4:
                view_lesson_by_id(db);
                break;
            case 5:
                delete_lesson(db);
                break;
            case 6:
                list_by_category(db);
                break;
            case 7:
                list_by_difficulty(db);
                break;
            case 0:
                printf("Exiting...\n");
                close_database(db);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    close_database(db);
    return 0;
}
