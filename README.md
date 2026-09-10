# MP3 Tag Reader using C

A command-line based MP3 Tag Reader and Editor developed in C to read and modify metadata stored in MP3 audio files using the ID3v2.3 tagging format.

## Project Overview

The MP3 Tag Reader allows users to view important metadata from an MP3 file and edit selected tag information directly from the terminal.

The project demonstrates practical usage of Advanced C Programming concepts including file handling, structures, pointers, command-line arguments, dynamic memory allocation, string manipulation, and modular programming.

## Features

### View MP3 Tags

Displays commonly used ID3 metadata such as:

* Title
* Artist
* Album
* Year
* Genre
* Comment

### Edit MP3 Tags

Allows modification of selected MP3 tag information through command-line arguments.

Supported options include:

* Title
* Artist
* Album
* Year
* Genre
* Comment

### Command-Line Interface

The application uses command-line arguments to select the required operation.

View tags:

```bash
./a.out -v song.mp3
```

Edit a tag:

```bash
./a.out -e song.mp3 -t "New Title"
```

## Technologies Used

* C Programming
* GCC Compiler
* File Handling
* Structures
* Pointers
* Dynamic Memory Allocation
* String Handling
* Command-Line Arguments
* ID3v2.3 Metadata Format

## ID3v2.3 Structure

The program works with the ID3v2.3 metadata structure stored inside an MP3 file.

The general structure handled by the project is:

```text
MP3 File
   |
   └── ID3 Header
         |
         ├── Title
         ├── Artist
         ├── Album
         ├── Year
         ├── Genre
         └── Comment
```

The application reads the tag header and frame information, extracts the required metadata, and displays it in a user-friendly format.

## Project Structure

```text
MP3-Tag-Reader/
│
├── main.c
├── view.c
├── view.h
├── edit.c
├── edit.h
├── types.h
├── common.h
├── Makefile
└── README.md
```

## Sample Output

### View Operation

```text
$ ./a.out -v song.mp3

---------------- MP3 TAG READER ----------------

MP3 Version : V2.3.0
Tag Size    : 264601

Title       : Sunny Sunny
Artist      : Yo Yo Honey Singh
Album       : Yaariyan
Year        : 2013
Genre       : Bollywood Music
Comment     : eng

--------------------------------------------------
```

### Edit Operation

```text
$ ./a.out -e song.mp3 -t "New Song Title"

Title updated successfully.
```

## Concepts Demonstrated

This project provided practical experience with:

* Reading and writing binary file data
* Navigating file offsets using `fseek()` and `ftell()`
* Reading data using `fread()`
* Writing data using `fwrite()`
* Working with structures
* Pointer manipulation
* Dynamic memory allocation
* String processing
* Command-line argument validation
* Modular C programming
* Error handling
* MP3 ID3 metadata processing

## How to Compile

Using GCC:

```bash
gcc *.c
```

Run the application:

```bash
./a.out
```

Or, if a Makefile is provided:

```bash
make
```

## How to Use

### View Metadata

```bash
./a.out -v filename.mp3
```

### Edit Metadata

```bash
./a.out -e filename.mp3 -t "New Title"
```

The command-line arguments determine the operation and the tag that needs to be modified.

## Learning Outcome

Through this project, I gained hands-on experience in handling structured data inside real-world file formats and strengthened my understanding of Advanced C Programming concepts.

The project helped me understand how file formats can be analyzed at the byte level and how C can be used to build practical command-line utilities.

## Training

Developed as part of **Advanced C Programming Training at Emertxe Information Technologies**, with guidance from experienced mentors.

## Author

**Harsha G**

GitHub: `theharshaG`

## License

This project is intended for educational and learning purposes.
