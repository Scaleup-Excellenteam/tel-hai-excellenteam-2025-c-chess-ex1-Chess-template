<table>
<tr style="border: none">
<td style="border: none">

# THE Chess Template Repository

</td>
<td align="right" style="border: none">
<img src="./img/scaleup.png" alt="Scaleup" height="100">
</td>
</tr>

</table>
This is a template repository for the Tel Hai Excellenteam (THE) CPP course.

All exercises and submissions should follow the format of this repository. For your convenience, you can start each assignment by cloning this template.

## Environment Setup
During the course we will utilize Linux based operating system (OS), to run and execute programs.

### Windows Installation
1. Install [WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install
) , with Ubuntu distribution.


### Mac Installation
1. Follow the instruction presented in the following [video]( https://www.youtube.com/watch?v=LjL_N0OZxvY
), install Ubuntu (no GUI) version
2. To install GUI, use following [guide](https://askubuntu.com/questions/53822/how-do-you-run-ubuntu-server-with-a-gui
)
3. In case you forgot the default credentials, read following [article](https://www.debugpoint.com/virtualbox-id-password/
).


### Linux Installation
Ensure you have `g++`, `CMake`, and `make` installed:

```sh
sudo apt update
sudo apt install -y g++ cmake make
```

## Grading and Conventions
Your assignment will be graded according to the following criteria. Please make sure your assignment follows the standards and conventions outlined below:


For best practices please use examples presented in [PRACTICES.md](PRACTICES.md), if still there is an uncertainty or missing use cases, you are highly encouraged to contact the staff for further clarifications.

### Branching
Use the [CONTRIBUTING.md](CONTRIBUTING.md) file as your guideline for proper use of Git. For more information, you are encouraged to search online for "[GitHub Workflow Methodology](https://www.youtube.com/watch?v=U_IFGpJDbeU&ab_channel=DevOpsToolkit)."


### Repository Structure
Please follow the guidelines in this section strictly.

- The repository should include a `CMakeLists.txt` file at the root directory.
- The repository **MUST NOT** include any compiled binaries (e.g., build/, .o files, a.out, or any other generated executables).(put those file names in `.gitignore` file)

<p align="center">
  <img src="./img/cpp-logo.png" alt="C++ Logo" width="100" height="100">
  <img src="./img/linux-logo.jpg" alt="Linux Logo" width="100" height="100">
</p>    

#### GitHub Configuration
* The repository should include a `README.md` file at the root directory.
* The repository should include a `CONTRIBUTING.md` file at the root directory.
* You must have a `.gitignore` file, and there shouldn’t be any unnecessary files in the repository.
* 🚨 **CRITICAL:** Repositories without a workflow file at `.github/workflows/c-cpp.yml` will not be graded. :(
* You must have an `img` directory at the root directory.

#### Project's Files

- The repository should include a `main.cpp` file in src directory.
- The repository should include a `src` directory at the root.
  - All source files should be placed in the `src` directory.
- The repository should include a `include` directory at the root.
  - All `.h` files should be placed in the `include` directory.
- The repository should include a `test` directory at the root.
  - All tests should be placed in the `test` directory.


#### Project Tree

Project tree should match the following structure:

```bash
.
├── CONTRIBUTING.md
├── .git
│   ├── ...
├── .github
│   └── workflows
│       └── c-cpp.yml
├── .gitignore
├── img
│   ├── excellenteam.png
│   ├── scaleup.png
│   └── cpp_logo.png
├── README.md
├── CMakeLists.txt
├── src
│   ├── main.cpp
│   ├── example.cpp
│   ├── ...
├── include
│   ├── example.h
│   ├── ...
├── tests
│   ├── ...
```
## Compilation Instructions
The evaluators will compile your code using **CMake**. Ensure your project follows the CMake structure correctly.

To compile your project manually, use the following commands:

```sh
mkdir -p build
cd build
cmake ..
make
./Chess 
```

Any project that does not compile properly using these steps will not be graded.


## How to Submit an Exercise
You are required to submit each exercise using "GitHub Classroom". To do this, you must upload a link to your "GitHub Classroom" repository via Moodle..

### Branching and Pull Request Guidelines

- 🚨 Your `main` branch **must remain clean and stable** at all times. Never push directly to `main`.  
- Create a **new branch** from `main` for each exercise (e.g., `exercise1`, `feature/queen-movement`, etc.).  
- Work on those branches and push your changes there.  
- Once you're done, open a **Pull Request (PR)** from your branch where all the changes where made into `main`.  
- In the PR, review your changes carefully using GitHub’s diff viewer.  
- Make sure all checks pass (e.g., linter, build).  
- After merging the PR into `main`, you can submit the repository link via Moodle.


### Pre-submission Checkup
1. Make sure you've answered all the questions.  
2. Review and refactor your code for better readability (ideally, review your code one or two days later — sometimes it's better to review with fresh eyes).  
3. Ensure that all intended files are uploaded to Git and follow the structure convention outlined in the [Repository Structure](#repository-structure) section.  
4. Ensure that your code is running.  
5. Once you open a PR, review the changes **carefully**. You can leverage GitHub's built-in diff viewer.  
6. Wait and confirm that the linter test completed successfully. If the linter test fails, assess the errors and refactor accordingly — otherwise, each error will negatively impact your grade.  
7. Make sure you followed the [Branching and Pull Request Guidelines](#branching-and-pull-request-guidelines).  
8. Upload the repository link to Moodle.  
9. Good luck :)


# Chess Move Recommendation Algorithm

This project implements an algorithm that recommends the optimal next move for a player based on an analysis of the board states, using three main parameters. The algorithm scans the board up to a user-defined depth (i.e., how many moves ahead to check) and evaluates the quality of each possible move.

## Parameters Evaluated for Each Move

### 1. **Threatening a Weaker Piece**
If the player's move leads to a situation where their piece is threatened by a weaker opponent's piece, this is considered a poor move. The player is risking a more valuable piece for an unprofitable move, so points are subtracted from the score for such a move.

### 2. **Threatening a Stronger Piece**
If the player's move results in threatening an opponent's piece that is stronger than the player's attacking piece, this is considered a beneficial move and adds points to the score. For example, if a bishop threatens a queen, this is valued higher due to the strategic advantage it offers.

### 3. **Capturing an Opponent's Piece**
If an opponent's piece is captured during a move, the algorithm adds the value of the captured piece to the score. More valuable pieces (e.g., queen, rook) are worth more points.

## How the Algorithm Chooses a Move

- **Minimax Algorithm**: The algorithm uses the Minimax method, which simulates every possible option (player's and opponent's moves alternately) up to a given depth.
- **Simulation**: The algorithm simulates the move by moving the pieces on the board and evaluates what will happen as a result of choosing that move.
- **Evaluation**: The state of the board is evaluated based on the parameters mentioned above.
- **Move Selection**: Finally, the move with the highest score is selected, assuming the opponent also plays optimally.

## Value of Pieces

- Pawn: 10 points
- Knight/Bishop: 30 points
- Rook: 50 points
- Queen: 100 points
- King: 1000 points (for evaluation purposes only)

## Time Complexity

The time complexity of the algorithm is **O(b^d)**, where:
- **b** = the average number of possible moves (approximately 30–40 in chess)
- **d** = the search depth (i.e., how many turns ahead the algorithm evaluates)

## Multithreading Optimization
To improve performance and reduce computation time, the algorithm implements multithreading at the top
level of the move search:
Each thread evaluates the possible moves for a single piece independently by simulating those moves and running the Minimax evaluation.
A thread pool manages a fixed number of worker threads to avoid overhead from excessive thread creation.
Each thread works on a local copy of the game board to prevent data races and maintain thread safety.
Results from all threads are collected concurrently, using mutexes to synchronize access to shared data structures.
The main thread waits until all worker threads complete their tasks and then selects the best overall move based on the aggregated results.
This parallelization allows the program to utilize multiple CPU cores effectively, significantly speeding up the computation, especially on machines with many cores.

<!-- Center Excellenteam image -->
<p align="center">
  <img src="./img/excellenteam.png" alt="Excellenteam">
</p>
