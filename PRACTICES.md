# Excellenteam C++ Coding Practices
This document serves as the de facto practice guideline standard; any deviation from the practices presented below is a valid reason for grade reduction.

## C++ Best Practices
The following section presents general C++ practices.

## Useful Links

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)  
- [C++ Best Practices (Lefticus)](https://lefticus.gitbooks.io/cpp-best-practices/content/03-Style.html)

### Incorrect or Missing Use of Exception Handling

✅ **Positive Example**:
```cpp
#include <iostream>
#include <fstream>
#include <stdexcept>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: " + filename + " not found");
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}
```

❌ **Not using exception handling when needed**:
```cpp
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // Undefined behavior if file is missing
}
```

❌ **Using a broad catch statement**:
```cpp
try {
    int x = std::stoi("abc"); // Will throw std::invalid_argument
} catch (...) {
    std::cerr << "Something went wrong!"; // Hides actual issue
}
```

🔴 **What's wrong?**
- A broad `catch (...)` clause hides actual exceptions.
- Specific exceptions (e.g., `std::invalid_argument`, `std::runtime_error`) should be handled explicitly.
- No helpful error messages to the user.

### Input Verification

✅ **Validate Input Properly**:
```cpp
#include <iostream>
#include <stdexcept>

int getUserAge() {
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    if (std::cin.fail() || age <= 0) {
        throw std::invalid_argument("Age must be a valid positive integer.");
    }
    return age;
}
```

💡 **Why is this better?**
- Validation is done at input handling.
- Prevents invalid values from spreading in the program.
- Avoids repeated checks throughout the code.

❌ **Over-validating in every function**:
```cpp
void processPrice(double price) {
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative!");
    }
    // Do something with price
}
```

💡 **Why is this bad?**
- Should be validated at input boundaries.
- Adds unnecessary redundancy.

### Using Magic Numbers/Strings

✅ **Positive Example**:
```cpp
const int MAX_USERS = 100;
const double PI = 3.14159;
std::string ADMIN_ROLE = "admin";
```

❌ **Negative Example**:
```cpp
if (userRole == "admin") {
    int maxAttempts = 3;
}
```

### Non-Descriptive Variable Names

✅ **Positive Example**:
```cpp
int maxUsers = 10;
double timeoutSeconds = 20.5;
```

❌ **Negative Example**:
```cpp
int x = 10;
double y = 20.5;
```

### Handling Edge Cases in Functions

✅ **Positive Example**:
```cpp
double divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Cannot divide by zero");
    }
    return a / b;
}
```

❌ **Negative Example**:
```cpp
double divide(double a, double b) {
    return a / b; // Doesn't handle division by zero
}
```

### Code Duplication

✅ **Use Functions to Reduce Duplication**:
```cpp
void greet(const std::string& timeOfDay) {
    std::cout << "Good " << timeOfDay << "!" << std::endl;
}

int main() {
    greet("morning");
    greet("evening");
    return 0;
}
```

❌ **Hardcoded Messages**:
```cpp
void greetMorning() {
    std::cout << "Good morning!" << std::endl;
}

void greetEvening() {
    std::cout << "Good evening!" << std::endl;
}
```

## Naming Conventions

### Constants Names

✅ **Use UPPER_CASE_WITH_UNDERSCORES**:
```cpp
const int MAX_USERS = 1000;
const double PI = 3.14159;
```

❌ **Avoid lowercase or mixed case**:
```cpp
const int maxUsers = 1000;
const int MaxUsers = 1000;
```

### Class Names

✅ **Use PascalCase**:
```cpp
class UserProfile {
public:
    std::string getFullName() const;
};
```

❌ **Avoid:**
- `snake_case`: `class user_profile {}`
- `lowercase`: `class userprofile {}`

### Variable and Function Names

✅ **Use `camelCase` for variables and functions**:
```cpp
double calculateDiscount(double price, double discountRate) {
    return price * discountRate;
}
```

❌ **Avoid:**
- `snake_case`
- `PascalCase` for functions
- Single-letter names unless standard (e.g., `i`, `j`, `k` in loops)

### Private and Protected Attributes & Methods

✅ **Use underscore prefix for private members**:
```cpp
class User {
private:
    std::string _sessionToken;
};
```

✅ **Use double underscores for strongly private members**:
```cpp
class User {
private:
    std::string __password;
};
```

### File Naming

✅ **Use `snake_case` for source files**:
```cpp
user_controller.cpp
database_handler.cpp
```

❌ **Avoid:**
- `UserController.cpp`
- `user controller.cpp`

## Documentation

### Insufficient or Incorrect Documentation Format

✅ **Positive Example**:
```cpp
/**
 * Adds two numbers and returns the sum.
 * @param a First number
 * @param b Second number
 * @return Sum of a and b
 */
double add(double a, double b) {
    return a + b;
}
```

❌ **Negative Example**:
```cpp
double add(double a, double b) {
    // This function adds
    return a + b; // Not detailed enough
}
```

## Final Notes
- Follow C++ best practices to ensure maintainability and readability.
- Always validate input at the correct level.
- Use meaningful names and avoid magic numbers.
- Keep functions focused and avoid unnecessary complexity.
- Maintain good documentation to improve code clarity.

🚀 Happy coding!

