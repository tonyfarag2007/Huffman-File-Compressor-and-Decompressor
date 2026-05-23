# Huffman File Compressor/Decompressor

A lossless file compression tool built in C++ that implements Huffman coding to compress and decompress any file type — including binary files like images.

## What is Huffman Compression?

Huffman coding is a lossless data compression algorithm that assigns shorter binary codes to more frequently occurring bytes and longer codes to less frequent ones. Instead of using a fixed 8 bits per byte (as standard), Huffman coding can represent common bytes in as few as 1-2 bits, significantly reducing file size for files with non-uniform byte distributions.

## Features

- **Compress any file type**: works on text files, images (BMP, PNG), and any other binary file
- **Lossless**: decompressed files are bit-for-bit identical to the originals
- **Self-describing compressed format**: the `.huff` file stores everything needed to reconstruct the original, including the original file extension and frequency table
- **Accurate padding handling**: stores total character count in the header to avoid phantom bits corrupting the end of decompressed files
- **Dynamic output naming**: decompressed files are restored to their original filename and extension automatically
- **Minimum file size enforcement**: rejects files under 1000 bytes, since the header overhead would make the compressed file larger than the original
- **Input validation**: checks for valid file paths, correct `.huff` extension on decompression, and invalid menu input

## How It Works

### Compression
1. Reads the input file and counts the frequency of every byte value (0–255)
2. Builds a Huffman tree using a min-heap priority queue — less frequent bytes end up deeper in the tree, more frequent bytes closer to the root
3. Traverses the tree to generate a unique binary code for each byte
4. Writes a binary header containing:
   - Original file extension length and string
   - Number of unique byte values
   - Each unique byte value and its frequency
   - Total byte count of the original file
5. Re-reads the input file and writes each byte as its Huffman code, packing bits into bytes

### Decompression
1. Reads and parses the header to reconstruct the exact same Huffman tree
2. Traverses the tree bit by bit, writing a byte to the output file each time a leaf node is reached
3. Stops after writing exactly as many bytes as the original file contained (eliminating padding artifacts)
4. Restores the output file with the original filename and extension

## File Structure

```
Huffman/
├── main.cpp              # Entry point, user interface, program flow
├── HuffmanTree.cpp/h     # Node class and tree construction
├── FrequencyCodes.cpp/h  # Recursive code generation from tree
├── BitStreamWriter.cpp/h # Header writing and bit-level compression
├── Decompressor.cpp/h    # Header parsing and decompression
└── CMakeLists.txt        # CMake build configuration
```

## Build Instructions

**Requirements:** C++17, CMake 3.20+

```bash
git clone https://github.com/yourusername/Huffman.git
cd Huffman
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

Run the executable and follow the prompts:

```
(C)ompress or (D)ecompress?
```

**Compressing:**
```
Enter the path to the file: C:\Users\user\Downloads\photo.bmp
File Size: 1598714 bytes
Compressed File Size: 1201053 bytes
That is a 24.8% decrease!
```
Output: `photo(compressed).huff` in the same directory as the input file.

**Decompressing:**
```
Enter the path to the compressed .huff file: C:\Users\user\Downloads\photo(compressed).huff
File successfully decompressed!
```
Output: `photo.bmp` restored in the same directory.

## Why It's Useful

- Works on real binary data, not just text
- The compressed format is self-contained and portable, no external configuration needed to decompress due to metadata
- Useful for understanding how real compression tools like ZIP and DEFLATE work under the hood at the entropy coding layer
