# 🔧 Developer Notes - Cricket Simulator

## Code Architecture

### Module Overview

```
┌─────────────────────────────────────────────────────────┐
│                    Main Application                     │
│                    (main function)                      │
└────────────┬────────────────────────────────────────────┘
             │
    ┌────────┴────────┐
    │  Initialization  │
    │  welcome_screen()│
    │  initialize_game()│
    └────────┬─────────┘
             │
    ┌────────▼─────────┐
    │   Main Menu Loop │
    │   main_menu()    │
    └────────┬─────────┘
             │
    ┌────────┴─────────────────────────────────────────────┐
    │                                                        │
┌───▼────┐  ┌──────────┐  ┌──────────┐  ┌──────────────┐
│ Auth   │  │  Team    │  │  Match   │  │  Statistics  │
│ Module │  │  Mgmt    │  │  Engine  │  │  & Display   │
└────────┘  └──────────┘  └──────────┘  └──────────────┘
```

### Data Structures

#### Player Structure
```c
typedef struct {
    char name[MAX_NAME_LENGTH];      // Player name
    int runs;                         // Runs scored
    int balls_faced;                  // Balls faced while batting
    int fours, sixes;                 // Boundaries hit
    int is_out;                       // Dismissal status
    int is_bowler;                    // Can bowl flag
    float strike_rate;                // Batting efficiency
    float overs;                      // Overs bowled
    int balls_bowled;                 // Total balls delivered
    int runs_conceded;                // Runs given while bowling
    int wickets;                      // Wickets taken
    float economy;                    // Bowling economy
} Player;
```

#### Team Structure
```c
typedef struct {
    char name[MAX_NAME_LENGTH];
    Player players[MAX_PLAYERS];     // Array of players
    int player_count;                 // Active players
    int total_runs;                   // Team score
    int total_wickets;                // Wickets lost
    int total_balls;                  // Balls faced
    float current_rr;                 // Current run rate
} Team;
```

#### User Structure
```c
typedef struct {
    char username[MAX_USERNAME];
    char password_hash[65];           // SHA-like hash
    int team_index;                   // 0 or 1
} User;
```

## Key Algorithms

### 1. Ball Simulation Algorithm

```c
void simulate_ball(void) {
    // Generate random run (0-6)
    int runs = rand() % 7;
    
    // Determine wicket (15% probability)
    int is_wicket = (rand() % 100) < 15;
    
    // Reduce runs if wicket falls
    if (is_wicket && runs > 2) 
        runs = rand() % 3;
    
    // Update statistics
    // - Batsman: runs, balls, SR
    // - Bowler: runs_conceded, economy
    // - Team: total_runs, total_balls, wickets
}
```

**Design Decisions:**
- Random number generation for realistic unpredictability
- Wicket probability configurable (default 15%)
- Lower runs on wicket balls for realism
- Immediate statistics calculation

### 2. Man of the Match Calculation

```c
float calculate_motm_score(Player* p) {
    float score = 0.0f;
    
    // Batting contribution
    if (p->balls_faced > 0)
        score += p->runs + (p->strike_rate * 0.2f);
    
    // Bowling contribution  
    if (p->is_bowler && p->balls_bowled > 0)
        score += (p->wickets * 25.0f) - (p->economy * 2.0f);
    
    return score;
}
```

**Scoring Weights:**
- Each run = 1 point
- Strike rate bonus = SR × 0.2
- Each wicket = 25 points
- Economy penalty = Economy × 2

### 3. Password Hashing

```c
void simple_hash(const char* input, char* output) {
    unsigned long hash = 5381;      // DJB2 hash initial value
    int c;
    
    while ((c = *input++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    
    snprintf(output, 65, "%lx", hash);
}
```

**Note:** This is NOT cryptographically secure. For production:
- Use bcrypt or Argon2
- Add salt
- Use proper key derivation

## File I/O Operations

### Binary User Data Format

```
[4 bytes: user_count]
[sizeof(User) * user_count bytes: user array]
```

**Advantages:**
- Fast read/write
- Fixed-size records
- Efficient storage

**Limitations:**
- Not human-readable
- Platform-dependent (endianness)
- Version migration challenges

### Text Match History Format

```
Date: Wed Dec 24 2025 15:30:45
OMOR EKUSHEY HALL vs SHAHID SMRITY HALL
Winner: OMOR EKUSHEY HALL
Margin: 15 RUNS
----------------------------------
```

**Advantages:**
- Human-readable
- Easy to parse
- Cross-platform

## Cross-Platform Compatibility

### Windows vs Unix Differences

| Feature | Windows | Unix/Linux |
|---------|---------|------------|
| Clear screen | `system("cls")` | `system("clear")` |
| Password input | `_getch()` | Custom termios |
| ANSI colors | Virtual terminal | Native support |
| Headers | `windows.h`, `conio.h` | `termios.h`, `unistd.h` |

### Conditional Compilation

```c
#ifdef _WIN32
    // Windows-specific code
    #include <windows.h>
    #include <conio.h>
#else
    // Unix-specific code
    #include <termios.h>
    #include <unistd.h>
#endif
```

## Memory Management

### Static vs Dynamic Allocation

**Current Approach:** Static arrays
```c
static Team teams[MAX_TEAMS];
static User users[MAX_USERS];
```

**Advantages:**
- Simple, no malloc/free needed
- Predictable memory usage
- No memory leaks

**Trade-offs:**
- Fixed capacity limits
- Memory used even if not needed
- Not scalable for large datasets

### Potential Memory Issues

1. **Buffer Overflows**
   - `strncpy()` used for safety
   - Always null-terminate strings
   - `fgets()` with size limits

2. **Stack vs Heap**
   - Large arrays on stack (OK for small programs)
   - Consider heap for scalability

## Code Quality & Best Practices

### Current Strengths

✅ **Modular Design** - Separate functions for distinct tasks
✅ **Input Validation** - `safe_input()`, `safe_int_input()`
✅ **Error Handling** - File operation checks
✅ **Code Readability** - Clear naming conventions
✅ **Cross-platform** - Conditional compilation

### Areas for Improvement

#### 1. Error Handling
```c
// Current
FILE* file = fopen("cricket_users.dat", "rb");
if (!file) return 0;

// Better
FILE* file = fopen("cricket_users.dat", "rb");
if (!file) {
    perror("Failed to open user data");
    return ERROR_FILE_NOT_FOUND;
}
```

#### 2. Magic Numbers
```c
// Current
int is_wicket = (rand() % 100) < 15;

// Better
#define WICKET_PROBABILITY_PERCENT 15
int is_wicket = (rand() % 100) < WICKET_PROBABILITY_PERCENT;
```

#### 3. Password Security
```c
// Current: Simple hash
simple_hash(password, hash);

// Better: Use bcrypt or PBKDF2
// External library needed
```

## Performance Characteristics

### Time Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Ball simulation | O(1) | Constant time |
| Add player | O(1) | Array append |
| Find player | O(n) | Linear search |
| Login | O(n) | Linear user search |
| Save data | O(n) | Write all users |

### Optimization Opportunities

1. **Player Lookup**
   - Current: Linear search O(n)
   - Improved: Hash table O(1)

2. **Bowler Rotation**
   - Current: Sequential search for next bowler
   - Improved: Maintain bowler queue

3. **Statistics Calculation**
   - Current: Recalculate on every ball
   - Improved: Incremental updates only

## Testing Guidelines

### Unit Testing Checklist

- [ ] `simple_hash()` - consistent output for same input
- [ ] `calculate_strike_rate()` - edge cases (0 balls)
- [ ] `calculate_economy()` - edge cases (0 overs)
- [ ] `safe_input()` - buffer overflow protection
- [ ] File I/O - handle corrupted data

### Integration Testing

1. **Complete Match Flow**
   - Signup → Login → Add players → Simulate match → Result

2. **Edge Cases**
   - Maximum players (11)
   - All players out
   - No balls remaining
   - Empty teams

3. **Data Persistence**
   - Save and reload
   - Multiple sessions
   - Corrupted file recovery

### Manual Testing Scenarios

```bash
# Test 1: Basic match
1. Create two users
2. Each adds 3 players, 2 bowlers
3. Simulate complete match
4. Verify statistics

# Test 2: Edge cases
1. Try adding 12th player (should fail)
2. Try match with 1 player (should fail)
3. Test password validation (<6 chars)

# Test 3: Persistence
1. Add users and play match
2. Exit and save
3. Restart program
4. Verify data retained
```

## Build & Deployment

### Compilation Flags

```bash
# Debug build
gcc -g -Wall -Wextra -std=c99 cricket_simulator.c -o cricket_debug

# Release build  
gcc -O2 -Wall -std=c99 cricket_simulator.c -o cricket_simulator

# With all warnings
gcc -Wall -Wextra -Wpedantic -Werror cricket_simulator.c -o cricket_simulator
```

### Recommended Compiler Flags

- `-Wall` - Enable all warnings
- `-Wextra` - Extra warnings
- `-std=c99` - C99 standard
- `-O2` - Optimization level 2
- `-g` - Debug symbols (development)

## Future Enhancements - Implementation Guide

### 1. Database Integration (SQLite)

```c
// Replace file I/O with SQLite
#include <sqlite3.h>

sqlite3* db;
sqlite3_open("cricket.db", &db);

// Create tables
const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                  "id INTEGER PRIMARY KEY, "
                  "username TEXT UNIQUE, "
                  "password_hash TEXT, "
                  "team_index INTEGER)";
```

### 2. Network Multiplayer

```c
// Use sockets for client-server
#include <sys/socket.h>

// Server listens on port
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
bind(server_fd, ...);
listen(server_fd, 5);

// Clients connect
int client_fd = socket(AF_INET, SOCK_STREAM, 0);
connect(client_fd, ...);
```

### 3. Tournament Mode

```c
typedef struct {
    Team teams[MAX_TOURNAMENT_TEAMS];
    int team_count;
    int current_round;
    Match fixtures[MAX_MATCHES];
} Tournament;
```

### 4. GUI Version

Recommended frameworks:
- **GTK** - Cross-platform, C-native
- **Qt** - Modern, extensive
- **raylib** - Lightweight, game-focused

## Debugging Tips

### Common Issues

1. **Segmentation Fault**
   - Check array bounds
   - Verify pointer validity
   - Use gdb: `gdb ./cricket_simulator`

2. **File Not Saving**
   - Check file permissions
   - Verify directory exists
   - Print errno for details

3. **Colors Not Working**
   - Windows: Call `enable_colors()`
   - Check terminal support
   - Test with `printf(RED "test" RESET)`

### GDB Commands

```bash
gdb ./cricket_simulator
(gdb) break main
(gdb) run
(gdb) next          # Step over
(gdb) step          # Step into
(gdb) print variable_name
(gdb) backtrace     # Stack trace
```

## Contributing Guidelines

### Code Style

- **Indentation**: 4 spaces (no tabs)
- **Braces**: K&R style
- **Names**: snake_case for functions/variables
- **Constants**: UPPER_CASE for macros
- **Comments**: Explain why, not what

### Pull Request Process

1. Fork repository
2. Create feature branch
3. Write tests for new features
4. Ensure code compiles without warnings
5. Update documentation
6. Submit PR with clear description

### Commit Message Format

```
[TYPE] Brief description (50 chars max)

Detailed explanation of changes (if needed)

Fixes #issue_number
```

Types: `feat`, `fix`, `docs`, `refactor`, `test`, `style`

## Resources

### Documentation
- [C Standard Library](https://en.cppreference.com/w/c)
- [GCC Manual](https://gcc.gnu.org/onlinedocs/)
- [POSIX API](https://pubs.opengroup.org/onlinepubs/9699919799/)

### Tools
- **Valgrind** - Memory leak detection
- **GDB** - Debugging
- **Clang-format** - Code formatting
- **Cppcheck** - Static analysis

---

**Last Updated:** December 2025
