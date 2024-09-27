#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define width 40
#define height 15
#define height2048 10
#define width2048 13
#define ROW 8
#define COL 8

// functie generare data si ora cu ajutorul bibliotecii time.h
// afisez ora, minutele, secundele, ziua, luna, anul
void generareTimp(void) {
  time_t current_time;
  // dau ora curenta varibilei
  time(&current_time);
  struct tm *local_time = localtime(&current_time);
  if (local_time == NULL) {
    printf("eroare");
    return;
  }
  mvprintw(2, 50, "%02d/%02d/%d\n", local_time->tm_mday, local_time->tm_mon + 1,
           local_time->tm_year + 1900);
  mvprintw(2, 65, "%02d:%02d:%02d\n", local_time->tm_hour, local_time->tm_min,
           local_time->tm_sec);
  refresh();
}

// initializez matrice, adica tabla de joc cu 0
void initializare(int matrice[8][8], int n) {
  int i, j;
  for (i = 1; i < n; i += 2) {
    for (j = 1; j < n; j += 2) {
      matrice[i][j] = 0;
    }
  }
}

void panou_de_control(WINDOW *win, int n, int matrice[8][8]);

// Functie pentru miscarea celulelor, generare random
void celuleMiscare(WINDOW *win, int n, int matrice[8][8]) {
  int linia1, coloana1, linia2, coloana2;
  int i, ch, j, liniaCautata, linieCurenta, coloanaCautata, coloanaCurenta;

  keypad(win, TRUE);

  initializare(matrice, n);

  // generarea valoriilor si a pozitiilor aleatorii
  do {
    linia1 = rand() % ROW;
    coloana1 = rand() % COL;
  } while (linia1 % 2 == 0 || coloana1 % 2 == 0);
  matrice[linia1][coloana1] = (rand() % 4) ? 2 : 4;
  mvwprintw(win, linia1 + 1, coloana1 + 1, "%d", matrice[linia1][coloana1]);
  // generare a doua celula random
  do {
    linia2 = rand() % ROW;
    coloana2 = rand() % COL;
  } while ((linia2 % 2 == 0 || coloana2 % 2 == 0) &&
           (linia1 != linia2 || coloana1 != coloana2));
  matrice[linia2][coloana2] = (rand() % 4) ? 2 : 4;
  mvwprintw(win, linia2 + 1, coloana2 + 1, "%d", matrice[linia2][coloana2]);

  // miscare celule
  while (1) {
    // afisez data si ora
    generareTimp();
    ch = wgetch(win);

    // testez daca sa ies
    if (tolower(ch) == 'q') {
      return;
    }

    // testez in ce sens trebuie sa mut celulele
    switch (ch) {
      case KEY_UP:
        // din 2 in 2, deoarece pe coloanele pare pun |
        for (j = 1; j < n; j += 2) {
          // ultima pozitie unde a avut loc o combinare
          liniaCautata = 1;
          for (i = 3; i < n; i += 2) {
            if (matrice[i][j] != 0) {
              linieCurenta = i;
              while (liniaCautata < linieCurenta) {
                // daca e 0, mut numarul, daca sunt egale le adun, alfel cresc
                // liniaCautata
                if (matrice[liniaCautata][j] == 0) {
                  matrice[liniaCautata][j] = matrice[linieCurenta][j];
                  mvwprintw(win, liniaCautata + 1, j + 1, "%d",
                            matrice[liniaCautata][j]);
                  mvwprintw(win, linieCurenta + 1, j + 1, " ");
                  matrice[linieCurenta][j] = 0;
                  break;
                } else if (matrice[liniaCautata][j] ==
                           matrice[linieCurenta][j]) {
                  matrice[liniaCautata][j] *= 2;
                  mvwprintw(win, liniaCautata + 1, j + 1, "%d",
                            matrice[liniaCautata][j]);
                  mvwprintw(win, linieCurenta + 1, j + 1, " ");
                  matrice[linieCurenta][j] = 0;
                  liniaCautata += 2;
                  break;
                } else {
                  liniaCautata += 2;
                }
              }
            }
          }
        }
        break;
      case KEY_DOWN:
        for (j = 1; j < n; j += 2) {
          liniaCautata = 7;
          for (i = n - 3; i >= 1; i -= 2) {
            if (matrice[i][j] != 0) {
              linieCurenta = i;
              while (liniaCautata > linieCurenta) {
                if (matrice[liniaCautata][j] == 0) {
                  matrice[liniaCautata][j] = matrice[linieCurenta][j];
                  mvwprintw(win, liniaCautata + 1, j + 1, "%d",
                            matrice[liniaCautata][j]);
                  mvwprintw(win, linieCurenta + 1, j + 1, " ");
                  matrice[linieCurenta][j] = 0;
                  break;
                } else if (matrice[liniaCautata][j] ==
                           matrice[linieCurenta][j]) {
                  matrice[liniaCautata][j] *= 2;
                  mvwprintw(win, liniaCautata + 1, j + 1, "%d",
                            matrice[liniaCautata][j]);
                  mvwprintw(win, linieCurenta + 1, j + 1, " ");
                  matrice[linieCurenta][j] = 0;
                  liniaCautata -= 2;
                  break;
                } else {
                  liniaCautata -= 2;
                }
              }
            }
          }
        }
        break;
      case KEY_RIGHT:
        for (i = 1; i < n; i += 2) {
          coloanaCautata = 7;
          for (j = n - 3; j >= 1; j -= 2) {
            if (matrice[i][j] != 0) {
              coloanaCurenta = j;
              while (coloanaCautata > coloanaCurenta) {
                if (matrice[i][coloanaCautata] == 0) {
                  matrice[i][coloanaCautata] = matrice[i][coloanaCurenta];
                  mvwprintw(win, i + 1, coloanaCautata + 1, "%d",
                            matrice[i][coloanaCautata]);
                  mvwprintw(win, i + 1, coloanaCurenta + 1, " ");
                  matrice[i][coloanaCurenta] = 0;
                  break;
                } else if (matrice[i][coloanaCautata] ==
                           matrice[i][coloanaCurenta]) {
                  matrice[i][coloanaCautata] *= 2;
                  mvwprintw(win, i + 1, coloanaCautata + 1, "%d",
                            matrice[i][coloanaCautata]);
                  mvwprintw(win, i + 1, coloanaCurenta + 1, " ");
                  matrice[i][coloanaCurenta] = 0;
                  coloanaCautata -= 2;
                  break;
                } else {
                  coloanaCautata -= 2;
                }
              }
            }
          }
        }
        break;
      case KEY_LEFT:
        for (i = 1; i < n; i += 2) {
          coloanaCautata = 1;
          for (j = 3; j < n; j += 2) {
            if (matrice[i][j] != 0) {
              coloanaCurenta = j;
              while (coloanaCautata < coloanaCurenta) {
                if (matrice[i][coloanaCautata] == 0) {
                  matrice[i][coloanaCautata] = matrice[i][coloanaCurenta];
                  mvwprintw(win, i + 1, coloanaCautata + 1, "%d",
                            matrice[i][coloanaCautata]);
                  mvwprintw(win, i + 1, coloanaCurenta + 1, " ");
                  matrice[i][coloanaCurenta] = 0;
                  break;
                } else if (matrice[i][coloanaCautata] ==
                           matrice[i][coloanaCurenta]) {
                  matrice[i][coloanaCautata] *= 2;
                  mvwprintw(win, i + 1, coloanaCautata + 1, "%d",
                            matrice[i][coloanaCautata]);
                  mvwprintw(win, i + 1, coloanaCurenta + 1, " ");
                  matrice[i][coloanaCurenta] = 0;
                  coloanaCautata += 2;
                  break;
                } else {
                  coloanaCautata += 2;
                }
              }
            }
          }
        }
        break;
    }

    // generare alta celula
    do {
      do {
        linia1 = rand() % ROW;
        coloana1 = rand() % COL;
      } while (matrice[linia1][coloana1] != 0);
    } while ((linia1 % 2 == 0 || coloana1 % 2 == 0));
    matrice[linia1][coloana1] = (rand() % 4) ? 2 : 4;
    mvwprintw(win, linia1 + 1, coloana1 + 1, "%d", matrice[linia1][coloana1]);
  }

  wrefresh(win);
  delwin(win);
}

// Aici am definit layoutul tablei de joc si panoul de control. Am desenat o
// bordura si coloanele cu |. Iar in panoul de control am generat ceea ce se
// cerea.
void panou_de_control(WINDOW *win, int n, int matrice[8][8]) {
  int x, y, i, j;
  x = 3;
  y = 2;
  box(win, 0, 0);
  wrefresh(win);
  mvprintw(1, 35, "2048");
  mvprintw(4, 50, "Iesire: Q");
  mvprintw(3, 50, "Miscare tabla: sageti");
  mvprintw(5, 50, "Score: 0");
  mvprintw(6, 50, "Key Up: sus");
  mvprintw(7, 50, "Key Down: jos");
  mvprintw(8, 50, "Key Left: stanga");
  mvprintw(9, 50, "Key Right: dreapta");

  for (i = 0; i < 8; i += 2) {
    for (j = 0; j < 4; j += 2) {
      mvwprintw(win, y, x, "|");
      x += 2;
    }
    mvwprintw(win, y, x, "|");
    x = 3;
    y += 2;
  }
  celuleMiscare(win, n, matrice);

  wrefresh(win);
  wgetch(win);
}

// Am implementat pentru meniu, o bordura, iar pentru variantele din
// meniu am adaugat highlight cand sunt selectate
void print_meniu(WINDOW *win, int highlight, char **meniu, int optiuni) {
  int x, y, i;
  x = 15;
  y = 5;
  // bordura
  box(win, 0, 0);

  // parcurg vectorul cu optiuni pentru meniu
  for (i = 0; i < optiuni; i++) {
    // Adaug highlight
    if (highlight != i + 1) {
      mvwprintw(win, y, x, "%s", meniu[i]);
    } else {
      wattron(win, A_STANDOUT);
      mvwprintw(win, y, x, "%s", meniu[i]);
      wattroff(win, A_STANDOUT);
    }
    y++;
  }
  wrefresh(win);
}

// Am stabilit folosirea sagetilor ca taste, am adaugat cateva detalii, precum
// numele jocului si ce taste pot fi folosite, apelez functia pentru highlight
// si bordura. Citesc o tasta, verific in ce caz ma aflu si stabilesc ce optiune
// are nevoie de highlight
void meniu(WINDOW *win, int *tasta, char **meniu, int optiuni) {
  int highlight = 1, ch;

  // pt a putea folosi sageti
  keypad(win, TRUE);
  mvprintw(2, 20, "2048");
  mvprintw(3, 15, "Folositi sagetile");
  mvprintw(3, 50, "BEST SCORE: ");
  refresh();
  print_meniu(win, highlight, meniu, optiuni);

  while (1) {
    ch = wgetch(win);
    switch (ch) {
      case KEY_UP:
        if (highlight == 1)
          highlight = optiuni;
        else
          highlight--;
        break;
      case KEY_DOWN:
        if (highlight == optiuni)
          highlight = 1;
        else
          highlight++;
        break;
      case 10:
        *tasta = highlight;
        break;
      default:
        break;
    }
    print_meniu(win, highlight, meniu, optiuni);
    if (*tasta != 0) break;
  }
}

// In main, am definit optiunile pentru meniu, fereastra.
// Generez meniul in functie de vector, apoi pentru fiecare optiune aleasa,
// implementez ce se intampla mai departe.
int main(void) {
  int start_y, start_x, tasta = 0, resume;
  int n, matrice[8][8];
  WINDOW *win;

  // optiuni meniu
  char *meniuprincipal[] = {
      "New Game",
      "Resume",
      "Quit",
  };

  // initializare generator de numere aleatorii
  srand(time(NULL));
  n = 8;

  // initializare, incepe modul ncurses
  initscr();
  clear();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  // ascunde cursorul
  curs_set(0);

  while (1) {
    clear();
    win = newwin(0, 0, 0, 0);
    refresh();
    start_x = 4;
    start_y = 4;
    win = newwin(height, width, start_y, start_x);
    wrefresh(win);

    // genereaza meniu in functie de vectorul meniuprincipal
    meniu(win, &tasta, meniuprincipal, 3);
    refresh();

    // New game
    if (tasta == 1) {
      clear();
      resume = 1;

      win = newwin(0, 0, 0, 0);
      refresh();
      win = newwin(height2048, width2048, 2, 4);
      panou_de_control(win, n, matrice);
      refresh();
    }

    // Resume
    if (tasta == 2 && resume == 1) {
      win = newwin(0, 0, 0, 0);
      refresh();
      win = newwin(height, width, start_y, start_x);
      refresh();
    }

    // Quit
    if (tasta == 3) {
      break;
    }
    tasta = 0;
  }

  // lasa cursorul unde este si sterge toate caracterele de la acea pozitie pana
  // la sfarsitul linie, inclusiv spatiile
  // clrtoeol();

  refresh();

  // elibereaza memoria, iese din modul ncurses
  endwin();
  return 0;
}
