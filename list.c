#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "list.h"

/*
Returns an empty list of initial_capacity
*/
List new_list(int32_t initial_capacity) {
    List list;
    list.size = 0;
    list.capacity = initial_capacity;
    list.data = malloc(initial_capacity * sizeof(struct UStr));
    return list;
}

/*
Initializes an list of length size with elements from array
*/
List new_list_from_array(struct UStr* array, int32_t size) {
    List list = new_list(size);
    for (int32_t i = 0; i < size; i++) {
        list.data[i] = array[i];
    }
    list.size = size;
    return list;
}


/*
Given a list of strings and a separator string, returns a single string 
containing all the strings in list joined by the separator.
*/
UStr join(List* list, UStr separator) {
    // TODO: implement this
if (list->size == 0) {
        return new_ustr("");
    }

    int32_t total_bytes = 0;
    int32_t sep_count = list->size - 1;

    for (int32_t i = 0; i < list->size; i++) {
        total_bytes += list->data[i].bytes;
    }

    total_bytes += sep_count * separator.bytes;

    char* result = malloc(total_bytes + 1);
    int32_t offset = 0;

    for (int32_t i = 0; i < list->size; ++i) {
        UStr current = list->data[i];
        memcpy(result + offset, current.contents, current.bytes);
        offset += current.bytes;

        if (i < list->size - 1) {
            memcpy(result + offset, separator.contents, separator.bytes);
            offset += separator.bytes;
        }
    }

    result[total_bytes] = '\0';

    UStr joined = new_ustr(result);
    free(result);

    return joined;


	if(list->size == 0) {
		return new_ustr("");
	}

	//Calculate total bytes and codepoints for joined string
	
	int32_t total_bytes = 0;
	int32_t total_codepoints = 0;

	for(int32_t i = 0; i < list->size; i++) {
		total_bytes = total_bytes + list->data[i].bytes;
		total_codepoints = total_codepoints + list->data[i].codepoints;

		if(i < list->size - 1) { // minus the null terminator

			total_bytes = total_bytes + separator.bytes;
			total_codepoints = total_codepoints + separator.codepoints;
		}
	}

	//Allocating memory for buffer for concatenated contents
	
	char *result = malloc(total_bytes + 1);
	if(result == NULL) {
		fprintf(stderr, "Memory allocation failed in join\n");
		exit(1);
	}

	//Copy contents into result buffer
	
	char *cursor = result;
	for(int32_t i = 0; i < list->size; i++) {
		memcpy(cursor, list->data[i].contents, list->data[i].bytes);
		cursor = cursor + list->data[i].bytes;
		if(i < list->size - 1) {
			memcpy(cursor, separator.contents, separator.bytes);
			cursor += separator.bytes;
		}
	}

	result[total_bytes] = '\0';

	//wrap the joined string in a UStr struct
	
	UStr joined = new_ustr(result);

	free(result); //new_ustr copies the string, so we can free our buffer
	return joined;

}

/*
Inserts string s into list at index s, shifting elements to the right.
Expands the list's capacity if necessary (double the capacity, or set to 1 if 0).

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t insert(List* list, UStr s, int32_t index) {
    if (index < 0 || index > list->size) {
        return 0;
    }

    int32_t new_capacity;
    if (list->size == list->capacity) {
        new_capacity = (list->capacity == 0) ? 1 : list->capacity * 2;

        UStr* new_data = malloc(new_capacity * sizeof(UStr));
        if (!new_data) return 0;

        // Copy old data to new_data
        for (int32_t i = 0; i < list->size; i++) {
            new_data[i] = list->data[i];
        }

        free(list->data);
        list->data = new_data;
        list->capacity = new_capacity;
    }

    for (int32_t i = list->size; i > index; --i) {
        list->data[i] = list->data[i - 1];
    }

    list->data[index] = s;
    list->size += 1;

    return 1;
}

/*
Removes the element at the given index and shifts all subsequent elements left.

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t listRemoveAt(List* list, int32_t index) {
    // TODO: implement this

	if (index < 0 || index >= list->size) {
        return 0;  
    }

    free_ustr(list->data[index]);

    for (int32_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->size--;

    return 1;
}

/*
Splits the given string s into substrings separated by the given delimiter string.

Returns a List of String objects, each containing a segment between delimiters.

If the delimiter is the empty string, return a list containing the original 
string as a single element.

If the input string ends with the delimiter, include an empty string at the 
end of the result.

Note that the delimiter could be of a length of more than 1 character
*/
List split(UStr s, UStr separator) {
    // TODO: implement this

	//Edge case: empty separator, if separator.bytes == 0 just return a lsit with the original string:
	
	if(separator.bytes == 0) {
		List list = new_list(1);
		list.data[0] = s;
		list.size = 1;
		return list;
	}

	//Scan s.contents for separator.contents
	
	List result = new_list(4); //Initial capacity; grow if needed

	char* start = s.contents;
	char* end = s.contents;
	int32_t remaining_bytes = s.bytes;

	while(remaining_bytes >= separator.bytes) {
		
		if(memcmp(end, separator.contents, separator.bytes) == 0) {
                        //Found a match: create substring from start to end

                        int32_t segment_len = end - start;
                        char* segment = malloc(segment_len + 1);
                        memcpy(segment, start, segment_len);
                        segment[segment_len] = '\0';

                        result.data[result.size++] = new_ustr(segment);
                        free(segment);

                        end = end + seperator.bytes;
                        start = end;
                        remaining_bytes = s.bytes - (start - s.contents);
                }
		
		 else {
                        end++;
                        remaining_bytes--;

                }
	}

        //add last segment

        int32_t tail_len = s.contents + s.bytes - start;
        char *tail = malloc(tail_len + 1);
        memcpy(tail, start, tail_len);
        tail[tail_len] = '\0';

        result.data[result.size++] = new_ustr(tail);
        free(tail);

        return result;
	

}
