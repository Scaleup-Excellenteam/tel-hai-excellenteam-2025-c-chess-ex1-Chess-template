#!/bin/bash

# 1. List current directory files sorted by size (biggest to smallest)
ls -lS

# 2. Show the content of the biggest file in /etc
cat "$(find /etc -maxdepth 1 -type f -exec du -b {} + 2>/dev/null | sort -nr | head -n 1 | cut -f2-)"

# 3. The Command to show content as above
# cat

# 4. Show files sorted by size (smallest to biggest)
ls -lSr 

# 5. Create nested directories: grandpa/father/son
mkdir -p grandpa/father/son

# 6. Calculate a simple math expression without opening a graphical interface
echo "2^3" | bc

# 7. Run the calculator without CopyRight
echo "2^3" | bc -q

# 8. Show current month calendar
cal

# 9. Show calendars of all months of the current year
cal -y