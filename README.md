<p align="center">
  <h1>MathematicallyCorrectEmpire</h1>
  <p align="center">Unlocking peak resource efficiency in Demacia Rising through rigorous computational optimization.</p>
  <p align="center">
    <a href="https://github.com/your-username/MathematicallyCorrectEmpire/actions/workflows/ci.yml">
      <img src="https://img.shields.io/badge/build-passing-brightgreen?style=flat-square" alt="Build Status">
    </a>
    <a href="https://github.com/your-username/MathematicallyCorrectEmpire/pulls">
      <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square" alt="PRs Welcome">
    </a>
</p>

---

## Overview

> The inherent complexity of resource management and build order optimization in like Demacia Rising often leads to suboptimal strategies, wasted resources, and slower progress. Players struggle to identify the truly "best" path amidst numerous variables and dependencies, relying on intuition or trial-and-error, which can significantly hinder their in-game potential and competitiveness.

**MathematicallyCorrectEmpire** provides a data-driven, exhaustive, and greedy search framework to systematically explore and identify the highest-yielding build orders and resource allocation strategies. It uses brute-force and greedy optimization to find the best distribution of combat structures (such as Quartermasters and Aviary) and econ structures (such as Marketplaces and Academies).
---

## Key Features

*   💡 **Optimal Build Order Generation**: Automatically discovers the most efficient sequence of actions to maximize resource output over time.
*   🔍 **Exhaustive Search Capabilities**: Explores a vast decision space to guarantee globally optimal solutions for defined parameters, ensuring no superior strategy is overlooked.
*   🚀 **Greedy Optimization Engine**: Accelerates solution finding for larger, more complex problem sets by intelligently prioritizing high-impact decisions, balancing speed with optimality.
*   📊 **Data-Driven Insights**: Generates comprehensive output files (`.csv`, `.xlsx`) for in-depth analysis, allowing users to visualize and understand the mechanics behind optimal strategies.
*   🛡️ **Strategic Advantage**: Empowers players to dominate Demacia Rising by implementing computationally verified, superior resource management and build orders.
*   ⚙️ **High-Performance C++ Core**: Engineered in C++ for unparalleled speed and efficiency in executing complex computational tasks, ensuring rapid analysis.

---

## Technical Architecture

### Tech Stack

| Technology         | Purpose                                         | Key Benefit                                  |
| :----------------- | :---------------------------------------------- | :------------------------------------------- |
| **C++**            | Core Logic & Algorithm Implementation           | High Performance, Low-Level Control, Speed   |
| **Exhaustive Search** | Optimal Solution Guarantee                      | Finds Global Maxima for Defined Constraints  |
| **Greedy Optimization** | Heuristic Search Acceleration                   | Efficient for Large Problem Spaces, Scalability |
| **CSV/XLSX Output** | Data Export & Analysis                          | Interoperability, Visual Insights, Reporting |
| **Visual Studio Solution** | Project Management, Build System (Windows)      | Streamlined Development Workflow, Debugging  |

### Directory Structure

```
MathematicallyCorrectEmpire/
├── 📁 .git/
├── 📄 .gitattributes
├── 📄 .gitignore
├── 📄 CombatFirst.cpp
├── 📄 CombatFirst.h
├── 📄 Construct.cpp
├── 📄 Construct.h
├── 📄 EconFirst.cpp
├── 📄 EconFirst.h
├── 📄 MCE_QM_AV_BR_first.csv
├── 📄 MCE_QM_AV_BR_second.csv
├── 📄 MathematicallyCorrectEmpire.cpp
├── 📄 MathematicallyCorrectEmpire.sln
├── 📄 MathematicallyCorrectEmpire.vcxproj
├── 📄 MathematicallyCorrectEmpire.vcxproj.filters
├── 📄 PtrStruct.h
├── 📄 README.md
├── 📄 Settlement.cpp
├── 📄 Settlement.h
├── 📄 le_test.txt
├── 📄 mathematicaly_correct_empire.xlsx
├── 📄 optimal.xlsx
└── 📄 output.csv
```

---

## Operational Setup

### Prerequisites

Before you begin, ensure you have the following installed:

*   **Git**: For cloning the repository.
*   **C++ Compiler**:
    *   **Windows**: Visual Studio (with C++ Desktop Development workload) is recommended due to the `.sln` and `.vcxproj` files.
    *   **Linux/macOS**: GCC or Clang (C++11 or newer compatible).
*   **Build System**:
    *   **Windows**: MSBuild (comes with Visual Studio).
    *   **Linux/macOS**: `make` (if a `Makefile` is created, otherwise direct compilation with `g++` or `clang++`).

### Installation

Follow these steps to get your development environment set up:

1.  **Clone the Repository**:
    ```bash
    git clone https://github.com/your-username/MathematicallyCorrectEmpire.git
    cd MathematicallyCorrectEmpire
    ```

2.  **Build the Project**:

    **Option A: Using Visual Studio (Recommended for Windows users)**
    *   Open the `MathematicallyCorrectEmpire.sln` solution file in Visual Studio.
    *   Select `Build` -> `Build Solution` from the menu.
    *   The executable will typically be found in the `x64/Release` or `x64/Debug` directory within the project folder.

    **Option B: Using MSBuild from the Developer Command Prompt for Visual Studio**
    *   Ensure you have the Visual Studio Build Tools installed.
    *   Open the "Developer Command Prompt for VS" (e.g., "x64 Native Tools Command Prompt for VS 2022") which sets up the necessary environment variables.
    *   Navigate to the `MathematicallyCorrectEmpire` directory.
    *   Execute the build command:
        ```bash
        msbuild MathematicallyCorrectEmpire.vcxproj /p:Configuration=Release /p:Platform=x64
        ```
    *   The executable will be located in the `x64\Release` directory.

    **Option C: Generic C++ Compilation (For Linux/macOS or advanced users)**
    *   This is a simplified example and might require adjustments based on specific dependencies or configurations.
    *   Ensure all `.cpp` files are included in the compilation command.
        ```bash
        g++ -std=c++17 -O3 -o MathematicallyCorrectEmpire MathematicallyCorrectEmpire.cpp CombatFirst.cpp Construct.cpp EconFirst.cpp Settlement.cpp -I.
        ```
    *   The executable will be created in the current directory.
