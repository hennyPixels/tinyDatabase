# SQLite-backed Systems Programming Lesson Database
# Makefile for building all components

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lsqlite3

# Targets
TARGETS = db_manager seeder learning_game test_db

# Object files
COMMON_OBJ = db_common.o

# Default target
all: $(TARGETS)

# Common object file
db_common.o: db_common.c db_common.h
	$(CC) $(CFLAGS) -c db_common.c -o db_common.o

# Database manager (main CLI program)
db_manager: db_manager.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) db_manager.c $(COMMON_OBJ) -o db_manager $(LDFLAGS)

# Seeder program (populates database with lessons)
seeder: seeder.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) seeder.c $(COMMON_OBJ) -o seeder $(LDFLAGS)

# Learning game (interactive C programming tutorial)
learning_game: learning_game.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) learning_game.c $(COMMON_OBJ) -o learning_game $(LDFLAGS)

# Test program (verify database functionality)
test_db: test_db.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) test_db.c $(COMMON_OBJ) -o test_db $(LDFLAGS)

# Initialize database with seed data
seed: seeder
	./seeder

# Run the learning game
game: learning_game
	./learning_game

# Run the database manager
run: db_manager
	./db_manager

# Test database functionality
test: test_db
	./test_db

# Run demo script
demo: all test_db
	./demo.sh

# Clean build artifacts
clean:
	rm -f $(TARGETS) $(COMMON_OBJ)

# Clean everything including database
clean-all: clean
	rm -f lessons.db

# Show help
help:
	@echo "SQLite-backed Systems Programming Lesson Database"
	@echo ""
	@echo "Available targets:"
	@echo "  make all         - Build all programs (default)"
	@echo "  make db_manager  - Build database manager only"
	@echo "  make seeder      - Build seeder program only"
	@echo "  make learning_game - Build learning game only"
	@echo "  make seed        - Build and run seeder to populate database"
	@echo "  make game        - Build and run the interactive learning game"
	@echo "  make run         - Build and run the database manager"
	@echo "  make test        - Build and run database tests"
	@echo "  make demo        - Run comprehensive demo"
	@echo "  make clean       - Remove compiled programs"
	@echo "  make clean-all   - Remove programs and database file"
	@echo "  make help        - Show this help message"
	@echo ""
	@echo "Quick Start:"
	@echo "  1. make all      - Build everything"
	@echo "  2. make seed     - Populate database with lessons"
	@echo "  3. make demo     - Run demonstration (or make game for learning)"
	@echo "  4. make test     - Verify database functionality"

.PHONY: all clean clean-all seed game run test demo help
