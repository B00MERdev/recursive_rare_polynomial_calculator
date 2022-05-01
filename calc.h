#ifndef POPRAWKA_DUZE_ZADANIE_CALC_H
#define POPRAWKA_DUZE_ZADANIE_CALC_H
/** @file
  Interfejs kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

/** To jest typ reprezentujący operacje dwuargumentowe. */
enum TwoArgumentOperation {add, mul, sub, is_eq};

/** To jest typ reprezentujący operacje jednoargumentowe. */
enum OneArgumentOperation {is_coeff, is_zero, clone, neg, deg, print, pop};

#endif //POPRAWKA_DUZE_ZADANIE_CALC_H
