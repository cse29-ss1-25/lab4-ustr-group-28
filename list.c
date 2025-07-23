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
    // TODO: implement this

}

/*
Removes the element at the given index and shifts all subsequent elements left.

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t listRemoveAt(List* list, int32_t index) {
    // TODO: implement this

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

}
