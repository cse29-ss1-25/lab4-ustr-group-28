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

// Helper method to count bytes
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
	// Checks for invalid inputs
        if (start < 0 || end < 0 ||start > end || end > s.codepoints || !s.contents) {
                return new_ustr("");
        }

	// Checks if string contains only ascii
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

	// If contains UTF 8 byte counting is required
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
    // Malloc s1's bytes and s2's bytes combined
    int32_t new_byte_length = s1.byte_length + s2.byte_length;
    char* combined = malloc(new_byte_length + 1);

    // Copies s1 to combined, s2 to the end of combined, and finally the null terminator
    memcpy(combined, s1.data, s1.byte_length);
    memcpy(combined + s1.byte_length, s2.data, s2.byte_length);
    combined[new_byte_length] = '\0';

    // Creates a new ustr with combined and returns it
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

int32_t len = s.codepoints;
    char** codepoints = malloc(sizeof(char*) * len);
    int32_t* lengths = malloc(sizeof(int32_t) * len);

    int32_t i = 0, cp = 0;
    while (s.contents[i] != '\0') {
        int32_t cp_len = utf8_codepoint_len(&s.contents[i]);
        codepoints[cp] = &s.contents[i];
        lengths[cp] = cp_len;
        i += cp_len;
        cp++;
    }

    int32_t total_bytes = s.bytes;
    char* reversed = malloc(total_bytes + 1);
    int32_t offset = 0;

    for (int32_t j = len - 1; j >= 0; j--) {
        memcpy(reversed + offset, codepoints[j], lengths[j]);
        offset += lengths[j];
    }

    reversed[total_bytes] = '\0';
    UStr result = new_ustr(reversed);
    free(reversed);
    free(codepoints);
    free(lengths);
    return result;


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

