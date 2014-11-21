
#ifndef h_EMBEDED
#define h_EMBEDED


/** Sort function
 * \brief Embeded function for sorting
 * \param in Input array
 * \param size size of array
 * \return Output array
 */
char *EMB_sort(char *in, int size);

/** Copy function
* \brief Embeded function for copy string
* \param in Input array
* \param from index from which copy
* \param size size of substring
* \return Output array
*/
char *EMB_copy(char *in, int from, int size);

/** Length function
 * \brief Embeded function for size of string
 * \param *s Input string
 * \return Length of string
 */
int EMB_length(char *s);

/** Find function
 * \brief Embeded function for finding substrings (KMP algorithm)
 * \param *s Input string
 * \param *search Substring to be found
 * \return Index of first occurence of substring (Pascal notation), 0 if no substring to be found
*/
int EMB_find(char *s, char *search);

#endif /*defined(h_EMBEDED)*/