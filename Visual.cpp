#include "Visual.h"
#include "raylib.h"

using std::time;
using std::time_t;

// DECLARACION DE VARIABLES
int ancho_ventana;
int alto_ventana;
bool pantalla_devs = true;
// DECLARACION DE VARIABLES RAYLIB
// Colores
Color fondo = { 54, 54, 54, 255 };
Color verde_oscuro = { 7, 59, 58, 255 };
Color verde_esmeralda = { 11, 110, 79, 255 };
Color verde_pasto = { 8, 160, 69, 255 };
Color verde_manzana = { 107, 191, 89, 255 };
Color arena = { 221, 183, 113, 255 };
// Archivos
Font fuente1 = { 0 };
Font fuente2 = { 0 };
Texture2D logo = { 0 };
Texture2D logo_devs = { 0 };
//**********************************************************************************************************************************************
// CARGA Y DESCARGA DE ARCHIVOS
void InicializarFuentes()
{
	fuente1 = LoadFontEx("assets/BebasNeue-Regular.ttf", 120, 0, 0);
	fuente2 = LoadFontEx("assets/ChauPhilomeneOne-Regular.ttf", 120, 0, 0);

	SetTextureFilter(fuente1.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(fuente2.texture, TEXTURE_FILTER_BILINEAR);
}

void InicializarImagenes()
{
	logo = LoadTexture("assets/logo.png");
	logo_devs = LoadTexture("assets/logo_devs.png");

	SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(logo_devs, TEXTURE_FILTER_BILINEAR);
}

void DescargarFuentes()
{
	UnloadFont(fuente1);
	UnloadFont(fuente2);
}

void DescargarImagenes()
{
	UnloadTexture(logo);
	UnloadTexture(logo_devs);
}
//**********************************************************************************************************************************************
void DrawTextCenterHorizontal(Font fuente, const char* texto, float tamanio_fuente, float espaciado, float pos_y, Color color)
{
	Vector2 tamano_medido = MeasureTextEx(fuente, texto, tamanio_fuente, espaciado);
	float ancho_texto = tamano_medido.x;
	float posision_x = (ancho_ventana - ancho_texto) / 2;
	Vector2 posicion_dibujo = { posision_x, pos_y };
	DrawTextEx(fuente, texto, posicion_dibujo, tamanio_fuente, espaciado, color);
}
//**********************************************************************************************************************************************
Color OscurecerColor(Color original, int cantidad)
{
	Color oscuro = original;

	oscuro.r = (original.r > cantidad) ? original.r - cantidad : 0;
	oscuro.g = (original.g > cantidad) ? original.g - cantidad : 0;
	oscuro.b = (original.b > cantidad) ? original.b - cantidad : 0;

	return oscuro;
}
//**********************************************************************************************************************************************
bool ManejarBoton(BotonSimple boton)
{
	Vector2 posision_mouse = GetMousePosition();
	Color color_btn = WHITE;
	bool boton_presionado = false;

	if (CheckCollisionPointRec(posision_mouse, boton.area))
	{
		color_btn = GRAY;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			boton_presionado = true;
		}
	}

	DrawTexturePro(boton.textura, { 0, 0, (float)boton.textura.width, (float)boton.textura.height }, boton.area, { 0, 0 }, 0.0f, color_btn);

	return boton_presionado;
}
//**********************************************************************************************************************************************
bool ManejarBotonTexto(BotonTexto boton)
{
	Vector2 posision_mouse = GetMousePosition();
	Color color_base = boton.color;
	Color color_actual = color_base;
	bool boton_presionado = false;

	if (CheckCollisionPointRec(posision_mouse, boton.area))
	{

		color_actual = OscurecerColor(color_base, 40);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			boton_presionado = true;
		}
	}

	DrawTextEx(boton.fuente, boton.texto, { boton.area.x, boton.area.y }, (float)boton.tamanio_fuente, 0, color_actual);

	return boton_presionado;
}

//**********************************************************************************************************************************************
void PantallaDesarrolladores(Texture2D logo, Font fuente1, Font fuente2)
{
	bool tiempo_terminado = false;
	double tiempo_inicio = GetTime();
	double duracion = 4.0;
	float escala = 0.7f;

	while (!WindowShouldClose() && !tiempo_terminado)
	{
		int ancho_actual = GetScreenWidth();
		int alto_actual = GetScreenHeight();

		float posision_x = (ancho_actual - logo.width * escala) / 2;
		float posision_y = (alto_actual - logo.height * escala) / 2;

		if (GetTime() - tiempo_inicio >= duracion)
		{
			tiempo_terminado = true;
		}

		BeginDrawing();
		ClearBackground(fondo);
		DrawTextureEx(logo, { posision_x, posision_y }, 0.0f, escala, WHITE);

		DrawTextCenterHorizontal(fuente2, "DESARROLLADO POR:", alto_actual * 0.05, 0, alto_actual * 0.70, WHITE);
		DrawTextCenterHorizontal(fuente1, "ANGEL REY RODRIGUEZ", alto_actual * 0.08, 0, alto_actual * 0.74, arena);
		DrawTextCenterHorizontal(fuente1, "ISIDRO FRANCISCO PEREZ PAZ", alto_actual * 0.08, 0, alto_actual * 0.8, arena);

		EndDrawing();
	}
}
//**********************************************************************************************************************************************
int MenuLateral(Texture2D logo, Font fuente1, Font fuente2)
{
	float alto_menu = alto_ventana;
	float ancho_menu = ancho_ventana / 3.5f;
	float ceja_izquierda = ancho_menu * 0.15f;
	float tamanio_fuente = alto_menu * 0.08f;

	DrawRectangleRec({ 0, 0, ancho_menu, alto_menu }, verde_esmeralda);

	Vector2 tamanioTexto;

	const char* text1 = "RESERVAR";
	tamanioTexto = MeasureTextEx(fuente1, text1, tamanio_fuente, 0);
	BotonTexto btnReservar = { text1, { ceja_izquierda, alto_menu * 0.3f, tamanioTexto.x, tamanioTexto.y }, fuente1, tamanio_fuente, arena };
	if (ManejarBotonTexto(btnReservar))
		return 1;

	const char* text2 = "EDITAR RESERVAR";
	tamanioTexto = MeasureTextEx(fuente1, text2, tamanio_fuente, 0);
	BotonTexto btnEditar = {
		text2,
		{ ceja_izquierda, alto_menu * 0.4f, tamanioTexto.x, tamanioTexto.y },
		fuente1,
		tamanio_fuente,
		arena
	};
	if (ManejarBotonTexto(btnEditar)) {
		return 2;
	}

	const char* text3 = "ELIMINAR RESERVAR";
	tamanioTexto = MeasureTextEx(fuente1, text3, tamanio_fuente, 0);
	BotonTexto btnEliminar = {
		text3,
		{ ceja_izquierda, alto_menu * 0.5f, tamanioTexto.x, tamanioTexto.y },
		fuente1,
		tamanio_fuente,
		arena
	};
	if (ManejarBotonTexto(btnEliminar)) {
		return 3;
	}

	const char* text4 = "SALAS";
	tamanioTexto = MeasureTextEx(fuente1, text4, tamanio_fuente, 0);
	BotonTexto btnSalas = {
		text4,
		{ ceja_izquierda, alto_menu * 0.6f, tamanioTexto.x, tamanioTexto.y },
		fuente1,
		tamanio_fuente,
		arena
	};
	if (ManejarBotonTexto(btnSalas)) {
		return 4;
	}

	const char* text5 = "COMPUTADORAS";
	tamanioTexto = MeasureTextEx(fuente1, text5, tamanio_fuente, 0);
	BotonTexto btnComputadoras = {
		text5,
		{ ceja_izquierda, alto_menu * 0.7f, tamanioTexto.x, tamanioTexto.y },
		fuente1,
		tamanio_fuente,
		arena
	};
	if (ManejarBotonTexto(btnComputadoras)) {
		return 5;
	}

	const char* text6 = "USUARIOS";
	tamanioTexto = MeasureTextEx(fuente1, text6, tamanio_fuente, 0);
	BotonTexto btnUsuarios = {
		text6,
		{ ceja_izquierda, alto_menu * 0.8f, tamanioTexto.x, tamanioTexto.y },
		fuente1,
		tamanio_fuente,
		arena
	};
	if (ManejarBotonTexto(btnUsuarios)) {
		return 6;
	}

	return 0;
}
//**********************************************************************************************************************************************
void Reloj(Font fuente)
{
	time_t tiempo_actual = time(nullptr);
	struct tm info_tiempo;
	localtime_s(&info_tiempo, &tiempo_actual);
	const struct tm* ptr_info = &info_tiempo;
	char buffer[100];
	// Posision en ventana
	float posision_x = ancho_ventana * 0.85;
	float posision_y = alto_ventana * 0.15;

	std::strftime(buffer, sizeof(buffer), "%I:%M %p", ptr_info);

	DrawTextEx(fuente, buffer, { posision_x, posision_y }, 100, 0, arena);
}

//**********************************************************************************************************************************************