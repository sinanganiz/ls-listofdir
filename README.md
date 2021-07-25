# ls-listofdir

This project for listing files in directory. Like **ls** command in Linux. While listing, it makes coloring according to 5 different types.
- Read Only Files
- Directories
- Regular File
- Link
- Hidden File

While listing the files, the type of the file, the name of the file, the size of the file and the absolute path of the file are listed. In addition, at the end, the information of how many files of which type are in the specified path is summarized.

### Algorithm
1. Browse all files and folders in the entered file path.
2. Keep the file information (file name, file type, file path, size) in the array.
3. Sort the array according to the entered sorting input.
4. Color and print all sorted files and folders.


### Compiling
<pre>
gcc listofdir.c jval.o jrb.o -o listofdir
</pre>

### Execute
Example:
<pre>
- ./listofdir . 0
- ./listofdir /home 1
- ./listofdir /etc 2
- ./listofdir ../ 3
</pre>

This project takes 2 input.

First Input:  Path

Second Input: Sorting Method (0, 1, 2, 3)
  - 0: File size A-Z
  - 1: File size Z-A
  - 2: File name A-Z
  - 3: File name Z-A
