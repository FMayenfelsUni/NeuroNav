# NeuroNav Readme

Brain Navigation System Demo for optical tracking of head and tools in 3D space. 

---

## Table of Contents
1. [About the Project](#about-the-project)
2. [Features](#features)
3. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
4. [Usage](#usage)
5. [Contributing](#contributing)
6. [License](#license)
7. [Acknowledgments](#acknowledgments)

---

## About the Project

### Purpose
The Brain Navigation System Demo is designed for optical tracking in 3D space, specifically focusing on the head and tools used in medical applications. The goal is to improve hand-eye coordination and streamline navigation workflows for professionals, such as medical practitioners.

### Overview
This project reverse-engineers existing brain navigation software. It provides functionalities for marking positions, aligning tools to these positions, and storing alignment data. A key innovation behind the project is the concept of integrating a screen directly onto the tool, eliminating the need for users to look away at a separate display and thereby reducing hand-eye coordination challenges. Future iterations may explore augmented reality (AR) to further enhance usability and precision.

### Context
This project serves as a research-oriented endeavor to demonstrate the potential of a Brain Navigation System. It is an academic effort rooted in reverse engineering and innovation, drawing from existing systems to explore new possibilities in optical tracking and navigation.

### Status
The project is currently in a rudimentary development phase. It includes:

- A basic brain navigation visualization with a virtual head object.
- Functionalities for creating, deleting, storing, and loading marked points.
- Basic alignment calculations.
- Displaying a tool's orientation relative to marked points.
- Tracking movement of tools in the 3D virtual environment.
- Interactive UI for managing navigation and alignment tasks.

However, support for camera integration and functional data streaming is not yet implemented and remains a critical next step for future development.

---

## Getting Started

### Prerequisites

Before proceeding, ensure you have the following installed on your system:

- **Qt Framework**: Download and install from Qt's official website.
- **Qt Creator IDE**: Installed alongside the Qt framework.
- **C++ Compiler**: Comes with Qt Creator (e.g., MinGW for Windows or GCC/Clang for Linux/Mac).

### Installation

Clone the repository from GitHub by entering this into your command line console:

```bash
git clone https://github.com/FMayenfelsUni/NeuroNav.git 
```

Alternatively, download the ZIP file from GitHub, then extract it to a directory of your choice.

### Setting Up the Project Structure

To create a new Qt project and include the provided files, follow these steps:

1. **Create a New Project:**
    - Open Qt Creator.
    - Select **File > New File or Project**.
    - Choose **Qt Console Application** or **Empty Qt Project** (depending on your preference).
    - Name the project (e.g., `application`) and choose a folder to save it.

2. **Organize the Files:**
    - Copy the following files into your project directory:

      ```
      application/
      ├── main.cpp                  # Entry point for the application
      ├── Interactive3DWidget.h     # Header file for the 3D widget
      ├── Interactive3DWidget.cpp   # Implementation file for the 3D widget
      ├── assets/
      │   └── head.obj              # 3D model file 
      └── application.pro           # Project file (you'll set this up in step 4)
      ```
    - If the `assets` folder doesn’t exist, create it and place your `.obj` file inside.

3. **Setting Up the `.pro` File**

    The `.pro` file configures the project. If not automatically generated, create a file named `application.pro` in the project directory and add the following content:

    ```
    QT += core gui widgets 3dextras 3dcore 3drender

    CONFIG += c++17

    SOURCES += \
        main.cpp \
        Interactive3DWidget.cpp

    HEADERS += \
        Interactive3DWidget.h

    RESOURCES += \
        assets/head.obj
    ```

    This configuration:

    - Specifies the required Qt modules (`3dextras`, `3dcore`, `3drender`, and `widgets`).
    - Lists all source (`.cpp`) and header (`.h`) files.
    - Ensures compatibility with modern C++ (C++17 or later).

### Open the Project in Qt Creator

1. Launch Qt Creator.
2. Open the `application.pro` file by navigating to the project folder and selecting it.
3. If prompted, choose a build kit:
    - For Windows, use MinGW or the appropriate MSVC kit.
    - For Linux, use GCC.
    - For macOS, use Clang.

### Configure Dependencies

- Ensure your Qt installation includes the necessary modules:
    - `Qt3DExtras`
    - `Qt3DCore`
    - `Qt3DRender`
    - `QtWidgets`
- If these modules are missing, you can install them via the Qt Maintenance Tool.

### Build and Run

1. Click **Build** in the toolbar or press `Ctrl+B` to compile the project.
2. Once the build completes successfully, click **Run** or press `Ctrl+R` to launch the application.

---

## Contributing
TBA

---

## License
TBA

---

## Acknowledgments
Thank you for your patience and an open ear to Max Köhler.
