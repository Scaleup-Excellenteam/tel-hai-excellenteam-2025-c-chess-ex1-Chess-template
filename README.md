# Chess Engine - C++ Project
This is a simple chess engine implemented in C++. It features a command-line interface (CLI)
for playing chess against a basic AI using the minimax algorithm with alpha-beta pruning. 


## 📂 Project Structure

```text
.
├── CMakeLists.txt
├── CONTRIBUTING.md
├── Chess/
│   ├── include/                # Public headers
│   │   ├── AI/                 # Minimax & alpha-beta interfaces
│   │   ├── Board.h             # Board representation
│   │   ├── Chess.h             # High-level engine API
│   │   ├── GameEngine.h        # Game loop & rule enforcement
│   │   ├── Pieces/             # King, Queen, Rook, Bishop, Knight, Pawn
│   │   └── Utils/              # ThreadPool, move structs, ANSI colors
│   ├── src/                    # Implementation files
│   │   ├── AI/                 # Minimax & alpha-beta implementation
│   │   ├── Board.cpp
│   │   ├── Chess.cpp
│   │   ├── GameEngine.cpp
│   │   ├── Pieces/             # .cpp files for each piece
│   │   ├── Utils/              # ThreadPool.cpp, color definitions, helpers
│   │   └── main.cpp            # CLI entry point
│   └── test/                   # Unit tests with Google Test
│       └── GameTests.cpp
├── PRACTICES.md                # Coding guidelines & best practices
├── README.md                   # (You are here!)
├── build/                      # Local CMake build artifacts
├── img/                        # Project logos & screenshots
└── run.sh                      # One-liner to build & launch
```

## 🎯 Key Features

- **AI Opponent**  
  - Minimax search with α-β pruning  
  - Configurable depth (1–5)  
  - Multithreaded move evaluation  

- **Complete Chess Rules**  
  - Castling (king- and queen-side)  
  - En passant  
  - Pawn promotion (Queen, Rook, Bishop, Knight)  
  - Check, checkmate & stalemate  

- **Colorful CLI**  
  - ANSI-colored board & Unicode pieces  
  - Styles defined in `Utils/colors.h`  
  - Grandma-approved aesthetics  

- **Three Game Modes**  
  1. Interactive (Player vs. Player)  
  2. Classic AI (Player vs. Computer)  
  3. Benchmark (Computer vs. Computer speed test)  

- **Smart Hints**  
  - AI suggests its top move each turn  
  - Perfect for polishing your opening repertoire  

---

## 🚀 Quick Start

```bash
git clone https://github.com/Scaleup-Excellenteam/chess-ex1-EB-WildEye.git
cd chess-ex1-EB-WildEye
chmod +x run.sh
./run.sh
```

Or build manually:

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
cd ..
./run.sh
```

### Getting Started

On launch, you’ll be greeted with:

```text
╔═══════════════════════════════════════╗
║          ♔   CHESS GAME ♚             ║
║      Classic Strategy Game v1.0       ║
╚═══════════════════════════════════════╝

◆ Enter AI search depth (1–5, recommended: 3): 5

◆ Select game mode:
  1. Interactive (Player vs. Player)
  2. Classic AI (Player vs. Computer)
  3. Benchmark (Computer vs. Computer)
◆ Enter mode (1, 2 or 3):
- **Interactive**: Two humans, one board—let the friendly duel begin.  
- **Classic AI**: Take on our minimax champion—no mercy shown.  
- **Benchmark**: Popcorn ready? Watch two AIs race through a speedmatch.  

Each move gets a thumbs-up and a tip:

```text
▶ Move accepted
💡 Pro Tip: G2→F2 (+1)
♙ White’s turn ➤
```

---

🛠️ Under the Hood

- **ThreadPool**: Powers concurrent α-β pruning for lightning-fast AI reflexes.  
- **Modular Core**: Swap in new rules, UIs or custom pieces without breaking a sweat.  
- **Test Suite** (`Chess/test/GameTests.cpp`): Guards against illegal moves, stalemate puzzles and trap-filled endgames.  
- **ANSI Styling**: Colors & Unicode icons defined in `Utils/colors.h` to make your board pop.  

Outsmart your opponents—or your own code—just watch out for sneaky forks!  
