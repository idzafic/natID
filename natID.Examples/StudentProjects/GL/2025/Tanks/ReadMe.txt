Tenkici - 3D igra za dva igrača
==============================

Opis igre
---------
Tenkici je jednostavna 3D igra napravljena pomoću natID frameworka. 
Igru igraju dva igrača na jednom računaru, naizmjenično preuzimajući kontrolu nad tenkovima. 
Cilj igre je pogoditi protivnički tenk ispaljivanjem projektila.

Svaki igrač ima 20 sekundi u svojoj rundi da ispali metak i pokuša pogoditi protivnika. 
Ako igrač ispali metak ili mu istekne vrijeme, kontrolu preuzima drugi igrač. 
Igra se nastavlja sve dok health jednog od tenkova ne padne na 0.

Pobjednik je onaj igrač koji prvi isprazni protivnički health.

Pravila igre
------------
- Igraju dva igrača na jednom računaru.
- Na početku runde aktivni igrač ima 20 sekundi da ispali metak.
- Igrač može upravljati tenkom i ciljati mišem.
- Runda završava kada:
  - Igrač ispali metak.
  - Igraču istekne vrijeme od 20 sekundi.
- Nakon toga kontrolu preuzima drugi igrač.
- Igra se nastavlja sve dok health jednog od tenkova ne dosegne 0.

Upravljanje
-----------
- Aktivni igrač upravlja tenkom pomoću tipki:
  - W - kretanje naprijed
  - A - rotacija ulijevo
  - S - kretanje unazad
  - D - rotacija udesno
- Miš - ciljanje topa
- Primarni klik mišem - ispaljivanje metka
- Prozor se zatvara kombinacijom Ctrl+Q ili Alt+Q (Cmd+Q na macOS)

Nakon što igrač ispali metak ili istekne vrijeme, kontrolu preuzima drugi igrač, koji koristi iste kontrole.

Instalacija i pokretanje
------------------------
Za pokretanje igre potrebno je imati instalirano:

- Microsoft Visual Studio
- natID framework: https://github.com/idzafic/natID

Koraci za build i pokretanje:
1. Preuzmite i instalirajte natID framework prema uputama sa github stranice.
2. Preuzmite kod ove igre.
4. U root direktorijumu igre (gdje se nalazi CMakeLists.txt), pokrenite CMake konfiguraciju i build:
5. Otvorite generisani Tenkici.sln fajl u Visual Studio-u.
6. Buildajte i pokrenite projekat iz Visual Studija.
