#!/bin/bash

# 1. Create a text file with your name - 2 different ways
echo "Raneen" > ~/name.txt
cat <<< "Raneen" > ~/name.txt

# 2. Create a new directory named 'new'
mkdir -p ~/new

# 3. Copy the text file into 'new' directory
cp ~/name.txt ~/new/

# 4. Rename the copied file
mv ~/new/name.txt ~/new/Raneen.txt

# 5. Edit the file with your name and favorite animal using gedit
# This step opens gedit
gedit ~/new/Raneen.txt

# 6. Create a new file in 'new' directory named newFile with same content
cp ~/new/Raneen.txt ~/new/newFile

# 7. List the files in 'new' directory
cd ~/new
ls .

# 8. Return to the home directory 
cd 
cd ..

# 9. Remove the 'new' directory and its contents
rm -r ~/new
