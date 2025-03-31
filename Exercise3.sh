#!/bin/bash

# Set the base path to the directory containing alice.txt
ALICE_FILE="../alice.txt"

# 1. Write who -a output into who_is_logged file
who -a > who_is_logged

# 2. Create a file named fact with the string "The answer is 42" without using a text editor
echo "The answer is 42" > fact

# 3. Append the content of who_is_logged into fact
cat who_is_logged >> fact

# 4. Show only lines that contain the word Alice
grep "Alice" "$ALICE_FILE"

# 5. Count how many lines contain the word "Why"
grep -c "Why" "$ALICE_FILE"

# 6. Create chapters.txt that contains only lines starting with "CHAPTER"
grep '^CHAPTER' "$ALICE_FILE" > chapters.txt

# 7. Show all lines that contain the word 'fear' but replacing 'e' with 'o'
grep 'fear' "$ALICE_FILE" | sed 's/e/o/g'

# 8. Create numbered_alice.txt with lines containing "Alice", numbered
grep "Alice" "$ALICE_FILE" | cat -n > numbered_alice.txt

# 9. Show lines that do not contain the words "fear" or "rabbit"
grep -v -e "fear" -e "rabbit" "$ALICE_FILE"

# 10. Show lines that contain '*', print only one if duplicate
grep '\*' "$ALICE_FILE" | sort | uniq
