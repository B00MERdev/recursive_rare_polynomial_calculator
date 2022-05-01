/** @file
  Realizacja wejścia i wyjścia kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "input-output.h"
#include "poly.h"
#include "data_structures.h"

/** To jest makrodefinicja reprezentująca znak końca linii. */
#define ENDLINE '\n'
/** To jest makrodefinicja reprezentująca początkową długość tablic
 * dynamicznych. */
#define STARTING_ARRA_YSIZE 1
/** To jest makrodefinicja reprezentująca znak przecinka. */
#define COLON ','
/** To jest makrodefinicja reprezentująca znak plus. */
#define PLUS '+'
/** To jest makrodefinicja reprezentująca znak minus. */
#define MINUS '-'
/** To jest makrodefinicja reprezentująca znak otwarcia nawiasu. */
#define OPEN_BRACKET '('
/** To jest makrodefinicja reprezentująca znak zamknięcia nawiasu. */
#define CLOSE_BRACKET ')'
/** To jest makrodefinicja reprezentująca znak '#'. */
#define HASH '#'
/** To jest makrodefinicja reprezentująca znak 'A'. */
#define A_UPPERCASE 'A'
/** To jest makrodefinicja reprezentująca znak 'Z'. */
#define Z_UPPERCASE 'Z'
/** To jest makrodefinicja reprezentująca znak 'a'. */
#define A_LOWERCASE 'a'
/** To jest makrodefinicja reprezentująca znak 'z'. */
#define Z_LOWERCASE 'z'
/** To jest makrodefinicja reprezentująca znak '\0'. */
#define NULL_CHARACTER '\0'
/** To jest makrodefinicja reprezentująca znak '0'. */
#define ZEROCHAR '0'
/** To jest makrodefinicja reprezentująca znak '9'. */
#define NINECHAR '9'
/** To jest makrodefinicja reprezentująca znak na pewno niebędący
 * poprawnym znakiem w wielomianie. */
#define NOTPOLYCHAR '&'

/**
 * Sprawdza, czy podany znak jest jednym ze znaków '0' - '9' lub minusem.
 * @param[in] c : znak @f$c@f$
 * @return Czy podany znak jest jednym ze znaków '0' - '9' lub minusem?
 */
bool charIsDecOrMinus(char c) {
    return (c >= ZEROCHAR && c <= NINECHAR) || c == MINUS;
}

/**
 * Sprawdza, czy podany znak jest jednym ze znaków '0' - '9'.
 * @param[in] c : znak @f$c@f$
 * @return Czy podany znak jest jednym ze znaków '0' - '9'?
 */
bool charIsDec(char c) {
    return (c >= ZEROCHAR && c <= NINECHAR);
}

bool IsNextLine() {
    int nextChar = getchar();

    if (nextChar == EOF) {
        return false;

    } else {
        ungetc(nextChar, stdin);

        return true;
    }
}

/**
 * Wpisuje znak na koniec ciagu znaków.
 * @param[in] line : linia @f$line@f$
 * @param[in] allocatedLength : zaalokowana długość tablicy
 * @f$allocatedLength@f$
 * @param[in] lineLength : długość linii @f$lineLength@f$
 * @param[in] nextChar : znak @f$nextChar@f$
 */
void writeCharToString(char **line, size_t *allocatedLength, size_t
*lineLength, char nextChar) {
    if (*line == NULL) {
        (*line) = secureMalloc(STARTING_ARRA_YSIZE * sizeof(char));
        *allocatedLength = STARTING_ARRA_YSIZE;
    }

    if (*lineLength == *allocatedLength) {
        *allocatedLength = *allocatedLength * 2;
        char *tmp = secureMalloc(*allocatedLength * sizeof(char));

        for (size_t i = 0; i < *lineLength; i++) {
            tmp[i] = (*line)[i];
        }

        free(*line);
        *line = tmp;
    }

    (*line)[*lineLength] = nextChar;
    (*lineLength)++;
}

Line LineRead() {
    char *string = NULL;
    size_t allocatedLength = 0;
    size_t lineLength = 0;
    int nextChar = getchar();

    while (nextChar != EOF && nextChar != ENDLINE) {
        if (nextChar > CHAR_MAX) {
            nextChar = NOTPOLYCHAR;
        }
        writeCharToString(&string, &allocatedLength, &lineLength, (char)
        nextChar);
        nextChar = getchar();
    }

    writeCharToString(&string, &allocatedLength, &lineLength, '\0');
    lineLength--;

    return (Line) {.string = string, .lineLength = lineLength};
}

void LineDestroy(Line line) {
    free(line.string);
}

/**
 * Wypisuje wielomian na wyjście bez kończenia linii.
 * @param[in] p : wielomian @f$p@f$
 */
void PolyPrintH(const Poly *p);

/**
 * Wypisuje jednomian na wyjście bez kończenia linii.
 * @param[in] m : jednomian @f$m@f$
 */
void MonoPrint(const Mono *m) {
    printf("(");
    PolyPrintH(&m->p);
    printf(",");
    printf("%d", m->exp);
    printf(")");
}

void PolyPrintH(const Poly *p) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    } else {
        for (size_t i = 0; i < p->size; i++) {
            MonoPrint(&p->arr[i]);
            if (i < p->size - 1) {
                printf("+");
            }
        }
    }
}

void PolyPrint(const Poly *p) {
    PolyPrintH(p);
    printf("\n");
}

/**
 * Przedłuża tablicę jednomianów.
 * @param[in] arr : tablica jednomianów @f$arr@f$
 * @param[in] arraySize : długość tablicy jednomianów @f$arraySize@f$
 */
void ExtendArray(Mono **arr, size_t *arraySize) {
    Mono *newArr;
    if (*arraySize == 0) {
        newArr = secureMalloc(STARTING_ARRA_YSIZE * sizeof(Mono));
        *arraySize = STARTING_ARRA_YSIZE;
    } else {
        newArr = secureMalloc((*arraySize) * 2 * sizeof(Mono));

        for (size_t i = 0; i < (*arraySize); i++) {
            newArr[i] = (*arr)[i];
        }

        (*arraySize) = (*arraySize) * 2;
        free(*arr);
    }

    (*arr) = newArr;
}

/**
 * Wpisuje jednomian do tablicy jednomianów.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] i : liczba już wpisanych jednomianów @f$i@f$
 * @param[in] arr : tablica jednomianów @f$arr@f$
 * @param[in] arraySize : długość tablicy jednomianów @f$arraySize@f$
 */
void WriteMonoToArray(const Mono *m, size_t *i, Mono **arr,
                     size_t *arraySize) {
    if (*i == *arraySize) {
        ExtendArray(arr, arraySize);
    }

    (*arr)[*i] = *m;
    (*i)++;
}

poly_coeff_t ReadValueCoeff(Line line, size_t *index, bool *isEmpty, bool
*nonDecimalChars) {
    size_t firstIndex = *index;
    char *end;
    errno = 0;
    poly_coeff_t value = strtol(&line.string[*index], &end, 10);
    if ((value == LONG_MAX || value == LONG_MIN) && errno == ERANGE)
    {
        *nonDecimalChars = true;
    }

    for (size_t i = *index; i <= line.lineLength; i++) { // jest to
        // bezpieczne bo na koniec każdej linii dopisuję '\0' w LineRead
        if (&(line.string[i]) == end) {
            if (*index == i) {
                (*isEmpty) = true;
            }
            (*index) = i;
        }
    }

    for (size_t i = firstIndex; i < *index; i++) {
        if (!charIsDecOrMinus(line.string[i])) {
            *nonDecimalChars = true;
        }
    }

    return value;
}

size_t ReadValueSizeT(Line line, size_t *index, bool *isEmpty, bool
*nonDecimalChars) {
    size_t firstIndex = *index;
    char *end;
    errno = 0;
    size_t value = strtoul(&line.string[*index], &end, 10);
    if ((value == ULONG_MAX) && errno == ERANGE)
    {
        (*nonDecimalChars) = true;
    }

    for (size_t i = *index; i <= line.lineLength; i++) { // jest to
        // bezpieczne bo na koniec każdej linii dopisuję '\0' w LineRead
        if (&(line.string[i]) == end) {
            if (*index == i) {
                (*isEmpty) = true;
            }
            (*index) = i;
        }
    }

    for (size_t i = firstIndex; i < *index; i++) {
        if (!charIsDec(line.string[i])) {
            *nonDecimalChars = true;
        }
    }

    return value;
}

/**
 * Wczytuje wielomian z wejścia.
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początku wczytywania danych z linii @f$index@f$
 * @return Wielomian zawarty w podanej linii.
 */
Poly PolyReadH(Line line, size_t *index);

/**
 * Wczytuje jednomian z wejścia.
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początku wczytywania danych z linii @f$index@f$
 * @return jednomian zawarty w podanej linii.
 */
Mono MonoRead(Line line,  size_t *index) {
    Poly p = PolyReadH(line, index);
    (*index)++;
    bool isEmpty = false;
    bool nonDecChars = false;
    poly_exp_t exp = (int) ReadValueCoeff(line, index, &isEmpty, &nonDecChars);
    (*index)++;

    return (Mono) {.p = p, .exp = exp};
}

Poly PolyReadH(Line line, size_t *index) {
    Mono *arr = NULL;
    size_t arraySize = 0;
    size_t arrayIndex = 0;

    while (*index < line.lineLength && line.string[*index] !=
    COLON) {
        if (line.string[*index] == OPEN_BRACKET) {
            (*index)++;
            Mono nextMono = MonoRead(line, index);
            if (!MonoIsZero(&nextMono)) {
                WriteMonoToArray(&nextMono, &arrayIndex, &arr, &arraySize);
            }
        } else if (line.string[*index] == PLUS){
            (*index)++;
        } else {
            bool isEmpty = false;
            bool nonDecChars = false;
            return PolyFromCoeff(
                    ReadValueCoeff(line, index, &isEmpty, &nonDecChars));
        }
    }

    if (arraySize == 0) {

        return PolyZero();
    }

    Poly result = PolyAddMonos(arrayIndex, arr);
    free(arr);

    return result;
}

Poly PolyRead(Line line) {
    size_t index = 0;

    return PolyReadH(line, &index);
}

/**
 * Sprawdza, czy część linii jest prawidłowym jednomianem.
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początkowy @f$index@f$
 * @param[in] endIndex : indeks końcowy @f$endIndex@f$
 * @return Czy część linii jest prawidłowym jednomianem?
 */
bool MonoIsCorrect(Line line, size_t *index, size_t endIndex);

/**
 * Sprawdza, czy część linii jest prawidłowym wielomianem.
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początkowy @f$index@f$
 * @param[in] endIndex : indeks końcowy @f$endIndex@f$
 * @return Czy część linii jest prawidłowym wielomianem?
 */
bool PolyIsCorrectH(Line line, size_t *index, size_t endIndex) {
    if (*index >= endIndex) {

        return false;
    }
    bool atLeastOneMono = false;

    while (*index < endIndex) {
        if (line.string[*index] == OPEN_BRACKET) {
            (*index)++;

            if (!MonoIsCorrect(line, index, endIndex)) {

                return false;
            }

            atLeastOneMono = true;
        } else if (line.string[*index] == PLUS) {
            if (!atLeastOneMono) {

                return false;
            }

            (*index)++;

            if (*index >= endIndex || line.string[*index] != OPEN_BRACKET) {

                return false;
            }

            (*index)++;

            if (!MonoIsCorrect(line, index, endIndex)) {

                return false;
            }
        } else {
            bool isEmpty = false;
            bool nonDecChars = false;
            ReadValueCoeff(line, index, &isEmpty, &nonDecChars);

            if (isEmpty || nonDecChars) {

                return false;
            }
        }
    }

    return true;
}

bool MonoIsCorrect(Line line, size_t *index, size_t endIndex) {
    if (*index >= endIndex) {
        return false;
    }

    size_t hIndex = *index;
    size_t newEndIndex = *index;
    bool endLoop = false;
    size_t nOfOpeningBraces = 1;
    size_t nOfClosingBraces = 0;
    while (!endLoop && hIndex < endIndex) {
        if (line.string[hIndex] == COLON) {
            newEndIndex = hIndex;
        } else if (line.string[hIndex] == OPEN_BRACKET) {
            nOfOpeningBraces++;
        } else if (line.string[hIndex] == CLOSE_BRACKET) {
            nOfClosingBraces++;
            if (nOfOpeningBraces != nOfClosingBraces) {
                newEndIndex = *index;
            }
        } else if (line.string[hIndex] == PLUS && nOfOpeningBraces ==
        nOfClosingBraces) {
            endLoop = true;
        }
        hIndex++;
    }

    if (!PolyIsCorrectH(line, index, newEndIndex) || *index >= endIndex) {
        return false;
    }

    (*index)++;

    if (*index >= endIndex) {
        return false;
    }

    bool isEmpty = false;
    bool nonDecChars = false;
    poly_coeff_t exp = ReadValueCoeff(line, index, &isEmpty, &nonDecChars);

    if (isEmpty || nonDecChars || exp < 0 || exp > INT_MAX || *index >=
    endIndex) {
        return false;
    }

    if (line.string[*index] != CLOSE_BRACKET) {
        return false;
    }
    (*index)++;

    return true;
}

bool PolyIsCorrect(Line line) {
    size_t index = 0;

    return PolyIsCorrectH(line, &index, line.lineLength);
}

bool ShouldIgnoreLine(Line line) {

    return line.lineLength == 0 || line.string[0] == HASH;
}

bool LineIsCommand(Line line) {
    assert(line.lineLength > 0);
    char firstChar = line.string[0];

    return (firstChar >= A_UPPERCASE && firstChar <= Z_UPPERCASE) ||
    (firstChar >= A_LOWERCASE && firstChar <= Z_LOWERCASE);
}

bool LineBeginsWith (Line line, char *string) {
    size_t i = 0;

    while (string[i] != NULL_CHARACTER) {
        if (line.string[i] != string[i]) {

            return false;
        }
        i++;
    }

    return true;
}

bool LineIsValidCommand(Line line) {
    size_t size = line.lineLength; // zwiększenie czytelności
    if (size < 2) {

        return false;
    } else if (LineBeginsWith(line, AT)) {

        return true;
    }

    if (size >= 6 && LineBeginsWith(line, DEG_BY)) {

        return true;
    }

    if (size >= 7 && LineBeginsWith(line, COMPOSE)) {

        return true;
    }

    if (size == 3) {
        if (LineBeginsWith(line, ADD) || LineBeginsWith(line, MUL) ||
        LineBeginsWith(line, NEG) || LineBeginsWith(line, SUB) ||
        LineBeginsWith(line, DEG) || LineBeginsWith(line, POP)) {

            return true;
        } else {

            return false;
        }
    } else if (size == 4) {
        if (LineBeginsWith(line, ZERO)) {

            return true;
        } else {

            return false;
        }
    } else if (size == 5) {
        if (LineBeginsWith(line, CLONE) || LineBeginsWith(line, IS_EQ) ||
        LineBeginsWith(line, PRINT)) {

            return true;
        } else {

            return false;
        }
    } else if (size == 7) {
        if (LineBeginsWith(line, IS_ZERO)) {

            return true;
        } else {

            return false;
        }
    } else if (size == 8) {

        return LineBeginsWith(line, IS_COEFF);
    } else {

        return false;
    }
}