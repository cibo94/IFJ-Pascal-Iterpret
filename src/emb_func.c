#include "inc.h"

static inline void arr_swap(char *frst, char *sec) {
    char pom = *sec;
    *sec = *frst;
    *frst = pom;
}
char *EMB_sort(char *in, int size) {
    char *out = malloc(size+1);                             // alokovanie navratoveho pola
    if (out == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    memcpy(out, in, size);                                  // vytvorenie kopie
    int krok = size / 2;                                    // nastavenie kroku na polovicu velkosti pola
    while (krok > 0) {                                      // Kazdy cyklus sa krok zmensi o polovicu
        for (int j = krok; j < size; j++) {
            for (int i = j-krok;
                 i >= 0 && out[i] > out[i+krok];
                 i = i - krok) {                            // kazdym cyklom sa porovnaju hodnoty obkrok dozadu
                arr_swap(&out[i], &out[i+krok]);            // a vymenia sa
            }
        }
        krok = krok / 2;
    }
    out[size] = '\0';                                       // ukoncovacia nula ... ta sa nezoradovala
    return out;
}

char *EMB_copy(char *in, int from, int size) {
    char *out = malloc(size*sizeof(char) +1);                  // Alokovanie navratoveho pola
    if (out == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    memcpy(out, &in[from-1], (size_t)size);                 // Kopirovanie z pozicie from do from+size do out
    out[size]=0;
    return out;
}

int EMB_length(char *s){
    char *p = s;                                            // Nastavenie ukazovatela na zaciatok pola
    while (*(p++));                                         // Posunutie ukazovatela na koniec pola
    return (int)(p-s)-1;                                    // Rozdiel medzi zaciatkom a koncom pola -1 je jeho dlzka
}

int EMB_find(char *s, char *search){
    int inputLength = EMB_length(s),                        // Dlzka vstupneho retazca
        searchLength = EMB_length(search),                  // Dlzka hladaneho retazca
        sInd = 1,                                           // Pozicia vo vstupe
        searchInd = 1,                                      // Pozicia v hladanom retazci
        r,                                                  // Pomocna premenna pre urcenie vektora spatnych indexov
        c = 0;                                              // Posledny index urcenia spatneho indexu

    char *fail = malloc(inputLength*sizeof(char));          // Pole navratovych indexov v pripade neuspesneho hladania
    if (fail == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    fail[0] = 0;

    while ((sInd <= inputLength) && (searchInd <= searchLength))
    {
        if (sInd != c){                                     // Urcenie vektora pre sInd prvok vstupneho retazca
            r = fail[(sInd+1)-2];
            while ((r>0) && (s[r-1] != s[(sInd+1)-2]))
                r = fail[r-1];
            fail[(sInd+1)-1] = (char)(r + 1);
            c = sInd;
        }

        if ((searchInd == 0) || (s[sInd-1] == search[searchInd-1])){
                                                            // Hladanie podretazca
            sInd++;
            searchInd++;
        }
        else
            searchInd = fail[searchInd-1];
    }
    free(fail);                                             // uvolnenie pamate

    if (searchInd > searchLength)                           // ukoncenie hladania, vratenie retazca
        return (sInd - searchLength);
    else
        return 0;
}
