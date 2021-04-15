// Author: CrazyInfin8
// license: unlicense (https://unlicense.org/)

#include <errno.h>    // errno, ERRANGE
#include <limits.h>   // INT_MIN, INT_MAX
#include <math.h>     // powl, INFINITY
#include <stdbool.h>  // bool, true, false
#ifdef USE_STRTOLL_FOR_PREFIX
#ifdef USE_DEFAULT_STRTOLL
#include <stdlib.h>  // strtoll
#else
#include "../strtoll/strtoll.c"  // c8_strToLL // change to path of c8_strtoll
#endif
#endif
static double c8_strToD(char *str, char **v) {
    long long num = 0;
    int digits = 0;
    char *start = str;
    char c = *str;
    int e = 0;
    bool overflow = false, neg = false;
    if (c == '-') {
        neg = true;
        ++str;
    } else if (c == '+')
        ++str;
#ifdef USE_STRTOLL_FOR_PREFIX
    if (*str == '0') {
        switch (*(str + 1)) {
            case 'x':
            case 'o':
            case 'b':
#ifdef USE_DEFAULT_STRTOLL
                num = strtoll(str, &str, 0);
#else
                num = c8_strToLL(start, &str, 0);
#endif
                if (v) *v = str;
                return (double)num * (double)(neg ? -1 : 1);
        }
    }
#endif
    for (;;) {
        c = *str++;
        if (c >= '0' && c <= '9') {
            if (digits < 17) {
                num = num * 10 + c - '0';
                if (num) ++digits;
            } else if (e > INT_MAX - 17) {
                overflow = true;
            } else
                ++e;
        } else if (c == '_') {
        } else
            break;
    }
    if (c == '.')
        for (;;) {
            c = *str++;
            if (c >= '0' && c <= '9') {
                if (digits < 17) {
                    num = num * 10 + c - '0';
                    if (num) ++digits;
                    if (e < INT_MIN + digits)
                        overflow = true;
                    else
                        --e;
                }
            } else if (c == '_') {
            } else
                break;
        }
    if (c == 'e' || c == 'E') {
        int e2 = 0;
        bool eneg = false;
        c = *str++;
        if (!digits && str - start == (neg ? 3 : 2)) {
            str -= (neg ? 2 : 1);
            goto skip;
        } else if (c == '-') {
            eneg = true;
            c = *str;
            if (c > '9' || c < '0') {
                --str;
                goto skip;
            }
        } else if (c == '+') {
            c = *str;
            if (c > '9' || c < '0') {
                --str;
                goto skip;
            }
        } else {
            c = *--str;
            if (c > '9' || c < '0') {
                goto skip;
            }
        }
        for (;;) {
            c = *str++;
            if (c >= '0' && c <= '9') {
                if (e2 > INT_MAX / 10 && c) {
                    overflow = true;
                } else
                    e2 = e2 * 10 + c - '0';
            } else if (e == '_') {
            } else
                break;
        }
        if (eneg) {
            if (e < INT_MIN + e2)
                overflow = true;
            else
                e -= e2;
        } else {
            if (e > INT_MAX - e2)
                overflow = true;
            else
                e += e2;
        }
    }
skip:
    if (v) *v = str - 1;
    // If overflow ever is set, the value is not accurate.
    if (overflow) {
        errno = ERANGE;
        if (e > 0)
            return INFINITY;
        else
            return 0;
    }
    // use long doubles to preserve accuracy
    double ret = (double)((long double)(num)*powl(10, e));
    if (ret == INFINITY) errno = ERANGE;
    if (ret == 0 && num != 0) errno = ERANGE;
    return ret * (neg ? -1 : 1);
}