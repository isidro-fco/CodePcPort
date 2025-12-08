#include "Visual.h"
#include "raylib.h"
#include <time.h>
#include <vector>

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
Texture2D menu_img = { 0 };
Texture2D salida_img = { 0 };

Color BotonTexto::OscurecerColor(Color original, int cantidad) const
{
	Color oscuro = original;

	oscuro.r = (original.r > cantidad) ? (unsigned char)(original.r - cantidad) : 0;
	oscuro.g = (original.g > cantidad) ? (unsigned char)(original.g - cantidad) : 0;
	oscuro.b = (original.b > cantidad) ? (unsigned char)(original.b - cantidad) : 0;

	return oscuro;
}

void BotonTexto::Dibujar() const
{
	Vector2 posision_mouse = GetMousePosition();
	Color color_actual = color_base;

	if (CheckCollisionPointRec(posision_mouse, area))
	{
		color_actual = OscurecerColor(color_base, 40);
	}

	DrawTextEx(fuente, texto, { area.x, area.y }, tamanio_fuente, 0.0f, color_actual);
}

int BotonTexto::ManejarEntrada() const
{
	Vector2 posision_mouse = GetMousePosition();

	if (CheckCollisionPointRec(posision_mouse, area))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			return id_accion;
		}
	}

	return 0;
}

MenuLateral::MenuLateral(float width, float height, Font f1, Color fondo, Color c_texto)
{
	ancho = width / 3.5f;
	alto = height;
	color_fondo = fondo;

	float ceja_izquierda = ancho * 0.15f;
	float tamanio_fuente = alto * 0.08f;
	
	// Arreglo que guarda las informacion de los botones
	BotonDato datos_botones[] = { {"RESERVAR", 0.3f, 1}, {"EDITAR RESERVAR", 0.4f, 2},{"ELIMINAR RESERVAR",0.5f, 3},
		{"SALAS",0.6f, 4},{"COMPUTADORAS", 0.7f, 5},{"USUARIOS",0.8f, 6} };

	for (const auto& dato : datos_botones)
	{
		Vector2 tamanio_texto = MeasureTextEx(f1, dato.texto, tamanio_fuente, 0);
		Rectangle area_boton = { ceja_izquierda, alto * dato.pos_y, tamanio_texto.x, tamanio_texto.y };

		// Crea la instancia y la almacena en el vector
		Botones.emplace_back(dato.texto, area_boton, f1, tamanio_fuente, c_texto, dato.id);
	}
}

void MenuLateral::RecalcularDimensiones(float nuevo_ancho, float nuevo_alto, Font f1)
{
	ancho = nuevo_ancho / 3.5f;
	alto = nuevo_alto;

	float ceja_izquierda = ancho * 0.15f;
	float tamanio_fuente = alto * 0.08f;

	BotonDato datos_botones[] = {
		{"RESERVAR", 0.3f, 1}, {"EDITAR RESERVAR", 0.4f, 2}, {"ELIMINAR RESERVAR", 0.5f, 3},
		{"SALAS", 0.6f, 4}, {"COMPUTADORAS", 0.7f, 5}, {"USUARIOS", 0.8f, 6 }
	};

	for (size_t i = 0; i < Botones.size(); ++i)
	{
		const auto& dato = datos_botones[i];

		Vector2 tamanio_texto = MeasureTextEx(f1, dato.texto, tamanio_fuente, 0);

		Rectangle area_boton = { ceja_izquierda, alto * dato.pos_y, tamanio_texto.x, tamanio_texto.y };
		Botones[i] = BotonTexto(dato.texto, area_boton, f1, tamanio_fuente, Botones[i].ObtenerColorBase(), dato.id);
	}
}

int MenuLateral::ActualizarYDibujar(Texture2D logo) const
{
	DrawRectangleRec({ 0,0,ancho, alto }, color_fondo);

	for (const auto& boton : Botones)
	{
		boton.Dibujar();

		int resultado = boton.ManejarEntrada();

		if (resultado != 0)
		{
			return resultado;
		}
	}

	return 0;
}

void InicializarFuentes()
{
	fuente1 = LoadFontEx("assets/BebasNeue-Regular.ttf", 120.0f, 0, 0.0f);
	fuente2 = LoadFontEx("assets/ChauPhilomeneOne-Regular.ttf", 120.0f, 0, 0.0f);

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

void DrawTextCenterHorizontal(Font fuente, const char* texto, float tamanio_fuente, float espaciado, float pos_y, Color color)
{
	Vector2 tamano_medido = MeasureTextEx(fuente, texto, tamanio_fuente, espaciado);
	float ancho_texto = tamano_medido.x;
	float posision_x = (ancho_ventana - ancho_texto) / 2.0f;
	Vector2 posicion_dibujo = { posision_x, pos_y };
	DrawTextEx(fuente, texto, posicion_dibujo, tamanio_fuente, espaciado, color);
}

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

		float posision_x = (ancho_actual - logo.width * escala) / 2.0f;
		float posision_y = (alto_actual - logo.height * escala) / 2.0f;

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

	std::strftime(buffer, sizeof(buffer), "%I:%M %p", ptr_info);

	DrawTextEx(fuente, buffer, { posision_x, posision_y }, 100.0f, 0.0f, arena);
}
