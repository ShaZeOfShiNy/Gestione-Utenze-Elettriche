/*
	Programma eseguito da: Matteo Galletta
	
	Data: 09/05/2020
	
	Il programma dev'essere in grado di gestire
	l'impianto elettrico di un'abitazione, avendo
	a disposizione diverse utenze con differenti
	potenze elettriche.
	
	
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <math.h>

#define MAX 100 // Numero massimo di utenze accettate
#define nameSize 15 // Numero massimo di caratteri accettabili per il nome dell'utenza.

//#define StampaX for(int i = 0; i < arrSize; i++){gotoxy(50,i+3);printf("%5d -> %d", i, isIdle[i]);} // debug
//#define StampaY for(int i = 0; i < arrSize; i++){gotoxy(60,i+3);printf("%5d -> %d", i, arr[i].isOn);} // debug
//#define DEBUG StampaX StampaY

typedef struct {
	char Nome[nameSize];
	float Potenza;
	int Priorita;
	bool isOn;
} Utenza ;

void AggiornaUtenza(Utenza* arr, int arrSize, int position, float &arrUsed, float arrMAX); // Aggiorna lo stato di un utenza, aggiornando anche la potenza utilizzata.
void InterfacciaGrafica(Utenza* arr, int arrSize, float arrUsed, float arrMAX); // Stampa l'interfaccia grafica.
bool LeggiUtenza(Utenza* arr, int &arrSize, float &arrMAX, const char fileName[]); // La funzione legge il file di testo contenente le utenze.
void input(Utenza* arr, int arrSize, float &arrUsed, float arrMAX); // Legge il tasto premuto dall'utente, quindi l'utenza da attivare.
bool InsertSort(Utenza* arr, int &arrSize, Utenza temp); // Inserisce all'interno della struttura dati le varie utenze
void Gestione(Utenza* arr, int arrSize, float &arrUsed, float arrMAX, int fixed); // Gestisce le utenze disattivando quelle opportune.
void windowSize(int arrSize);

int main(void) {
	system("title Utenze Elettriche");
//	Utenza arr[MAX];
	Utenza *arr = (Utenza *) malloc(sizeof(Utenza) * MAX); // Allocazione dinamica in memoria centrale.
	int arrSize = 0; // Numero di utenze disponibili.
	float arrUsed = 0; // Potenza attuale utilizzata.
	float arrMAX = 0; // Potenza massima supportata dall'impianto.
	
	if( !LeggiUtenza(arr, arrSize, arrMAX, "utenze.txt") ) return 1; // Se il file specificato non esiste, il programma si chiude.
	
	windowSize(arrSize); // Imposta le dimensioni della finestra
	
	InterfacciaGrafica(arr, arrSize, arrUsed, arrMAX);
	
	input(arr, arrSize, arrUsed, arrMAX);
	system("CLS");
	textcolor(15);
	getch();
	free(arr);
	return 0;
}

void windowSize(int arrSize) {
	char windowMessage[30];
	sprintf(windowMessage,"mode con: cols=%d lines=%d", 33, 3 + arrSize*2);
	system(windowMessage);
}

void Gestione(Utenza* arr, int arrSize, float &arrUsed, float arrMAX, int fixed) {
	// l'elemento 'fixed' non può essere disattivato, in quanto è stato appena inserito.
	
	bool isIdle[MAX];
	
	int i = arrSize - 1;
	while( i >= 0 && arrUsed > arrMAX ) {
		// Gli elementi disattivati momentaneamente, vengono memorizzati.
		if( arr[i].isOn && i != fixed ) {
			
//			arr[i].isOn = false;
			AggiornaUtenza(arr, arrSize, i, arrUsed, arrMAX);
			isIdle[i] = true;
			
		} else isIdle[i] = false;
		
//		DEBUG // debug
		
		i--;
	}
	
	// Gli elementi restanti, hanno valore isIdle = false
	while( i >= 0) isIdle[i--] = false;
	
	i = 0;
	while( i < arrSize ) {
		if( isIdle[i] && arr[i].Potenza + arrUsed <= arrMAX ) {
//			arrUsed += arr[i].Potenza;
			AggiornaUtenza(arr, arrSize, i, arrUsed, arrMAX);
		}
		i++;
	}
}

void AggiornaUtenza(Utenza* arr, int arrSize, int position, float &arrUsed, float arrMAX) {
	// Cambia il valore, se spenta diventa accesa 
	// e viceversa.
	if( arr[position].Potenza > arrMAX ) return;
	arr[position].isOn = !arr[position].isOn;
	
	if(arr[position].isOn) {
		textcolor(2);
		arrUsed+=arr[position].Potenza;
	} else {
		textcolor(12);
		arrUsed-=arr[position].Potenza;
	}
	
	// Aggiorna Lo stato dell'elemento nella posizione 'position'.
	gotoxy(15 + nameSize, position*2 + 4);
	printf("%c%c", 219, 219);
	
	// Aggiorna lo stato della potenza utilizzata.
	gotoxy(9, 2);
	
	// Qualora lo zero sia negativo, viene convertito in uno zero positivo.
	arrUsed = fabs(arrUsed); // fabs(...) -> Valore assoluto di un float
	
	textcolor(6); printf("[");
	
	arrUsed >= arrMAX ? textcolor(12) : textcolor(2);
	printf("%.2f", arrUsed);
	
	textcolor(14); printf("/%.2f", arrMAX);
	
	textcolor(6); printf("kW]");
	
//	for(int i = 0; i < 5; i++) printf(" "); // Cancella caratteri aggiuntivi sulla destra
	clreol();
	
}

void input(Utenza* arr, int arrSize, float &arrUsed, float arrMAX){
	int key;
	do {
		key = getch();
		if(key >= 'a' && key <= 'z') key-='a' - 'A';
		
		if(key >= 'A' && key <= 'A' + arrSize - 1 ) AggiornaUtenza(arr, arrSize, key-'A', arrUsed, arrMAX);
		
		if( arrUsed > arrMAX ) Gestione(arr, arrSize, arrUsed, arrMAX, key-'A');
	} while(key != '0');
}

void InterfacciaGrafica(Utenza* arr, int arrSize, float arrUsed, float arrMAX){
	
	gotoxy(9, 2);
	
	textcolor(6); printf("[");
	
	textcolor(2);
	printf("%.2f", arrUsed);
	
	textcolor(14); printf("/%.2f", arrMAX);
	
	textcolor(6); printf("kW]");
	
	for(int i = 0; i < 10; i++) printf(" "); // Cancella caratteri aggiuntivi sulla destra
	
	// Stampa l'elenco delle utenze, contenente le seguenti informazioni:
	//	-Numero Utenza;
	//	-Potenza Utilizzata Utenza;
	//	-Priorità Utenza; [implicita, l'ordine ne esprime la priorità.]
	//	-Stato Attuale (Attivo / Disattivo);
	//	-Tasto di Attivazione/Disattivazione Utenza.
	
	for(int i = 0; i < arrSize; i++ ) {
		// Stampa il pulsante per l'attivazione / disattivazione dell'utenza.
		textcolor(8);
		gotoxy(2, i*2 + 4);
		printf("[%c]", 'A'+i);
		
		// Stampa il nome
		textcolor(15);
		gotoxy(6, i*2 + 4);
		printf("%s", arr[i].Nome);
		
		// Stampa la potenza
		textcolor(2);
		gotoxy(6 + nameSize, i*2 + 4);
		printf("%3.2f", arr[i].Potenza);
		
		// Stampa la priorità dell'utenza.
//		textcolor(9);
//		gotoxy(10 + nameSize, i*2 + 5);
//		printf("%d", arr[i].Priorita);
		
		// Stampa lo stato attuale dell'utenza.
		// Se l'utenza è attiva, sarà stampato un quadrato verde,
		// altrimenti sarà rosso.
		arr[i].isOn ? textcolor(2) : textcolor(12);
		gotoxy(15 + nameSize, i*2 + 4);
		printf("%c%c", 219, 219); // Stampa un quadrato.
	}
}

bool InsertSort(Utenza* arr, int &arrSize, Utenza temp){
	if( arrSize < 0 || arrSize >= MAX) return false;
	int f = arrSize - 1;
	// "Order by ..., then ..."
	// Ordinare secondo X, poi secondo Y:
	// X: Priorità dell'utenza.
	// Y: Potenza inferiore.
	float arrComparison = arr[f].Priorita + 1.0 - arr[f].Potenza / pow(10,6);
	float tempComparison = temp.Priorita + 1.0 - temp.Potenza / pow(10,6);
	
	while( f >= 0 && arrComparison < tempComparison){
		arr[f + 1] = arr[f];
		f--;
		arrComparison = arr[f].Priorita + 1.0 - arr[f].Potenza / pow(10,6); 
	}
	
	arr[f + 1] = temp; arrSize++;
	return true;
}

bool LeggiUtenza(Utenza* arr, int &arrSize, float &arrMAX, const char fileName[]) {
	FILE *File = fopen(fileName, "rt"); // Apertura file di lettura.
	if(File == NULL){
		printf("Errore, file non trovato.\nAllegare un file nominato \"utenze.txt\" e riempirlo seguendo il seguente esempio:\n\n");
		printf("22 // Potenza massima\nUtenza1 5 99 // Nome Utenza | Potenza Utenza | Priorita' Utenza\nUtenza2 7 95\nUtenza3 3.4 89\n");
		return false;
	}
	arrSize = 0;
	Utenza temp;
	// Potenza massima supportata dall'abitazione.
	fscanf(File, "%f", &arrMAX);
	while(!feof(File)) { // Finchè il file non finisce
		
		temp.Nome[0] = NULL;
		
		// Esempio formato di input:
		//    NOME   kW  Priorità
		// Lavatrice 0.4    10
		fscanf(File, "%s %f %d", &temp.Nome, &temp.Potenza, &temp.Priorita);
		
		temp.isOn = false; // Inizialmente, tutte le utenze sono spente.
		
		// Inserisce all'interno della struttura dati le varie utenze, man mano che vengono lette
		if(temp.Nome[0]!=NULL) if(!InsertSort(arr, arrSize, temp)) break; // In questo modo, le utenze saranno ordinate secondo la loro priorità.
		
//		arrSize++; // Adesso la struttura dati contiene un'utenza in più. Avviene nella funzione InsertSort
	}
	fclose(File); // Chiusura file di lettura.
	if(arrSize == 27){
		printf("Errore, troppe utenze.\nIl limite e' impostato a 26.");
		return false;
	} else if(arrSize == 0 || arrSize == 1 ){
		printf("Errore, il minimo di utenze e' impostato a 1.");
	}
	return true;
}

