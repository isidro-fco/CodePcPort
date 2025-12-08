#pragma once
#include "raylib.h"
#include <iostream>
#include <string>
#include <cstdbool>
#include <time.h>

typedef struct
{
	Texture2D textura;
	Rectangle area;
	bool activo;
} BotonSimple;

typedef struct
{
	const char *texto;
	Rectangle area;
	Font fuente;
	float tamanio_fuente;
	Color color;
} BotonTexto;

// DECLARACION DE VARIABLES
extern int ancho_ventana;
extern int alto_ventana;
extern bool pantalla_devs;
// DECLARACION DE VARIABLES RAYLIB
// Colores
extern Color fondo;
extern Color verde_oscuro;
extern Color verde_esmeralda;
extern Color verde_pasto;
extern Color verde_manzana;
extern Color arena;
// Archivos
extern Font fuente1;
extern Font fuente2;
extern Texture2D logo;
extern Texture2D logo_devs;


// CARGA Y DESCARGA DE ARCHIVOS
void InicializarFuentes();
void InicializarImagenes();
void DescargarFuentes();
void DescargarImagenes();

void DrawTextCenterHorizontal(Font fuente, const char *texto, float tamanio_fuente, float espaciado, float pos_y, Color color);

Color OscurecerColor(Color original, int cantidad);

bool ManejarBoton(BotonSimple boton);

bool ManejarBotonTexto(BotonTexto boton);

void PantallaDesarrolladores(Texture2D logo, Font fuente1, Font fuente2);

int MenuLateral(Texture2D logo, Font fuente1, Font fuente2);

void Reloj(Font fuente);