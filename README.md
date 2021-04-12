# Custom `strtod` implementation

A custom implementation of `strtod` which functions very similar to the standard C `strtod` function with some minor differences:
1. this function skips over underscores to allow for underscore seperated numbers
2. `errno` is not set to `ERANGE` when the normalized value of the number would be less tha `DBL_MIN` but is instead set if the resulting double would be 0 where the value is not 0.
3. this function only works base 10 whereas, the standard C `strtod` may check for a `0x`, `0o`, or `0b` to determine the base to use (may fix this by adding an `strtoll` function which may be called if those prefixes are detected)
