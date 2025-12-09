#include "Visual.h"
#include "raylib.h"

using std::time;
using std::time_t;
using std::strftime;

// DECLARACION DE VARIABLES
int ancho_ventana;
int alto_ventana;
bool pantalla_devs = true;
bool desplegar_menu = false;
int opcion_menu = 0;

// BOTONES
BotonImagen boton_menu;
BotonImagen boton_salida;

// DECLARACION DE VARIABLES RAYLIB
Vector2 pos_mouse;
Rectangle zona_no_menu;
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
Texture2D menu_img = { 0 };
Texture2D salida_img = { 0 };
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
	menu_img = LoadTexture("assets/menu.png");
	salida_img = LoadTexture("assets/salida.png");

	SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(logo_devs, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(menu_img, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(salida_img, TEXTURE_FILTER_BILINEAR);
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
	UnloadTexture(menu_img);
	UnloadTexture(salida_img);
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
bool ManejarBotonImagen(BotonImagen boton)
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
void ConfigurarBotonImagen()
{
	boton_menu.textura = menu_img;
	boton_menu.activo = true;
	boton_menu.area = { (ancho_ventana / 3.5f) * 0.15f, alto_ventana * 0.1f, alto_ventana * 0.1f, alto_ventana * 0.1f };

	boton_salida.textura = salida_img;
	boton_salida.activo = true;
	boton_salida.area = { (ancho_ventana / 3.5f) * 0.15f, alto_ventana * 0.85f, alto_ventana * 0.1f, alto_ventana * 0.1f };
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

		DrawTextCenterHorizontal(fuente2, "DESARROLLADO POR:", alto_actual * 0.05f, 0.0f, alto_actual * 0.70f, WHITE);
		DrawTextCenterHorizontal(fuente1, "ANGEL REY RODRIGUEZ", alto_actual * 0.08f, 0.0f, alto_actual * 0.74f, arena);
		DrawTextCenterHorizontal(fuente1, "ISIDRO FRANCISCO PEREZ PAZ", alto_actual * 0.08f, 0.0f, alto_actual * 0.8f, arena);

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

	const float porcentaje_ancho_deseado = 0.80f;
	float ancho_deseado = ancho_menu * porcentaje_ancho_deseado;
	float escala = ancho_deseado / (float)logo.width;
	float ancho_logo_escalado = logo.width * escala;
	float alto_logo_escalado = logo.height * escala;
	float margen_rect = 10.0f;
	float ancho_rect = ancho_logo_escalado + (2 * margen_rect);
	float alto_rect = alto_logo_escalado + (2 * margen_rect);

	float rect_posicion_x = (ancho_menu - ancho_rect) / 2.0f;
	float rect_posicion_y = alto_menu * 0.05f;

	Rectangle area_rect_fondo = { rect_posicion_x,rect_posicion_y,ancho_rect,alto_rect };

	float radio_redondeo = 0.5f;
	int segmentos = 10;
	DrawRectangleRounded(area_rect_fondo, radio_redondeo, segmentos, fondo);

	float logo_posicion_x = rect_posicion_x + margen_rect;
	float logo_posicion_y = rect_posicion_y + margen_rect;

	DrawTextureEx(logo, { logo_posicion_x, logo_posicion_y }, 0.0f, escala, WHITE);

	Vector2 tamanio_texto;

	const char* texto_reservar = "RESERVAR";
	tamanio_texto = MeasureTextEx(fuente1, texto_reservar, tamanio_fuente, 0);
	BotonTexto btn_reservar = { texto_reservar, { ceja_izquierda, alto_menu * 0.2f, tamanio_texto.x, tamanio_texto.y }, fuente1, tamanio_fuente, arena };
	if (ManejarBotonTexto(btn_reservar))
		return 1;

	const char* texto_editar = "EDITAR RESERVAR";
	tamanio_texto = MeasureTextEx(fuente1, texto_editar, tamanio_fuente, 0);
	BotonTexto btn_editar = { texto_editar,{ ceja_izquierda, alto_menu * 0.3f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_editar))
		return 2;

	const char* texto_eliminar = "ELIMINAR RESERVAR";
	tamanio_texto = MeasureTextEx(fuente1, texto_eliminar, tamanio_fuente, 0);
	BotonTexto btn_eliminar = { texto_eliminar,{ ceja_izquierda, alto_menu * 0.4f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_eliminar))
		return 3;

	const char* texto_salas = "SALAS";
	tamanio_texto = MeasureTextEx(fuente1, texto_salas, tamanio_fuente, 0);
	BotonTexto btn_salas = { texto_salas,{ ceja_izquierda, alto_menu * 0.5f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_salas))
		return 4;


	const char* texto_computadoras = "COMPUTADORAS";
	tamanio_texto = MeasureTextEx(fuente1, texto_computadoras, tamanio_fuente, 0);
	BotonTexto btn_computadoras = { texto_computadoras,{ ceja_izquierda, alto_menu * 0.6f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_computadoras))
		return 5;

	const char* texto_usuarios = "USUARIOS";
	tamanio_texto = MeasureTextEx(fuente1, texto_usuarios, tamanio_fuente, 0);
	BotonTexto btn_usuarios = { texto_usuarios,{ ceja_izquierda, alto_menu * 0.7f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_usuarios))
		return 6;

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
	float posision_x = ancho_ventana * 0.85f;
	float posision_y = alto_ventana * 0.15f;

	strftime(buffer, sizeof(buffer), "%I:%M %p", ptr_info);

	DrawTextEx(fuente, buffer, { posision_x, posision_y }, 100, 0, arena);
}
//**********************************************************************************************************************************************
void usuarios(Texture2D logo, Font fuente1, Font fuente2)
{
	Vector2 tamanio_texto;
	float tamanio_fuente = alto_ventana * 0.08f;
	int opcion = 0;
	const char* texto_profesores = "PROFESORES";
	tamanio_texto = MeasureTextEx(fuente1, texto_profesores, tamanio_fuente, 0);
	BotonTexto btn_profesores = { texto_profesores,{ ancho_ventana * 0.3f, alto_ventana * 0.7f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_profesores))
		opcion = 1;

	const char* texto_alumnos = "ALUMNOS";
	tamanio_texto = MeasureTextEx(fuente1, texto_alumnos, tamanio_fuente, 0);
	BotonTexto btn_alumnos = { texto_alumnos,{ ancho_ventana * 0.6f, alto_ventana * 0.7f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,arena };
	if (ManejarBotonTexto(btn_alumnos))
		opcion = 2;

}
