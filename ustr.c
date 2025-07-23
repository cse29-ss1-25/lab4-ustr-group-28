#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

int utf8CharLen(const char *p) {
        unsigned char c = (unsigned char)*p;
        if ((c & 0x80) == 0x00) return 1;
        if ((c & 0xE0) == 0xC0) return 2;
        if ((c & 0xF0) == 0xE0) return 3;
        if ((c & 0xF8) == 0xF0) return 4;
        return 1;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	UStr result = {0};
        if (start < 0 || end < 0 ||start > end || end > s.codepoints || !s.contents) {
                return new_ustr("");
        }

        if (s.is_ascii == 1) {
                int32_t len = end - start;
                char* copy = malloc(len + 1);
                if (!copy) {
                        return new_ustr("");
                }
                memcpy (copy, s.contents + start, len);
                copy[len] = '\0';
                result = new_ustr(copy);
                free(copy);
                return result;
        }

        char* p = s.contents;
        int32_t idx = 0;
        char* byteStart = NULL;
        char* byteEnd = NULL;
        while (*p) {
                if (idx == start) {
                        byteStart = p;
                }

                if (idx == end) {
                        byteEnd = p;
                        break;
                }
                p += utf8CharLen(p);
                idx++;
        }

        if (byteStart == NULL) {
                return new_ustr("");
        }

        if (byteEnd == NULL) {
                byteEnd = p;
        }

        int32_t byteLen = byteEnd - byteStart;
        char* copy = malloc (byteLen + 1);
        if (!copy) {
                return new_ustr("");
        }
        memcpy(copy, byteStart, byteLen);
        copy[byteLen] = '\0';
        result = new_ustr(copy);
        free(copy);
        return result;
}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
    int32_t new_byte_length = s1.byte_length + s2.byte_length;
    char* combined = malloc(new_byte_length + 1);

    memcpy(combined, s1.data, s1.byte_length);
    memcpy(combined + s1.byte_length, s2.data, s2.byte_length);
    combined[new_byte_length] = '\0';

    UStr result = new_ustr(combined);

    return result;
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	// TODO: implement this

}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this

}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

