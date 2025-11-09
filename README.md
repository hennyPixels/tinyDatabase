# SQLite-Backed Systems Programming Lesson Database

A comprehensive C program that implements a SQLite-backed database for storing and managing systems programming lesson plans. This project demonstrates practical database usage, secure SQL practices, and includes an interactive learning game with spaced repetition techniques.

## Features

- **Persistent Storage**: All data stored in SQLite database file (`lessons.db`)
- **Full CRUD Operations**: Create, Read, Update, Delete lessons
- **Secure SQL**: Uses prepared statements to prevent SQL injection
- **Rich Lesson Content**: 25+ comprehensive lessons on advanced systems programming topics
- **Interactive Learning Game**: Progressive C programming tutorial with spaced repetition
- **Search & Filter**: Search by keyword, category, or difficulty level
- **Project-Based Learning**: Hands-on challenges and real-world examples

## Components

### 1. Database Manager (`db_manager`)
Interactive CLI for managing lesson database:
- Add new lessons
- View all lessons
- Search lessons by keyword
- View specific lesson by ID
- Delete lessons
- Filter by category or difficulty level

### 2. Seeder (`seeder`)
Populates database with comprehensive 2024-2025 systems programming lessons:
- **Database Internals**: B+ trees, MVCC, LSM trees, query optimization
- **Security**: Memory safety (Rust), side-channel attacks, OAuth 2.1, container security
- **Networking**: io_uring, HTTP/3/QUIC, eBPF, gRPC
- **Distributed Systems**: Raft consensus, event sourcing, distributed tracing, consistent hashing
- **Modern Languages**: Rust async/await, Go generics, C++20, Zig
- **Performance**: SIMD/AVX-512, lock-free data structures, memory allocators, cache optimization

### 3. Learning Game (`learning_game`)
Interactive tutorial teaching C programming from scratch:
- 10 progressive lessons from "Hello World" to hash tables
- Spaced repetition system for optimal retention
- Confidence tracking and personalized review schedules
- Big-picture thinking: connects concepts to real-world applications
- Hands-on challenges with solutions

## Building

### Prerequisites
```bash
# Install SQLite development libraries
# Ubuntu/Debian:
sudo apt-get install libsqlite3-dev

# macOS:
brew install sqlite3

# Fedora/RHEL:
sudo dnf install sqlite-devel
```

### Compile
```bash
# Build all components
make all

# Or build individually
make db_manager
make seeder
make learning_game
```

## Quick Start

### For Beginners: Start with the Learning Game
```bash
make all          # Build everything
make game         # Launch interactive tutorial
```

The learning game will teach you C programming from scratch with:
- Progressive lessons building on each other
- Spaced repetition reminders
- Confidence tracking
- Solutions available when needed

### For Database Management
```bash
make all          # Build everything
make seed         # Populate database with lessons
make run          # Launch database manager
```

## Usage Examples

### Using the Database Manager
```
=== Systems Programming Lesson Database ===
1. Add new lesson
2. View all lessons
3. Search lessons
4. View lesson by ID
5. Delete lesson
6. List by category
7. List by difficulty
0. Exit
```

### Using the Seeder
```bash
./seeder
# Outputs:
# Seeding database with 25 lessons...
# ✓ Added: B+ Tree Indexing in Modern Databases
# ✓ Added: MVCC: Multi-Version Concurrency Control
# ...
```

### Using the Learning Game
```
MENU:
1. Start next lesson
2. Review lessons due today
3. View progress
4. View solution for current lesson
0. Exit
```

## Database Schema

### lessons table
```sql
CREATE TABLE lessons (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    topic TEXT NOT NULL,
    category TEXT NOT NULL,
    difficulty INTEGER NOT NULL CHECK(difficulty >= 1 AND difficulty <= 4),
    content TEXT NOT NULL,
    timestamp INTEGER NOT NULL
);
```

### learning_progress table
```sql
CREATE TABLE learning_progress (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    lesson_id INTEGER NOT NULL,
    last_reviewed INTEGER NOT NULL,
    review_count INTEGER DEFAULT 0,
    confidence_level INTEGER DEFAULT 1,
    next_review INTEGER,
    FOREIGN KEY(lesson_id) REFERENCES lessons(id)
);
```

### game_lessons table
```sql
CREATE TABLE game_lessons (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    level INTEGER NOT NULL,
    title TEXT NOT NULL,
    description TEXT NOT NULL,
    code_example TEXT,
    challenge TEXT,
    solution TEXT,
    completed INTEGER DEFAULT 0,
    timestamp INTEGER NOT NULL
);
```

## Difficulty Levels

1. **Beginner**: Fundamental concepts, no prior experience needed
2. **Intermediate**: Requires basic understanding of systems programming
3. **Advanced**: Complex topics requiring solid foundation
4. **Expert**: Cutting-edge techniques and deep technical knowledge

## Learning Categories

- **Database Internals**
- **Security**
- **Networking**
- **Distributed Systems**
- **Modern Languages** (Rust, Go, C++, Zig)
- **Performance Optimization**

## Spaced Repetition Schedule

The learning game uses scientifically-proven spaced repetition:
- First review: 1 day
- Second review: 3 days
- Third review: 7 days
- Fourth review: 14 days
- Fifth+ review: 30 days

## Security Features

- **Prepared Statements**: All SQL queries use prepared statements
- **Input Validation**: Difficulty levels checked, buffer sizes enforced
- **No SQL Injection**: Parameterized queries prevent injection attacks
- **Safe String Handling**: Uses strncpy, fgets with size limits

## Educational Philosophy

This project emphasizes:

1. **Project-Based Learning**: Build real systems, not toy examples
2. **Big Picture Thinking**: Connect concepts to real-world applications
3. **Spaced Repetition**: Review at optimal intervals for retention
4. **Progressive Complexity**: Start simple, build to advanced topics
5. **Modern Practices**: All content based on 2024-2025 industry standards

## File Structure

```
tinyDatabase/
├── db_common.h          # Shared definitions and constants
├── db_common.c          # Database initialization and utilities
├── db_manager.c         # Main database manager CLI
├── seeder.c             # Database seeder with lesson content
├── learning_game.c      # Interactive C programming tutorial
├── Makefile             # Build system
├── README.md            # This file
└── lessons.db           # SQLite database (created on first run)
```

## Extending the Database

### Adding New Lessons
Use the database manager:
```bash
./db_manager
# Select option 1 (Add new lesson)
# Enter topic, category, difficulty, and content
```

Or modify `seeder.c` and rebuild:
```bash
# Edit seeder.c, add to lessons[] array
make seeder
./seeder
```

### Adding New Categories
Simply use new category names when adding lessons. The system is flexible and doesn't enforce a fixed category list.

## Makefile Targets

```bash
make all         # Build all programs
make clean       # Remove compiled programs
make clean-all   # Remove programs and database
make seed        # Build and run seeder
make game        # Build and run learning game
make run         # Build and run database manager
make help        # Show help message
```

## Tips for Learning

1. **Start with the Game**: If you're new to C, begin with `./learning_game`
2. **Practice Daily**: Short, consistent sessions beat long cramming
3. **Trust the Repetition**: Review when prompted, even if it feels easy
4. **Read the Advanced Lessons**: After mastering basics, explore advanced topics
5. **Build Projects**: Apply what you learn by building your own programs

## 2024-2025 Content Highlights

All lesson content reflects current industry practices:
- **Rust** memory safety patterns (Linux kernel adoption)
- **io_uring** for modern async I/O (PostgreSQL 16+, Redis 7+)
- **HTTP/3** and QUIC (70% of Google traffic)
- **eBPF** for kernel programming (Facebook, Netflix, Google)
- **AVX-512** SIMD optimization
- **OAuth 2.1** and WebAuthn standards
- **Lock-free** data structures with modern C atomics

## Contributing

This is an educational project. Feel free to:
- Add more lessons
- Improve the learning game
- Add new features to the database manager
- Enhance the spaced repetition algorithm

## License

Educational project - free to use and modify.

## Troubleshooting

### "cannot find -lsqlite3"
Install SQLite development libraries (see Prerequisites section)

### "lessons.db: database is locked"
Only one program can write to the database at a time. Close other instances.

### "Seeder added 0 lessons"
Check if lessons already exist. Use `make clean-all` and `make seed` to reset.

## Author

Built with Claude Code as a comprehensive example of:
- SQLite C API usage
- Secure database programming
- Educational software design
- Modern systems programming education
