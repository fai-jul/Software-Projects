#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#define MAX_PLAYERS 11
#define MAX_TEAMS 2
#define DEFAULT_OVERS 3
#define MAX_USERNAME 50
#define MAX_PASSWORD 100
#define MAX_USERS 100
#define MAX_NAME_LENGTH 50

#ifdef _WIN32
    #define RESET ""
    #define RED ""
    #define GREEN ""
    #define YELLOW ""
    #define BLUE ""
    #define MAGENTA ""
    #define CYAN ""
    #define WHITE ""
#else
    #define RESET "\033[0m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
    #define MAGENTA "\033[35m"
    #define CYAN "\033[36m"
    #define WHITE "\033[37m"
#endif

typedef struct {
    char name[MAX_NAME_LENGTH];
    int runs;
    int balls_faced;
    int fours;
    int sixes;
    int is_out;
    int is_bowler;
    float strike_rate;
    float overs;
    int balls_bowled;
    int runs_conceded;
    int wickets;
    float economy;
} Player;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Player players[MAX_PLAYERS];
    int player_count;
    int total_runs;
    int total_wickets;
    int total_balls;
    float current_rr;
} Team;

typedef struct {
    char username[MAX_USERNAME];
    char password_hash[65]; 
    int team_index;
} User;

typedef struct {
    int max_overs;
    float wicket_probability;
    char tournament_name[100];
} GameConfig;

static Team teams[MAX_TEAMS];
static User users[MAX_USERS];
static int user_count = 0;
static int logged_in_team_index = -1;
static int currently_batting = 0;
static int first_innings = 1;
static int match_over = 0;
static GameConfig config = {DEFAULT_OVERS, 0.15f, "CRICMANIA"};

void initialize_game(void);
void main_menu(void);
void welcome_screen(void);
int login_system(void);
int signup_system(void);
void manage_team(Team* team);
void simulate_match(void);
void simulate_ball(void);
void display_scoreboard(int clear);
void display_match_result(void);
void save_game_data(void);
int load_game_data(void);
void cleanup_and_exit(void);
void show_man_of_the_match(void);
void clear_screen(void);
void safe_input(char* buffer, int size, const char* prompt);
int safe_int_input(const char* prompt);
void simple_hash(const char* input, char* output);
float calculate_strike_rate(int runs, int balls);
float calculate_economy(int runs, float overs);
void save_match_history(Team *winner, int margin, const char *type);

#ifdef _WIN32
void enable_colors(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

char getch_silent(void) {
    return _getch();
}
#else
void enable_colors(void) {
}

char getch_silent(void) {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

int main(void) {
    srand(time(NULL));
    enable_colors();
    
    welcome_screen();
    initialize_game();
    main_menu();
    cleanup_and_exit();
    
    return 0;
}

void initialize_game(void) {
    strncpy(teams[0].name, "OMOR EKUSHEY HALL", MAX_NAME_LENGTH - 1);
    strncpy(teams[1].name, "SHAHID SMRITY HALL", MAX_NAME_LENGTH - 1);
    
    for (int i = 0; i < MAX_TEAMS; i++) {
        teams[i].player_count = 0;
        teams[i].total_runs = 0;
        teams[i].total_wickets = 0;
        teams[i].total_balls = 0;
        teams[i].current_rr = 0.0f;
    }
    
    user_count = load_game_data();
    printf(CYAN "Game initialized successfully!\n" RESET);
}

void welcome_screen(void) {
    clear_screen();
    printf(CYAN "╔══════════════════════════════════════════════════════════╗\n");
    printf("║                    CRICKET SIMULATOR                     ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n" RESET);
    printf(YELLOW "\nEXPERIENCE THE THRILL OF CRICKET SIMULATION!\n");
    printf("TWO LEGENDARY TEAMS BATTLE IT OUT IN AN EPIC SHOWDOWN.\n\n" RESET);
    
    printf("PRESS ANY KEY TO CONTINUE...");
    getch_silent();
    printf("\n");
}

void main_menu(void) {
    int choice;
    
    do {
        if (match_over) break;
        
        clear_screen();
        printf(WHITE "\n╔═══════════════ MAIN MENU ═══════════════╗\n");
        printf("║  1. LOGIN                               ║\n");
        printf("║  2. SIGNUP                              ║\n");
        printf("║  3. MANAGE TEAM                         ║\n");
        printf("║  4. SIMULATE MATCH                      ║\n");
        printf("║  5. VIEW STATISTICS                     ║\n");
        printf("║  6. SAVE & EXIT                         ║\n");
        printf("╚═════════════════════════════════════════╝\n" RESET);
        
        choice = safe_int_input("ENTER YOUR CHOICE (1–6):");
        
        switch (choice) {
            case 1:
                if (login_system()) {
                    printf(GREEN "\nLOGIN SUCCESSFUL! MANAGING... %s\n" RESET,
                           teams[logged_in_team_index].name);
                    printf("PRESS ANY KEY TO CONTINUE...");
                    getch_silent();
                } else {
                    printf(RED "\nLogin failed! Invalid credentials.\n" RESET);
                    printf("PRESS ANY KEY TO CONTINUE...");
                    getch_silent();
                }
                break;
                
            case 2:
                if (signup_system()) {
                    printf(GREEN "\nSIGNUP SUCCESSFUL! YOU CAN NOW LOGIN.\n" RESET);
                    save_game_data();
                } else {
                    printf(RED "\nSIGNUP FAILED! USERNAME MAY EXIST OR PASSWORD IS WEAK.\n" RESET);
                }
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
                
            case 3:
                if (logged_in_team_index != -1) {
                    manage_team(&teams[logged_in_team_index]);
                } else {
                    printf(RED "\nPLEASE LOGIN FIRST!\n" RESET);
                    printf("PRESS ANY KEY TO CONTINUE...");
                    getch_silent();
                }
                break;
                
            case 4:
                if (logged_in_team_index != -1) {
                    simulate_match();
                } else {
                    printf(RED "\nPLEASE LOGIN FIRST!\n" RESET);
                    printf("PRESS ANY KEY TO CONTINUE...");
                    getch_silent();
                }
                break;
                
            case 5:
                display_scoreboard(1);
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
                
            case 6:
                save_game_data();
                printf(CYAN "\nGAME SAVED SUCCESSFULLY! THANKS FOR PLAYING!\n" RESET);
                break;
                
            default:
                printf(RED "\nINVALID CHOICE! PLEASE TRY AGAIN.\n" RESET);
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
        }
    } while (choice != 6);
}

int login_system(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char password_hash[65];
    
    safe_input(username, MAX_USERNAME, "ENTER USERNAME: ");
    
    printf("ENTER PASSWORD: ");
    int i = 0;
    char ch;
    while ((ch = getch_silent()) != '\r' && ch != '\n' && i < MAX_PASSWORD - 1) {
        if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
    
    simple_hash(password, password_hash);
    
    for (int j = 0; j < user_count; j++) {
        if (strcmp(users[j].username, username) == 0 &&
            strcmp(users[j].password_hash, password_hash) == 0) {
            logged_in_team_index = users[j].team_index;
            return 1;
        }
    }
    
    return 0;
}

int signup_system(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int team_choice;
    
    if (user_count >= MAX_USERS) {
        printf(RED "MAXIMUM USER REACHED!\n" RESET);
        return 0;
    }
    
    safe_input(username, MAX_USERNAME, "ENTER NEW USERNAME: ");
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0;
        }
    }
    
    safe_input(password, MAX_PASSWORD, "ENTER NEW PASSWORD: ");
    
    if (strlen(password) < 6) {
        printf(RED "PASSWORD TOO SHORT! MINIMUM 6 CHARACTERS.\n" RESET);
        return 0;
    }
    
    printf("\nCHOOSE TEAM:\n");
    printf("1. %s\n", teams[0].name);
    printf("2. %s\n", teams[1].name);
    team_choice = safe_int_input("ENTER CHOICE (1-2): ");
    
    if (team_choice < 1 || team_choice > 2) {
        return 0;
    }
    
    strncpy(users[user_count].username, username, MAX_USERNAME - 1);
    simple_hash(password, users[user_count].password_hash);
    users[user_count].team_index = team_choice - 1;
    user_count++;
    
    return 1;
}

void manage_team(Team* team) {
    int choice;
    
    do {
        clear_screen();
        printf(YELLOW "╔═══════════ MANAGING %s ═══════════╗\n", team->name);
        printf("║  1. ADD PLAYER                                      ║\n");
        printf("║  2. MARK PLAYER AS BOWLER                           ║\n");
        printf("║  3. VIEW SQUAD                                      ║\n");
        printf("║  4. REMOVE PLAYER                                   ║\n");
        printf("║  5. BACK TO MAIN MENU                               ║\n");
        printf("╚═════════════════════════════════════════════════════╝\n" RESET);
        
        choice = safe_int_input("ENTER YOUR CHOICE (1-5): ");
        
        switch (choice) {
            case 1: {
                if (team->player_count >= MAX_PLAYERS) {
                    printf(RED "TEAM IS FULL! CANNOT ADD MORE PLAYERS.\n" RESET);
                } else {
                    char name[MAX_NAME_LENGTH];
                    safe_input(name, MAX_NAME_LENGTH, "ENTER PLAYER NAME: ");
                    
                    strncpy(team->players[team->player_count].name, name, MAX_NAME_LENGTH - 1);
                    team->players[team->player_count].runs = 0;
                    team->players[team->player_count].balls_faced = 0;
                    team->players[team->player_count].fours = 0;
                    team->players[team->player_count].sixes = 0;
                    team->players[team->player_count].is_out = 0;
                    team->players[team->player_count].is_bowler = 0;
                    team->players[team->player_count].strike_rate = 0.0f;
                    team->players[team->player_count].overs = 0.0f;
                    team->players[team->player_count].balls_bowled = 0;
                    team->players[team->player_count].runs_conceded = 0;
                    team->players[team->player_count].wickets = 0;
                    team->players[team->player_count].economy = 0.0f;
                    team->player_count++;
                    
                    printf(GREEN "PLAYER ADDED SUCCESSFULLY!\n" RESET);
                }
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
            }
            
            case 2: {
                if (team->player_count == 0) {
                    printf(RED "NO PLAYERS TO MARK AS BOWLER!\n" RESET);
                } else {
                    char name[MAX_NAME_LENGTH];
                    safe_input(name, MAX_NAME_LENGTH, "ENTER PLAYER NAME TO MARK AS BOWLER: ");
                    
                    int found = 0;
                    for (int i = 0; i < team->player_count; i++) {
                        if (strcmp(team->players[i].name, name) == 0) {
                            team->players[i].is_bowler = 1;
                            found = 1;
                            printf(GREEN "PLAYER MARKED AS BOWLER SUCCESSFULLY!\n" RESET);
                            break;
                        }
                    }
                    
                    if (!found) {
                        printf(RED "PLAYER NOT FOUND!\n" RESET);
                    }
                }
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
            }
            
            case 3: {
                clear_screen();
                printf(CYAN "╔═══════════ %s SQUAD ═══════════╗\n" RESET, team->name);
                
                printf(YELLOW "\nPlayers:\n" RESET);
                if (team->player_count == 0) {
                    printf("NO PLAYERS ADDED YET.\n");
                } else {
                    for (int i = 0; i < team->player_count; i++) {
                        printf("%d. %s%s\n", i + 1, team->players[i].name,
                               team->players[i].is_bowler ? " (BOWLER)" : "");
                    }
                }
                
                printf("\nPRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
            }
            
            case 4: {
                if (team->player_count == 0) {
                    printf(RED "NO PLAYERS TO REMOVE!\n" RESET);
                } else {
                    char name[MAX_NAME_LENGTH];
                    safe_input(name, MAX_NAME_LENGTH, "ENTER PLAYER NAME TO REMOVE: ");
                    
                    int found = 0;
                    for (int i = 0; i < team->player_count; i++) {
                        if (strcmp(team->players[i].name, name) == 0) {
                            for (int j = i; j < team->player_count - 1; j++) {
                                team->players[j] = team->players[j + 1];
                            }
                            team->player_count--;
                            found = 1;
                            printf(GREEN "PLAYER REMOVED SUCCESSFULLY!\n" RESET);
                            break;
                        }
                    }
                    
                    if (!found) {
                        printf(RED "PLAYER NOT FOUND!\n" RESET);
                    }
                }
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
                break;
            }
            
            case 5:
                break;
                
            default:
                printf(RED "INVALID CHOICE! PLEASE TRY AGAIN.\n" RESET);
                printf("PRESS ANY KEY TO CONTINUE...");
                getch_silent();
        }
    } while (choice != 5);
}

void simulate_match(void) {
    int bowler_count_team0 = 0, bowler_count_team1 = 0;
    
    for (int i = 0; i < teams[0].player_count; i++) {
        if (teams[0].players[i].is_bowler) bowler_count_team0++;
    }
    for (int i = 0; i < teams[1].player_count; i++) {
        if (teams[1].players[i].is_bowler) bowler_count_team1++;
    }
    
    if (teams[0].player_count < 2 || teams[1].player_count < 2 ||
        bowler_count_team0 < 2 || bowler_count_team1 < 2) {
        printf(RED "BOTH TEAMS NEED AT LEAST 2 PLAYERS AND 2 BOWLERS TO START!\n" RESET);
        printf("PRESS ANY KEY TO CONTINUE...");
        getch_silent();
        return;
    }
    
    match_over = 0;
    first_innings = 1;
    currently_batting = rand() % 2;
    
    printf(CYAN "\nTOSS: %s WILL BAT FIRST!\n" RESET, teams[currently_batting].name);
    printf("PRESS ANY KEY TO CONTINUE...");
    getch_silent();
    
    for (int t = 0; t < MAX_TEAMS; t++) {
        teams[t].total_runs = 0;
        teams[t].total_wickets = 0;
        teams[t].total_balls = 0;
        teams[t].current_rr = 0.0f;
        
        for (int i = 0; i < teams[t].player_count; i++) {
            teams[t].players[i].runs = 0;
            teams[t].players[i].balls_faced = 0;
            teams[t].players[i].fours = 0;
            teams[t].players[i].sixes = 0;
            teams[t].players[i].is_out = 0;
            teams[t].players[i].strike_rate = 0.0f;
            teams[t].players[i].overs = 0.0f;
            teams[t].players[i].balls_bowled = 0;
            teams[t].players[i].runs_conceded = 0;
            teams[t].players[i].wickets = 0;
            teams[t].players[i].economy = 0.0f;
        }
    }
    
    printf(CYAN "\n╔══════════════ MATCH STARTED ══════════════╗\n");
    printf("║  %s  VS  %s  ║\n", teams[0].name, teams[1].name);
    printf("╚════════════════════════════════════════════╝\n" RESET);
    
    int ball_choice;
    do {
        if (match_over) break;
        
        printf(WHITE "\n1. SIMULATE NEXT BALL\n");
        printf("2. VIEW CURRENT STATUS\n");
        printf("3. AUTO-COMPLETE MATCH\n");
        printf("4. BACK TO MAIN MENU\n" RESET);
        
        ball_choice = safe_int_input("ENTER CHOICE: ");
        
        switch (ball_choice) {
            case 1:
                simulate_ball();
                break;
            case 2:
                display_scoreboard(1);
                break;
            case 3:
                while (!match_over && teams[currently_batting].total_balls < config.max_overs * 6) {
                    simulate_ball();
                }
                break;
            case 4:
                return;
            default:
                printf(RED "INVALID CHOICE!\n" RESET);
        }
        
        if (match_over) {
            display_match_result();
            break;
        }
        
    } while (ball_choice != 4);
}

void simulate_ball(void) {
    if (match_over || teams[currently_batting].total_balls >= config.max_overs * 6) {
        return;
    }
    
    static int current_batsman = 0;
    static int current_bowler_idx = 0;
    
    if (current_batsman >= teams[currently_batting].player_count) {
        current_batsman = 0;
    }
    
    int bowling_team = 1 - currently_batting;
    Player* bowler = NULL;
    int bowler_count = 0;
    
    for (int i = 0; i < teams[bowling_team].player_count; i++) {
        if (teams[bowling_team].players[i].is_bowler) {
            if (bowler_count == current_bowler_idx) {
                bowler = &teams[bowling_team].players[i];
                break;
            }
            bowler_count++;
        }
    }
    
    if (bowler == NULL) {
        printf(RED "NO BOWLER AVAILABLE!\n" RESET);
        return;
    }
    
    Player* batsman = &teams[currently_batting].players[current_batsman];
    
    int runs = rand() % 7;
    int is_wicket = (rand() % 100) < (config.wicket_probability * 100);
    
    if (is_wicket && runs > 2) runs = rand() % 3;
    
    batsman->runs += runs;
    batsman->balls_faced++;
    if (runs == 4) batsman->fours++;
    if (runs == 6) batsman->sixes++;
    batsman->strike_rate = calculate_strike_rate(batsman->runs, batsman->balls_faced);
    
    bowler->runs_conceded += runs;
    bowler->balls_bowled++;
    bowler->overs = bowler->balls_bowled / 6 + (bowler->balls_bowled % 6) / 10.0f;
    bowler->economy = calculate_economy(bowler->runs_conceded, bowler->overs);
    
    teams[currently_batting].total_runs += runs;
    teams[currently_batting].total_balls++;
    teams[currently_batting].current_rr = calculate_economy(teams[currently_batting].total_runs,
                                                           teams[currently_batting].total_balls / 6.0f);
    
    printf(YELLOW "\nBall Result: %d runs" RESET, runs);
    if (is_wicket) {
        printf(RED " - WICKET!" RESET);
        batsman->is_out = 1;
        teams[currently_batting].total_wickets++;
        bowler->wickets++;
        current_batsman++;
    }
    
    printf("\nSCORE: %d/%d (%d.%d overs)\n",
           teams[currently_batting].total_runs,
           teams[currently_batting].total_wickets,
           teams[currently_batting].total_balls / 6,
           teams[currently_batting].total_balls % 6);
    
    if (teams[currently_batting].total_balls >= config.max_overs * 6 ||
        teams[currently_batting].total_wickets >= teams[currently_batting].player_count - 1 ||
        (!first_innings && teams[currently_batting].total_runs > teams[1 - currently_batting].total_runs)) {
        
        if (first_innings) {
            printf(GREEN "\nFIRST INNINGS COMPLETE!\n" RESET);
            printf("TARGET: %d RUNS\n", teams[currently_batting].total_runs + 1);
            
            first_innings = 0;
            currently_batting = 1 - currently_batting;
            current_batsman = 0;
            current_bowler_idx = 0;
        } else {
            match_over = 1;
        }
    }
    
    if (bowler->balls_bowled % 6 == 0 && bowler->balls_bowled > 0) {
        int total_bowlers = 0;
        for (int i = 0; i < teams[bowling_team].player_count; i++) {
            if (teams[bowling_team].players[i].is_bowler) total_bowlers++;
        }
        current_bowler_idx = (current_bowler_idx + 1) % total_bowlers;
    }
}

void display_scoreboard(int clear) {
    if (clear) {
        clear_screen();
    }
    printf(CYAN "╔════════════════ SCOREBOARD ════════════════╗\n" RESET);
    
    for (int t = 0; t < MAX_TEAMS; t++) {
        printf(YELLOW "\n%s: %d/%d (%d.%d overs)\n" RESET,
               teams[t].name,
               teams[t].total_runs,
               teams[t].total_wickets,
               teams[t].total_balls / 6,
               teams[t].total_balls % 6);
        
        if (teams[t].total_balls > 0) {
            printf("CURRENT RR: %.2f\n", teams[t].current_rr);
        }
        
        printf("\nBATSMEN:\n");
        printf("%-15s %s %s %s %s %s\n", "NAME", "RUNS", "BALLS", "4s", "6s", "SR");
        printf("─────────────────────────────────────────────\n");
        
        for (int i = 0; i < teams[t].player_count; i++) {
            Player* p = &teams[t].players[i];
            if (p->balls_faced > 0) {
                printf("%-15s %4d %5d %2d %2d %5.1f%s\n",
                       p->name, p->runs, p->balls_faced, p->fours, p->sixes, p->strike_rate,
                       p->is_out ? " *" : "");
            }
        }
        
        printf("\nBOWLERS:\n");
        printf("%-15s %s %s %s %s\n", "NAME", "OVERS", "RUNS", "WKTS", "ECON");
        printf("─────────────────────────────────────────────\n");
        
        for (int i = 0; i < teams[t].player_count; i++) {
            Player* p = &teams[t].players[i];
            if (p->is_bowler && p->balls_bowled > 0) {
                printf("%-15s %5.1f %4d %4d %5.2f\n",
                       p->name, p->overs, p->runs_conceded, p->wickets, p->economy);
            }
        }
        
        if (t == 0) printf("\n" CYAN "═════════════════════════════════════════════\n" RESET);
    }
    
    printf("\n");
}

void display_match_result(void) {
    clear_screen();

    printf(BLUE "╔════════════════ MATCH RESULT ════════════════╗\n" RESET);

    Team *teamA = &teams[0];
    Team *teamB = &teams[1];

    Team *winner = NULL;
    char margin_type[20];
    int margin = 0;

    if (teamA->total_runs > teamB->total_runs) {
        winner = teamA;
        margin = teamA->total_runs - teamB->total_runs;
        strcpy(margin_type, "RUNS");
    }
    else if (teamB->total_runs > teamA->total_runs) {
        winner = teamB;
        margin = (teamB->player_count - 1) - teamB->total_wickets;
        strcpy(margin_type, "WICKETS");
    }

    if (winner) {
        printf("║   %s WINS THE MATCH!                        ║\n", winner->name);
        printf("║   WON BY %d %s                              ║\n", margin, margin_type);
    } else {
        printf("║                MATCH TIED!                  ║\n");
    }

    printf("╚═══════════════════════════════════════════════╝\n" RESET);

    show_man_of_the_match();
    save_match_history(winner, margin, margin_type);
    display_scoreboard(0);

    printf("\nPRESS ANY KEY TO CONTINUE...");
    getch_silent();
}

void show_man_of_the_match(void) {
    Player *best = NULL;
    Team *best_team = NULL;
    float best_score = -1.0f;

    for (int t = 0; t < MAX_TEAMS; t++) {
        for (int i = 0; i < teams[t].player_count; i++) {
            Player *p = &teams[t].players[i];
            float score = 0.0f;

            if (p->balls_faced > 0)
                score += p->runs + p->strike_rate * 0.2f;

            if (p->is_bowler && p->balls_bowled > 0)
                score += p->wickets * 25.0f - p->economy * 2.0f;

            if (score > best_score) {
                best_score = score;
                best = p;
                best_team = &teams[t];
            }
        }
    }

    printf(MAGENTA "\n╔════════════ MAN OF THE MATCH ═════════════╗\n");

    if (best && best_score > 0) {
        printf("║  %s (%s)\n", best->name, best_team->name);
        printf("║  RUNS: %d | WICKETS: %d\n", best->runs, best->wickets);
    } else {
        printf("║  No standout performance\n");
    }

    printf("╚════════════════════════════════════════════╝\n" RESET);
}

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void safe_input(char* buffer, int size, const char* prompt) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

int safe_int_input(const char* prompt) {
    char buffer[20];
    safe_input(buffer, sizeof(buffer), prompt);
    return atoi(buffer);
}

void simple_hash(const char* input, char* output) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *input++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    snprintf(output, 65, "%lx", hash);
}

float calculate_strike_rate(int runs, int balls) {
    return balls > 0 ? (runs * 100.0f) / balls : 0.0f;
}

float calculate_economy(int runs, float overs) {
    return overs > 0 ? runs / overs : 0.0f;
}

int load_game_data(void) {
    FILE* file = fopen("cricket_users.dat", "rb");
    if (!file) return 0;
    
    int count = 0;
    fread(&count, sizeof(int), 1, file);
    if (count > 0 && count <= MAX_USERS) {
        fread(users, sizeof(User), count, file);
    }
    
    fclose(file);
    return count;
}

void save_game_data(void) {
    FILE* file = fopen("cricket_users.dat", "wb");
    if (!file) {
        printf(RED "Error saving game data!\n" RESET);
        return;
    }
    
    fwrite(&user_count, sizeof(int), 1, file);
    fwrite(users, sizeof(User), user_count, file);
    
    fclose(file);
}

void cleanup_and_exit(void) {
    printf(CYAN "\nTHANKS FOR PLAYING CRICKET SIMULATOR!\n" RESET);
}
void save_match_history(Team *winner, int margin, const char *type) {
    FILE *fp = fopen("history.txt", "a");
    if (!fp) return;

    time_t now = time(NULL);
    fprintf(fp, "Date: %s", ctime(&now));
    fprintf(fp, "%s vs %s\n", teams[0].name, teams[1].name);

    if (winner)
        fprintf(fp, "Winner: %s\nMargin: %d %s\n",
                winner->name, margin, type);
    else
        fprintf(fp, "Result: Match Tied\n");

    fprintf(fp, "----------------------------------\n\n");
    fclose(fp);
}
