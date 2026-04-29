#include "string.h"
#include <stdarg.h>

size_t strlen(const char *s) {
    size_t len = 0;
    while (s && s[len] != '\0') len++;
    return len;
}

int strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (int)((unsigned char)*s1) - (int)((unsigned char)*s2);
}

int strncmp(const char *s1, const char *s2, size_t n) {
    if (!s1 || !s2 || n == 0) return 0;
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return (int)((unsigned char)s1[i]) - (int)((unsigned char)s2[i]);
        if (s1[i] == '\0') return 0;
    }
    return 0;
}

char* strcpy(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *d = dest;
    while ((*d++ = *src++) != '\0');
    return dest;
}

char* strncpy(char *dest, const char *src, size_t n) {
    if (!dest || !src) return dest;
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char* strcat(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *d = dest;
    while (*d != '\0') d++;
    while ((*d++ = *src++) != '\0');
    return dest;
}

char* strncat(char *dest, const char *src, size_t n) {
    if (!dest || !src) return dest;
    char *d = dest;
    while (*d != '\0') d++;
    for (size_t i = 0; i < n && src[i] != '\0'; i++) {
        *d++ = src[i];
    }
    *d = '\0';
    return dest;
}

char* strchr(const char *s, int c) {
    if (!s) return NULL;
    while (*s != '\0') {
        if ((unsigned char)*s == (unsigned char)c) return (char *)s;
        s++;
    }
    return NULL;
}

void* memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

void* memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) return (int)p1[i] - (int)p2[i];
    }
    return 0;
}

static unsigned int rand_seed = 1;

int rand(void) {
    rand_seed = rand_seed * 1103515245 + 12345;
    return (rand_seed / 65536) % 32768;
}

void srand(unsigned int seed) {
    rand_seed = seed;
}

int sprintf(char *str, const char *format, ...) {
    // Simple sprintf implementation for %d and %s
    va_list args;
    va_start(args, format);
    
    char *str_ptr = str;
    const char *fmt_ptr = format;
    
    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            if (*fmt_ptr == 'd') {
                int num = va_arg(args, int);
                if (num < 0) {
                    *str_ptr++ = '-';
                    num = -num;
                }
                char buf[16];
                int i = 0;
                do {
                    buf[i++] = '0' + (num % 10);
                    num /= 10;
                } while (num > 0);
                while (i > 0) {
                    *str_ptr++ = buf[--i];
                }
            } else if (*fmt_ptr == 's') {
                char *s = va_arg(args, char *);
                while (*s) {
                    *str_ptr++ = *s++;
                }
            } else {
                *str_ptr++ = '%';
                *str_ptr++ = *fmt_ptr;
            }
        } else {
            *str_ptr++ = *fmt_ptr;
        }
        fmt_ptr++;
    }
    
    *str_ptr = '\0';
    va_end(args);
    return str_ptr - str;
}
