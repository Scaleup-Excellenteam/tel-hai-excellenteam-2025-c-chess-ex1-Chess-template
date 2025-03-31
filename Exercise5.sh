#!/bin/bash

mkdir subs 
cd subs

# 1. Create files FileA.txt, FileB.txt, ..., FileF.txt inside subs
touch File{A..F}.txt

# 2. Delete File{C-F}.txt
rm File{C..F}.txt

# 3. Rename FileA.txt to FileA.old
mv FileA.txt FileA.old

# 4. Delete all files in directory
rm *

# 5. opy all files from /etc ending with .conf to subs, ignoring permission errors
cp /etc/*.conf ./ 2>/dev/null 

# 6. Show the content of the files that start with 'l'
cat l*

# 7. Find files where the filename (not including extension) is exactly 4 letters.
grep user ????* 2>/dev/null

# 8a. How to identify the last modified file in a directory
modified=$(ls -t | head -n 1)

# 8b. Print the last modified file
echo "The last modified file is $modified"

# 9. Create a folder for each group of etc, named after the group
cut -d: -f1 /etc/group | xargs -I{} mkdir {}

# 10. Print the string
echo "*-: (-; {}-:"