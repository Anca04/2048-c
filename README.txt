Joc 2048

Am inceput prin generarea timpului si a datei, cu ajutorul bibliotecii time.h. 
Am initializat matricea(tabla de joc) cu 0. 

Am implementat functia pentru miscarea celulelor. Se genereaza mai intai 2 
numere random, apoi se testeaza daca tasta citita presupune mutarea celulei sau
iesire(q). Am luat 4 cazuri pentru sageti, mutare sus/jos/dreapta/stanga si am
testat daca este posibila mutarea, daca trebuie adunate. De asemenea, am 
generat urmatoarea celula.

In panoul de control, afisez comenzile valide, conturez tabla de joc(bordura, 
coloane |) si apelez functia pentru miscarea celulelor.

Functia print_meniu adauga highlight pentru optiunea selectata, iar pentru 
celelalte nu.

Functia meniu creeaza meniul, stabilesc navigarea cu ajutorul tastelor, 
am desenat o bordura. Apoi, in functie de tasta citita, retin cine are nevoie
de highlight.

In main, am definit vectorul pentru meniu ce cuprinde optiunile. Am creat 
fereastra, iar, in functie de taste, se alege o optiune din meniu(New Game, 
Resume, Quit). 
