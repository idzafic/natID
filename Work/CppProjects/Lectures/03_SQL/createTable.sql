CREATE TABLE [IF NOT EXISTS] nazivTabele (
   kolona1 tipPodatka PRIMARY KEY,
      kolona2 tipPodatka NOT NULL,
	  ....
   kolonaN tipPodatka DEFAULT 0,
	opcionaOgraničenjaIRelacijeMeđuTabelama);




CREATE TABLE IF NOT EXISTS Klijenti (
   Sifra INTEGER PRIMARY KEY,
   Naziv TEXT NOT NULL,
   Adresa TEXT NOT NULL);
   
   
CREATE TABLE IF NOT EXISTS Racuni (
	Broj INTEGER PRIMARY KEY,
	Datum INTEGER NOT NULL,
	SifraKlijenta INTEGER NOT NULL,
	Iznos INTEGER NOT NULL,
	FOREIGN KEY (SifraKlijenta)
	       REFERENCES Klijenti (Sifra));


CREATE [UNIQUE] INDEX nazivIndeksa 
	ON nazivTabele(listaSaNazivimaKolona);
	

	CREATE TABLE IF NOT EXISTS Klijenti (
	   Sifra INTEGER PRIMARY KEY,
	   Naziv TEXT NOT NULL,
	   Adresa TEXT NOT NULL);
   
   
	CREATE TABLE IF NOT EXISTS Racuni (
		Broj INTEGER PRIMARY KEY,
		Datum INTEGER NOT NULL,
		SifraKlijenta INTEGER NOT NULL,
		Iznos INTEGER NOT NULL,
		FOREIGN KEY (SifraKlijenta)
		       REFERENCES Klijenti (Sifra));
			   
CREATE INDEX RacunKlijentDatum 
	ON Racuni(SifraKlijenta, Datum, Broj);
	
INSERT INTO nazivTabele (kolona1,kolona2 ,..) VALUES(vrijednost1, vrijednost2,...);

INSERT INTO Klijenti (Sifra, Naziv, Adresa) VALUES(10, 'ETF Sarajevo', "Zmaja od Bosne bb");
INSERT INTO Klijenti (Sifra, Naziv, Adresa) VALUES(20, 'Neko', "Neka adresa");

INSERT INTO Racuni (Broj, Datum, SifraKlijenta, Iznos) VALUES(1, 0, 10, 1000);
INSERT INTO Racuni (Broj, Datum, SifraKlijenta, Iznos) VALUES(2, 0, 20, 2000);
INSERT INTO Racuni (Broj, Datum, SifraKlijenta, Iznos) VALUES(3, 0, 20, 3000);
INSERT INTO Racuni (Broj, Datum, SifraKlijenta, Iznos) VALUES(4, 0, 10, 4000);
INSERT INTO Racuni (Broj, Datum, SifraKlijenta, Iznos) VALUES(5, 0, 10, 5000);


SELECT kolona1, kolona2, ..., kolonaN FROM nazivTabele

SELECT Sifra, Naziv, Adresa from Klijenti;

SELECT kolona1, kolona2, ..., kolonaN 
	FROM nazivTabele 
	WHERE uslov 
	ORDER BY listaNazivaKolona [ASC | DESC];

SELECT Broj, Iznos from Racuni WHERE SifraKlijenta=10 ORDER BY Iznos DESC;

SELECT MAX(Iznos) as maxIznos from Racuni WHERE SifraKlijenta=10;

SELECT SUM(Iznos) as ukupanIznos from Racuni WHERE SifraKlijenta=10;

SELECT COUNT(*) as brRacuna from Racuni WHERE SifraKlijenta=10;

SELECT A.Broj, A.SifraKlijenta, B.Naziv, B.Adresa, A.Iznos 
FROM Racuni A, Klijenti B 
WHERE A.SifraKlijenta=B.Sifra;


UPDATE nazivTabele
SET kolona1=vrijednost1, kolona2=vrijednost2,.., kolonaN=vrijednostN
WHERE uslov;

UPDATE Racuni
SET Iznos=2500
WHERE Broj=2;

UPDATE Klijenti
SET Naziv='ASA Grupacija', Adresa='Dzemala Bijedica 110'
WHERE Sifra=20;


DELETE FROM nazivTabele WHERE uslov;

DELETE FROM Racuni WHERE Broj=5;

DELETE FROM Klijenti WHERE Sifra=20;


	