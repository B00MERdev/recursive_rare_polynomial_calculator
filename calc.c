/** @file
  Realizacja kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include <stdio.h>
#include "calc.h"
#include "input-output.h"
#include "data_structures.h"
/** To jest makrodefinicja reprezentująca znak spacji. */
#define SPACE ' '

/**
 * Wypisuje na standardowe wyjście błędów błąd złej komendy.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void wrongCommandError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld WRONG COMMAND\n", lineNumber);
}

/**
 * Wypisuje na standardowe wyjście błędów błąd argumentu funkcji deg_by.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void wrongValueDegByError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", lineNumber);
}

/**
 * Wypisuje na standardowe wyjście błędów błąd argumentu funkcji Poly_at.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void wrongValueAtError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", lineNumber);
}

/**
 * Wypisuje na standardowe wyjście błędów błąd stosu.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void stackError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", lineNumber);
}

/**
 * Wypisuje na standardowe wyjście błędów błąd nieprawidłowego wielomianu.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void wrongPolyError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld WRONG POLY\n", lineNumber);
}

/**
 * Wypisuje na standardowe wyjście błędów błąd argumentu funkcji Poly_compose.
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void wrongComposeError(size_t lineNumber) {
    fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", lineNumber);
}

/**
 * Wstawia na stos wielomian równy zero.
 * @param[in] s : stos @f$s@f$
 */
void zero(PolyStack *s) {
    PolyStackPush(s, PolyZero());
}

/**
 * Przeprowadza operacje kalkulatora związane z komendą AT.
 * W przypadku problemów z wykonaniem tej komendy pokazuje odpowiednie błędy.
 * @param[in] s : stos @f$s@f$
 * @param[in] l : linia @f$l@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void at(PolyStack *s, Line l, size_t lineNumber) {
    if (l.lineLength == 2 || l.lineLength == 3) {
            wrongValueAtError(lineNumber);
    } else {
        size_t index = 2;

        if (l.string[index] != SPACE) {
            wrongValueAtError(lineNumber);
        } else {
            index++;
            bool isEmpty = false;
            bool nonDecimalChars = false;
            poly_coeff_t coeff = ReadValueCoeff(l, &index, &isEmpty,
                                                &nonDecimalChars);
            if (isEmpty || nonDecimalChars || index != l.lineLength) {
                wrongValueAtError(lineNumber);
            } else {
                if (PolyStackIsEmpty(*s)) {
                    stackError(lineNumber);
                } else {
                    Poly lastPoly = PolyStackPop(s);
                    PolyStackPush(s, PolyAt(&lastPoly, coeff));
                    PolyDestroy(&lastPoly);
                }
            }
        }
    }
}

/**
 * Przeprowadza operacje kalkulatora związane z komendą DEG_BY.
 * W przypadku problemów z wykonaniem tej komendy pokazuje odpowiednie błędy.
 * @param[in] s : stos @f$s@f$
 * @param[in] l : linia @f$l@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void degBy(PolyStack *s, Line l, size_t lineNumber) {
    if (l.lineLength == 6 || l.lineLength == 7) {
        wrongValueDegByError(lineNumber);
    } else {
        size_t index = 6;

        if (l.string[index] != SPACE) {
            wrongValueDegByError(lineNumber);
        } else {
            index++;
            bool isEmpty = false;
            bool nonDecimalChars = false;
            size_t depth = ReadValueSizeT(l, &index, &isEmpty,
                                                &nonDecimalChars);
            if (isEmpty || nonDecimalChars || index != l.lineLength) {
                wrongValueDegByError(lineNumber);
            } else {
                if (PolyStackIsEmpty(*s)) {
                    stackError(lineNumber);
                } else {
                    Poly lastPoly = PolyStackPop(s);
                    PolyStackPush(s, lastPoly);
                    printf("%d\n", PolyDegBy(&lastPoly, depth));
                }
            }
        }
    }
}

/**
 * Przeprowadza operacje kalkulatora związane z komendą COMPOSE.
 * W przypadku problemów z wykonaniem tej komendy pokazuje odpowiednie błędy.
 * @param[in] s : stos @f$s@f$
 * @param[in] l : linia @f$l@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void compose(PolyStack *s, Line l, size_t lineNumber) {
    if (l.lineLength == 7 || l.lineLength == 8) {
        wrongComposeError(lineNumber);
    } else {
        size_t index = 7;

        if (l.string[index] != SPACE) {
            wrongComposeError(lineNumber);
        } else {
            index++;
            bool isEmpty = false;
            bool nonDecimalChars = false;
            size_t parameter = ReadValueSizeT(l, &index, &isEmpty,
                                          &nonDecimalChars);
            if (isEmpty || nonDecimalChars || index != l.lineLength) {
                wrongComposeError(lineNumber);
            } else {
                if (PolyStackIsEmpty(*s)) {
                    stackError(lineNumber);
                } else {
                    Poly p = PolyStackPop(s);
                    Poly *polyArr = secureMalloc(STARTING_ARRAY_SIZE *
                            sizeof(Poly));
                    size_t resultSize = 0;
                    bool underflow = false;
                    size_t allocatedSize = STARTING_ARRAY_SIZE;

                    while (!underflow && resultSize < parameter) {
                        if (PolyStackIsEmpty(*s)) {
                            underflow = true;
                            stackError(lineNumber);
                        } else {
                            WritePolyToArray(PolyStackPop(s), &polyArr,
                                             &resultSize, &allocatedSize);
                        }
                    }

                    if (!underflow) {
                        PolyStackPush(s, PolyCompose(&p, resultSize, polyArr));
                        PolyDestroy(&p);
                        for (size_t i = 0; i < resultSize; i++) {
                            PolyDestroy(&(polyArr[i]));
                        }
                    } else {
                        for (size_t i = resultSize; i > 0; i--) {
                            PolyStackPush(s, polyArr[i - 1]);
                        }

                        PolyStackPush(s, p);
                    }

                    free(polyArr);
                }
            }
        }
    }
}

/**
 * Przeprowadza jednoargumentową operację kalkulatora.
 * W przypadku problemów z wykonaniem tej operacji pokazuje odpowiednie błędy.
 * @param[in] s : stos @f$s@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 * @param[in] op : rodzaj operacji @f$op@f$
 */
void onePolyOperation(PolyStack *s, size_t lineNumber, enum
        OneArgumentOperation op) {
    if (PolyStackIsEmpty(*s)) {
        stackError(lineNumber);
    } else {
        Poly p = PolyStackPop(s);
        PolyStackPush(s, p);
        switch (op) {
            case is_coeff:
                printf("%d\n", PolyIsCoeff(&p));
                break;
            case is_zero:
                printf("%d\n", PolyIsZero(&p));
                break;
            case clone:
                PolyStackPush(s, PolyClone(&p));
                break;
            case neg:
                PolyStackPop(s);
                PolyStackPush(s, PolyNeg(&p));
                PolyDestroy(&p);
                break;
            case deg:
                printf("%d\n", PolyDeg(&p));
                break;
            case print:
                PolyPrint(&p);
                break;
            case pop:
                PolyStackPop(s);
                PolyDestroy(&p);
                break;
            default:;
                break;
        }
    }
}

/**
 * Przeprowadza dwuargumentową operację kalkulatora.
 * W przypadku problemów z wykonaniem tej operacji pokazuje odpowiednie błędy.
 * @param[in] s : stos @f$s@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 * @param[in] op : rodzaj operacji @f$op@f$
 */
void twoPolyOperation(PolyStack *s, size_t lineNumber, enum
TwoArgumentOperation op) {
    if (PolyStackIsEmpty(*s)) {
        stackError(lineNumber);
    } else {
        Poly p1 = PolyStackPop(s);
        if (PolyStackIsEmpty(*s)) {
            stackError(lineNumber);
            PolyStackPush(s, p1);
        } else {
            Poly p2 = PolyStackPop(s);
            Poly result;
            switch (op) {
                case add: result = PolyAdd(&p2, &p1);
                break;
                case mul: result = PolyMul(&p2, &p1);
                break;
                case sub: result = PolySub(&p2, &p1);
                break;
                case is_eq:
                    printf("%d\n", PolyIsEq(&p2, &p1));
                break;
                default:;
                break;
            }
            if (op == is_eq) {
                PolyStackPush(s, p2);
                PolyStackPush(s, p1);
            } else {
                PolyStackPush(s, result);
                PolyDestroy(&p1);
                PolyDestroy(&p2);
            }
        }
    }
}

/**
 * Rozpoznaje oraz zleca operację kalkulatora funkcjom podrzędnym.
 * @param[in] s : stos @f$s@f$
 * @param[in] l : linia @f$l@f$
 * @param[in] lineNumber : numer linii @f$lineNumber@f$
 */
void executeCommand(PolyStack *s, Line l, size_t lineNumber) {
    if (LineBeginsWith(l, AT)) {
        at(s, l, lineNumber);
    } else if (LineBeginsWith(l, ADD)) {
        twoPolyOperation(s, lineNumber, add);
    } else if (LineBeginsWith(l, MUL)) {
        twoPolyOperation(s, lineNumber, mul);
    } else if (LineBeginsWith(l, NEG)) {
        onePolyOperation(s, lineNumber, neg);
    } else if (LineBeginsWith(l, SUB)) {
        twoPolyOperation(s, lineNumber, sub);
    } else if (LineBeginsWith(l, DEG) && l.lineLength == 3) {
        onePolyOperation(s, lineNumber, deg);
    } else if (LineBeginsWith(l, POP)) {
        onePolyOperation(s, lineNumber, pop);
    } else if (LineBeginsWith(l, ZERO)) {
        zero(s);
    } else if (LineBeginsWith(l, CLONE)) {
        onePolyOperation(s, lineNumber, clone);
    } else if (LineBeginsWith(l, IS_EQ)) {
        twoPolyOperation(s, lineNumber, is_eq);
    } else if (LineBeginsWith(l, PRINT)) {
        onePolyOperation(s, lineNumber, print);
    } else if (LineBeginsWith(l, DEG_BY)) {
        degBy(s, l, lineNumber);
    } else if (LineBeginsWith(l, COMPOSE)) {
        compose(s, l, lineNumber);
    }else if (LineBeginsWith(l, IS_ZERO)) {
        onePolyOperation(s, lineNumber, is_zero);
    } else if (LineBeginsWith(l, IS_COEFF)) {
        onePolyOperation(s, lineNumber, is_coeff);
    }
}

/**
 * Przeprowadza ciąg czynności charakterystycznych dla kalkulatora.
 * @return 0 jeśli wszystko przebiegło pomyślnie, 1 wpp.
 */
int main() {
    PolyStack s = PolyStackInit();
    size_t lineNumber = 1;

    while (IsNextLine()) {
        Line nextLine = LineRead();

        if (ShouldIgnoreLine(nextLine)) {
        } else if (LineIsCommand(nextLine)) {
            if (LineIsValidCommand(nextLine)) {
                executeCommand(&s, nextLine, lineNumber);
            } else {
                wrongCommandError(lineNumber);
            }
        } else if (PolyIsCorrect(nextLine)) {
            PolyStackPush(&s, PolyRead(nextLine));
        } else {
            wrongPolyError(lineNumber);
        }

        LineDestroy(nextLine);
        lineNumber++;
    }

    PolyStackDestroy(&s);
}