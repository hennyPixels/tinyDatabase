/*
 * ADVANCED GARAGE ADVENTURE - C23 Edition
 * ========================================
 *
 * A comprehensive text adventure game featuring:
 * - C23 modern standards (nullptr, constexpr, built-in bool)
 * - Garage inventory system with vehicle management
 * - A* pathfinding algorithm for optimal navigation
 * - Dijkstra's algorithm for shortest path calculation
 * - Physics-based calculations for vehicle performance
 * - Advanced data structures and memory management
 *
 * Compile: gcc -std=c23 -Wall -Wextra -O2 -lm advanced_garage_adventure_c23.c -o garage_adventure
 * Run: ./garage_adventure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

// ============================================================================
// CONSTANTS AND CONFIGURATION
// ============================================================================

constexpr int MAX_ROOMS = 20;
constexpr int MAX_INPUT = 256;
constexpr int MAX_NAME = 64;
constexpr int MAX_DESCRIPTION = 512;
constexpr int MAX_VEHICLES = 10;
constexpr int MAX_PARTS = 50;
constexpr int MAX_PATH_LENGTH = 100;

constexpr double GRAVITY = 9.81;  // m/s²
constexpr double PI = 3.14159265358979323846;
constexpr double AIR_DENSITY = 1.225;  // kg/m³ at sea level

// Pathfinding constants
constexpr int INFINITY_COST = 999999;
constexpr double HEURISTIC_WEIGHT = 1.0;

// ============================================================================
// TYPE DEFINITIONS
// ============================================================================

typedef enum {
    ROOM_GARAGE_ENTRANCE,
    ROOM_MAIN_GARAGE,
    ROOM_WORKSHOP,
    ROOM_PARTS_STORAGE,
    ROOM_PAINT_BOOTH,
    ROOM_TESTING_TRACK,
    ROOM_OFFICE,
    ROOM_TOOL_ROOM,
    ROOM_COMPUTER_LAB,
    ROOM_SHOWROOM,
    ROOM_COUNT
} RoomID;

typedef enum {
    DIR_NORTH,
    DIR_SOUTH,
    DIR_EAST,
    DIR_WEST,
    DIR_COUNT
} Direction;

typedef enum {
    VEHICLE_NONE,
    VEHICLE_SPORTS_CAR,
    VEHICLE_TRUCK,
    VEHICLE_MOTORCYCLE,
    VEHICLE_RACE_CAR,
    VEHICLE_CLASSIC_CAR
} VehicleType;

typedef enum {
    PART_ENGINE,
    PART_TRANSMISSION,
    PART_WHEELS,
    PART_BRAKES,
    PART_SUSPENSION,
    PART_TURBOCHARGER,
    PART_EXHAUST,
    PART_ECU
} PartType;

typedef enum {
    VERB_UNKNOWN,
    VERB_GO,
    VERB_LOOK,
    VERB_EXAMINE,
    VERB_INVENTORY,
    VERB_TAKE,
    VERB_DROP,
    VERB_USE,
    VERB_INSTALL,
    VERB_CALCULATE,
    VERB_PATH,
    VERB_NAVIGATE,
    VERB_TUNE,
    VERB_HELP,
    VERB_QUIT
} Verb;

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    double x;
    double y;
} Point2D;

typedef struct {
    char name[MAX_NAME];
    PartType type;
    double weight;      // kg
    double cost;        // dollars
    int performance_boost;  // percentage
    bool installed;
} Part;

typedef struct {
    char name[MAX_NAME];
    VehicleType type;
    double mass;           // kg
    double engine_power;   // horsepower
    double drag_coefficient;  // Cd
    double frontal_area;   // m²
    double max_speed;      // m/s
    double acceleration;   // m/s²
    int parts_count;
    Part parts[MAX_PARTS];
} Vehicle;

typedef struct {
    Vehicle vehicles[MAX_VEHICLES];
    int vehicle_count;
    Part loose_parts[MAX_PARTS];
    int loose_parts_count;
    int money;
} GarageInventory;

typedef struct {
    RoomID id;
    char name[MAX_NAME];
    char description[MAX_DESCRIPTION];
    RoomID connections[DIR_COUNT];
    Point2D coordinates;  // For pathfinding
    bool has_vehicle_access;
    bool has_computer;
} Room;

typedef struct {
    Verb verb;
    char object[MAX_INPUT];
    char target[MAX_INPUT];
    Direction direction;
} Command;

typedef struct {
    int parent[MAX_ROOMS];
    double cost[MAX_ROOMS];
    bool visited[MAX_ROOMS];
} PathfindingState;

// ============================================================================
// GLOBAL GAME STATE
// ============================================================================

typedef struct {
    Room rooms[MAX_ROOMS];
    RoomID current_room;
    GarageInventory garage;
    Vehicle *current_vehicle;
    bool running;
    int moves_count;
} GameState;

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

// Initialization
void init_game(GameState *game);
void init_rooms(Room rooms[]);
void init_garage(GarageInventory *garage);

// Game loop
void game_loop(GameState *game);
bool parse_command(const char *input, Command *cmd);
void execute_command(GameState *game, const Command *cmd);

// Commands
void cmd_go(GameState *game, Direction dir);
void cmd_look(GameState *game);
void cmd_examine(GameState *game, const char *object);
void cmd_inventory(GameState *game);
void cmd_take(GameState *game, const char *object);
void cmd_install(GameState *game, const char *part_name);
void cmd_calculate(GameState *game, const char *calc_type);
void cmd_navigate(GameState *game, const char *destination);
void cmd_help(void);

// Physics calculations
double calculate_terminal_velocity(const Vehicle *v);
double calculate_acceleration_time(const Vehicle *v, double target_speed);
double calculate_braking_distance(const Vehicle *v, double initial_speed);
double calculate_drag_force(const Vehicle *v, double velocity);
double calculate_power_required(const Vehicle *v, double velocity);
double calculate_lap_time(const Vehicle *v, double track_length, int turns);

// Pathfinding algorithms
void dijkstra_shortest_path(const Room rooms[], RoomID start, RoomID end,
                           PathfindingState *state);
double heuristic_distance(Point2D a, Point2D b);
bool astar_pathfinding(const Room rooms[], RoomID start, RoomID end,
                      RoomID path[], int path_length);
void print_path(const Room rooms[], const RoomID path[], int path_length);

// Utilities
const char* direction_to_string(Direction dir);
Direction string_to_direction(const char *str);
const char* vehicle_type_to_string(VehicleType type);
const char* part_type_to_string(PartType type);
void to_lowercase(char *str);
void print_separator(void);

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main(void) {
    GameState game = {};  // C23: zero initialization

    printf("\n");
    print_separator();
    printf("    ADVANCED GARAGE ADVENTURE - C23 Edition\n");
    printf("    A Pathfinding & Physics Simulation Game\n");
    print_separator();
    printf("\n");

    init_game(&game);
    game_loop(&game);

    printf("\nThanks for playing! Total moves: %d\n", game.moves_count);
    return 0;
}

// ============================================================================
// INITIALIZATION FUNCTIONS
// ============================================================================

void init_game(GameState *game) {
    game->current_room = ROOM_GARAGE_ENTRANCE;
    game->current_vehicle = nullptr;
    game->running = true;
    game->moves_count = 0;

    init_rooms(game->rooms);
    init_garage(&game->garage);

    printf("Welcome to Advanced Garage Adventure!\n");
    printf("Type 'help' for available commands.\n\n");

    cmd_look(game);
}

void init_rooms(Room rooms[]) {
    // Room 0: Garage Entrance
    rooms[ROOM_GARAGE_ENTRANCE] = (Room){
        .id = ROOM_GARAGE_ENTRANCE,
        .name = "Garage Entrance",
        .description = "You stand at the entrance of a state-of-the-art automotive facility. "
                      "The main garage lies to the north, and an office is to the east.",
        .connections = {
            [DIR_NORTH] = ROOM_MAIN_GARAGE,
            [DIR_SOUTH] = ROOM_GARAGE_ENTRANCE,
            [DIR_EAST] = ROOM_OFFICE,
            [DIR_WEST] = ROOM_GARAGE_ENTRANCE
        },
        .coordinates = {.x = 0, .y = 0},
        .has_vehicle_access = true,
        .has_computer = false
    };

    // Room 1: Main Garage
    rooms[ROOM_MAIN_GARAGE] = (Room){
        .id = ROOM_MAIN_GARAGE,
        .name = "Main Garage Bay",
        .description = "A spacious garage with hydraulic lifts and diagnostic equipment. "
                      "The workshop is to the north, parts storage to the east, and the entrance to the south.",
        .connections = {
            [DIR_NORTH] = ROOM_WORKSHOP,
            [DIR_SOUTH] = ROOM_GARAGE_ENTRANCE,
            [DIR_EAST] = ROOM_PARTS_STORAGE,
            [DIR_WEST] = ROOM_TOOL_ROOM
        },
        .coordinates = {.x = 0, .y = 10},
        .has_vehicle_access = true,
        .has_computer = false
    };

    // Room 2: Workshop
    rooms[ROOM_WORKSHOP] = (Room){
        .id = ROOM_WORKSHOP,
        .name = "Workshop",
        .description = "A fully equipped workshop with engine hoists, welding equipment, and precision tools. "
                      "The paint booth is to the east, and the main garage is south.",
        .connections = {
            [DIR_NORTH] = ROOM_WORKSHOP,
            [DIR_SOUTH] = ROOM_MAIN_GARAGE,
            [DIR_EAST] = ROOM_PAINT_BOOTH,
            [DIR_WEST] = ROOM_WORKSHOP
        },
        .coordinates = {.x = 0, .y = 20},
        .has_vehicle_access = true,
        .has_computer = false
    };

    // Room 3: Parts Storage
    rooms[ROOM_PARTS_STORAGE] = (Room){
        .id = ROOM_PARTS_STORAGE,
        .name = "Parts Storage",
        .description = "Shelves lined with automotive parts, from engines to electronics. "
                      "The main garage is to the west, and the computer lab is north.",
        .connections = {
            [DIR_NORTH] = ROOM_COMPUTER_LAB,
            [DIR_SOUTH] = ROOM_PARTS_STORAGE,
            [DIR_EAST] = ROOM_PARTS_STORAGE,
            [DIR_WEST] = ROOM_MAIN_GARAGE
        },
        .coordinates = {.x = 10, .y = 10},
        .has_vehicle_access = false,
        .has_computer = false
    };

    // Room 4: Paint Booth
    rooms[ROOM_PAINT_BOOTH] = (Room){
        .id = ROOM_PAINT_BOOTH,
        .name = "Paint Booth",
        .description = "A climate-controlled paint booth with professional spray equipment. "
                      "The workshop is to the west, and the testing track is north.",
        .connections = {
            [DIR_NORTH] = ROOM_TESTING_TRACK,
            [DIR_SOUTH] = ROOM_PAINT_BOOTH,
            [DIR_EAST] = ROOM_SHOWROOM,
            [DIR_WEST] = ROOM_WORKSHOP
        },
        .coordinates = {.x = 10, .y = 20},
        .has_vehicle_access = true,
        .has_computer = false
    };

    // Room 5: Testing Track
    rooms[ROOM_TESTING_TRACK] = (Room){
        .id = ROOM_TESTING_TRACK,
        .name = "Testing Track",
        .description = "An indoor testing track with sensors and timing equipment. "
                      "Perfect for calculating vehicle performance. The paint booth is south.",
        .connections = {
            [DIR_NORTH] = ROOM_TESTING_TRACK,
            [DIR_SOUTH] = ROOM_PAINT_BOOTH,
            [DIR_EAST] = ROOM_TESTING_TRACK,
            [DIR_WEST] = ROOM_TESTING_TRACK
        },
        .coordinates = {.x = 10, .y = 30},
        .has_vehicle_access = true,
        .has_computer = true
    };

    // Room 6: Office
    rooms[ROOM_OFFICE] = (Room){
        .id = ROOM_OFFICE,
        .name = "Office",
        .description = "A comfortable office with blueprints on the walls and a desk covered in paperwork. "
                      "The entrance is to the west.",
        .connections = {
            [DIR_NORTH] = ROOM_OFFICE,
            [DIR_SOUTH] = ROOM_OFFICE,
            [DIR_EAST] = ROOM_OFFICE,
            [DIR_WEST] = ROOM_GARAGE_ENTRANCE
        },
        .coordinates = {.x = 10, .y = 0},
        .has_vehicle_access = false,
        .has_computer = true
    };

    // Room 7: Tool Room
    rooms[ROOM_TOOL_ROOM] = (Room){
        .id = ROOM_TOOL_ROOM,
        .name = "Tool Room",
        .description = "Organized racks of tools from basic wrenches to advanced diagnostic scanners. "
                      "The main garage is to the east.",
        .connections = {
            [DIR_NORTH] = ROOM_TOOL_ROOM,
            [DIR_SOUTH] = ROOM_TOOL_ROOM,
            [DIR_EAST] = ROOM_MAIN_GARAGE,
            [DIR_WEST] = ROOM_TOOL_ROOM
        },
        .coordinates = {.x = -10, .y = 10},
        .has_vehicle_access = false,
        .has_computer = false
    };

    // Room 8: Computer Lab
    rooms[ROOM_COMPUTER_LAB] = (Room){
        .id = ROOM_COMPUTER_LAB,
        .name = "Computer Lab",
        .description = "Advanced computer systems for vehicle diagnostics, pathfinding algorithms, "
                      "and performance calculations. The parts storage is south.",
        .connections = {
            [DIR_NORTH] = ROOM_COMPUTER_LAB,
            [DIR_SOUTH] = ROOM_PARTS_STORAGE,
            [DIR_EAST] = ROOM_SHOWROOM,
            [DIR_WEST] = ROOM_COMPUTER_LAB
        },
        .coordinates = {.x = 10, .y = 20},
        .has_vehicle_access = false,
        .has_computer = true
    };

    // Room 9: Showroom
    rooms[ROOM_SHOWROOM] = (Room){
        .id = ROOM_SHOWROOM,
        .name = "Showroom",
        .description = "A pristine showroom displaying finished vehicles under bright lights. "
                      "The computer lab is to the west, and the paint booth is also west.",
        .connections = {
            [DIR_NORTH] = ROOM_SHOWROOM,
            [DIR_SOUTH] = ROOM_SHOWROOM,
            [DIR_EAST] = ROOM_SHOWROOM,
            [DIR_WEST] = ROOM_COMPUTER_LAB
        },
        .coordinates = {.x = 20, .y = 20},
        .has_vehicle_access = true,
        .has_computer = false
    };
}

void init_garage(GarageInventory *garage) {
    garage->vehicle_count = 3;
    garage->loose_parts_count = 5;
    garage->money = 50000;

    // Initialize vehicles
    garage->vehicles[0] = (Vehicle){
        .name = "Lightning GT",
        .type = VEHICLE_SPORTS_CAR,
        .mass = 1400.0,
        .engine_power = 450.0,
        .drag_coefficient = 0.28,
        .frontal_area = 2.2,
        .max_speed = 95.0,  // m/s (~212 mph)
        .acceleration = 12.0,
        .parts_count = 0
    };

    garage->vehicles[1] = (Vehicle){
        .name = "Thunder Truck",
        .type = VEHICLE_TRUCK,
        .mass = 2500.0,
        .engine_power = 380.0,
        .drag_coefficient = 0.42,
        .frontal_area = 3.5,
        .max_speed = 55.0,  // m/s (~123 mph)
        .acceleration = 6.0,
        .parts_count = 0
    };

    garage->vehicles[2] = (Vehicle){
        .name = "Velocity Viper",
        .type = VEHICLE_RACE_CAR,
        .mass = 1100.0,
        .engine_power = 600.0,
        .drag_coefficient = 0.25,
        .frontal_area = 1.8,
        .max_speed = 105.0,  // m/s (~235 mph)
        .acceleration = 15.0,
        .parts_count = 0
    };

    // Initialize loose parts
    garage->loose_parts[0] = (Part){
        .name = "Twin-Turbo Kit",
        .type = PART_TURBOCHARGER,
        .weight = 25.0,
        .cost = 5000.0,
        .performance_boost = 30,
        .installed = false
    };

    garage->loose_parts[1] = (Part){
        .name = "Racing Exhaust",
        .type = PART_EXHAUST,
        .weight = 15.0,
        .cost = 2000.0,
        .performance_boost = 10,
        .installed = false
    };

    garage->loose_parts[2] = (Part){
        .name = "Performance ECU",
        .type = PART_ECU,
        .weight = 2.0,
        .cost = 3500.0,
        .performance_boost = 20,
        .installed = false
    };

    garage->loose_parts[3] = (Part){
        .name = "Carbon Brakes",
        .type = PART_BRAKES,
        .weight = 20.0,
        .cost = 4500.0,
        .performance_boost = 15,
        .installed = false
    };

    garage->loose_parts[4] = (Part){
        .name = "Racing Suspension",
        .type = PART_SUSPENSION,
        .weight = 30.0,
        .cost = 3000.0,
        .performance_boost = 12,
        .installed = false
    };
}

// ============================================================================
// GAME LOOP
// ============================================================================

void game_loop(GameState *game) {
    char input[MAX_INPUT];
    Command cmd;

    while (game->running) {
        printf("\n> ");
        if (fgets(input, sizeof(input), stdin) == nullptr) {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        if (parse_command(input, &cmd)) {
            execute_command(game, &cmd);
            game->moves_count++;
        } else {
            printf("I don't understand that command. Type 'help' for assistance.\n");
        }
    }
}

// ============================================================================
// COMMAND PARSING
// ============================================================================

bool parse_command(const char *input, Command *cmd) {
    char buffer[MAX_INPUT];
    strncpy(buffer, input, MAX_INPUT - 1);
    buffer[MAX_INPUT - 1] = '\0';

    to_lowercase(buffer);

    // Initialize command
    *cmd = (Command){
        .verb = VERB_UNKNOWN,
        .object = "",
        .target = "",
        .direction = DIR_COUNT
    };

    char *token = strtok(buffer, " \t");
    if (token == nullptr) {
        return false;
    }

    // Parse verb
    if (strcmp(token, "go") == 0 || strcmp(token, "move") == 0) {
        cmd->verb = VERB_GO;
    } else if (strcmp(token, "look") == 0 || strcmp(token, "l") == 0) {
        cmd->verb = VERB_LOOK;
    } else if (strcmp(token, "examine") == 0 || strcmp(token, "x") == 0) {
        cmd->verb = VERB_EXAMINE;
    } else if (strcmp(token, "inventory") == 0 || strcmp(token, "i") == 0) {
        cmd->verb = VERB_INVENTORY;
    } else if (strcmp(token, "take") == 0 || strcmp(token, "get") == 0) {
        cmd->verb = VERB_TAKE;
    } else if (strcmp(token, "install") == 0) {
        cmd->verb = VERB_INSTALL;
    } else if (strcmp(token, "calculate") == 0 || strcmp(token, "calc") == 0) {
        cmd->verb = VERB_CALCULATE;
    } else if (strcmp(token, "navigate") == 0 || strcmp(token, "nav") == 0) {
        cmd->verb = VERB_NAVIGATE;
    } else if (strcmp(token, "path") == 0) {
        cmd->verb = VERB_PATH;
    } else if (strcmp(token, "help") == 0 || strcmp(token, "?") == 0) {
        cmd->verb = VERB_HELP;
    } else if (strcmp(token, "quit") == 0 || strcmp(token, "exit") == 0 || strcmp(token, "q") == 0) {
        cmd->verb = VERB_QUIT;
    } else if (strcmp(token, "north") == 0 || strcmp(token, "n") == 0) {
        cmd->verb = VERB_GO;
        cmd->direction = DIR_NORTH;
        return true;
    } else if (strcmp(token, "south") == 0 || strcmp(token, "s") == 0) {
        cmd->verb = VERB_GO;
        cmd->direction = DIR_SOUTH;
        return true;
    } else if (strcmp(token, "east") == 0 || strcmp(token, "e") == 0) {
        cmd->verb = VERB_GO;
        cmd->direction = DIR_EAST;
        return true;
    } else if (strcmp(token, "west") == 0 || strcmp(token, "w") == 0) {
        cmd->verb = VERB_GO;
        cmd->direction = DIR_WEST;
        return true;
    } else {
        return false;
    }

    // Parse object
    token = strtok(nullptr, " \t");
    if (token != nullptr) {
        strncpy(cmd->object, token, MAX_INPUT - 1);

        // For GO command, check if object is a direction
        if (cmd->verb == VERB_GO) {
            cmd->direction = string_to_direction(token);
        }

        // Parse target
        token = strtok(nullptr, " \t");
        if (token != nullptr) {
            strncpy(cmd->target, token, MAX_INPUT - 1);
        }
    }

    return true;
}

void execute_command(GameState *game, const Command *cmd) {
    switch (cmd->verb) {
        case VERB_GO:
            cmd_go(game, cmd->direction);
            break;
        case VERB_LOOK:
            cmd_look(game);
            break;
        case VERB_EXAMINE:
            cmd_examine(game, cmd->object);
            break;
        case VERB_INVENTORY:
            cmd_inventory(game);
            break;
        case VERB_TAKE:
            cmd_take(game, cmd->object);
            break;
        case VERB_INSTALL:
            cmd_install(game, cmd->object);
            break;
        case VERB_CALCULATE:
            cmd_calculate(game, cmd->object);
            break;
        case VERB_NAVIGATE:
        case VERB_PATH:
            cmd_navigate(game, cmd->object);
            break;
        case VERB_HELP:
            cmd_help();
            break;
        case VERB_QUIT:
            printf("Are you sure you want to quit? (yes/no): ");
            char answer[10];
            if (fgets(answer, sizeof(answer), stdin) != nullptr) {
                to_lowercase(answer);
                if (strncmp(answer, "yes", 3) == 0 || strncmp(answer, "y", 1) == 0) {
                    game->running = false;
                    printf("Saving your progress...\n");
                }
            }
            break;
        default:
            printf("Command not implemented yet.\n");
            break;
    }
}

// ============================================================================
// COMMAND IMPLEMENTATIONS
// ============================================================================

void cmd_go(GameState *game, Direction dir) {
    if (dir >= DIR_COUNT) {
        printf("Which direction? (north, south, east, west)\n");
        return;
    }

    Room *current = &game->rooms[game->current_room];
    RoomID next_room = current->connections[dir];

    if (next_room == game->current_room) {
        printf("You can't go that way.\n");
        return;
    }

    game->current_room = next_room;
    printf("You move %s.\n\n", direction_to_string(dir));
    cmd_look(game);
}

void cmd_look(GameState *game) {
    Room *room = &game->rooms[game->current_room];

    printf("\n=== %s ===\n", room->name);
    printf("%s\n", room->description);

    if (room->has_computer) {
        printf("\n[Computer Terminal Available - Use 'calculate' commands here]\n");
    }

    if (room->has_vehicle_access) {
        printf("\n[Vehicle Access Bay]\n");
    }

    printf("\nExits: ");
    bool first = true;
    for (int i = 0; i < DIR_COUNT; i++) {
        if (room->connections[i] != game->current_room) {
            if (!first) printf(", ");
            printf("%s", direction_to_string((Direction)i));
            first = false;
        }
    }
    printf("\n");
}

void cmd_examine(GameState *game, const char *object) {
    if (strlen(object) == 0) {
        printf("Examine what?\n");
        return;
    }

    // Check vehicles
    for (int i = 0; i < game->garage.vehicle_count; i++) {
        char name_lower[MAX_NAME];
        strncpy(name_lower, game->garage.vehicles[i].name, MAX_NAME - 1);
        to_lowercase(name_lower);

        if (strstr(name_lower, object) != nullptr) {
            Vehicle *v = &game->garage.vehicles[i];
            printf("\n=== %s ===\n", v->name);
            printf("Type: %s\n", vehicle_type_to_string(v->type));
            printf("Mass: %.0f kg\n", v->mass);
            printf("Engine Power: %.0f HP\n", v->engine_power);
            printf("Drag Coefficient: %.2f\n", v->drag_coefficient);
            printf("Frontal Area: %.1f m²\n", v->frontal_area);
            printf("Max Speed: %.1f m/s (%.1f mph)\n", v->max_speed, v->max_speed * 2.237);
            printf("Acceleration: %.1f m/s²\n", v->acceleration);

            if (v->parts_count > 0) {
                printf("\nInstalled Parts:\n");
                for (int j = 0; j < v->parts_count; j++) {
                    printf("  - %s (+%d%% performance)\n",
                           v->parts[j].name, v->parts[j].performance_boost);
                }
            }
            return;
        }
    }

    printf("You don't see that here.\n");
}

void cmd_inventory(GameState *game) {
    printf("\n=== GARAGE INVENTORY ===\n");
    printf("Money: $%d\n\n", game->garage.money);

    printf("VEHICLES (%d):\n", game->garage.vehicle_count);
    for (int i = 0; i < game->garage.vehicle_count; i++) {
        Vehicle *v = &game->garage.vehicles[i];
        printf("  %d. %s (%s) - %.0f HP\n",
               i + 1, v->name, vehicle_type_to_string(v->type), v->engine_power);
    }

    printf("\nAVAILABLE PARTS (%d):\n", game->garage.loose_parts_count);
    for (int i = 0; i < game->garage.loose_parts_count; i++) {
        Part *p = &game->garage.loose_parts[i];
        if (!p->installed) {
            printf("  - %s (%s) - $%.0f - +%d%% performance\n",
                   p->name, part_type_to_string(p->type), p->cost, p->performance_boost);
        }
    }
}

void cmd_take(GameState *game, const char *object) {
    if (strlen(object) == 0) {
        printf("Take what?\n");
        return;
    }

    printf("The '%s' is too heavy to carry. Use 'install' to add parts to vehicles.\n", object);
}

void cmd_install(GameState *game, const char *part_name) {
    if (strlen(part_name) == 0) {
        printf("Install what part?\n");
        return;
    }

    // Find the part
    Part *part_to_install = nullptr;
    int part_index = -1;

    for (int i = 0; i < game->garage.loose_parts_count; i++) {
        char name_lower[MAX_NAME];
        strncpy(name_lower, game->garage.loose_parts[i].name, MAX_NAME - 1);
        to_lowercase(name_lower);

        if (strstr(name_lower, part_name) != nullptr && !game->garage.loose_parts[i].installed) {
            part_to_install = &game->garage.loose_parts[i];
            part_index = i;
            break;
        }
    }

    if (part_to_install == nullptr) {
        printf("You don't have that part available.\n");
        return;
    }

    // For simplicity, install on first vehicle
    if (game->garage.vehicle_count == 0) {
        printf("No vehicles available.\n");
        return;
    }

    Vehicle *v = &game->garage.vehicles[0];

    if (v->parts_count >= MAX_PARTS) {
        printf("Vehicle has no more room for parts.\n");
        return;
    }

    // Install the part
    v->parts[v->parts_count++] = *part_to_install;
    part_to_install->installed = true;

    // Apply performance boost
    v->engine_power *= (1.0 + part_to_install->performance_boost / 100.0);
    v->max_speed *= (1.0 + part_to_install->performance_boost / 200.0);
    v->mass += part_to_install->weight;

    printf("Installed %s on %s!\n", part_to_install->name, v->name);
    printf("New engine power: %.0f HP\n", v->engine_power);
    printf("New max speed: %.1f m/s (%.1f mph)\n", v->max_speed, v->max_speed * 2.237);
}

void cmd_calculate(GameState *game, const char *calc_type) {
    Room *room = &game->rooms[game->current_room];

    if (!room->has_computer) {
        printf("You need to be at a computer terminal to perform calculations.\n");
        printf("Try the Computer Lab, Office, or Testing Track.\n");
        return;
    }

    if (game->garage.vehicle_count == 0) {
        printf("No vehicles available for calculations.\n");
        return;
    }

    Vehicle *v = &game->garage.vehicles[0];  // Use first vehicle

    printf("\n=== PERFORMANCE CALCULATIONS FOR %s ===\n\n", v->name);

    if (strlen(calc_type) == 0 || strcmp(calc_type, "all") == 0) {
        // Calculate everything
        double terminal_v = calculate_terminal_velocity(v);
        double accel_time = calculate_acceleration_time(v, 26.8);  // 0-60 mph time
        double braking = calculate_braking_distance(v, 26.8);
        double drag = calculate_drag_force(v, v->max_speed);
        double power_req = calculate_power_required(v, v->max_speed);
        double lap_time = calculate_lap_time(v, 5000.0, 12);

        printf("Terminal Velocity: %.2f m/s (%.1f mph)\n", terminal_v, terminal_v * 2.237);
        printf("0-60 mph time: %.2f seconds\n", accel_time);
        printf("Braking distance from 60 mph: %.2f meters (%.1f feet)\n", braking, braking * 3.281);
        printf("Drag force at max speed: %.0f N\n", drag);
        printf("Power required at max speed: %.0f kW (%.0f HP)\n",
               power_req / 1000.0, power_req / 745.7);
        printf("Estimated lap time (5km, 12 turns): %.1f seconds\n", lap_time);

    } else if (strcmp(calc_type, "terminal") == 0) {
        double terminal_v = calculate_terminal_velocity(v);
        printf("Terminal Velocity: %.2f m/s (%.1f mph)\n", terminal_v, terminal_v * 2.237);

    } else if (strcmp(calc_type, "acceleration") == 0) {
        double accel_time = calculate_acceleration_time(v, 26.8);
        printf("0-60 mph time: %.2f seconds\n", accel_time);

    } else if (strcmp(calc_type, "braking") == 0) {
        double braking = calculate_braking_distance(v, 26.8);
        printf("Braking distance from 60 mph: %.2f meters (%.1f feet)\n",
               braking, braking * 3.281);
    } else {
        printf("Unknown calculation type. Try: all, terminal, acceleration, braking\n");
    }
}

void cmd_navigate(GameState *game, const char *destination) {
    if (strlen(destination) == 0) {
        printf("Navigate to where?\n");
        printf("Available rooms: garage, workshop, parts, paint, track, office, tools, computer, showroom\n");
        return;
    }

    // Find destination room
    RoomID dest_id = ROOM_COUNT;

    for (int i = 0; i < ROOM_COUNT; i++) {
        char name_lower[MAX_NAME];
        strncpy(name_lower, game->rooms[i].name, MAX_NAME - 1);
        to_lowercase(name_lower);

        if (strstr(name_lower, destination) != nullptr) {
            dest_id = (RoomID)i;
            break;
        }
    }

    if (dest_id >= ROOM_COUNT) {
        printf("Unknown destination.\n");
        return;
    }

    if (dest_id == game->current_room) {
        printf("You're already there!\n");
        return;
    }

    printf("\n=== PATHFINDING: %s to %s ===\n\n",
           game->rooms[game->current_room].name,
           game->rooms[dest_id].name);

    // Use A* pathfinding
    RoomID path[MAX_PATH_LENGTH];
    int path_length = 0;

    if (astar_pathfinding(game->rooms, game->current_room, dest_id, path, path_length)) {
        printf("Optimal path found (%d steps):\n", path_length - 1);
        print_path(game->rooms, path, path_length);

        printf("\nWould you like to follow this path? (yes/no): ");
        char answer[10];
        if (fgets(answer, sizeof(answer), stdin) != nullptr) {
            to_lowercase(answer);
            if (strncmp(answer, "yes", 3) == 0 || strncmp(answer, "y", 1) == 0) {
                game->current_room = dest_id;
                printf("\nYou navigate to the destination.\n");
                cmd_look(game);
            }
        }
    } else {
        printf("No path found to destination.\n");
    }
}

void cmd_help(void) {
    print_separator();
    printf("AVAILABLE COMMANDS:\n");
    print_separator();
    printf("\nMovement:\n");
    printf("  go <direction>     - Move in a direction (north, south, east, west)\n");
    printf("  n, s, e, w         - Quick movement shortcuts\n");
    printf("  navigate <room>    - Use A* pathfinding to find optimal route\n");
    printf("\nInformation:\n");
    printf("  look               - Look around current room\n");
    printf("  examine <object>   - Examine an object in detail\n");
    printf("  inventory          - View garage inventory\n");
    printf("\nVehicle Management:\n");
    printf("  install <part>     - Install a part on a vehicle\n");
    printf("  calculate <type>   - Perform physics calculations\n");
    printf("                       Types: all, terminal, acceleration, braking\n");
    printf("\nOther:\n");
    printf("  help               - Show this help message\n");
    printf("  quit               - Exit the game\n");
    print_separator();
}

// ============================================================================
// PHYSICS CALCULATIONS
// ============================================================================

double calculate_terminal_velocity(const Vehicle *v) {
    // Terminal velocity occurs when drag force equals driving force
    // F_drag = 0.5 * ρ * Cd * A * v²
    // Simplified calculation
    double power_watts = v->engine_power * 745.7;  // Convert HP to watts
    double drag_factor = 0.5 * AIR_DENSITY * v->drag_coefficient * v->frontal_area;

    // v = (P / (0.5 * ρ * Cd * A))^(1/3)
    return cbrt(power_watts / drag_factor);
}

double calculate_acceleration_time(const Vehicle *v, double target_speed) {
    // Simplified calculation: t = v / a
    // In reality, this is more complex due to changing force with speed
    double effective_accel = v->acceleration * (v->engine_power / 100.0) / (v->mass / 1000.0);
    return target_speed / effective_accel;
}

double calculate_braking_distance(const Vehicle *v, double initial_speed) {
    // Using kinematic equation: d = v² / (2 * a)
    // Assume braking deceleration is about 0.8 * g for good brakes
    double braking_decel = 0.8 * GRAVITY;
    return (initial_speed * initial_speed) / (2.0 * braking_decel);
}

double calculate_drag_force(const Vehicle *v, double velocity) {
    // F_drag = 0.5 * ρ * Cd * A * v²
    return 0.5 * AIR_DENSITY * v->drag_coefficient * v->frontal_area * velocity * velocity;
}

double calculate_power_required(const Vehicle *v, double velocity) {
    // Power = Force * velocity
    double drag_force = calculate_drag_force(v, velocity);
    return drag_force * velocity;
}

double calculate_lap_time(const Vehicle *v, double track_length, int turns) {
    // Simplified lap time calculation
    // Straight sections at near max speed, slow down for turns
    double avg_turn_speed = v->max_speed * 0.6;  // 60% of max speed in turns
    double straight_speed = v->max_speed * 0.9;  // 90% of max speed on straights

    double turn_distance = turns * 50.0;  // Assume 50m per turn
    double straight_distance = track_length - turn_distance;

    double turn_time = turn_distance / avg_turn_speed;
    double straight_time = straight_distance / straight_speed;

    // Add time for accelerating out of turns
    double accel_penalty = turns * 1.5;

    return turn_time + straight_time + accel_penalty;
}

// ============================================================================
// PATHFINDING ALGORITHMS
// ============================================================================

double heuristic_distance(Point2D a, Point2D b) {
    // Euclidean distance for A* heuristic
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

void dijkstra_shortest_path(const Room rooms[], RoomID start, RoomID end,
                           PathfindingState *state) {
    // Initialize
    for (int i = 0; i < MAX_ROOMS; i++) {
        state->cost[i] = INFINITY_COST;
        state->visited[i] = false;
        state->parent[i] = -1;
    }

    state->cost[start] = 0;

    // Main algorithm loop
    for (int count = 0; count < ROOM_COUNT; count++) {
        // Find minimum cost unvisited node
        double min_cost = INFINITY_COST;
        int min_node = -1;

        for (int i = 0; i < ROOM_COUNT; i++) {
            if (!state->visited[i] && state->cost[i] < min_cost) {
                min_cost = state->cost[i];
                min_node = i;
            }
        }

        if (min_node == -1 || min_node == end) {
            break;
        }

        state->visited[min_node] = true;

        // Update neighbors
        for (int dir = 0; dir < DIR_COUNT; dir++) {
            RoomID neighbor = rooms[min_node].connections[dir];

            if (neighbor != min_node && !state->visited[neighbor]) {
                double edge_cost = heuristic_distance(
                    rooms[min_node].coordinates,
                    rooms[neighbor].coordinates
                );

                double new_cost = state->cost[min_node] + edge_cost;

                if (new_cost < state->cost[neighbor]) {
                    state->cost[neighbor] = new_cost;
                    state->parent[neighbor] = min_node;
                }
            }
        }
    }
}

bool astar_pathfinding(const Room rooms[], RoomID start, RoomID end,
                      RoomID path[], int path_length) {
    // A* pathfinding implementation
    double g_score[MAX_ROOMS];  // Cost from start
    double f_score[MAX_ROOMS];  // g_score + heuristic
    bool closed_set[MAX_ROOMS];
    bool open_set[MAX_ROOMS];
    int came_from[MAX_ROOMS];

    // Initialize
    for (int i = 0; i < MAX_ROOMS; i++) {
        g_score[i] = INFINITY_COST;
        f_score[i] = INFINITY_COST;
        closed_set[i] = false;
        open_set[i] = false;
        came_from[i] = -1;
    }

    g_score[start] = 0;
    f_score[start] = heuristic_distance(rooms[start].coordinates, rooms[end].coordinates);
    open_set[start] = true;

    int actual_path_length = 0;

    // Main A* loop
    while (true) {
        // Find node in open_set with lowest f_score
        double min_f = INFINITY_COST;
        int current = -1;

        for (int i = 0; i < ROOM_COUNT; i++) {
            if (open_set[i] && f_score[i] < min_f) {
                min_f = f_score[i];
                current = i;
            }
        }

        if (current == -1) {
            return false;  // No path found
        }

        if (current == end) {
            // Reconstruct path
            actual_path_length = 0;
            int node = end;

            while (node != -1 && actual_path_length < MAX_PATH_LENGTH) {
                path[actual_path_length] = (RoomID)node;
                actual_path_length++;
                node = came_from[node];
            }

            // Reverse path
            for (int i = 0; i < actual_path_length / 2; i++) {
                RoomID temp = path[i];
                path[i] = path[actual_path_length - 1 - i];
                path[actual_path_length - 1 - i] = temp;
            }

            return true;
        }

        open_set[current] = false;
        closed_set[current] = true;

        // Check neighbors
        for (int dir = 0; dir < DIR_COUNT; dir++) {
            RoomID neighbor = rooms[current].connections[dir];

            if (neighbor == current || closed_set[neighbor]) {
                continue;
            }

            double tentative_g = g_score[current] +
                               heuristic_distance(rooms[current].coordinates,
                                                rooms[neighbor].coordinates);

            if (!open_set[neighbor]) {
                open_set[neighbor] = true;
            } else if (tentative_g >= g_score[neighbor]) {
                continue;
            }

            came_from[neighbor] = current;
            g_score[neighbor] = tentative_g;
            f_score[neighbor] = g_score[neighbor] +
                              HEURISTIC_WEIGHT * heuristic_distance(rooms[neighbor].coordinates,
                                                                   rooms[end].coordinates);
        }
    }

    return false;
}

void print_path(const Room rooms[], const RoomID path[], int path_length) {
    for (int i = 0; i < path_length; i++) {
        printf("  %d. %s", i + 1, rooms[path[i]].name);

        if (i < path_length - 1) {
            // Figure out direction to next room
            for (int dir = 0; dir < DIR_COUNT; dir++) {
                if (rooms[path[i]].connections[dir] == path[i + 1]) {
                    printf(" -> %s", direction_to_string((Direction)dir));
                    break;
                }
            }
        }
        printf("\n");
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

const char* direction_to_string(Direction dir) {
    switch (dir) {
        case DIR_NORTH: return "north";
        case DIR_SOUTH: return "south";
        case DIR_EAST: return "east";
        case DIR_WEST: return "west";
        default: return "unknown";
    }
}

Direction string_to_direction(const char *str) {
    if (strcmp(str, "north") == 0 || strcmp(str, "n") == 0) return DIR_NORTH;
    if (strcmp(str, "south") == 0 || strcmp(str, "s") == 0) return DIR_SOUTH;
    if (strcmp(str, "east") == 0 || strcmp(str, "e") == 0) return DIR_EAST;
    if (strcmp(str, "west") == 0 || strcmp(str, "w") == 0) return DIR_WEST;
    return DIR_COUNT;
}

const char* vehicle_type_to_string(VehicleType type) {
    switch (type) {
        case VEHICLE_SPORTS_CAR: return "Sports Car";
        case VEHICLE_TRUCK: return "Truck";
        case VEHICLE_MOTORCYCLE: return "Motorcycle";
        case VEHICLE_RACE_CAR: return "Race Car";
        case VEHICLE_CLASSIC_CAR: return "Classic Car";
        default: return "Unknown";
    }
}

const char* part_type_to_string(PartType type) {
    switch (type) {
        case PART_ENGINE: return "Engine";
        case PART_TRANSMISSION: return "Transmission";
        case PART_WHEELS: return "Wheels";
        case PART_BRAKES: return "Brakes";
        case PART_SUSPENSION: return "Suspension";
        case PART_TURBOCHARGER: return "Turbocharger";
        case PART_EXHAUST: return "Exhaust";
        case PART_ECU: return "ECU";
        default: return "Unknown";
    }
}

void to_lowercase(char *str) {
    for (; *str; ++str) {
        *str = tolower((unsigned char)*str);
    }
}

void print_separator(void) {
    printf("========================================================\n");
}
