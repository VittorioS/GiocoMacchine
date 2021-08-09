#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define true  1
#define false 0
#define ALTEZZA 20
#define LARGHEZZA 20
#define MURO_SUP 0
#define MURO_INF 19
#define MURO_DES 19
#define MURO_SIN 0
#define MURO 206
#define MACCHINA 254
#define MURO_SPAZIO 32
#define EFFETTO_SCOPPIO_SUP     205
#define EFFETTO_SCOPPIO_SUP_SIN 201
#define EFFETTO_SCOPPIO_SUP_DES 187
#define EFFETTO_SCOPPIO_INF     205
#define EFFETTO_SCOPPIO_INF_SIN 200
#define EFFETTO_SCOPPIO_INF_DES 188
#define EFFETTO_SCOPPIO_SIN     186
#define EFFETTO_SCOPPIO_DES     186
#define PERCENTUALE_MASSIMA     10

struct vettore {
	int x;
	int y;
};
typedef struct vettore Vettore;

struct percentuale {
	int su[PERCENTUALE_MASSIMA];
	int giu[PERCENTUALE_MASSIMA];
	int destra[PERCENTUALE_MASSIMA];
	int sinistra[PERCENTUALE_MASSIMA];
};
typedef struct percentuale Percentuale;

struct tipo_macchina {
	Vettore posizione;
	Percentuale percentuale;
};
typedef struct tipo_macchina Tipo_macchina;


Tipo_macchina macchina1;
Tipo_macchina macchina2;
Tipo_macchina macchina3;
Tipo_macchina macchina4;
char mappa[LARGHEZZA][ALTEZZA];

void inizializzazione();
void carica_contenuti();
int update();
int draw(int time_game);
void crea_muro();
void draw_map();
void handle_map();
void clear_muro();
void handle_macchine(Tipo_macchina macchina);
Tipo_macchina crea_macchina(int x, int y, int su, int giu, int destra, int sinistra);
Tipo_macchina muovi_macchina(Tipo_macchina macchina);
Tipo_macchina muovi_macchina_su(Tipo_macchina macchinam, int passo);
Tipo_macchina muovi_macchina_giu(Tipo_macchina macchinam, int passo);
Tipo_macchina muovi_macchina_destra(Tipo_macchina macchinam, int passo);
Tipo_macchina muovi_macchina_sinistra(Tipo_macchina macchinam, int passo);
int check_scontro(Tipo_macchina macchina1, Tipo_macchina macchina2);
void disegna_cerchio(int x, int y);
Tipo_macchina crea_percentuale(Tipo_macchina macchina, int su, int giu, int destra, int sinistra);

void inizializzazione() {
	macchina1 = crea_macchina(LARGHEZZA / 2, MURO_SUP + 1, 2, 3, 2, 3);
	macchina2 = crea_macchina(LARGHEZZA / 2, MURO_INF - 1, 2, 3, 2, 3);
	macchina3 = crea_macchina(MURO_DES - 2, ALTEZZA / 2, 2, 3, 2, 3);
	macchina4 = crea_macchina(MURO_SIN + 2, ALTEZZA / 2, 2, 3, 2, 3);

	//CASO DI TEST PER SCONTRO
	//macchina3 = crea_macchina(LARGHEZZA / 2, ALTEZZA / 2);
	//macchina4 = crea_macchina(LARGHEZZA / 2, ALTEZZA / 2);

	//macchina1 = crea_percentuale(macchina1);
	//macchina2 = crea_percentuale(macchina2, 2, 3, 2, 3);
	//macchina3 = crea_percentuale(macchina3, 2, 3, 2, 3);
	//macchina4 = crea_percentuale(macchina4, 2, 3, 2, 3);

}

void carica_contenuti() {

}

int update() {
	macchina1 = muovi_macchina(macchina1);
	macchina2 = muovi_macchina(macchina2);
	macchina3 = muovi_macchina(macchina3);
	macchina4 = muovi_macchina(macchina4);

	return true;
}

void clearScreen() {
	system("CLS");
}

void handle_map() {

}

int draw(int time_game) {
	clearScreen();
	crea_muro();
	handle_macchine(macchina1);
	handle_macchine(macchina2);
	handle_macchine(macchina3);
	handle_macchine(macchina4);

	if (check_scontro(macchina1, macchina2) == true) {
		time_game = false;
	}
	if (check_scontro(macchina1, macchina3) == true) {
		time_game = false;
	}
	if (check_scontro(macchina1, macchina4) == true) {
		time_game = false;
	}
	if (check_scontro(macchina2, macchina3) == true) {
		time_game = false;
	}
	if (check_scontro(macchina2, macchina4) == true) {
		time_game = false;
	}
	if (check_scontro(macchina3, macchina4) == true) {
		time_game = false;
	}

	draw_map();

	return time_game;
}


void clear_muro() {
	for (int x = 1; x < LARGHEZZA; x++) {
		for (int y = 1; y < ALTEZZA; y++) {
			mappa[x][y] = MURO_SPAZIO;
		}
	}
}

void crea_muro() {
	clear_muro();
	for (int x = 0; x < LARGHEZZA; x++) {
		mappa[MURO_SUP][x] = MURO;
	}
	for (int x = 0; x < ALTEZZA; x++) {
		mappa[x][MURO_SIN] = MURO;
	}
	for (int x = 0; x < ALTEZZA; x++) {
		mappa[x][MURO_DES] = MURO;
	}
	for (int x = 0; x < LARGHEZZA; x++) {
		mappa[MURO_INF][x] = MURO;
	}
}

void draw_map() {
	for (int x = 0; x < LARGHEZZA; x++) {
		for (int y = 0; y < ALTEZZA; y++) {
			printf("%c", mappa[x][y]);
		}
		printf("\n");
	}
}



// Macchina - Inizio

Tipo_macchina crea_macchina(int x, int y, int su, int giu, int destra, int sinistra) {
	Tipo_macchina nuova_macchina;
	nuova_macchina.posizione.x = x;
	nuova_macchina.posizione.y = y;
	nuova_macchina = crea_percentuale(nuova_macchina, su, giu, destra, sinistra);
	return nuova_macchina;
}

Tipo_macchina muovi_macchina(Tipo_macchina macchina) {
	int scelta = rand() % PERCENTUALE_MASSIMA;
	int continua = true;
	//time(NULL)
	Vettore posizione;
	Vettore posizione_new;
	Percentuale percentuale;
	Tipo_macchina macchina_new;

	macchina_new = macchina;
	posizione = macchina.posizione;
	percentuale = macchina.percentuale;
	mappa[posizione.y][posizione.x] = MURO_SPAZIO;

	//if (scelta == 1 || scelta == 2) {
	//	macchina_new = muovi_macchina_su(macchina, 1);
	//}
	//else if (scelta == 3 || scelta == 4) {
	//	macchina_new = muovi_macchina_giu(macchina, 1);
	//}
	//else if (scelta == 5 || scelta == 6) {
	//	macchina_new = muovi_macchina_destra(macchina, 1);
	//}
	//else if (scelta == 7 || scelta == 8) {
	//	macchina_new = muovi_macchina_sinistra(macchina, 1);
	//}
	//else if (scelta == 9 || scelta == 10) {
	//
	//}
	int i = 0;
	for (i = 0; i < PERCENTUALE_MASSIMA; i++)
	{
		if (scelta == percentuale.su[i])
		{
			macchina_new = muovi_macchina_su(macchina, 1);
			continua = false;
		}
	}
	if (continua == true) {
		for (i = 0; i < PERCENTUALE_MASSIMA; i++)
		{
			if (scelta == percentuale.giu[i])
			{
				macchina_new = muovi_macchina_giu(macchina, 1);
				continua = false;
			}
		}
	}
	if (continua == true) {
		for (i = 0; i < PERCENTUALE_MASSIMA; i++)
		{
			if (scelta == percentuale.destra[i])
			{
				macchina_new = muovi_macchina_destra(macchina, 1);
				continua = false;
			}
		}
	}
	if (continua == true) {
		for (i = 0; i < PERCENTUALE_MASSIMA; i++)
		{
			if (scelta == percentuale.sinistra[i])
			{
				macchina_new = muovi_macchina_sinistra(macchina, 1);
				continua = false;
			}
		}
	}
	posizione_new = macchina_new.posizione;
	if (posizione_new.x > MURO_SIN + 1 && posizione_new.x < LARGHEZZA - 1
		&& posizione_new.y > MURO_SUP && posizione_new.y < ALTEZZA - 1) {
		macchina = macchina_new;
		posizione = macchina.posizione;
	}
	mappa[posizione.y][posizione.x] = MACCHINA;

	return macchina;
}

Tipo_macchina muovi_macchina_su(Tipo_macchina macchina, int passo) {
	Vettore posizione = macchina.posizione;
	posizione.y -= passo;
	macchina.posizione = posizione;
	return macchina;
}

Tipo_macchina muovi_macchina_giu(Tipo_macchina macchina, int passo) {
	Vettore posizione = macchina.posizione;
	posizione.y += passo;
	macchina.posizione = posizione;
	return macchina;
}

Tipo_macchina muovi_macchina_destra(Tipo_macchina macchina, int passo) {
	Vettore posizione = macchina.posizione;
	posizione.x += passo;
	macchina.posizione = posizione;
	return macchina;
}

Tipo_macchina muovi_macchina_sinistra(Tipo_macchina macchina, int passo) {
	Vettore posizione = macchina.posizione;
	posizione.x -= passo;
	macchina.posizione = posizione;
	return macchina;
}

void disegna_cerchio(int x, int y) {
	int x1 = x - 1;
	int y1 = y - 1;
	int x2 = x + 1;
	int y2 = y + 1;
	mappa[y1][x1] = EFFETTO_SCOPPIO_SUP_SIN;
	mappa[y1][x] = EFFETTO_SCOPPIO_SUP;
	mappa[y1][x2] = EFFETTO_SCOPPIO_SUP_DES;
	mappa[y][x1] = EFFETTO_SCOPPIO_SIN;
	mappa[y][x2] = EFFETTO_SCOPPIO_DES;
	mappa[y2][x1] = EFFETTO_SCOPPIO_INF_SIN;
	mappa[y2][x] = EFFETTO_SCOPPIO_INF;
	mappa[y2][x2] = EFFETTO_SCOPPIO_INF_DES;
}

int check_scontro(Tipo_macchina macchina1, Tipo_macchina macchina2) {
	Vettore posizione1 = macchina1.posizione;
	Vettore posizione2 = macchina2.posizione;

	if (posizione1.x == posizione2.x && posizione1.y == posizione2.y) {
		disegna_cerchio(posizione1.x, posizione1.y);
		return true;
	}
	return false;
}

void handle_macchine(Tipo_macchina macchina) {
	Vettore posizione = macchina.posizione;
	mappa[posizione.y][posizione.x] = MACCHINA;
}

Tipo_macchina crea_percentuale(Tipo_macchina macchina, int su, int giu, int destra, int sinistra) {
	Percentuale percentuale = macchina.percentuale;

	int i = 1;
	int x = 0;
	for (x = 0; x < su; x++)
	{
		percentuale.su[x] = i;
		i++;
	}
	for (x = 0; x < giu; x++)
	{
		percentuale.giu[x] = i;
		i++;
	}
	for (x = 0; x < destra; x++)
	{
		percentuale.destra[x] = i;
		i++;
	}
	for (x = 0; x < sinistra; x++)
	{
		percentuale.sinistra[x] = i;
		i++;
	}

	macchina.percentuale = percentuale;

	return macchina;
}

void clear_percentuale(int percentuale[]) {
	for (int i = 0; i < PERCENTUALE_MASSIMA; i++)
	{
		percentuale[i] = 0;
	}
}

// Macchina - Fine




int main()
{
	inizializzazione();
	carica_contenuti();
	int time_game = true;
	srand(time(NULL));
	while (time_game == true) {
		time_game = update();
		time_game = draw(time_game);
		//Sleep(50);
	}

	printf("Scontro!\n");
	system("PAUSE");
	return 0;
}