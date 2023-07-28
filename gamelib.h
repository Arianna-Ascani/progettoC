#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// definizione delle strutture

typedef struct Zona_mappa Zona_mappa;
typedef struct Giocatore Giocatore;

struct Giocatore {
    char nome_giocatore[21];
    int sanita_mentale;
    Zona_mappa* posizione;
    int zaino[4];
};

enum Tipo_oggetto_zona {
    adrenalina = 5,
    cento_dollari,
    coltello,
    calmanti,
    sale,
    nessun_oggetto
};

enum Tipo_zona {
    caravan = 0,
    cucina = 1,
    soggiorno = 2,
    camera = 3,
    bagno = 4,
    garage = 5,
    seminterrato = 6
};

enum Tipo_prova {
    prova_EMF = 20,
    prova_spirit_box,
    prova_videocamera,
    nessuna_prova
};

struct Zona_mappa {
  enum Tipo_zona zona;
  enum Tipo_oggetto_zona oggetto_zona;
  enum Tipo_prova prova;
  Zona_mappa* prossima_zona;
};


enum Tipo_oggetto_iniziale {
    EMF = 0,
    spirit_box,
    videocamera,
    calmanti_,
    sale_
};



// dichiarazione delle funzioni

void imposta_gioco();

void gioca();

void termina_gioco();

#endif
