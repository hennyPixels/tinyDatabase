#!/bin/bash

# Demo script for SQLite-backed Systems Programming Lesson Database

echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║    SQLite-Backed Systems Programming Lesson Database - DEMO               ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""

# Check if programs are built
if [ ! -f "db_manager" ] || [ ! -f "seeder" ] || [ ! -f "learning_game" ]; then
    echo "Building programs..."
    make all
    echo ""
fi

# Check if database exists
if [ ! -f "lessons.db" ]; then
    echo "Database not found. Running seeder..."
    ./seeder
    echo ""
fi

echo "--- Database Statistics ---"
./test_db
echo ""

echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║                          AVAILABLE PROGRAMS                                ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""
echo "1. Database Manager (./db_manager)"
echo "   - Add, view, search, and delete lessons"
echo "   - Filter by category or difficulty"
echo "   - Full CRUD operations"
echo ""
echo "2. Learning Game (./learning_game)"
echo "   - Interactive C programming tutorial"
echo "   - 10 progressive lessons from basics to advanced"
echo "   - Spaced repetition system"
echo "   - Confidence tracking and personalized reviews"
echo ""
echo "3. Seeder (./seeder)"
echo "   - Populates database with 24 comprehensive lessons"
echo "   - Topics: Databases, Security, Networking, Distributed Systems,"
echo "     Modern Languages, Performance Optimization"
echo "   - All content based on 2024-2025 industry standards"
echo ""
echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║                          QUICK START GUIDE                                 ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""
echo "For Beginners (Learn C Programming):"
echo "  ./learning_game"
echo ""
echo "For Managing Lessons:"
echo "  ./db_manager"
echo ""
echo "To Reset Database:"
echo "  make clean-all && make all && make seed"
echo ""
echo "For Help:"
echo "  make help"
echo ""
echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║                          PROJECT FEATURES                                  ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""
echo "✓ Persistent SQLite database (lessons.db)"
echo "✓ Secure SQL with prepared statements"
echo "✓ 24 advanced systems programming lessons"
echo "✓ Interactive learning game with spaced repetition"
echo "✓ Full CRUD operations"
echo "✓ Search and filter capabilities"
echo "✓ Progress tracking and confidence levels"
echo "✓ Modern 2024-2025 content"
echo ""
echo "Database file: lessons.db"
echo "Size: $(du -h lessons.db | cut -f1)"
echo ""
echo "Ready to learn! Choose a program to start:"
echo "  ./learning_game  (recommended for beginners)"
echo "  ./db_manager     (for lesson management)"
