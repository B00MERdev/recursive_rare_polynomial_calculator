#ifndef POPRAWKA_DUZE_ZADANIE_INPUT_OUTPUT_H
#define POPRAWKA_DUZE_ZADANIE_INPUT_OUTPUT_H
/** @file
  Interfejs wejścia i wyjścia kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include <stdlib.h>
#include "poly.h"
/** To jest makrodefinicja reprezentująca ciąg znaków "ZERO". */
#define ZERO "ZERO"
/** To jest makrodefinicja reprezentująca ciąg znaków "IS_COEFF". */
#define IS_COEFF "IS_COEFF"
/** To jest makrodefinicja reprezentująca ciąg znaków "IS_ZERO". */
#define IS_ZERO "IS_ZERO"
/** To jest makrodefinicja reprezentująca ciąg znaków "CLONE". */
#define CLONE "CLONE"
/** To jest makrodefinicja reprezentująca ciąg znaków "ADD". */
#define ADD "ADD"
/** To jest makrodefinicja reprezentująca ciąg znaków "MUL". */
#define MUL "MUL"
/** To jest makrodefinicja reprezentująca ciąg znaków "NEG". */
#define NEG "NEG"
/** To jest makrodefinicja reprezentująca ciąg znaków "SUB". */
#define SUB "SUB"
/** To jest makrodefinicja reprezentująca ciąg znaków "IS_EQ". */
#define IS_EQ "IS_EQ"
/** To jest makrodefinicja reprezentująca ciąg znaków "DEG". */
#define DEG "DEG"
/** To jest makrodefinicja reprezentująca ciąg znaków "DEG_BY". */
#define DEG_BY "DEG_BY"
/** To jest makrodefinicja reprezentująca ciąg znaków "AT". */
#define AT "AT"
/** To jest makrodefinicja reprezentująca ciąg znaków "PRINT". */
#define PRINT "PRINT"
/** To jest makrodefinicja reprezentująca ciąg znaków "POP". */
#define POP "POP"
/** To jest makrodefinicja reprezentująca ciąg znaków "COMPOSE". */
#define COMPOSE "COMPOSE"

/**
 * To jest struktura przechowująca linię.
 * Linia składa się z tablicy znaków i długości linii.
 */
typedef struct Line {
    char *string; ///< tablica znaków
    size_t lineLength; ///< długość linii
} Line;

/**
 * Sprawdza, czy istnieje kolejna linia na wejściu.
 * @return Czy istnieje kolejna linia na wejściu.
 */
bool IsNextLine();

/**
 * Wczytuje ciąg znaków i zmienia go na wartość liczbową typu poly_coeff_t
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początku wczytywania z linii @f$index@f$
 * @param[in] isEmpty : sprawdza, czy wczytany ciąg znaków jest pusty
 * @f$isEmpty@f$
 * @param[in] nonDecimalChars : sprawdza, czy w przetworzonych znakach znajduje
 * się znak różny od zapisu dziesiętnego @f$nonDecimalChars@f$
 * @return Wartość liczbowa wynikająca z ciągu znaków z linii.
 */
poly_coeff_t ReadValueCoeff(Line line, size_t *index, bool *isEmpty, bool
*nonDecimalChars);

/**
 * Wczytuje ciąg znaków i zmienia go na wartość liczbową typu size_t
 * @param[in] line : linia @f$line@f$
 * @param[in] index : indeks początku wczytywania z linii @f$index@f$
 * @param[in] isEmpty : sprawdza, czy wczytany ciąg znaków jest pusty
 * @f$isEmpty@f$
 * @param[in] nonDecimalChars : sprawdza, czy w przetworzonych znakach znajduje
 * się znak różny od zapisu dziesiętnego i minusa @f$nonDecimalChars@f$
 * @return Wartość liczbowa wynikająca z ciągu znaków z linii.
 */
size_t ReadValueSizeT(Line line, size_t *index, bool *isEmpty, bool
*nonDecimalChars);

/**
 * Wczytuje linię z wejścia.
 * @return Linia z ciągu znaków na wejściu.
 */
Line LineRead();

/**
 * Usuwa linię z pamięci.
 * @param[in] line : linia @f$line@f$
 */
void LineDestroy(Line line);

/**
 * Wypisuje wielomian na wyjście.
 * @param[in] p : wielomian @f$p@f$
 */
void PolyPrint(const Poly *p);

/**
 * Wczytuje wielomian z wejścia.
 * @param[in] line : linia @f$line@f$
 * @return Wielomian zawarty w podanej linii.
 */
Poly PolyRead(Line line);

/**
 * Sprawdza, czy linia jest prawidłowym wielomianem.
 * @param[in] line : linia @f$line@f$
 * @return Czy linia jest prawidłowym wielomianem?
 */
bool PolyIsCorrect(Line line);

/**
 * Sprawdza, czy należy ignorować linię.
 * @param[in] line : linia @f$line@f$
 * @return Czy należy ignorować linię?
 */
bool ShouldIgnoreLine(Line line);

/**
 * Sprawdza, czy linia jest komendą.
 * @param[in] line : linia @f$line@f$
 * @return Czy linia jest komendą?
 */
bool LineIsCommand(Line line);

/**
 * Sprawdza, czy linia jest prawidłową komendą.
 * @param[in] line : linia @f$line@f$
 * @return Czy linia jest prawidłową komendą?
 */
bool LineIsValidCommand(Line line);

/**
 * Sprawdza, czy linia rozpoczyna się podanym ciągiem znaków.
 * @param[in] line : linia @f$line@f$
 * @param[in] string : ciąg znaków @f$string@f$
 * @return Czy linia rozpoczyna się podanym ciągiem znaków?
 */
bool LineBeginsWith (Line line, char *string);

#endif //POPRAWKA_DUZE_ZADANIE_INPUT_OUTPUT_H
