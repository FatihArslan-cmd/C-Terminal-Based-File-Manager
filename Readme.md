# C Terminal-Based File Manager

A powerful and user-friendly terminal-based file manager written in C, providing essential file and directory management operations with a clean interface.

## Features

- **Directory Navigation**
  - `spwd`: Display current working directory
  - `slist [directory]`: List contents of a directory with detailed information (type, size, permissions)

- **File Operations**
  - `screatefile <filename>`: Create a new file
  - `sremovefile <filename>`: Delete a file
  - `scopy <source> <destination>`: Copy files
  - `smove <source> <destination>`: Move/rename files
  - `sdisplay <filename>`: Display file contents

- **Directory Operations**
  - `screatedir <dirname>`: Create a new directory
  - `sremovefolder <dirname>`: Remove a directory and its contents (with confirmation)
  - `ssearch <directory> <pattern>`: Search for files in a directory

- **Permission Management**
  - `spermission <file/directory> <mode>`: Change file/directory permissions (e.g., 755, 644)

- **User Interface**
  - `sclear`: Clear the screen and show the menu
  - Colorful and intuitive command-line interface
  - Detailed operation logging

## Installation

### Prerequisites
- GCC compiler
- Make build system
- Unix-like operating system (Linux/macOS)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/FatihArslan-cmd/C-Terminal-Based-File-Manager.git
cd C-Terminal-Based-File-Manager
```

2. Create the bin directory:
```bash
mkdir -p bin
```

3. Build the project:
```bash
make
```

4. Run the file manager:
```bash
./bin/file_manager
```

## Project Structure

```
.
├── include/                 # Header files
│   ├── file_manager.h      # Main header file
│   └── file_manager_logo.h # Logo and UI related declarations
├── src/                    # Source files
│   ├── main.c             # Main program entry
│   ├── file_operations.c  # File manipulation functions
│   ├── directory_ops.c    # Directory manipulation functions
│   ├── permissions.c      # Permission management
│   └── logger.c           # Logging functionality
├── utils/                  # Utility functions
│   └── logo.c             # ASCII art logo and menu
├── Makefile               # Build configuration
└── README.md              # Project documentation
```

## Usage Examples

1. List directory contents:
```bash
> slist .
```

2. Copy a file:
```bash
> scopy source.txt destination.txt
```

3. Change file permissions:
```bash
> spermission myfile.txt 644
```

4. Search for files:
```bash
> ssearch /path/to/dir searchterm
```

## Logging

All operations are automatically logged to `app_log.txt` with timestamps, making it easy to track file system changes and debug issues.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is open source and available under the MIT License.

## Acknowledgments

- Thanks to all contributors who have helped with bug fixes and improvements
- Special thanks to the C programming community for their valuable resources

## Contact

Project Link: [https://github.com/FatihArslan-cmd/C-Terminal-Based-File-Manager](https://github.com/FatihArslan-cmd/C-Terminal-Based-File-Manager)
