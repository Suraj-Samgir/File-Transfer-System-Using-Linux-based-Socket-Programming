# Socket Programming Project

This project demonstrates the concept of socket programming in a Linux operating system. It includes two folders: `server` and `client`. The client can interact with the server to perform various file operations such as uploading, downloading, deleting, and viewing different types of files including text files, audio files, video files, and image files.

## Features

- **Client Operations:**
  - **Upload Files:** Upload various types of files (text, audio, video, image) to the server.
  - **Download Files:** Download files from the server to the client.
  - **Delete Files:** Delete files stored on the server.
  - **View Files:** View a list of available files on the server.

## Technology

- **Programming Language:** C/C++ 
- **Operating System:** Linux

## Project Structure

- **server/**: Contains the server-side code to handle client requests and manage files.
- **client/**: Contains the client-side code to interact with the server and perform file operations.

## Requirements

- Linux operating system
- GCC compiler (if using C/C++)
- Make (for building the project, if applicable)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/socket-programming-project.git
   cd socket-programming-project

2. Navigate to the `server` directory and compile the server code:<br>
cd server
make  # or use gcc server.c -o server

3. Navigate to the `client` directory and compile the client code:<br>
cd ../client
make  # or use gcc client.c -o client

## Usage 

### Running the Server :

1. Start the Server <br>
./server

### Running the Client :

1. Start the Client <br>
./client

2. Use the provided options to upload, download, delete, or view files.

## Contributing 

Contributions are welcome! Please open an issue or submit a pull request for any changes or enhancements.
