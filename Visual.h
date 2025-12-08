#pragma once
#include "raylib.h"
#include <iostream>
#include <string>
#include <cstdbool>
#include <time.h>
#include <vector>

using std::vector;

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

class BotonTexto
{
private:
	const char* texto;
	Rectangle area;
	Font fuente;
	float tamanio_fuente;
	Color color_base;
	int id_accion;
	Color OscurecerColor(Color original, int cantidad) const;
public:
	Color ObtenerColorBase() const { return color_base; }
	BotonTexto(const char* t, Rectangle a, Font f, float tf, Color c, int id) : texto(t), area(a), fuente(f), tamanio_fuente(tf), color_base(c), id_accion(id) {}
	void Dibujar()const;
	int ManejarEntrada() const;
};

class MenuLateral
{
private:
	float ancho;
	float alto;
	Color color_fondo;
	vector<BotonTexto> Botones;
public:
	MenuLateral(float width, float height, Font f1, Color fondo, Color c_texto);
	void RecalcularDimensiones(float nuevo_ancho, float nuevo_alto, Font f1);
	int ActualizarYDibujar(Texture2D logo) const;
};

struct BotonDato
{
	const char* texto;
	float pos_y;
	int id;
};



// CARGA Y DESCARGA DE ARCHIVOS
void InicializarFuentes();
void InicializarImagenes();
void DescargarFuentes();
void DescargarImagenes();

void DrawTextCenterHorizontal(Font fuente, const char* texto, float tamanio_fuente, float espaciado, float pos_y, Color color);

void PantallaDesarrolladores(Texture2D logo, Font fuente1, Font fuente2);

void Reloj(Font fuente);