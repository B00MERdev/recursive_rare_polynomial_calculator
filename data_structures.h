#ifndef POPRAWKA_DUZE_ZADANIE_DATA_STRUCTURES_H
#define POPRAWKA_DUZE_ZADANIE_DATA_STRUCTURES_H
/** @file
  Interfejs struktur danych kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include "poly.h"

/** To jest makrodefinicja reprezentująca początkową długość tablic
 * dynamicznych. */
#define STARTING_ARRAY_SIZE 1

/**
 * To jest struktura przechowująca stos wielomianów.
 * Składa się z tablicy wielomianów, długości tej tablicy i indexu, do
 * którego poziomu jest zapełniona.
 */
typedef struct PolyStack {
    Poly *arr; ///< tablica wielomianów
    size_t arraySize; ///< rozmiar tablicy wielomianów
    size_t index; ///< indeks poziomu zapełnienia
} PolyStack;

/**
 * Inicjalizuje stos.
 * @return Zainicjalizowany stos.
 */
PolyStack PolyStackInit();

/**
 * Sprawdza, czy stos jest pusty.
 * @param[in] s : stos @f$s@f$
 * @return Czy stos jest pusty?
 */
bool PolyStackIsEmpty(PolyStack s);

/**
 * Wkłada wielomian na szczyt stosu.
 * @param[in] s : stos @f$s@f$
 * @param[in] p : wielomian @f$p@f$
 */
void PolyStackPush(PolyStack *s, Poly p);

/**
 * Zdejmuje jeden wielomian ze szczytu stosu.
 * @param[in] s : stos @f$s@f$
 * @return wielomian ze szczytu stosu
 */
Poly PolyStackPop(PolyStack *s);

/**
 * Usuwa stos i zwalnia pamięć po nim.
 * @param[in] s : stos @f$s@f$
 */
void PolyStackDestroy(PolyStack *s);

/**
 * Wpisuje wielomian do tablicy wielomianów.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] arr : tablica jednomianów @f$arr@f$
 * @param[in] index : liczba już wpisanych jednomianów @f$index@f$
 * @param[in] allocatedSize : długość tablicy jednomianów @f$allocatedSize@f$
 */
void WritePolyToArray(Poly p, Poly **arr, size_t *index, size_t
*allocatedSize);

/**
 * Bezpiecznie alokuje pamięć podanej wielkości.
 * W przypadku, gdy malloc nie zaalokuje prawidłowo pamięci zakańcza program
 * z awaryjnym kodem 1.
 * @param[in] size : wielkość do zaalokowania @f$size@f$
 * @return adres z zaalokowaną pamięcią
 */
void *secureMalloc(size_t size);

#endif //POPRAWKA_DUZE_ZADANIE_DATA_STRUCTURES_H
