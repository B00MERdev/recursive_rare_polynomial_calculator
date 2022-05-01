/** @file
  Realizacja biblioteki wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include "poly.h"
#include "data_structures.h"
#include <stdlib.h>

/**
 * Dodaje dwa wielomiany, ale przejmuje je na własność.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
Poly PolyAddH(Poly *p, Poly *q);


/**
 * Neguje jednomian, ale przejmuje go na własność.
 * @param[in] m : jednomian @f$m@f$
 * @return @f$-m@f$
 */
Mono MonoNeg(Mono *m);

void PolyDestroy(Poly *p) {
    if (p->arr != NULL) {
        for (size_t i = 0; i < p->size; i++) {
            MonoDestroy(&p->arr[i]);
        }
        free(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    Poly result;
    if (p->arr != NULL) {
        result.size = p->size;
        result.arr = secureMalloc(result.size * sizeof(Mono));

        for (size_t i = 0; i < result.size; ++i) {
            result.arr[i] = MonoClone(&p->arr[i]);
        }
    } else { // p->arr == NULL
        result.arr = NULL;
        result.coeff = p->coeff;
    }

    return result;
}

/**
 * Dodaje dwa jednomiany.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m + n@f$
 */
Mono MonoAdd(Mono *m, Mono *n) {
    assert(MonoGetExp(m) == MonoGetExp(n));
    return (Mono) {.p = PolyAddH(&m->p, &n->p), .exp = m->exp};
}

/**
 * Dodaje dwa wielomiany, które są współczynnikami, przejmuje je na własność
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
Poly PolyAddCoeffs(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return (Poly) {.arr = NULL, .coeff = p->coeff + q->coeff};
}

/**
 * Wpisuje jednomian @p m na koniec tablicy @p monoArr.
 * Jeśli niezaalokowano jeszcze pamięci, to ją alokuje.
 * @param[in] m : jednoomian @f$m@f$
 * @param[in] i : index @f$i@f$
 * @param[in] arr : tablica jednomianów @f$arr@f$
 * @param[in] sizeToAllocate : liczba jednomianów do zaalokowania pamięci
 * @f$sizeToAllocate@f$
 * @return @f$monoArr@f$
 */
void WriteMonoToPoly(const Mono *m, size_t *i, Mono **arr,
                     size_t sizeToAllocate) {
    if (*arr == NULL) {
        *arr = secureMalloc(sizeToAllocate * sizeof(Mono));
    }

    (*arr)[*i] = *m;
    (*i)++;
}

/**
 * Usuwa jednomiany równe zero z tablicy @p monoArr
 * @param[in] arr : tablica jednomianów @f$arr@f$
 * @param[in] size : długość tablicy @f$size@f$
 * @param[in] resultSize : indeks początkowy @f$resultSize@f$
 */
void ClearZerosFromMonoArray(Mono *arr, size_t size, size_t *resultSize) {
    (*resultSize) = 0;
    for (size_t i = 0; i < size; i++) {
        if (!MonoIsZero(&(arr[i]))) {
            arr[*resultSize] = arr[i];
            (*resultSize)++;
        }
    }
}

/**
 * Dodaje dwa wielomiany, z których jeden jest spółczynnikiem, a drugi nie.
 * Funkcja przejmuje je na własność.
 * @param[in] c : wielomian będący współczynnikiem @f$c@f$
 * @param[in] n : wielomian nie będący współczynnikiem @f$n@f$
 * @return @f$c + n@f$
 */
Poly PolyAddCoeffAndNonCoeff (Poly *c, Poly *n) {
    assert(PolyIsCoeff(c) && !PolyIsCoeff(n));

    if (PolyIsZero(c)) {
        return *n;
    }

    size_t sizeToAllocate = n->size + 1;
    size_t ni = 0; // nonCoeff index
    size_t ri = 0; // result index
    Mono *arr = NULL;
    Mono cMono = MonoFromPoly(c, 0);

    if (MonoGetExp(&n->arr[0]) == 0) {
        n->arr[0] = MonoAdd(&n->arr[0], &cMono);

        ClearZerosFromMonoArray(n->arr, n->size, &ri);

        if (ri == 0) {
            free(n->arr);

            return PolyZero();
        } else if (ri == 1 && n->arr[0].exp == 0 && PolyIsCoeff(&(n->arr[0])
        .p)) {
            Poly result = PolyFromCoeff(n->arr[0].p.coeff);
            free(n->arr);

            return result;
        } else {
            n->size = ri;

            return *n;
        }

    } else {
        WriteMonoToPoly(&cMono, &ri, &arr, sizeToAllocate);

        while (ni < n->size) {
            WriteMonoToPoly(&n->arr[ni], &ri, &arr, sizeToAllocate);
            ni++;
        }

        free(n->arr);

        if (arr != NULL) {
            Poly result;

            if (ri == 1 && arr[0].exp == 0 && PolyIsCoeff(&arr[0].p)) {
                result = PolyFromCoeff(arr[0].p.coeff);
                free(arr);
            } else {
                result.size = ri;
                result.arr = arr;
            }

            return result;
        } else {

            return PolyZero();
        }
    }
}

/**
 * Sprawdza, czy wielomian @p p ma po co najmniej jednym wyrazie o wykładniku
 * równym każdemu wykładnikowi w wielomianie @p q.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$@p p ma po co najmniej jednym wyrazie o wykładniku
 * równym każdemu wykładnikowi w wielomianie @p q@f$
 */
bool PHasAllExpThatQHas(const Poly *p, const Poly *q) {
    size_t pi = 0;
    size_t qi = 0;
    while (qi < q->size && pi < p->size) {
        if (q->arr[qi].exp == p->arr[pi].exp) {
            if (qi < q->size) {
                qi++;
            }
        }
        pi++;
    }

    return qi == q->size;
}

/**
 * Dodaje wielomian @p q do wielomianu @p p, a następnie usuwa wielomian @p q
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
Poly PEatsQ(Poly *p, const Poly *q) {
    size_t pi = 0;
    size_t qi = 0;

    while (qi < q->size) {
        if (p->arr[pi].exp == q->arr[qi].exp) {
            p->arr[pi] = MonoAdd(&(p->arr[pi]), &(q->arr[qi]));
            qi++;
        }
        pi++;
    }

    free(q->arr);

    size_t resultSize = 0;

    ClearZerosFromMonoArray(p->arr, p->size, &resultSize);

    if (resultSize == 0) {
        free(p->arr);

        return PolyZero();
    } else if (resultSize == 1 && p->arr[0].exp == 0 && PolyIsCoeff(&(p->arr[0]
    .p))) {
        Poly result = PolyFromCoeff(p->arr[0].p.coeff);
        free(p->arr);

        return result;
    } else {
        p->size = resultSize;

        return *p;
    }
}

/**
 * Dodaje dwa wielomiany, które nie są współczynnikami, przejmuje je na własność
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p + q@f$
 */
Poly PolyAddNonCoeffs (Poly *p, Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    size_t sizeToAllocate = p->size + q->size;
    Mono *arr = NULL;
    size_t pi = 0; // p_index
    size_t qi = 0; // q_index
    size_t ri = 0; // result_index

    if (PHasAllExpThatQHas(p, q)) {

        return PEatsQ(p, q);
    } else if (PHasAllExpThatQHas(q, p)) {

        return PEatsQ(q, p);
    }

    while (pi < p->size && qi < q->size) {
        if (MonoGetExp(&p->arr[pi]) < MonoGetExp(&q->arr[qi])) {
            WriteMonoToPoly(&p->arr[pi], &ri, &arr, sizeToAllocate);
            pi++;
        } else if (MonoGetExp(&p->arr[pi]) > MonoGetExp(&q->arr[qi])) {
            WriteMonoToPoly(&q->arr[qi], &ri, &arr, sizeToAllocate);
            qi++;
        } else {
            Mono afterAddition = MonoAdd(&p->arr[pi], &q->arr[qi]);

            if (!MonoIsZero(&afterAddition)) {
                WriteMonoToPoly(&afterAddition, &ri, &arr, sizeToAllocate);
            }
            pi++;
            qi++;
        }
    }

    while (pi < p->size) {
        WriteMonoToPoly(&p->arr[pi], &ri, &arr, sizeToAllocate);
        pi++;
    }

    while (qi < q->size) {
        WriteMonoToPoly(&q->arr[qi], &ri, &arr, sizeToAllocate);
        qi++;
    }

    free(p->arr);
    free(q->arr);

    if (arr != NULL) {
        Poly result;

        if (ri == 1 && arr[0].exp == 0 && PolyIsCoeff(&arr[0].p)) {
            result = PolyFromCoeff(arr[0].p.coeff);
            free(arr);
        } else {
            result.size = ri;
            result.arr = arr;
        }

        return result;
    } else {

        return PolyZero();
    }
}

Poly PolyAddH(Poly *p, Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {

        return PolyAddCoeffs(p, q);
    } else {
        if (PolyIsCoeff(p)) {

            return PolyAddCoeffAndNonCoeff(p, q);
        } else if (PolyIsCoeff(q)) {

            return PolyAddCoeffAndNonCoeff(q, p);
        } else {

            return PolyAddNonCoeffs(p, q);
        }
    }
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly pCopy = PolyClone(p);
    Poly qCopy = PolyClone(q);

    return PolyAddH(&pCopy, &qCopy);
}

/**
 * Porównuje dwa jednomiany.
 * Jeśli wykładnik jednomianu @p m jest mniejszy, niż wykładnik jednomianu @p n,
 * to zwraca -1, i odwrotnie, a jeśli oba jednomiany mają taki sam wykładnik, to
 * zwraca 0.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$Jeśli wykładnik @p m jest mniejszy, niż wykładnik @p n,
 * to zwraca -1, i odwrotnie, a jeśli oba jednomiany mają taki sam wykładnik, to
 * zwraca 0.@f$
 */
int CompareMonos(const void *m, const void *n) {
    if (MonoGetExp(m) < MonoGetExp(n)) {

        return -1;
    } else if (MonoGetExp(m) > MonoGetExp(n)) {

        return 1;
    } else {

        return 0;
    }
}

/**
 * Łączy za sobą jednomiany o tych samych wykładnikach z tablicy @p monos.
 * @param[in] count : długość tablicy jednomianów przed połączeniem@f$count@f$
 * @param[in] resultSize : długość tablicy jednomianów po połączeniu
 * @f$resultSize@f$
 * @param[in] monos : tablica jednomianów @f$monos@f$
 * @return @f$monos@f$
 */
Mono* MonosMerge(size_t count, size_t *resultSize, Mono *monos) {
    size_t i = 0;
    Mono toInsert;

    while (i < count) {
        toInsert = monos[i];
        i++;

        while (i < count && MonoGetExp(&toInsert) == MonoGetExp(&(monos[i]))) {
            toInsert = MonoAdd(&toInsert, &(monos[i]));
            i++;
        }

        if (!MonoIsZero(&toInsert)) {
            monos[*resultSize] = toInsert;
            (*resultSize)++;
        }
    }

    return monos;
}

/**
 * Wykonuje pewną operację na tablicy jednomianów, tworząc wielomian.
 * @param[in] count : długość tablicy jednomianów @f$count@f$
 * @param[in] monos : tablica jednomianów @f$monos@f$
 * @param[in] ifClone : decyduje czy robić głęboką kopię jednomianów z tablicy
 * @f$ifClone@f$
 * @return @f$ Jeśli @p ifClone to 1, to robi głębokie kopie jednomianów, a
 * następnie je do siebie dodaje. Jeśli @p ifClone to 0, to robi zamiast
 * głebokich płytkie kopie.@f$
 */
Poly PolySthMonos(size_t count, const Mono monos[], bool ifClone) {
    if (count == 0 || monos == NULL) {
        return PolyZero();
    }

    Mono *monosCopy = secureMalloc(count * sizeof(Mono));

    for (size_t i = 0; i < count; i++) {
        if (ifClone) {
            monosCopy[i] = MonoClone(&(monos[i]));
        } else {
            monosCopy[i] = monos[i];
        }
    }

    return PolyOwnMonos(count, monosCopy);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {

    return PolySthMonos(count, monos, false);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {

    return PolySthMonos(count, monos, true);
}

Poly PolyOwnMonos(size_t count, Mono *monosCopy) {

    qsort(monosCopy, count, sizeof(Mono), CompareMonos);

    size_t resultSize = 0;
    monosCopy = MonosMerge(count, &resultSize, monosCopy);

    if (resultSize == 0) {
        free(monosCopy);

        return PolyZero();
    } else {
        Poly result;

        if (resultSize == 1 && monosCopy[0].exp == 0 && PolyIsCoeff
                (&monosCopy[0].p)) {
            result = PolyFromCoeff(monosCopy[0].p.coeff);
            free(monosCopy);
        } else {
            result.size = resultSize;
            result.arr = monosCopy;
        }

        return result;
    }
}

/**
 * Mnoży dwa jednomiany oraz przejmuje je na własność.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m * n@f$
 */
Mono MonoMul(const Mono *m, const Mono *n) {
    if (MonoIsZero(m) || MonoIsZero(n)) {
        Poly result = PolyZero();

        return MonoFromPoly(&result, 0);
    }

    return (Mono) {.p = PolyMul(&m->p, &n->p), .exp = m->exp + n->exp};
}

/**
 * Mnoży dwa wielomiany będące współczynnikami oraz przejmuje je na własność.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p * q@f$
 */
Poly PolyMulCoeffs(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return (Poly) {.coeff = p->coeff * q->coeff, .arr = NULL};
}

/**
 * Mnoży dwa wielomiany, z których jeden jest współczynnikiem, a drugi nie,
 * oraz przejmuje je na własność.
 * @param[in] c : wielomian będący współczynnikiem @f$c@f$
 * @param[in] n : wielomian niebędący współczynnikiem @f$n@f$
 * @return @f$c * n@f$
 */
Poly PolyMulCoeffAndNonCoeff(const Poly *c, const Poly *n) {
    assert(PolyIsCoeff(c) && !PolyIsCoeff(n));

    if (PolyIsZero(c)) {

        return PolyZero();
    }

    Poly cCopy = PolyClone(c);
    Mono cMono = MonoFromPoly(&cCopy, 0);
    Poly nCopy = PolyClone(n);
    Mono holder;
    size_t resultI = 0;

    for (size_t i = 0; i < n->size; i++) {
        holder = MonoMul(&cMono, &nCopy.arr[i]);
        MonoDestroy(&nCopy.arr[i]);

        if (!MonoIsZero(&holder)) {
            nCopy.arr[resultI] = holder;
            resultI++;
        }
    }

    if (resultI == 0) {
        free(nCopy.arr);

        return PolyZero();
    } else {
        nCopy.size = resultI;

        return nCopy;
    }
}

/**
 * Mnoży dwa wielomiany niebędące współczynnikami oraz przejmuje je na własność.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p * q@f$
 */
Poly PolyMulNonCoeffs(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    Mono *result = NULL;
    Mono holder;
    size_t resultI = 0;

    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            holder = MonoMul(&p->arr[i], &q->arr[j]);

            if (!MonoIsZero(&holder)) {

                if (result == NULL) {
                    result = secureMalloc(p->size * q->size * sizeof(Mono));
                }

                result[resultI] = holder;
                resultI++;
            }
        }
    }

    if (result != NULL) {
        Poly afterReduction = PolyAddMonos(resultI, result);
        free(result);

        return afterReduction;
    } else {

        return PolyZero();
    }
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {

        return PolyMulCoeffs(p, q);
    } else {
        if (PolyIsCoeff(p)) {

            return PolyMulCoeffAndNonCoeff(p, q);
        } else if (PolyIsCoeff(q)) {

            return PolyMulCoeffAndNonCoeff(q, p);
        } else {

            return PolyMulNonCoeffs(p, q);
        }
    }
}

/**
 * Sprawdza, czy jednomiany mają te same wartości.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m == n@f$
 */
bool MonoIsEq(const Mono *m, const Mono *n) {
    return PolyIsEq(&m->p, &n->p) && m->exp == n->exp;
}

/**
 * Sprawdza czy dwa wielomiany będące współczynnikami są równe.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p == q@f$
 */
bool PolyIsEqCoeffs(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return p->coeff == q->coeff;
}

/**
 * Sprawdza czy dwa wielomiany niebędące współczynnikami są równe.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p == q@f$
 */
bool PolyIsEqNonCoeffs(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));

    if (p->size == q->size) {

        for (size_t i = 0; i < p->size; i++) {

            if (!MonoIsEq(&p->arr[i], &q->arr[i])) {

                return false;
            }
        }

        return true;
    } else {

        return false;
    }
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {

        return PolyIsEqCoeffs(p, q);
    } else if (!PolyIsCoeff(p) && !PolyIsCoeff(q)) {

        return PolyIsEqNonCoeffs(p, q);
    } else {

        return false;
    }
}

/**
 * Neguje wartość wielomianu oraz przejmuje go na własność.
 * @param[in] pCopy : wielomian @f$pCopy@f$
 * @return @f$-pCopy@f$
 */
Poly PolyNegH(Poly *pCopy) {
    if (PolyIsCoeff(pCopy)) {
        pCopy->coeff = (-1) * pCopy->coeff;
    } else {
        for (size_t i = 0; i < pCopy->size; i++) {
            pCopy->arr[i] = MonoNeg(&pCopy->arr[i]);
        }
    }

    return *pCopy;
}

Mono MonoNeg(Mono *m) {

    return (Mono) {.p = PolyNegH(&m->p), .exp = m->exp};
}

Poly PolyNeg(const Poly *p) {
    Poly pCopy = PolyClone(p);

    return PolyNegH(&pCopy);
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly pCopy = PolyClone(p);
    Poly qCopy = PolyNeg(q);

    return PolyAddH(&pCopy, &qCopy);
}

/**
 * Zwraca stopień wielomianu jednomianu ze względu na zadaną zmienną (-1 dla
 * wielomianu
 * tożsamościowo równego zeru). Zmienne indeksowane są od 0.
 * Zmienna o indeksie 0 oznacza zmienną główną tego wielomianu.
 * Większe indeksy oznaczają zmienne wielomianów znajdujących się
 * we współczynnikach.
 * @param[in] m : jednomian
 * @param[in] var_idx : indeks zmiennej
 * @return stopień wielomianu jednomianu @p m z względu na zmienną o indeksie
 * @p var_idx
 */
poly_exp_t MonoDegBy(const Mono *m, size_t var_idx) {
    if (var_idx == 0) {
        return m->exp;
    } else {
        return PolyDegBy(&m->p, var_idx - 1);
    }
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p)) {

        return -1;
    }

    if (PolyIsCoeff(p)) {

        return 0;
    } else {
        poly_exp_t result = 0;

        for (size_t i = 0; i < p->size; i++) {
            poly_exp_t holder = MonoDegBy(&p->arr[i], var_idx);

            if ( holder > result) {
                result = holder;
            }
        }

        return result;
    }
}

/**
 * Zwraca stopień wielomianu jednomianu (-1 dla wielomianu tożsamościowo
 * równego zeru).
 * @param[in] m : jednomian
 * @return stopień wielomianu jednomianu @p m
 */
poly_exp_t MonoDeg(const Mono *m) {
    return PolyDeg(&m->p) + m->exp;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) {

        return -1;
    }

    if (PolyIsCoeff(p)) {

        return 0;
    } else {
        poly_exp_t result = 0;

        for (size_t i = 0; i < p->size; i++) {
            poly_exp_t holder = MonoDeg(&p->arr[i]);

            if ( holder > result) {
                result = holder;
            }
        }

        return result;
    }
}

/**
 * Podnosi @p x do potęgi @p n.
 * @param[in] x : zmienna @f$x@f$
 * @param[in] n : wykładnik @f$n@f$
 * @return @f$x^n@f$
 */
poly_coeff_t Power(poly_coeff_t x, poly_exp_t n) {
    if (x == 0) {
        return 0;
    }

    poly_coeff_t result = 1;
    for (poly_exp_t i = 0; i < n; i++) {
        result *= x;
    }

    return result;
}

/**
 * Wylicza wartość jednomianu w zadanym pukcie i zapisuje pod zmienną @p result
 * @param[in] m : jednomian @f$m@f$
 * @param[in] x : zmienna @f$x@f$
 * @param[in] result : wynik @f$result@f$
 */
void MonoAt(const Mono *m, poly_coeff_t x, Poly *result) {
    poly_coeff_t newCoeff = Power(x, m->exp);
    Poly polyFromCoeff = PolyFromCoeff(newCoeff);
    Poly afterMul = PolyMul(&m->p, &polyFromCoeff);
    Poly holder = PolyAdd(result, &afterMul);
    PolyDestroy(&afterMul);
    PolyDestroy(result);
    (*result) = holder;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsZero(p) || x == 0) {

        return PolyZero();
    } else {
        if (PolyIsCoeff(p)) {

            return PolyClone(p);
        } else {
        Poly result = PolyZero();

            for (size_t i = 0; i < p->size; i++) {
                MonoAt(&p->arr[i], x, &result);
            }

            return result;
        }
    }
}

/**
 * Podstawia wielomiany q1, q2, q3, ... qk pod kolejne zmienne w wielomianie
 * p, a także przejmuje je na własność. Jest funkcją pomocniczą funkcji
 * PolyCompose
 * @param[in] p : wielomian @f$p@f$
 * @param[in] k : długość tablicy wielomianów @f$k@f$
 * @param[in] q : tablica wielomianów @f$q@f$
 * @return @f$p(q1, q2, q3 ... qk)@f$
 */
Poly PolyComposeH(Poly *p, size_t k, const Poly *q);

/**
 * Podstawia wielomian qk pod zmienną w jednomianie @p m.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] k : długość tablicy wielomianów @f$k@f$
 * @param[in] q : tablica wielomianów @f$q@f$
 * @return @f$p * qk^exp@f$
 */
Poly MonoCompose(Mono *m, size_t k, const Poly *q) {
    if (k == 0) {

        return PolyZero();
    }

    Poly deeperPoly = PolyComposeH(&m->p, k - 1, q);
    Poly toMul = PolyFromCoeff(1);
    Poly toMulStartingCopy = PolyClone(&(q[k - 1]));
    Poly holder;
    bool toMulInitialized = false;

    for (size_t i = 0; i < (size_t)(m->exp); i++) {
        if (!toMulInitialized) {
            toMul = PolyClone(&(q[k - 1]));
            toMulInitialized = true;
        } else {
            holder = PolyMul(&toMul, &toMulStartingCopy);
            PolyDestroy(&toMul);
            toMul = holder;
        }
    }

    PolyDestroy(&toMulStartingCopy);

    Poly result = PolyMul(&deeperPoly, &toMul);
    PolyDestroy(&deeperPoly);
    PolyDestroy(&toMul);

    return result;
}

/**
 * Sumuje listę wielomianów i tworzy z nich wielomian.
 * Przejmuje na własność zawartość tablicy @p polyArr.
 * @param[in] polyArr : tablica wielomianów
 * @param[in] size : długość tablicy wielomianów
 * @return wielomian będący sumą wielomianów
 */
Poly PolyAddPolys(Poly *polyArr, size_t size) {
    Poly result = PolyZero();

    for (size_t i = 0; i < size; i++) {
        result = PolyAddH(&result, &polyArr[i]);
    }

    free(polyArr);

    return result;
}

Poly PolyComposeH(Poly *p, size_t k, const Poly *q) {
    if (PolyIsCoeff(p)) {

        return *p;
    } else {
        Poly *polyArr = secureMalloc(p->size * sizeof(Poly));

        for (size_t i = 0; i < p->size; i++) {
            polyArr[i] = MonoCompose(&(p->arr[i]), k, q);
        }

        return PolyAddPolys(polyArr, p->size);
    }
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    Poly pCopy = PolyClone(p);

    Poly result = PolyComposeH(&pCopy, k, q);

    PolyDestroy(&pCopy);

    return result;
}