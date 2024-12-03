# C Terminal-Based File Manager

A robust and secure terminal-based file manager written in C, providing essential file and directory management operations with comprehensive error handling and user-friendly interface.

## Features

### Core Functionality
- **Directory Navigation**
  - `spwd`: Display current working directory
  - `slist [directory]`: List contents of a directory with detailed information
    - Shows file type, size, and permissions
    - Displays total size and item count
    - Supports both files and directories

- **File Operations**
  - `screatefile <filename>`: Create a new file with proper permissions
  - `sremovefile <filename>`: Safely delete a file with confirmation
  - `scopy <source> <destination>`: Copy files with progress indication
  - `smove <source> <destination>`: Move/rename files with cross-filesystem support
  - `sdisplay <filename>`: Display file contents with large file handling

- **Directory Operations**
  - `screatedir <dirname>`: Create a new directory with secure permissions
  - `sremovefolder <dirname>`: Safely remove directory and contents with confirmation
  - `ssearch <directory> <pattern>`: Search for files with detailed results

- **Permission Management**
  - `spermission <file/directory> <mode>`: Change permissions with validation
  - Displays both old and new permissions in human-readable format
  - Supports standard Unix permission modes (e.g., 755, 644)

### Security Features
- Path traversal protection
- Buffer overflow prevention
- Input validation and sanitization
- Safe memory management
- Proper error handling and reporting
- Secure file operations with confirmations
- Permission validation

### User Experience
- Colorful and intuitive interface
- Progress indicators for long operations
- Detailed error messages and confirmations
- Operation logging for auditing
- Large file handling with warnings
- Cross-platform compatibility (Unix/Linux/macOS)

## Installation

### Prerequisites
- GCC compiler
- Make build system
- Unix-like operating system (Linux/macOS)
- Minimum 1GB free disk space recommended

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
│   └── file_manager_logo.h # Logo and UI declarations
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

1. List directory contents with details:
```bash
> slist .
Contents of directory: .
---------------------------------------------------------------
Name                           Type         Size         Permissions
---------------------------------------------------------------
documents                      Directory    4096         rwxr-xr-x
example.txt                    File         1024         rw-r--r--
---------------------------------------------------------------
Total: 2 items, 5120 bytes (0.01 MB)
```

2. Copy a file with progress:
```bash
> scopy source.txt destination.txt
File 'destination.txt' already exists. Overwrite? (y/n): y
Copying: 75.5% complete
File copied successfully.
```

3. Change file permissions with details:
```bash
> spermission myfile.txt 644
Permissions changed for 'myfile.txt':
  Old permissions: rwxr-xr-x (755)
  New permissions: rw-r--r-- (644)
```

4. Search for files with detailed information:
```bash
> ssearch /path/to/dir searchterm
Searching for 'searchterm' in directory: /path/to/dir
---------------------------------------------------------------
Found: example.txt
  Type: File
  Size: 1024 bytes
  Permissions: rw-r--r--
---------------------------------------------------------------
```

## Safety Features

1. **Input Validation**
   - Path traversal prevention
   - Buffer overflow protection
   - Permission mode validation
   - File size checks

2. **Operation Safety**
   - Confirmation for destructive operations
   - Progress indication for long operations
   - Existing file checks
   - Cross-filesystem operation handling

3. **Error Handling**
   - Detailed error messages
   - Operation logging
   - Memory management
   - Resource cleanup

## Logging

All operations are automatically logged to `app_log.txt` with:
- Timestamp
- Operation type
- Details
- Success/Failure status
- Error messages when applicable

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Contribution Guidelines
- Follow the existing code style
- Add appropriate error handling
- Include input validation
- Update documentation
- Add tests if applicable

## License

This project is open source and available under the MIT License.

## Acknowledgments

- Thanks to all contributors who have helped with bug fixes and improvements
- Special thanks to the C programming community for their valuable resources

## Contact

Project Link: [https://github.com/FatihArslan-cmd/C-Terminal-Based-File-Manager](https://github.com/FatihArslan-cmd/C-Terminal-Based-File-Manager)
