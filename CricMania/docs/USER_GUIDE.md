# 🏏 Cricket Simulator - User Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [User Authentication](#user-authentication)
3. [Team Management](#team-management)
4. [Match Simulation](#match-simulation)
5. [Understanding Statistics](#understanding-statistics)
6. [Tips & Tricks](#tips--tricks)
7. [Troubleshooting](#troubleshooting)

## Getting Started

### First Launch
When you run the Cricket Simulator for the first time:
1. You'll see a welcome screen
2. Press any key to continue to the main menu
3. Two pre-configured teams are available:
   - **OMOR EKUSHEY HALL**
   - **SHAHID SMRITY HALL**

## User Authentication

### Creating an Account (Signup)

1. From the main menu, select option **2 (SIGNUP)**
2. Enter a unique username
   - Must be different from existing users
   - Maximum 50 characters
3. Create a password
   - Minimum 6 characters required
   - Will be hashed for security
4. Choose your team (1 or 2)
5. Success! You can now login

**Example:**
```
ENTER NEW USERNAME: captain_cool
ENTER NEW PASSWORD: cricket123
CHOOSE TEAM:
1. OMOR EKUSHEY HALL
2. SHAHID SMRITY HALL
ENTER CHOICE (1-2): 1
```

### Logging In

1. Select option **1 (LOGIN)** from main menu
2. Enter your username
3. Enter your password (displayed as asterisks for security)
4. Upon successful login, you'll manage your chosen team

## Team Management

### Adding Players

**Requirements:** You need at least 2 players to start a match

1. Login to your account
2. Select **3. MANAGE TEAM** from main menu
3. Choose **1. ADD PLAYER**
4. Enter player name
5. Player is added to your squad

**Example:**
```
ENTER PLAYER NAME: Shakib Al Hasan
PLAYER ADDED SUCCESSFULLY!
```

### Marking Bowlers

**Requirements:** You need at least 2 bowlers to start a match

1. In team management, select **2. MARK PLAYER AS BOWLER**
2. Enter the exact name of the player
3. Player is now designated as a bowler

**Important:** Players can be both batsmen and bowlers!

### Viewing Squad

- Select **3. VIEW SQUAD** to see:
  - All players in your team
  - Which players are bowlers (indicated by "BOWLER" tag)
  - Player count

### Removing Players

1. Select **4. REMOVE PLAYER**
2. Enter the exact player name
3. Player is removed from the squad

**Warning:** Make sure you maintain minimum requirements (2 players, 2 bowlers)

## Match Simulation

### Prerequisites
Before starting a match:
- ✅ Both teams need at least 2 players
- ✅ Both teams need at least 2 bowlers
- ✅ You must be logged in

### Starting a Match

1. From main menu, select **4. SIMULATE MATCH**
2. A random toss determines which team bats first
3. Match begins!

### Match Options

During the match, you have 4 options:

#### 1. Simulate Next Ball
- Simulates one delivery
- Shows run outcome and wicket information
- Updates scoreboard
- **Use this for:** Ball-by-ball excitement

#### 2. View Current Status
- Displays detailed scoreboard
- Shows both teams' statistics
- Includes batting and bowling figures
- **Use this for:** Checking match progress

#### 3. Auto-Complete Match
- Automatically simulates remaining balls
- Fast-forwards to match result
- **Use this for:** Quick results

#### 4. Back to Main Menu
- Exits match simulation
- Returns to main menu
- **Use this for:** Exiting mid-match

### Understanding Ball Outcomes

Each ball can result in:
- **0 runs** - Dot ball
- **1 run** - Single
- **2 runs** - Double
- **3 runs** - Triple
- **4 runs** - Boundary (FOUR)
- **6 runs** - Maximum (SIX)
- **WICKET** - Batsman dismissed

## Understanding Statistics

### Batting Statistics

| Stat | Description | Formula |
|------|-------------|---------|
| **Runs** | Total runs scored | Sum of all runs |
| **Balls** | Deliveries faced | Count of balls |
| **4s** | Number of boundaries | Count of 4-run shots |
| **6s** | Number of sixes | Count of 6-run shots |
| **SR** | Strike Rate | (Runs × 100) ÷ Balls |
| ***** | Out indicator | Shown if dismissed |

### Bowling Statistics

| Stat | Description | Formula |
|------|-------------|---------|
| **Overs** | Overs bowled | Balls ÷ 6 (e.g., 2.3 = 2 overs, 3 balls) |
| **Runs** | Runs conceded | Total runs given |
| **Wkts** | Wickets taken | Number of dismissals |
| **Econ** | Economy Rate | Runs ÷ Overs |

### Team Statistics

- **Total Runs/Wickets** - Overall score (e.g., 45/3)
- **Overs** - Total overs bowled (e.g., 2.4 means 2 overs + 4 balls)
- **Current RR** - Current run rate per over

### Man of the Match

Calculated based on:
- Batting performance (runs + strike rate)
- Bowling performance (wickets × 25 - economy × 2)
- Best combined performance wins

## Tips & Tricks

### Building a Balanced Team

1. **Mix of Batsmen and Bowlers**
   - Add 6-7 players total
   - Mark 3-4 as bowlers
   - This gives good rotation options

2. **Strategic Bowler Selection**
   - Mark your best players as bowlers
   - They can still bat!
   - Bowlers rotate every over (6 balls)

### Match Strategy

1. **First Innings**
   - Focus on building a good total
   - Watch the run rate
   - Protect wickets early

2. **Second Innings**
   - Note the target score
   - Calculate required run rate
   - Balance aggression with caution

### Maximizing Performance

- **Higher strike rates** = More aggressive batting
- **Lower economy rates** = Better bowling
- **Wickets** are more valuable than runs for bowlers

## Troubleshooting

### "BOTH TEAMS NEED AT LEAST 2 PLAYERS AND 2 BOWLERS"

**Solution:**
1. Exit match simulation
2. Go to team management
3. Add more players (if needed)
4. Mark additional players as bowlers

### "PLAYER NOT FOUND"

**Reason:** Player name doesn't match exactly

**Solution:**
- Check spelling carefully
- Names are case-sensitive
- Use exact name from squad view

### Login Failed

**Common Issues:**
- Wrong username or password
- Account doesn't exist (signup first)
- Password is case-sensitive

### Colors Not Showing

**Reason:** Terminal doesn't support ANSI colors

**Solution:**
- Use a modern terminal (Windows Terminal, iTerm2, etc.)
- On Windows: Enable virtual terminal processing
- Colors enhance experience but aren't required

### Match History Not Saving

**Check:**
- Write permissions in program directory
- Disk space available
- File `history.txt` is not read-only

## Advanced Features

### Game Configuration

Default settings (in code):
```c
max_overs = 3          // Overs per innings
wicket_probability = 0.15  // 15% chance per ball
```

To modify, edit the source code and recompile.

### Data Files

- **cricket_users.dat** - Stores user accounts (binary)
- **history.txt** - Stores match results (text)

**Note:** Don't manually edit binary files!

## Keyboard Shortcuts

- **Any Key** - Continue after messages
- **Enter** - Confirm input
- **Backspace** - Delete character (password entry)

## Best Practices

1. ✅ Always save before exiting (option 6)
2. ✅ Add descriptive player names
3. ✅ Keep at least 2 backups of data files
4. ✅ Review squad before starting match
5. ✅ Check match history for past results

## Need Help?

- Check this guide first
- Review error messages carefully
- Refer to README.md for technical details
- Submit issues on GitHub

---

**Happy Gaming! 🏏**
