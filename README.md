# Custom `strtod` implementation

A custom implementation of `strtod` which functions very similar to the standard C `strtod` function with some minor differences:
1. this function skips over underscores to allow for underscore seperated numbers
2. `errno` is not set to `ERANGE` when the normalized value of the number would be less tha `DBL_MIN` but is instead set if the resulting double would be 0 where the value is not 0.
3. where `strtod` can check for the `0x` prefix tp parse hexadecimals, `c8_strToD` does not by default. this behavior can optionally be enabled with a compiler flag. (see [below](##Options))
4. when enabled `c8_strToD` checks for any prefix (`0x`, `0o`, `0b`) and calls `c8_strToLL`. 
    1. Note that this is not the same behavior `strtod` does. `strtod` only seems to handle prefixes `0x` and `0X`. Also a number `0x` will be consumed as `0` with `x` remaining while `c8_strToLL` would not consume any bit of that number at all.

*There may be more inconsistancies. These are the minor ones due to design, simplicity, or what I have found through testing...*

## Options
There are some togglable options to change the parsers behavior. they can be enabled with `define`'s. Here are some options and what they do:
| Option | Description | Examples |
--- | --- | ---
|`USE_STRTOLL_FOR_PREFIX`|Allows the parser to to send numbers beginning with `0x`, `0o`, and `0b` to `c8_strToLL` to parse as a number of a different base. (note that this is currently included using a relative path)|numbers like `0xABCDEF`, `0x10101`, and `0o1234567` will be parsed|
|`USE_DEFAULT_STRTOLL`|(depends on `USE_STRTOLL_FOR_PREFIX` to be enabled) sends numbers to the default C `strtoll` function instead of the custom one||