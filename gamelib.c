#include "gamelib.h"
struct Zona_mappa * pFirst = NULL;
struct Zona_mappa * pLast = NULL;
struct Zona_mappa * caravan_zona;

static bool controlla_impostazione_corretta();
static int genera_nome_zona();
static void chiudi_mappa();
static void inserisci_zona();
static void cancella_zona();
static void stampa_mappa();
static char* get_nome_prova();
static char* get_nome_oggetto();
static bool hai_vinto();
static void torna_caravan();
static void stampa_giocatore();
static void stampa_zona();
static void avanza();
static void raccogli_prova();
static void raccogli_oggetto();
static void usa_oggetto();
static void passa();
static void libera_memoria();
static void schermata_sconfitta();
static void schermata_vittoria();
static void genera_ordine_turni();
static bool sono_tutti_morti();
static void decremento_sanita_mentale_dopo_azione();
static void guarda_avanti();

int numero_zone = 0;
int zona;
int numerogiocatori;
Giocatore * pGiocatore;
int difficoltà;
int probabilita_prova[] = {
  60,
  50,
  40
}; //60% difficolta dilettante, 40 incubo.
int turni[4];
int turno_attuale = 0;
bool tornato = 0;
bool avanzato = 0;
bool sale_usato = 0;
int cont_random_turni = 0;

char * nomi_oggetti[] = {
  "EMF",
  "spirit box",
  "videocamera",
  "calmanti",
  "sale"
};
char * nome_prova[] = {
  "prova_EMF",
  "prova_spirit_box",
  "prova_videocamera",
  "nessuna_prova"
};
int nessuna_prova_ = 23;
char * nome_oggetto_zona[] = {
  "adrenalina",
  "cento_dollari",
  "coltello",
  "calmanti",
  "sale",
  "nessun_oggetto"
};
int nessun_oggetto_ = 10;
char * nome_zona[] = {
  "cucina",
  "soggiorno",
  "camera",
  "bagno",
  "garage",
  "seminterrato",
  "caravan"
};

bool prove_trovate[] = {0,0,0};

//---------------------------------------------------------
bool controlla_impostazione_corretta() {
   if (pFirst == NULL) {
     return false;
   }
   if (pLast -> prossima_zona != pFirst) {
     return false;
   }
   return true;
 }

 int genera_nome_zona() {
    zona = rand() % 5;
    zona += 1;
    return zona; //zona è l'indice dell'array nome_zona
  }

  void chiudi_mappa(Zona_mappa * nuova_zona) {
    nuova_zona -> prossima_zona = pFirst; // questa riga potremmo volendo farla anche quando pigia 4, ma è piu corretto "chiudere" il circolo tutte le volte
    pLast = nuova_zona;
  }

  void inserisci_zona() {
    if (numero_zone == 0) {
      //aggiungo caravan alla mappa
      Zona_mappa * caravan_zona = (Zona_mappa * ) malloc(sizeof(Zona_mappa));
      caravan_zona -> zona = 5;
      caravan_zona->prossima_zona = pFirst;
      //printf("PIPPO");
    }
    // per ogni zona allochiamo la memoria
    Zona_mappa * nuova_zona = (Zona_mappa * ) malloc(sizeof(Zona_mappa));

    nuova_zona -> zona = genera_nome_zona(); // la funzione che abbiamo fatto oggi

    if (pFirst == NULL) { // prima zona è il nostro puntatore, possiamo controllare questo o ultima_zona
      //printf("pFirst è null");
      // Se non ci sono ancora zone, la nuova zona diventa la prima
      pFirst = nuova_zona; // stiamo sempre parlando di puntatori
      pLast = pFirst;
    } else {
      // se esiste gia almeno una zona dobbiamo collegare la nuova zona all'ultima, sempre tramite puntatori
      pLast -> prossima_zona = nuova_zona;
      pLast = nuova_zona;
    }
    nuova_zona->oggetto_zona = nessun_oggetto_;
    int numero_random = (rand() % 10) + 1; //se numero è tra 0 e 4 - nessun oggetto
    if (numero_random >= 5) {
      do{numero_random = (rand() % 11);
      }while(numero_random < 5);
      nuova_zona -> oggetto_zona = numero_random;
      //nuova_zona -> oggetto_zona = 4;
    }
    printf("la zona creata è: %s\n\n", nome_zona[nuova_zona -> zona]);
    // Collegamento della nuova zona all'ultima zona e aggiornamento dell'ultima zona
    chiudi_mappa(nuova_zona);
    numero_zone++;

  }

  void cancella_zona() {
    if (pLast == NULL) {
      printf("La mappa è vuota!\n");
      return;
    }

    // Prima di cancellare l'ultima zona, dobbiamo aggiornare la zona precedente
    Zona_mappa * zona_prec = pFirst;
    while (zona_prec -> prossima_zona != pLast) {
      zona_prec = zona_prec -> prossima_zona;
    }

    zona_prec -> prossima_zona = pFirst; // Collegamento della zona precedente alla prima zona

    // Salviamo l'ultima zona in una variabile temporanea, per poi liberare la memoria
    Zona_mappa * temp = pLast;

    // Liberiamo la memoria dell'ultima zona
    free(temp);

    // Aggiorniamo il puntatore all'ultima zona
    pLast = zona_prec;

    // Aggiorniamo il numero di zone
    numero_zone--;
  }

  void stampa_mappa() {
    // printf("%s",pFirst->zona);
    if (pLast == NULL) {
      printf("La mappa è vuota!\n");
      return;
    }
    Zona_mappa * current_zona = pFirst;
    int n_current = 0;
    do {
      printf("Zona %d: %s\n", n_current + 1, nome_zona[current_zona -> zona]);
      current_zona = current_zona -> prossima_zona;
      n_current++;
    } while (n_current != numero_zone);
  }



//-------------------------------------------------------------------------
void imposta_gioco() {
  int scelta;
  int ordine_gioco[0];
  char giocatore[21];
  srand(time(NULL));
  pFirst = NULL;
  pLast = NULL;
  system("clear");
  sleep(1);

  do {
    system("clear");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                   Scegli il numero di giocatori, ricorda che possono giocare al massimo 4 persone!                               |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                         Digita un numero da 1 a 4!                                                               |\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    scanf("%d", & numerogiocatori);

    getchar();

  } while (numerogiocatori < 1 || numerogiocatori > 4);

  pGiocatore = (Giocatore * ) calloc(numerogiocatori, sizeof(Giocatore));
  system("clear");

  for (int i = 0; i < numerogiocatori; i++) {
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                        Inserisci il nome del giocatore numero %d                                                                  |\n", i + 1);
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    scanf("%s", giocatore);
    while (strlen(giocatore) >= 21) {
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      printf("|                          Nome del giocatore troppo lungo. Inserisci un nome di massimo 20 caratteri.                                             |\n");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      scanf("%s", giocatore);
    }
    strcpy((char * ) pGiocatore[i].nome_giocatore, giocatore);
    pGiocatore[i].sanita_mentale = 100;
    pGiocatore[i].posizione = NULL;
    memset(pGiocatore[i].zaino, 0, sizeof(pGiocatore[i].zaino));

    for(int k = 0; k < 4; k++){
      pGiocatore[i].zaino[k] = nessun_oggetto_;
    }


  }
  do {
    //getchar();
    system("clear");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                   Seleziona il livello di difficoltà tra dilettante, intermedio e incubo!                                        |\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    //sleep(1);
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                          - Premi 1 -> dilettante                                                                 |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                          - Premi 2 -> intermedio                                                                 |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                                                                                                                  |\n");
    printf("|                                                          - Premi 3 -> incubo                                                                     |\n");
    printf("|                                                                                                                                                  |\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    scanf("%d", & difficoltà);
    getchar();
    switch (difficoltà) {
    case 1:
      system("clear");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      printf("|                                              Hai selezionato la difficoltà dilettante!                                                           |\n");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      sleep(2);
      break;

    case 2:
      system("clear");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      printf("|                                              Hai selezionato la difficoltà intermedia!                                                           |\n");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      sleep(2);
      break;

    case 3:
      system("clear");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      printf("|                                              Hai selezionato la difficoltà incubo!                                                               |\n");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      sleep(2);
      break;

    default:
      system("clear");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      printf("|                                                      Seleziona una difficoltà valida                                                             |\n");
      printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
      sleep(2);
      break;
    }
  } while (difficoltà != 1 && difficoltà != 2 && difficoltà != 3);

  system("clear");

  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
  printf("|                                          ora verrano scelti gli oggetti iniziali da mettere nello zaino.                                         |\n");
  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
  sleep(1);
  printf("\necco gli oggetti disponibili!\n");
  int oggetti_iniziali[numerogiocatori];
  oggetti_iniziali[0] = rand() % 3;
  printf("%s\n", nomi_oggetti[oggetti_iniziali[0]]);

  for (int i = 1; i < numerogiocatori; i++) {
    oggetti_iniziali[i] = rand() % 5;
    printf("%s\n", nomi_oggetti[oggetti_iniziali[i]]);
  } //ci sono tanti oggetti quanti numerogiocatori, ma almeno uno deve essere tra i primi 3, mentre gli altri da scegliere a caso tra tutti gli oggetti
  printf("\n");

  // Inizializzazione dell'array per rimescolare l'ordine dei giocatori
  for (int i = 0; i < numerogiocatori; i++) {
    ordine_gioco[i] = i + 1;
  }
  //l'array viene rimescolato
  int d = numerogiocatori - 1;
  for (int i = 0; i < numerogiocatori; i++) {
    int appoggio = ordine_gioco[d];
    int nrand = rand() % (d + 1);
    ordine_gioco[d] = ordine_gioco[nrand];
    ordine_gioco[nrand] = appoggio;
    d--;
  }

  //l'ordine di scelta cambia ogni volta che si imposta il gioco, se un giocatore sceglie un oggetto, questo non è disponibile per i giocatori successivi.
  printf("\n");
  // Inizializza l'array degli oggetti disponibili
  bool oggetti_disponibili[numerogiocatori];
  for (int j = 0; j < numerogiocatori; j++) {
    oggetti_disponibili[j] = true;
  }
  for (int i = 0; i < numerogiocatori; i++) {
    int giocatore_corrente = ordine_gioco[i] - 1;
    printf("\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                              %s, è il tuo turno di scegliere un oggetto.                                                   |\n", (char * ) pGiocatore[giocatore_corrente].nome_giocatore);
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("Ecco gli oggetti disponibili:\n");
    for (int j = 0; j < numerogiocatori; j++) {
      // Mostra solo gli oggetti disponibili
      if (oggetti_disponibili[j]) {
        printf("%d. %s\n", j + 1, nomi_oggetti[oggetti_iniziali[j]]);
      }
    }
    printf("\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                      Quale oggetto vuoi scegliere? Inserisci il numero corrispondente.                                           |\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    int scelta;
    scanf("%d", & scelta);
    while (scelta < 1 || scelta > numerogiocatori /* || !oggetti_disponibili[scelta-1]*/ ) {
      printf("Scelta non valida. Inserisci il numero corrispondente all'oggetto che vuoi scegliere.\n");
      scanf("%d", & scelta);
    }
    // Imposta l'oggetto come non disponibile
    oggetti_disponibili[scelta - 1] = false;
    // Inserisci l'oggetto nello zaino del giocatore corrente
    pGiocatore[i].zaino[0] = oggetti_iniziali[scelta - 1];
  /*  printf("giocatore:%d\n", i);
    printf("fffff%d\n", oggetti_iniziali[scelta - 1]);*/
  }

  system("clear");
  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
  printf("|                                                   impostiamo la mappa...                                                                         |\n");
  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
  sleep(1);
  do {
    printf("Inserisci un numero:\n");
    printf("1 per aggiungere una zona, 2 per cancellare una zona, 3 per stampare la mappa e 4 per terminare\n");
    printf("\n");
    scanf("%d", & scelta);

    if (scelta == 1) {
      inserisci_zona();
    }
    if (scelta == 2) {
      cancella_zona();
    }
    if (scelta == 3) {
      stampa_mappa();
    }
    if (scelta == 4) {
      chiudi_mappa(pLast);
    }

  } while (scelta != 4 || controlla_impostazione_corretta() == false);


  return;

} // fine imposta gioco
//------------------------------------------------------------------------------------------------


char* get_nome_prova(int indice){
  return nome_prova[indice-20];
}

char* get_nome_oggetto(int indice){
  if (indice >= 20){
      return get_nome_prova(indice);
  }
  if (indice < 5){
    return nomi_oggetti[indice];
  }
  else{
    return nome_oggetto_zona[indice - 5];
  }
}

bool hai_vinto(){
  for(int i = 0; i < 3; i++){
    if(prove_trovate[i] == 0){
      return 0;
    }
  }
  return 1;
}
void torna_caravan() {
  printf("\nsei tornato al caravan...\n");
  tornato = 1;
  pGiocatore[turni[turno_attuale] - 1].posizione = caravan_zona;
  int cont = 0;
  for(int i = 0; i < 4; i++){
    cont = 0;
    if(pGiocatore[turni[turno_attuale] - 1].zaino[i] >= 20 && pGiocatore[turni[turno_attuale] - 1].zaino[i] <= 22){
      if(prove_trovate[pGiocatore[turni[turno_attuale] - 1].zaino[i] - 20] == 0){
        prove_trovate[pGiocatore[turni[turno_attuale] - 1].zaino[i] - 20] = 1;
        printf("hai consegnato la prova: %s\n", get_nome_prova(pGiocatore[turni[turno_attuale] - 1].zaino[i]));
      }else{
        printf("hai già consegnato la prova %s\n",get_nome_prova(pGiocatore[turni[turno_attuale] - 1].zaino[i]));

      }
      pGiocatore[turni[turno_attuale] - 1].zaino[i] = nessun_oggetto_;//sia che sia trovata che non, la tolgo
      if(!hai_vinto()){
        do{
          int random_oggetto = rand() % 2;
          if(random_oggetto == 1){
            if(prove_trovate[cont]  == 0){
             pGiocatore[turni[turno_attuale] - 1].zaino[i] = cont;
            }
          }
          cont = (cont + 1) % 3;
        }while(pGiocatore[turni[turno_attuale] - 1].zaino[i] == nessun_oggetto_);
      }
    }
  }
  pGiocatore[turni[turno_attuale] - 1].posizione = pFirst;
}
void stampa_giocatore() {
  printf("\n");
  Zona_mappa * zona_attuale = pGiocatore[turni[turno_attuale] - 1].posizione;
  printf("Nome giocatore: %s\n", pGiocatore[turni[turno_attuale] - 1].nome_giocatore);
  printf("Sanità mentale: %d\n", pGiocatore[turni[turno_attuale] - 1].sanita_mentale);
  printf("Posizione: %s\n", nome_zona[zona_attuale -> zona]);
  printf("Il tuo zaino contiene: \n");
  for (int i = 0; i < 4; i++) {
    if(pGiocatore[turni[turno_attuale] - 1].zaino[i] >= 0){
    //  printf("%d--", pGiocatore[turni[turno_attuale] - 1].zaino[i]);
      printf("%s\n ", get_nome_oggetto(pGiocatore[turni[turno_attuale] - 1].zaino[i]));

    }

  }
  printf("\n");
}

void stampa_zona() {
  Zona_mappa * zona_attuale = pGiocatore[turni[turno_attuale] - 1].posizione;
  printf("Prova: %s [5]\n", get_nome_prova(zona_attuale -> prova));
  printf("Oggetto: %s [6]\n", get_nome_oggetto(zona_attuale -> oggetto_zona));
}
void avanza() {
  avanzato = 1;
  pGiocatore[turni[turno_attuale] - 1].posizione = pGiocatore[turni[turno_attuale] - 1].posizione -> prossima_zona;
  Zona_mappa * zona_attuale = pGiocatore[turni[turno_attuale] - 1].posizione;
  //zonaa->prova = prova
   printf("ti trovi nella stanza: %s\n", nome_zona[zona_attuale -> zona]);
  //per generare le prove
  if (((rand() % 100) + 1) <= probabilita_prova[difficoltà]) {
    zona_attuale -> prova = (rand() % 3)+20;
  } else {
    zona_attuale -> prova = nessuna_prova_; //23 sta per nessuna prova.
  }
//controllo di rimetterlo a 0 quando passo il turno
}


void guarda_avanti(){
  //dalla stanza in cui mi trovo devo guardare le stanzr rimanenti e dire quante ce ne sono per tipo.
  int cucina_n = 0;
  int soggiorno_n = 0;
  int camera_n = 0;
  int bagno_n = 0;
  int garage_n = 0;
  int seminterrato_n = 0;

  Zona_mappa* posizione = pGiocatore[turni[turno_attuale] - 1].posizione;
      do{
        posizione = posizione -> prossima_zona;
        if(posizione-> zona == 0){
        cucina_n++;
        }
        if(posizione-> zona == 1){
        soggiorno_n++;
        }
        if(posizione-> zona == 2){
        camera_n++;
        }
        if(posizione-> zona == 3){
        bagno_n++;
        }
        if(posizione-> zona == 4){
        garage_n++;
        }
        if(posizione-> zona == 5){
        seminterrato_n++;
        }

    }while(posizione != pLast);
printf("numero di cucine rimaste: %d\n", cucina_n);
printf("numero di soggiorni rimaste: %d\n", soggiorno_n);
printf("numero di camere rimaste: %d\n", camera_n);
printf("numero di bagni rimaste: %d\n", bagno_n);
printf("numero di garage rimasti: %d\n", garage_n);
printf("numero di seminterrati rimasti: %d\n", seminterrato_n);
printf("\n\n");
  }


//-------------------------------
void raccogli_prova() {
  if (pGiocatore[turni[turno_attuale] - 1].posizione -> prova >= 20 && pGiocatore[turni[turno_attuale] - 1].posizione -> prova < 23) { //se c'è una prova

    //controllo che il giocatore abbia l'oggetto adatto a raccogliere la prova
    for (int i = 0; i < 4; i++) {
      if (pGiocatore[turni[turno_attuale] - 1].zaino[i] + 20 == (int)pGiocatore[turni[turno_attuale] - 1].posizione -> prova) { // confonto oggetti zaino con i primi tre oggetti di nomi_oggetti, sono quelli per raccogliere la rpove
          pGiocatore[turni[turno_attuale] - 1].zaino[i] = pGiocatore[turni[turno_attuale] - 1].posizione -> prova; // metto prova nello zaino, nella prima posizione vuota che incontro
          printf("prova raccolta\n");
          pGiocatore[turni[turno_attuale] - 1].posizione -> prova = nessuna_prova_;
          return;
      }
    }
    printf("non hai l'oggetto per raccogliere la prova");

  } else {
    //se ho_l_oggetto_per_raccogliere_la_prova non è 1
    printf("non è presente nessuna prova\n");
  }
}
void raccogli_oggetto() {
  if(pGiocatore[turni[turno_attuale] - 1].posizione->oggetto_zona >= 0 && pGiocatore[turni[turno_attuale] - 1].posizione->oggetto_zona < 10){ // se ce un oggetto

    bool aggiunto = 0;
    for(int i = 0; i < 4; i++){
      if(pGiocatore[turni[turno_attuale] - 1].zaino[i] == nessun_oggetto_){
        //printf("%d",pGiocatore[turni[turno_attuale] - 1].posizione->oggetto_zona);
        pGiocatore[turni[turno_attuale] - 1].zaino[i] = pGiocatore[turni[turno_attuale] - 1].posizione->oggetto_zona;
        //printf("%d", pGiocatore[turni[turno_attuale] - 1].zaino[i]);
        pGiocatore[turni[turno_attuale] - 1].posizione->oggetto_zona = nessun_oggetto_;
        aggiunto = 1;
        break;
      }
    }

    if(aggiunto == 0){
      printf("zaino pieno\n");
    }else{
      printf("oggetto aggiunto\n");
    }

  }else {
    printf("non è presente nessun oggetto\n");
  }

}
void usa_oggetto() {
  int oggetto_da_usare;

  do {
    printf("Quale oggetto vuoi usare?: premi 1 per usare il primo oggetto nel tuo zaino, premi 2 per usare il secondo, 3 per il terzo e 4 per il quarto.\n");
  scanf("%d", &oggetto_da_usare);
  //printf("%d\n",pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1]);
   }while(oggetto_da_usare != 1 && oggetto_da_usare != 2 && oggetto_da_usare != 3);
    if(pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] == adrenalina/*nome_oggetto_zona[5]*/){ //adrenalina
      avanzato = 0;
      printf("Hai usato: Adrenalina. Ora puoi avanzare una volta in più.\n");
      avanza();
      pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = nessun_oggetto_;

    }
    if(pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] == cento_dollari/*nome_oggetto_zona[6]*/){ //cento dollari
      int rand_calmanti_sale = rand()%2;
      if(rand_calmanti_sale == 0){
        pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = calmanti;
      } else{
        pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = sale;
      }

      printf("Hai usato: Cento dollari. Ora verrà acquisito un oggetto tra Sale e Calmanti.\n");
    }
    if(pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] == coltello){
      printf("Hai usato: Coltello. I giocatori nella tua stessa zona verranno uccisi se la tua sanità mentale è inferiore a 30.\n");
      if(pGiocatore[turni[turno_attuale] - 1].sanita_mentale < 30){
        for(int i = 0; i < numerogiocatori; i++){
          if( i != turni[turno_attuale] - 1){ //guardo che non sia il giocatore che usa il coltello
            if( pGiocatore[turni[turno_attuale] - 1].posizione == pGiocatore[i].posizione){// se sono nella sua stessa posizione li uccide
              pGiocatore[i].sanita_mentale = 0;
              printf("Hai ucciso :%s\n",pGiocatore[i].nome_giocatore);
            }
          }
        }
        //si uccidono tutti i giocatori nella stessa zona
      }
      pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = nessun_oggetto_;

    }
   if(pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] == calmanti){
     pGiocatore[turni[turno_attuale] - 1].sanita_mentale += 40;
     pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = nessun_oggetto_;
     printf("Hai usato: Calmanti. La tua sanità mentale sale di 40.\n");
   }
   if(pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] == sale){
     sale_usato = 1;
     printf("Hai usato: Sale. Il fantasma non può farti paura.\n");
     pGiocatore[turni[turno_attuale] - 1].zaino[oggetto_da_usare - 1] = nessun_oggetto_;
   }
  }

void passa() {
  bool sono_morto = 0;
  int cont = 0;
  do{
  sono_morto = 0;
  turno_attuale = (turno_attuale + 1) % numerogiocatori; //aggiunge 1, quando arriva a 4 riparte da 0, il resto delle volte prosegue. ciclo dell'indice dei turni.
  //printf("%d\n", pGiocatore[turni[turno_attuale] - 1].sanita_mentale);
  if(pGiocatore[turni[turno_attuale] - 1].sanita_mentale < 1){
    sono_morto = 1;
    cont++;
  }
}while(sono_morto == 1 && cont < numerogiocatori);
  if( sono_morto == 0){
    /*if(cont_random_turni <numerogiocatori){
    printf("Ora è il turno del giocatore successivo: %s\n", pGiocatore[turni[turno_attuale] - 1].nome_giocatore);}*/
    avanzato = 0;
    tornato = 0;
    sale_usato = 0;
  }
}
void libera_memoria(){
  struct Zona_mappa* pNext;
  free(pGiocatore);
  while(pFirst != pLast){
    pNext = pFirst->prossima_zona;
    free(pFirst);
    pFirst = pNext;
  }
  free(pLast);
  pFirst = NULL;
  pLast = NULL;
  free(caravan_zona);
  caravan_zona = NULL;
}


  void schermata_sconfitta(){
    system("clear");
    system("clear");
  printf("                                 ▄██████▄     ▄████████   ▄▄▄▄███▄▄▄▄      ▄████████       ▄██████▄   ▄█    █▄     ▄████████    ▄████████ \n ");
  printf("                                ███    ███   ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███      ███    ███ ███    ███   ███    ███   ███    ███\n ");
  printf("                                ███    █▀    ███    ███ ███   ███   ███   ███    █▀       ███    ███ ███    ███   ███    █▀    ███    ███\n ");
  printf("                               ▄███          ███    ███ ███   ███   ███  ▄███▄▄▄          ███    ███ ███    ███  ▄███▄▄▄      ▄███▄▄▄▄██▀\n ");
  printf("                              ▀▀███ ████▄  ▀███████████ ███   ███   ███ ▀▀███▀▀▀          ███    ███ ███    ███ ▀▀███▀▀▀     ▀▀███▀▀▀▀▀\n ");
  printf("                                ███    ███   ███    ███ ███   ███   ███   ███    █▄       ███    ███ ███    ███   ███    █▄  ▀███████████\n ");
  printf("                                ███    ███   ███    ███ ███   ███   ███   ███    ███      ███    ███ ███    ███   ███    ███   ███    ███\n ");
  printf("                                ████████▀    ███    █▀   ▀█   ███   █▀    ██████████       ▀██████▀   ▀██████▀    ██████████   ███    ███\n ");
  printf("                                                                                                                               ██     ██\n ");
  sleep(2);
  libera_memoria();
  }
  void schermata_vittoria(){

      system("clear");
  printf("                                                                   \n ");
  printf("                                                                           \n ");
  printf("                                                                                                                                   \n ");
  printf("\n ");
  printf("\n ");
  printf("                                            ▄█    █▄   ▄█      ███         ███      ▄██████▄     ▄████████  ▄█     ▄████████ \n ");
  printf("                                           ███    ███ ███  ▀█████████▄ ▀█████████▄ ███    ███   ███    ███ ███    ███    ███ \n ");
  printf("                                           ███    ███ ███▌    ▀███▀▀██    ▀███▀▀██ ███    ███   ███    ███ ███▌   ███    ███ \n ");
  printf("                                           ███    ███ ███▌     ███   ▀     ███   ▀ ███    ███  ▄███▄▄▄▄██▀ ███▌   ███    ███ \n ");
  printf("                                           ███    ███ ███▌     ███         ███     ███    ███ ▀▀███▀▀▀▀▀   ███▌ ▀███████████ \n ");
  printf("                                           ███    ███ ███      ███         ███     ███    ███ ▀███████████ ███    ███    ███ \n ");
  printf("                                           ███    ███ ███      ███         ███     ███    ███   ███    ███ ███    ███    ███ \n ");
  printf("                                            ▀██████▀  █▀      ▄████▀      ▄████▀    ▀██████▀    ███    ███ █▀     ███    █▀  \n ");
  printf("                                                                                                ██     ██                  \n ");
  sleep(2);
libera_memoria();

  }
  void genera_ordine_turni() {
    cont_random_turni = 0;
    int posizione, posizionati;
    posizionati = 0;
    bool trovato;

    printf("L'ordine dei turni è: \n");

    do {
      do {
        trovato = false;
        posizione = rand() % (numerogiocatori) + 1;

        if (posizionati != 0) {
          for (int i = 0; i <= posizionati - 1; i++) {

            if (turni[i] == posizione) {
              trovato = true;
            }
          }

        }
      } while (trovato == true);
      turni[posizionati] = posizione;

      posizionati++;

    } while (numerogiocatori != posizionati);

    for (int i = 0; i < numerogiocatori; i++) {
      printf("%d\n", turni[i]);
    }

  } // fine genera turnazioni

  bool sono_tutti_morti(){
    for(int i = 0; i < numerogiocatori; i++){
      if(pGiocatore[i].sanita_mentale > 0){
        return 0;
      }
    }
    return 1;
  }//fine sono_tutti_morti

  void decremento_sanita_mentale_dopo_azione(){
    if(sale_usato == 0){
      int probabilita_sanita_mentale;
      //ad ogni azione compiuta c'è il 20% di probabilità che la sanità mentale -15
      probabilita_sanita_mentale = rand()%5;
      if(probabilita_sanita_mentale == 0){
        pGiocatore[turni[turno_attuale] - 1].sanita_mentale -=15;
        printf("è apparso il fantasma, la tua sanità mentale è scesa di 15 punti...\n");
        printf("Sanità mentale attuale = %d\n", pGiocatore[turni[turno_attuale] - 1].sanita_mentale);
        if(pGiocatore[turni[turno_attuale] - 1].sanita_mentale < 1){
          //printf("ciao");
          if(sono_tutti_morti() == 1){
            schermata_sconfitta();
          }
          passa();
        }
        sleep(1);
      }
    } else{
      printf("Il fantasma appare ma non può farti paura... ");
      sale_usato = 0;
    }
  }

//------------------------------------------------------------------------------------ gioca è qui
void gioca() {
  genera_ordine_turni();

  for (int i = 0; i < numerogiocatori; i++) {
    pGiocatore[i].posizione = pFirst;
  } // mette tutti i giocatori nella prima zona.
  pGiocatore[turni[turno_attuale] - 1].posizione -> prova = 0;
  Zona_mappa * zona_attuale = pGiocatore[turni[turno_attuale] - 1].posizione;
  //zonaa->prova = prova

  //da mettere su avanza.
  if (((rand() % 100) + 1) <= probabilita_prova[difficoltà]) {
    zona_attuale -> prova = (rand() % 3)+20;
  } else {
    zona_attuale -> prova = nessuna_prova_; //3 sta per nessuna prova.
  }

  printf("tocca a: %s\n", (char * ) pGiocatore[turni[turno_attuale] - 1].nome_giocatore);
  printf("ti trovi nella stanza: %s\n", nome_zona[zona_attuale -> zona]);

  //------------------------------------------------------
  int azione;
  do {

    if (sono_tutti_morti()) {
      system("clear");
      schermata_sconfitta();
      //schermata hai perso
      return;
    }
    printf("\n");
    printf("cosa vuoi fare %s?\n",pGiocatore[turni[turno_attuale] - 1].nome_giocatore);
    printf("1 per mostrare il giocatore;\n2 per mostrare la zona;\n3 per avanzare alla prossima stanza;\n4 per tornare al caravan;\n5 per raccogliere una prova;\n6 per raccogliere un oggetto;\n7 per usare un oggetto;\n8 per passare il turno al prossimo giocatore;\n 9 per la funzione richiesta.\n\n");
    scanf("%d", & azione);
    switch (azione) {
    case 1:
      stampa_giocatore();
      break;

    case 2:
      stampa_zona();
      break;

    case 3:
    if(avanzato == 0){
      avanza(); //fatto
      avanzato = 1;
      decremento_sanita_mentale_dopo_azione();
    } else{
      printf("\nnon puoi avanzare più di una volta per turno...\n");
    }
      break;

    case 4:
    if(tornato == 0){
      torna_caravan();
      if(hai_vinto()){
        schermata_vittoria();
        return;
      }
    } else{
      printf("\nnon puoi tornare al caravan più di una volta per turno...\n");
    }
      break;

    case 5:
      raccogli_prova();
      decremento_sanita_mentale_dopo_azione();
      break;

    case 6:
      raccogli_oggetto(); //fatto, problema zaino in stampa giocatore
      decremento_sanita_mentale_dopo_azione();
      break;

    case 7:
      usa_oggetto(); //da fare
      decremento_sanita_mentale_dopo_azione();
      break;

    case 8:
      cont_random_turni+=1;
      if(cont_random_turni == numerogiocatori){
        genera_ordine_turni();
      }
      passa(); //fatto
      break;

    case 9:
    guarda_avanti();

    default:
      printf("digita un numero da 1 a 8");
      sleep(1);
      //system("clear");
      break;

    }
  } while (azione != 10);

}

void termina_gioco() {
  sleep(1);
  system("clear");
  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");
  printf("|                                                      Grazie per aver giocato!                                                                    |\n");
  printf("+--------------------------------------------------------------------------------------------------------------------------------------------------+\n");

}
