# recursive rare polynomial calculator
Kalkulator wykonuje następujące polecenia:

Wielomian reprezentujemy jako stałą, jednomian lub sumę jednomianów. Stała jest liczbą całkowitą. 

Jednomian reprezentujemy jako parę (coeff,exp), gdzie współczynnik coeff jest wielomianem, a wykładnik exp jest liczbą nieujemną. 

Do wyrażenia sumy używamy znaku +. Jeśli wiersz zawiera wielomian, to program wstawia go na stos. Przykłady poprawnych wielomianów:

0

1

-2

(0,0)

(1,0)

(-2,0)

(1,1)

(1,0)+(1,2)

(1,2)+(1,0)

(1,2)+(-1,2)

(1,2)+(-2,2)

((1,2),15)+(-7,8)

(3,1)+(((4,4),100),2)

ZERO – wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru;

IS_COEFF – sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem – wypisuje na standardowe wyjście 0 lub 1;

IS_ZERO – sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru – wypisuje na standardowe wyjście 0 lub 1;

CLONE – wstawia na stos kopię wielomianu z wierzchołka;

ADD – dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę;

MUL – mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn;

NEG – neguje wielomian na wierzchołku stosu;

SUB – odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę;

IS_EQ – sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje na standardowe wyjście 0 lub 1;

DEG – wypisuje na standardowe wyjście stopień wielomianu (−1 dla wielomianu tożsamościowo równego zeru);

DEG_BY idx – wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze idx (−1 dla wielomianu tożsamościowo równego zeru);

AT x – wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji;

PRINT – wypisuje na standardowe wyjście wielomian z wierzchołka stosu;

POP – usuwa wielomian z wierzchołka stosu.

COMPOSE k - Polecenie to zdejmuje z wierzchołka stosu najpierw wielomian p, a potem kolejno wielomiany q[k - 1], q[k - 2], …, q[0] i umieszcza na stosie wynik operacji złożenia.
