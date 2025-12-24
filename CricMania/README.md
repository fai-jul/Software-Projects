# 🏏 Cricket Simulator - CRICMANIA

A feature-rich, terminal-based cricket match simulator built in C with user authentication, team management, and real-time match simulation capabilities.

## ✨ Features

- **User Authentication System** - Secure login/signup with password hashing
- **Team Management** - Add/remove players, assign bowlers, view squad details
- **Real-time Match Simulation** - Ball-by-ball commentary with realistic scoring
- **Comprehensive Statistics** - Detailed batting and bowling scorecards with strike rates, economy rates
- **Persistent Data Storage** - Save/load game progress and match history
- **Man of the Match** - Automatic performance-based award system
- **Cross-platform Support** - Works on both Windows and Linux/Unix systems

## 🛠️ Technologies Used

- **Language**: C (C99 standard)
- **Platform**: Cross-platform (Windows/Linux/Unix)
- **Storage**: Binary file I/O for user data, text files for match history
- **Libraries**: 
  - Standard C libraries (stdio, stdlib, string, time)
  - Platform-specific: Windows.h (Windows), termios.h (Unix)

## 📋 Prerequisites

- GCC compiler (MinGW for Windows, GCC for Linux)
- Terminal with ANSI color support (recommended)
- Minimum 1MB disk space for data storage

## 🚀 Installation & Setup

### Linux/Unix

```bash
# Clone the repository
git clone https://github.com/yourusername/cricket-simulator.git
cd cricket-simulator

# Compile the program
gcc cricket_simulator.c -o cricket_simulator

# Run the game
./cricket_simulator
```

### Windows

```cmd
# Clone the repository
git clone https://github.com/yourusername/cricket-simulator.git
cd cricket-simulator

# Compile with MinGW
gcc cricket_simulator.c -o cricket_simulator.exe

# Run the game
cricket_simulator.exe
```

## 🎮 Usage

### Getting Started

1. **First Time Setup**
   ```
   Run the program → Select "SIGNUP" → Create username and password → Choose your team
   ```

2. **Login**
   ```
   Enter your credentials → Access team management and matches
   ```

3. **Team Management**
   - Add at least 2 players to each team
   - Mark at least 2 players as bowlers (required for match simulation)
   - View squad details and remove players if needed

4. **Simulate Match**
   - Both teams need minimum 2 players and 2 bowlers
   - Choose ball-by-ball simulation or auto-complete
   - View live scoreboard and statistics

### Code Example

```c
// Core game initialization
initialize_game();

// User authentication
if (login_system()) {
    manage_team(&teams[logged_in_team_index]);
}

// Match simulation
simulate_match();
```

## 📊 Project Architecture

```
Cricket Simulator
│
├── Authentication Module
│   ├── Password hashing (simple_hash)
│   ├── User signup/login
│   └── Session management
│
├── Team Management Module
│   ├── Player CRUD operations
│   ├── Bowler assignment
│   └── Squad visualization
│
├── Match Engine
│   ├── Toss & innings management
│   ├── Ball-by-ball simulation
│   ├── Run scoring algorithm
│   └── Wicket probability system
│
├── Statistics Module
│   ├── Strike rate calculation
│   ├── Economy rate calculation
│   ├── Man of the Match algorithm
│   └── Scoreboard display
│
└── Data Persistence
    ├── Binary user data (cricket_users.dat)
    └── Match history (history.txt)
```

## 🎯 Game Configuration

Default settings (modifiable in code):
- **Overs per innings**: 3 overs
- **Wicket probability**: 15%
- **Maximum players per team**: 11
- **Maximum users**: 100

## 📈 Performance & Results

- **Memory footprint**: ~50KB runtime
- **File I/O**: Efficient binary serialization for user data
- **Response time**: Instant ball simulation (<1ms)
- **Scalability**: Supports up to 100 user accounts

## 🖼️ Screenshots

> **Note**: Please add screenshots to the `images/` folder:
> - `images/welcome_screen.png` - Welcome screen
> - `images/main_menu.png` - Main menu interface
> - `images/team_management.png` - Team management screen
> - `images/add_player.png` - Adding player & bowler
> - `images/view_squad.png` - Adding player & bowler
> - `images/match_simulation.png` - Live match simulation
> - `images/scoreboard.png` - Detailed scoreboard
> - `images/match_result & MOTM.png` - Match result display

## 📁 File Structure

```
cricket-simulator/
├── README.md
├── cricket_simulator.c          # Main source code
├── cricket_users.dat            # User data (auto-generated)
├── history.txt                  # Match history (auto-generated)
├── .gitignore
├── LICENSE
├── images/
│   └── (screenshots here)
└── docs/
    ├── USER_GUIDE.md
    └── DEVELOPER_NOTES.md
```

## 🔮 Future Improvements

- [ ] Implement DRS (Decision Review System)
- [ ] Add powerplay overs with modified scoring
- [ ] Player form and fatigue system
- [ ] Tournament mode with multiple teams
- [ ] Enhanced AI for strategic bowling changes
- [ ] Graphical user interface (GUI) version
- [ ] Network multiplayer support
- [ ] Advanced statistics and data visualization
- [ ] Database integration (SQLite)
- [ ] Web-based frontend

## 🐛 Known Issues

- Colors may not display on terminals without ANSI support
- Password stored with simple hash (not cryptographically secure)
- Limited to 2 teams in current version

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Your Name**
- GitHub: [@fai-jul](https://github.com/fai-jul)
- Email: faijulislamimp@gmail.com
- LinkedIn: 

## 🙏 Acknowledgments

- Inspired by classic cricket games
- Built as a learning project for C programming
- Thanks to the open-source community

---

⭐ **Star this repository if you found it helpful!**

🏏 **Made with passion for cricket and coding**
