#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 1
#define S 2
#define W 3
#define E 4

/* INPUT:
 - PIERWSZA LINIA WYMIARY TABLICY: LKOLUMN LWIERSZY
 - TABLICA ZER I JEDYNEK O PODANYCH WYMIARACH (0-droga, 1-sciana)
 - WSPOLRZEDNE STARTU
 - WSPOLRZEDNE METY
 */
 
int xstart, ystart; //wspolrzedne startu
int xmeta, ymeta; //wspolrzedne mety
int meta; //zakodowane wspolrzedne mety wraz z kierunkiem wejscia
int lwierszy, lkolumn; 
int lwierzcholkow; //wierzcholki grafu
int **Labirynt; //mapa labiryntu
int **Incydencje; //macierz incydencji
int *Odwiedziny; //tablica odwiedzin

typedef struct para_t {
    int x;
    int y;
} para;

/* LISTA */
typedef struct node {
    int val;
    struct node *next;
} node_t;

node_t *nowa_lista() {
    return NULL;
}

void push_back(node_t **head, int v) {
    if (*head == NULL) {
        *head = (node_t *) malloc(sizeof(node_t));
        (*head)->val = v;
        (*head)->next = NULL;
    } else {
        node_t *new_head = *head;

        while (new_head->next != NULL) {
            new_head = new_head->next;
        }

        new_head->next = (node_t *) malloc(sizeof(node_t));
        new_head->next->val = v;
        new_head->next->next = NULL;
    }
}

void push_front(node_t **head, int v) {
    node_t *new_head;
    new_head = malloc(sizeof(node_t));
    new_head->val = v;
    new_head->next = *head;
    *head = new_head;
}

void pop_front(node_t **head) {
    node_t *a = NULL;

    if (*head != NULL) {
        a = (*head)->next;
        free(*head);
        *head = a;
    }
}

bool czypusta(node_t *head) {
    if (head == NULL)
        return true;
    else
        return false;
}

void wypisz(node_t *head) {

    while (head != NULL) {
        printf("%d ", (*head).val);
        head = (*head).next;
    }
}

int podpatrz(node_t *head) {
    if (head != NULL) {
        return head->val;
    } else return -1;
}

/* KONIEC LISTY */

//funkcje koduj i dekoduj to powiązanie wspolrzednych
//na tablicach Incydencje, Odwiedziny oraz Labirynt (input)
para dekoduj(int i) {
    para wspolrzedne;

    wspolrzedne.x = (i / 4) % lkolumn;
    wspolrzedne.y = i / (4 * lkolumn);

    return wspolrzedne;
}

int koduj(int kierunek, int x, int y) {
    return y * lkolumn * 4 + 4 * x + kierunek - 1;
}

int **utworz_tablica(int x, int y) {
    int **tab = malloc(x * sizeof(int *));
    int i = 0;
    while (i < x) {
        tab[i] = calloc(1, y * sizeof(int));
        i++;
    }
    return tab;
}

//odczytuje input
void OdczytajDane2(FILE *F) {
    int iterx = 0, itery = 0;
    fscanf(F, "%d %d", &lkolumn, &lwierszy);
    Labirynt = utworz_tablica(lkolumn, lwierszy);
    lwierzcholkow = 4 * lkolumn * lwierszy;
    while (itery < lwierszy) {
        iterx = 0;
        while (iterx < lkolumn) {
            fscanf(F, " %d", &Labirynt[iterx][itery]);
            iterx++;
        }
        itery++;
    }
    fscanf(F, "%d %d", &xstart, &ystart);
    fscanf(F, "%d %d", &xmeta, &ymeta);
}

int bool_to_int(bool b) {
    if (b) {
        return 1;
    } else
        return 0;
}

//tworzenie macierzy incydencji i tablicy odwiedzin
//tablica odwiedzin pelni jednoczesnie tablicy w ktorej trzymam odleglosc od startu
//kierowca wjezdza z pola na osi X do pola na osi Y
//kierunekx - kierunek ustawienia kierowcy przed "skretem"
//kieruneky - kierunek ustawienia kierowcy po "skrecie"
//np. Incydencje[0][4*lkolumn*1]=1 zgodnie z kodowaniem oznacza
//mozliwosc przejazdu z pola <0,0> N do pola <0,1> N 
void init() {
    int iterx = 0, iterx_X = 0, iterx_Y = 0;
    int itery = 0, itery_X = 0, itery_Y = 0;
    int kierunekx = N, kieruneky = N;
    Incydencje = utworz_tablica(lwierzcholkow, lwierzcholkow);
    Odwiedziny = calloc(1, lwierzcholkow * sizeof(int));
    while (itery < lwierzcholkow) {
        while (iterx < lwierzcholkow) {
            if (Labirynt[iterx_X][iterx_Y] == 0 && Labirynt[itery_X][itery_Y] == 0) {
                if (kierunekx == N) {
                    if (kieruneky == N) {
                        Incydencje[iterx][itery] = bool_to_int(itery_X == iterx_X && itery_Y - 1 == iterx_Y);
                    }
                    if (kieruneky == E) {
                        Incydencje[iterx][itery] = bool_to_int((itery_Y == iterx_Y && itery_X + 1 == iterx_X));
                    }
                } else if (kierunekx == S) {
                    if (kieruneky == S) {
                        Incydencje[iterx][itery] = bool_to_int((itery_X == iterx_X && itery_Y + 1 == iterx_Y));
                    }
                    if (kieruneky == W) {
                        Incydencje[iterx][itery] = bool_to_int((itery_Y == iterx_Y && itery_X - 1 == iterx_X));
                    }
                } else if (kierunekx == W) {
                    if (kieruneky == N) {
                        Incydencje[iterx][itery] = bool_to_int(itery_X == iterx_X && itery_Y - 1 == iterx_Y);
                    }
                    if (kieruneky == W) {
                        Incydencje[iterx][itery] = bool_to_int((itery_Y == iterx_Y && itery_X - 1 == iterx_X));
                    }

                } else if (kierunekx == E) {
                    if (kieruneky == E) {
                        Incydencje[iterx][itery] = bool_to_int((itery_Y == iterx_Y && itery_X + 1 == iterx_X));
                    }
                    if (kieruneky == S) {
                        Incydencje[iterx][itery] = bool_to_int((itery_X == iterx_X && itery_Y + 1 == iterx_Y));
                    }
                }

            } else {
                Incydencje[iterx][itery] = 0;
            }

            iterx++;
            kierunekx = (kierunekx % 4) + 1;
            if (kierunekx == 1) {
                if (iterx_X + 1 == lkolumn) {
                    iterx_X = 0;
                    iterx_Y++;
                } else {
                    iterx_X++;
                }
            }
        }
        
        iterx_Y = 0;
        iterx_X = 0;
        iterx = 0;
        itery++;
        kieruneky = (kieruneky % 4) + 1;
        if (kieruneky == 1) {
            if (itery_X + 1 == lkolumn) {
                itery_X = 0;
                itery_Y++;
            } else {
                itery_X++;
            }
        }
    }
}

void WypiszTab(int **T, int liczba_kolumn, int liczba_wierszy) {
    int x = 0, y = 0;
    while (y < liczba_wierszy) {
        while (x < liczba_kolumn) {
            printf("%d ", T[x][y]);
            x++;
        }
        printf("\n");
        x = 0;
        y++;
    }
}

//ustawia "kierowce" na starcie
// np. kierunek N oznacza, ze kierowca przyjechal z polnocy
// moze się po tablicy przesunac na pole <x-1,y> (skrecic w prawo)
//lub na pole <x,y+1> (pojechac prosto)
int start() {
    if (ystart == 0) return N;
    else if (ystart == lwierszy - 1) return S;
    else if (xstart == 0) return W;
    else if (xstart == lkolumn - 1) return E;
    else return N;
}

bool bfs() {
    int u;
    node_t *queue;
    queue = nowa_lista();
    int kierunek_startowy = start();
    int s = koduj(kierunek_startowy, xstart, ystart);
    push_back(&queue, s);
    bool znaleziono = false;
    para czyMeta;
    int i;

    Odwiedziny[s] = 1;

    while (!czypusta(queue) && !znaleziono) {
        u = podpatrz(queue);
        pop_front(&queue);
        czyMeta = dekoduj(u);

        if (czyMeta.x == xmeta && czyMeta.y == ymeta) {
            znaleziono = true;
        } else {
            i = 0;
            while (i < lwierzcholkow) {
                if (Incydencje[u][i] == 1 && Odwiedziny[i] == 0) {
                    Odwiedziny[i] = Odwiedziny[u] + 1;
                    push_back(&queue, i);
                }
                i++;
            }
        }
    }
    meta = u;
    return znaleziono;
}

//wypisuje kolejne punkty, ktore "kierowca" musi odwiedzic
//podaje liczbe krokow
void PokazD() {
    node_t *lista;
    lista = nowa_lista();
    int u, s, i, j, x, y;
    int k = start();
    s = koduj(k, xstart, ystart);
    x = xmeta;
    y = ymeta;
    u = meta;
    push_front(&lista, y);
    push_front(&lista, x);
    while (u != s) {
        int j = -(5 * lkolumn);
        while (j < (lkolumn * 5) + 1) {
            if ((u + j) >= 0 && (u + j) < lwierzcholkow) {
                if ((Odwiedziny[u + j] + 1) == Odwiedziny[u] && Incydencje[u + j][u] == 1) {
                    x = dekoduj(u + j).x;
                    y = dekoduj(u + j).y;
                    Labirynt[x][y] = 8;
                    push_front(&lista, y);
                    push_front(&lista, x);
                    u = u + j;
                }
            }
            j++;
        }
    }
    printf("\nNajkrotsza sciezka to %d krokow", (Odwiedziny[meta] - 1));
    printf("\nKolejne kroki to:\n");
    i = podpatrz(lista);
    pop_front(&lista);
    j = podpatrz(lista);
    pop_front(&lista);
    printf("   <%d,%d> start\n", i, j);
    while (!czypusta(lista)) {
        i = podpatrz(lista);
        pop_front(&lista);
        j = podpatrz(lista);
        pop_front(&lista);
        if (i == xmeta && j == ymeta) printf("-> <%d,%d> meta", i, j);
        else printf("-> <%d,%d>\n", i, j);
    }
}

//jak GPS podaje kolejne ruchy jakie wykonac ma "kierowca", zeby dojechac do mety
void GPS() {
    int x, y, i, j, k;
    int s, u;
    k = start();
    node_t *gps;
    gps = nowa_lista();
    int l = start();
    s = koduj(l, xstart, ystart);
    x = xmeta;
    y = ymeta;
    u = meta;
    push_front(&gps, y);
    push_front(&gps, x);
    while (u != s) {
        int j = -(lkolumn * 5);
        while (j < lkolumn * 5) {
            if ((u + j) >= 0 && (u + j) < lwierzcholkow) {
                if ((Odwiedziny[u + j] + 1) == Odwiedziny[u] && Incydencje[u + j][u] == 1) {
                    x = dekoduj(u + j).x;
                    y = dekoduj(u + j).y;
                    push_front(&gps, y);
                    push_front(&gps, x);
                    u = u + j;
                }
            }
            j++;
        }
    }

    x = podpatrz(gps);
    pop_front(&gps);
    y = podpatrz(gps);
    pop_front(&gps);
    printf("\t ***** GPS *****\n");
    printf("\n Twoja lokalizacja to <%d,%d>", xstart, ystart);
    printf("\n Cel podrozy to: <%d,%d>", xmeta, ymeta);
    while (x != xmeta || y != ymeta) {
        i = podpatrz(gps);
        pop_front(&gps);
        j = podpatrz(gps);
        pop_front(&gps);
        if (k == 1) {
            if (y - j == -1) {
                printf("\n jedz prosto");
                k = 1;
                x = i;
                y = j;
            }else {
                printf("\n skrec w prawo");
                k = 4;
                x = i;
                y = j;
            } 
        } else if (k == 2) {
            if (x - i == -1) {
                printf("\n skrec w prawo");
                k = 3;
                x = i;
                y = j;
            } else {
                printf("\n jedz prosto");
                k = 2;
                x = i;
                y = j;
            }
        } else if (k == 3) {
            if (y - j == -1) {
                printf("\n skrec w prawo");
                k = 1;
                x = i;
                y = j;
            } else {
                printf("\n jedz prosto");
                k = 3;
                x = i;
                y = j;
            } 
        } else {
            if (y - j == 1) {
                printf("\n skrec w prawo");
                k = 2;
                x = i;
                y = j;
            } else {
                printf("\njedz prosto");
                k = 4;
                x = i;
                y = j;
            }
        }
    }

    printf("\n Jestes na miejscu!");
}

//mapa z zaznzczonym startem, meta i trasa
void rysujmapa(int len_x, int len_y, int **T) {
    int i = 0;
    int j = 0;
    printf("\n\n\t***** MAPA *****\n\n");
    printf("%c", 201);
    while (i < len_x) {
        printf("%c%c%c", 205, 205, 205);
        i++;
    }
    printf("%c\n", 187);
    while (j < len_y) {
        i = 0;
        printf("%c", 186);
        while (i < len_x) {
            if (i == xstart && j == ystart) {
                printf(" %c ", 83);
            } else if (i == xmeta && j == ymeta) {
                printf(" %c ", 77);
            } else if (T[i][j] == 0) {
                printf(" %c ", 255);
            } else if (T[i][j] == 1) {
                printf(" %c ", 254);
            } else {
                printf(" %c ", 43);
            }
            i++;
        }
        j++;
        printf("%c\n", 186);
    }
    i = 0;
    printf("%c", 200);
    while (i < len_x) {
        printf("%c%c%c", 205, 205, 205);
        i++;
    }
    printf("%c\n", 188);
}

int main() {
	//odczytanie danych wejsciowych i inicjalizacja tablic
    FILE *F;
    F = fopen("input.txt", "r");
    OdczytajDane2(F);
    init();
    fclose(F);
    //wypisanie danych wejsciowych
    printf("\nWymiary tablicy: X: %d na Y: %d\n", lkolumn, lwierszy);
    printf("\n");
    WypiszTab(Labirynt, lkolumn, lwierszy);
    printf("\n");
    printf("start: x: %d   y: %d\n", xstart, ystart);
    printf("meta:  x: %d   y: %d\n", xmeta, ymeta);
    printf("\n");
    //output
    if (!bfs())
        printf("\nNie ma drogi\n");
    else {
        PokazD();
        printf("\n\n");
        GPS();
        rysujmapa(lkolumn, lwierszy, Labirynt);
    }
    return 0;
}

