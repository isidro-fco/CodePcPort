#include "raylib.h"
#include <string>
#include <vector>
#include "Visual.h"
#include "Gestor.h"

using std::time;
using std::time_t;
using std::string;
using std::strftime;
using std::to_string;
using std::vector;

// DECLARACION DE VARIABLES
int ancho_ventana;
int alto_ventana;
bool pantalla_devs = true;
bool desplegar_menu = false;
int opcion_menu = 0;
static int etapa_edicion = 1;
static char entrada_matricula[16] = "\0";
static char entrada_hora_inicio[6] = "\0";
static char entrada_hora_fin[6] = "\0";
static char entrada_pc_num[3] = "\0";
static char entrada_hora[6] = "\0";
static char entrada_nueva_hora[6] = "\0";
static int campo_activo = 0;
static bool mostrar_error = false;
static bool reserva_encontrada = false;
static int tipo_reserva_seleccionado = 0;
static int sala_seleccionada = -1;
static int campo_activo_res = 0;
static bool mostrar_error_res = false;
static string tipo_usuario_detectado = "";
static string mensaje_error = "";

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
Color azul_marino = { 0, 78, 100, 255 };
Color azul_turquesa = { 0, 165, 207, 255 };
Color verde_menta = { 159, 255, 203, 255 };
Color verde_mar = { 37, 161, 142, 255 };
Color verde_lima = { 122, 229, 130, 255 };
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
	BotonTexto btn_reservar = { texto_reservar, { ceja_izquierda, alto_menu * 0.2f, tamanio_texto.x, tamanio_texto.y }, fuente1, tamanio_fuente, verde_menta };
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
	float posision_y = alto_ventana * 0.05f;

	strftime(buffer, sizeof(buffer), "%I:%M %p", ptr_info);

	DrawTextEx(fuente, buffer, { posision_x, posision_y }, 100, 0, verde_menta);
}
//**********************************************************************************************************************************************
void CrearReserva(Font fuente1, Font fuente2, Gestor& gestor)
{
	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_seccion_titulo = alto_ventana * 0.05f;
	float tamano_titulo_principal = alto_ventana * 0.06f;
	float espaciado = 0.0f;
	float altura_linea = tamano_fuente_datos * 1.5f;

	float pos_y_titulo_principal = alto_ventana * 0.3f;
	float inicio_y_campos = pos_y_titulo_principal + (alto_ventana * 0.08f);

	float ancho_campo = ancho_ventana * 0.4f;
	float altura_campo = alto_ventana * 0.07f;
	float altura_texto_simple = MeasureTextEx(fuente2, "A", tamano_fuente_datos, espaciado).y;

	Vector2 posicion_mouse = GetMousePosition();
	int grosor_borde = 2;
	Color color_fondo_campo = WHITE;

	const char* texto_btn_siguiente = "SIGUIENTE >>";
	const char* texto_btn_reservar = "CONFIRMAR RESERVA";
	float ancho_btn_base = ancho_ventana * 0.25f;
	float altura_btn_base = altura_campo;

	const char* titulo_texto = "CREAR NUEVA RESERVA";
	DrawTextCenterHorizontal(fuente1, titulo_texto, tamano_titulo_principal, espaciado, alto_ventana * 0.25f, verde_manzana);

	const char* descripcion_etapa;
	if (etapa_edicion == 1) {
		descripcion_etapa = "Identificacion del Usuario";
	}
	else if (etapa_edicion == 2) {
		descripcion_etapa = "Seleccion del Tipo de Reserva";
	}
	else if (etapa_edicion == 3) {
		descripcion_etapa = "Definicion de Horario";
	}
	else if (etapa_edicion == 4) {
		descripcion_etapa = "Seleccion de Sala / PC";
	}
	else {
		descripcion_etapa = "Reserva Creada";
	}
	DrawTextCenterHorizontal(fuente2, descripcion_etapa, tamano_seccion_titulo * 0.8f, espaciado, pos_y_titulo_principal, WHITE);

	float x_etiqueta = ancho_ventana * 0.3f;
	float x_campo = ancho_ventana * 0.45f;
	float y_actual = inicio_y_campos;

	Rectangle area_matricula = { x_campo, inicio_y_campos, ancho_campo, altura_campo };
	Rectangle area_hora_inicio = { x_campo, inicio_y_campos, ancho_campo, altura_campo };
	Rectangle area_hora_fin = { x_campo, inicio_y_campos + altura_campo + altura_linea, ancho_campo, altura_campo };
	Rectangle area_pc_num = { x_campo, inicio_y_campos, ancho_campo, altura_campo };

	if (campo_activo_res == 1) {
		ManejarEntradaMatricula();
	}
	else if (campo_activo_res == 3) {
		ManejarEntradaHora(entrada_hora_inicio);
	}
	else if (campo_activo_res == 4) {
		ManejarEntradaHora(entrada_hora_fin);
	}
	else if (campo_activo_res == 5) {
		ManejarEntradaNumerica(entrada_pc_num, 2);
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		campo_activo_res = 0;
		mostrar_error_res = false;
		mensaje_error = "";

		if (etapa_edicion == 1 && CheckCollisionPointRec(posicion_mouse, area_matricula))
			campo_activo_res = 1;
		else if (etapa_edicion == 3) {
			if (CheckCollisionPointRec(posicion_mouse, area_hora_inicio)) campo_activo_res = 3;
			else if (CheckCollisionPointRec(posicion_mouse, area_hora_fin)) campo_activo_res = 4;
		}
		else if (etapa_edicion == 4 && tipo_reserva_seleccionado == 2 && CheckCollisionPointRec(posicion_mouse, area_pc_num))
			campo_activo_res = 5;
	}
	// ETAPA 1: MATRÍCULA
	if (etapa_edicion == 1) {
		DrawTextEx(fuente2, "MATRICULA:", { x_etiqueta, area_matricula.y }, tamano_fuente_datos, espaciado, arena);
		Color color_borde = (campo_activo_res == 1) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_matricula, color_borde);
		DrawRectangle(area_matricula.x + grosor_borde, area_matricula.y + grosor_borde,
			area_matricula.width - 2 * grosor_borde, area_matricula.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_matricula, { area_matricula.x + 10, area_matricula.y + (altura_campo - altura_texto_simple) / 2 },
			tamano_fuente_datos, espaciado, BLACK);

		y_actual = area_matricula.y + altura_campo + altura_linea * 2.0f;

		// BOTÓN SIGUIENTE
		Rectangle area_btn_sig = { x_campo + ancho_campo / 2, y_actual, ancho_btn_base, altura_btn_base };
		DrawRectangleRounded(area_btn_sig, 0.5f, 10, azul_turquesa);
		DrawTextEx(fuente1, texto_btn_siguiente, { area_btn_sig.x + 20, area_btn_sig.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn_sig) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (strlen(entrada_matricula) > 0) {
				// Verificar si es profesor o alumno
				Usuario* usuario = gestor.buscarUsuario(entrada_matricula);
				if (usuario) {
					tipo_usuario_detectado = usuario->getTipo();
					if (tipo_usuario_detectado == "Profesor") {
						etapa_edicion = 2;
					}
					else if (tipo_usuario_detectado == "Alumno") {
						// Los alumnos solo pueden reservar PCs
						tipo_reserva_seleccionado = 2;
						etapa_edicion = 3;
					}
					mostrar_error_res = false;
				}
				else {
					mostrar_error_res = true;
					mensaje_error = "Matrícula no encontrada";
				}
			}
			else {
				mostrar_error_res = true;
				mensaje_error = "Ingrese una matrícula";
			}
		}
	}
	// ETAPA 2: TIPO DE RESERVA (Solo para Profesores)
	else if (etapa_edicion == 2) {
		DrawTextEx(fuente2, "TIPO DE RESERVA:", { x_etiqueta, y_actual }, tamano_fuente_datos, espaciado, arena);
		y_actual += altura_linea * 2.0f;

		float x_btn = ancho_ventana * 0.25f;
		float ancho_opcion = ancho_ventana * 0.2f;

		Rectangle area_sala = { x_btn, y_actual, ancho_opcion, altura_campo };
		Color color_sala = (tipo_reserva_seleccionado == 1) ? verde_esmeralda : LIGHTGRAY;
		DrawRectangleRounded(area_sala, 0.3f, 5, color_sala);
		DrawTextEx(fuente2, "SALA A/B", { area_sala.x + 10, area_sala.y + 10 }, tamano_fuente_datos, espaciado, BLACK);
		if (CheckCollisionPointRec(posicion_mouse, area_sala) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			tipo_reserva_seleccionado = 1;

		Rectangle area_pc = { x_btn + ancho_opcion + 20, y_actual, ancho_opcion, altura_campo };
		Color color_pc = (tipo_reserva_seleccionado == 2) ? verde_esmeralda : LIGHTGRAY;
		DrawRectangleRounded(area_pc, 0.3f, 5, color_pc);
		DrawTextEx(fuente2, "PC EN SALA C", { area_pc.x + 10, area_pc.y + 10 }, tamano_fuente_datos, espaciado, BLACK);
		if (CheckCollisionPointRec(posicion_mouse, area_pc) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			tipo_reserva_seleccionado = 2;

		y_actual += altura_campo + altura_linea * 2.0f;

		Rectangle area_btn_sig = { x_campo + ancho_campo / 2, y_actual, ancho_btn_base, altura_btn_base };
		DrawRectangleRounded(area_btn_sig, 0.5f, 10, azul_turquesa);
		DrawTextEx(fuente1, texto_btn_siguiente, { area_btn_sig.x + 20, area_btn_sig.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn_sig) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (tipo_reserva_seleccionado != 0) {
				etapa_edicion = 3;
				mostrar_error_res = false;
			}
			else {
				mostrar_error_res = true;
				mensaje_error = "Seleccione un tipo de reserva";
			}
		}
	}
	// ETAPA 3: HORARIO (Inicio y Fin)
	else if (etapa_edicion == 3) {
		Rectangle area_hora_inicio_draw = { x_campo, y_actual, ancho_campo, altura_campo };
		Rectangle area_hora_fin_draw = { x_campo, y_actual + altura_campo + altura_linea, ancho_campo, altura_campo };

		DrawTextEx(fuente2, "HORA INICIO (HH:MM):", { x_etiqueta, area_hora_inicio_draw.y }, tamano_fuente_datos, espaciado, arena);
		Color color_borde_hi = (campo_activo_res == 3) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_hora_inicio_draw, color_borde_hi);
		DrawRectangle(area_hora_inicio_draw.x + grosor_borde, area_hora_inicio_draw.y + grosor_borde,
			area_hora_inicio_draw.width - 2 * grosor_borde, area_hora_inicio_draw.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_hora_inicio, { area_hora_inicio_draw.x + 10, area_hora_inicio_draw.y + (altura_campo - altura_texto_simple) / 2 },
			tamano_fuente_datos, espaciado, BLACK);

		DrawTextEx(fuente2, "HORA FIN (HH:MM):", { x_etiqueta, area_hora_fin_draw.y }, tamano_fuente_datos, espaciado, arena);
		Color color_borde_hf = (campo_activo_res == 4) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_hora_fin_draw, color_borde_hf);
		DrawRectangle(area_hora_fin_draw.x + grosor_borde, area_hora_fin_draw.y + grosor_borde,
			area_hora_fin_draw.width - 2 * grosor_borde, area_hora_fin_draw.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_hora_fin, { area_hora_fin_draw.x + 10, area_hora_fin_draw.y + (altura_campo - altura_texto_simple) / 2 },
			tamano_fuente_datos, espaciado, BLACK);

		y_actual = area_hora_fin_draw.y + altura_campo + altura_linea * 2.0f;

		Rectangle area_btn_sig = { x_campo + ancho_campo / 2, y_actual, ancho_btn_base, altura_btn_base };
		DrawRectangleRounded(area_btn_sig, 0.5f, 10, azul_turquesa);
		DrawTextEx(fuente1, texto_btn_siguiente, { area_btn_sig.x + 20, area_btn_sig.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn_sig) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			bool hora_inicio_valida = ValidarFormatoHoraSimple(entrada_hora_inicio);
			bool hora_fin_valida = ValidarFormatoHoraSimple(entrada_hora_fin);

			if (hora_inicio_valida && hora_fin_valida) {
				etapa_edicion = 4;
				mostrar_error_res = false;
			}
			else {
				mostrar_error_res = true;
				if (!hora_inicio_valida && !hora_fin_valida) {
					mensaje_error = "Formato de hora invalido (HH:MM)";
				}
				else if (!hora_inicio_valida) {
					mensaje_error = "Hora inicio invalida";
				}
				else {
					mensaje_error = "Hora fin invalida";
				}
			}
		}
	}
	// ETAPA 4: SELECCIÓN FINAL (Sala A/B o PC #)
	else if (etapa_edicion == 4) {
		if (tipo_reserva_seleccionado == 1) {
			DrawTextEx(fuente2, "SELECCIONAR SALA:", { x_etiqueta, y_actual }, tamano_fuente_datos, espaciado, arena);
			y_actual += altura_linea;

			float x_btn = ancho_ventana * 0.25f;
			float ancho_opcion = ancho_ventana * 0.2f;

			Rectangle area_sala_a = { x_btn, y_actual, ancho_opcion, altura_campo };
			Color color_sala_a = (sala_seleccionada == 0) ? verde_esmeralda : LIGHTGRAY;
			DrawRectangleRounded(area_sala_a, 0.3f, 5, color_sala_a);
			DrawTextEx(fuente2, "SALA A", { area_sala_a.x + 10, area_sala_a.y + 10 }, tamano_fuente_datos, espaciado, BLACK);
			if (CheckCollisionPointRec(posicion_mouse, area_sala_a) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				sala_seleccionada = 0;

			Rectangle area_sala_b = { x_btn + ancho_opcion + 20, y_actual, ancho_opcion, altura_campo };
			Color color_sala_b = (sala_seleccionada == 1) ? verde_esmeralda : LIGHTGRAY;
			DrawRectangleRounded(area_sala_b, 0.3f, 5, color_sala_b);
			DrawTextEx(fuente2, "SALA B", { area_sala_b.x + 10, area_sala_b.y + 10 }, tamano_fuente_datos, espaciado, BLACK);
			if (CheckCollisionPointRec(posicion_mouse, area_sala_b) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				sala_seleccionada = 1;

			y_actual += altura_campo + altura_linea * 2.0f;
		}
		else {
			Rectangle area_pc_num_draw = { x_campo, y_actual, ancho_campo, altura_campo };

			DrawTextEx(fuente2, "NÚMERO DE PC (1-20):", { x_etiqueta, area_pc_num_draw.y }, tamano_fuente_datos, espaciado, arena);
			Color color_borde_pc = (campo_activo_res == 5) ? GREEN : LIGHTGRAY;
			DrawRectangleRec(area_pc_num_draw, color_borde_pc);
			DrawRectangle(area_pc_num_draw.x + grosor_borde, area_pc_num_draw.y + grosor_borde,
				area_pc_num_draw.width - 2 * grosor_borde, area_pc_num_draw.height - 2 * grosor_borde, color_fondo_campo);
			DrawTextEx(fuente1, entrada_pc_num, { area_pc_num_draw.x + 10, area_pc_num_draw.y + (altura_campo - altura_texto_simple) / 2 },
				tamano_fuente_datos, espaciado, BLACK);

			y_actual += altura_campo + altura_linea * 2.0f;
		}

		Rectangle area_btn_res = { x_campo + ancho_campo / 2, y_actual, ancho_btn_base * 1.5f, altura_btn_base };
		DrawRectangleRounded(area_btn_res, 0.5f, 10, verde_pasto);
		DrawTextEx(fuente1, texto_btn_reservar, { area_btn_res.x + 20, area_btn_res.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn_res) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			bool datos_validos = false;

			if (tipo_reserva_seleccionado == 1) {
				if (sala_seleccionada != -1) {
					datos_validos = true;
					string matricula_str = entrada_matricula;
					string hora_inicio_str = entrada_hora_inicio;
					string hora_fin_str = entrada_hora_fin;

					gestor.reservarSala(matricula_str, hora_inicio_str, hora_fin_str, sala_seleccionada);
				}
				else {
					mostrar_error_res = true;
				}
			}
			else {
				if (ValidarRangoPC(entrada_pc_num)) {
					datos_validos = true;
					int pc_num = atoi(entrada_pc_num) - 1;
					string matricula_str = entrada_matricula;
					string hora_inicio_str = entrada_hora_inicio;
					string hora_fin_str = entrada_hora_fin;

					gestor.reservarPC(matricula_str, hora_inicio_str, hora_fin_str, pc_num);
				}
				else {
					mostrar_error_res = true;
				}
			}

			if (datos_validos) {
				etapa_edicion = 5;
			}
		}
	}
	// ETAPA 5: CONFIRMACIÓN/ÉXITO
	else if (etapa_edicion == 5) {
		DrawTextCenterHorizontal(fuente1, "¡RESERVA CREADA CON ÉXITO!", tamano_seccion_titulo, espaciado, y_actual, verde_lima);
		y_actual += altura_linea * 2.0f;

		DrawTextCenterHorizontal(fuente2, "Resumen de la reserva:", tamano_fuente_datos, espaciado, y_actual, arena);
		y_actual += altura_linea;

		string resumen = "Matricula: " + string(entrada_matricula) + "\n" +
			"Hora inicio: " + string(entrada_hora_inicio) + "\n" +
			"Hora fin: " + string(entrada_hora_fin);

		if (tipo_reserva_seleccionado == 1) {
			resumen += "\nSala: " + string(sala_seleccionada == 0 ? "A" : "B");
		}
		else {
			resumen += "\nPC #: " + string(entrada_pc_num);
		}

		size_t pos = 0;
		string token;
		string delimiter = "\n";
		string temp = resumen;
		while ((pos = temp.find(delimiter)) != string::npos) {
			token = temp.substr(0, pos);
			DrawTextCenterHorizontal(fuente2, token.c_str(), tamano_fuente_datos, espaciado, y_actual, WHITE);
			y_actual += altura_linea;
			temp.erase(0, pos + delimiter.length());
		}
		DrawTextCenterHorizontal(fuente2, temp.c_str(), tamano_fuente_datos, espaciado, y_actual, WHITE);

		y_actual += altura_linea * 2.0f;

		Rectangle area_btn_volver = { x_campo + ancho_campo / 2, y_actual, ancho_btn_base, altura_btn_base };
		DrawRectangleRounded(area_btn_volver, 0.5f, 10, azul_turquesa);
		DrawTextEx(fuente1, "VOLVER AL MENU", { area_btn_volver.x + 20, area_btn_volver.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn_volver) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			LimpiarCamposEntrada();
			opcion_menu = 0;
		}
	}

	if (mostrar_error_res && !mensaje_error.empty()) {
		float ancho_error = MeasureTextEx(fuente2, mensaje_error.c_str(), tamano_fuente_datos * 0.8f, espaciado).x;
		Vector2 pos_error = { (ancho_ventana - ancho_error) / 2, alto_ventana * 0.9f };
		DrawTextEx(fuente2, mensaje_error.c_str(), pos_error, tamano_fuente_datos * 0.8f, espaciado, RED);
	}
}
//**********************************************************************************************************************************************
void EditarReserva(Font fuente1, Font fuente2, Gestor& gestor)
{
	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_seccion_titulo = alto_ventana * 0.05f;
	float tamano_titulo_principal = alto_ventana * 0.06f;
	float espaciado = 0.0f;
	float altura_linea = tamano_fuente_datos * 1.5f;

	float pos_y_titulo_principal = alto_ventana * 0.3f;
	float inicio_y_campos = pos_y_titulo_principal + (alto_ventana * 0.08f);

	float ancho_campo = ancho_ventana * 0.4f;
	float altura_campo = alto_ventana * 0.07f;
	float altura_texto_simple = MeasureTextEx(fuente2, "A", tamano_fuente_datos, espaciado).y;

	Vector2 posicion_mouse = GetMousePosition();
	int grosor_borde = 2;
	Color color_fondo_campo = WHITE;

	const char* titulo_texto = "EDITAR RESERVA";
	DrawTextCenterHorizontal(fuente1, titulo_texto, tamano_titulo_principal, espaciado, alto_ventana * 0.25f, verde_manzana);

	const char* subtitulo = (etapa_edicion == 1)
		? "Paso 1: Ingrese la reserva a modificar:"
		: "Paso 2: Ingrese los nuevos valores (ej. Nueva Hora):";
	DrawTextCenterHorizontal(fuente2, subtitulo, tamano_seccion_titulo * 0.8f, espaciado, pos_y_titulo_principal, WHITE);

	int tecla = GetCharPressed();
	while (tecla > 0)
	{
		if (tecla >= 32 && tecla <= 125)
		{
			if (campo_activo == 1 && strlen(entrada_matricula) < 15)
				entrada_matricula[strlen(entrada_matricula)] = (char)tecla;
			else if (campo_activo == 2 && strlen(entrada_hora) < 5)
				entrada_hora[strlen(entrada_hora)] = (char)tecla;
			else if (campo_activo == 3 && strlen(entrada_nueva_hora) < 5)
				entrada_nueva_hora[strlen(entrada_nueva_hora)] = (char)tecla;
		}
		tecla = GetCharPressed();
	}
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		if (campo_activo == 1 && strlen(entrada_matricula) > 0)
			entrada_matricula[strlen(entrada_matricula) - 1] = '\0';
		else if (campo_activo == 2 && strlen(entrada_hora) > 0)
			entrada_hora[strlen(entrada_hora) - 1] = '\0';
		else if (campo_activo == 3 && strlen(entrada_nueva_hora) > 0)
			entrada_nueva_hora[strlen(entrada_nueva_hora) - 1] = '\0';
	}

	float x_etiqueta = ancho_ventana * 0.3f;
	float x_campo = ancho_ventana * 0.45f;
	float y_actual = inicio_y_campos;

	Rectangle area_matricula = { x_campo, y_actual, ancho_campo, altura_campo };
	Rectangle area_hora_vieja = { x_campo, y_actual + altura_campo + altura_linea, ancho_campo, altura_campo };
	Rectangle area_nueva_hora = { x_campo, y_actual, ancho_campo, altura_campo };

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		campo_activo = 0;
		if (etapa_edicion == 1) {
			if (CheckCollisionPointRec(posicion_mouse, area_matricula))
				campo_activo = 1;
			else if (CheckCollisionPointRec(posicion_mouse, area_hora_vieja))
				campo_activo = 2;
		}
		else if (etapa_edicion == 2) {
			if (CheckCollisionPointRec(posicion_mouse, area_nueva_hora))
				campo_activo = 3;
		}
		mostrar_error = false;
	}
	// ETAPA 1: IDENTIFICACIÓN DE LA RESERVA (Matrícula y Hora Vieja)
	if (etapa_edicion == 1)
	{
		DrawTextEx(fuente2, "MATRICULA:", { x_etiqueta, area_matricula.y }, tamano_fuente_datos, espaciado, arena);
		Color color_borde = (campo_activo == 1) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_matricula, color_borde);
		DrawRectangle(area_matricula.x + grosor_borde, area_matricula.y + grosor_borde, area_matricula.width - 2 * grosor_borde, area_matricula.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_matricula, { area_matricula.x + 10, area_matricula.y + (altura_campo - altura_texto_simple) / 2 }, tamano_fuente_datos, espaciado, BLACK);

		DrawTextEx(fuente2, "HORA ACTUAL (HH:MM):", { x_etiqueta, area_hora_vieja.y }, tamano_fuente_datos, espaciado, arena);
		color_borde = (campo_activo == 2) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_hora_vieja, color_borde);
		DrawRectangle(area_hora_vieja.x + grosor_borde, area_hora_vieja.y + grosor_borde, area_hora_vieja.width - 2 * grosor_borde, area_hora_vieja.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_hora, { area_hora_vieja.x + 10, area_hora_vieja.y + (altura_campo - altura_texto_simple) / 2 }, tamano_fuente_datos, espaciado, BLACK);

		y_actual = area_hora_vieja.y + altura_campo + altura_linea * 2.0f;

		const char* texto_btn = "BUSCAR RESERVA";
		Vector2 tamano_btn = MeasureTextEx(fuente1, texto_btn, tamano_seccion_titulo, espaciado);
		float ancho_btn = tamano_btn.x + 40;
		float altura_btn = tamano_btn.y + 10;
		Rectangle area_btn = { x_campo, y_actual, ancho_btn, altura_btn };

		DrawRectangleRounded(area_btn, 0.5f, 10, azul_turquesa);
		DrawTextEx(fuente1, texto_btn, { area_btn.x + 20, area_btn.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (strlen(entrada_matricula) > 0 && strlen(entrada_hora) > 0)
			{
				reserva_encontrada = true;
				if (reserva_encontrada) {
					etapa_edicion = 2;
					mostrar_error = false;
					campo_activo = 3;
				}
				else {
					mostrar_error = true;
				}
			}
			else {
				mostrar_error = true;
			}
		}
	}
	// ETAPA 2: MODIFICACIÓN DE LA RESERVA ENCONTRADA (Nueva Hora)
	else if (etapa_edicion == 2)
	{
		DrawTextEx(fuente2, "NUEVA HORA INICIO (HH:MM):", { x_etiqueta, area_nueva_hora.y }, tamano_fuente_datos, espaciado, arena);
		Color color_borde = (campo_activo == 3) ? GREEN : LIGHTGRAY;
		DrawRectangleRec(area_nueva_hora, color_borde);
		DrawRectangle(area_nueva_hora.x + grosor_borde, area_nueva_hora.y + grosor_borde, area_nueva_hora.width - 2 * grosor_borde, area_nueva_hora.height - 2 * grosor_borde, color_fondo_campo);
		DrawTextEx(fuente1, entrada_nueva_hora, { area_nueva_hora.x + 10, area_nueva_hora.y + (altura_campo - altura_texto_simple) / 2 }, tamano_fuente_datos, espaciado, BLACK);

		y_actual = area_nueva_hora.y + altura_campo + altura_linea * 2.0f;

		const char* texto_btn = "CONFIRMAR EDICION";
		Vector2 tamano_btn = MeasureTextEx(fuente1, texto_btn, tamano_seccion_titulo, espaciado);
		float ancho_btn = tamano_btn.x + 40;
		float altura_btn = tamano_btn.y + 10;
		Rectangle area_btn = { x_campo, y_actual, ancho_btn, altura_btn };

		DrawRectangleRounded(area_btn, 0.5f, 10, verde_pasto);
		DrawTextEx(fuente1, texto_btn, { area_btn.x + 20, area_btn.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

		if (CheckCollisionPointRec(posicion_mouse, area_btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (strlen(entrada_nueva_hora) > 0)
			{
				gestor.editarReserva(entrada_matricula, entrada_hora, entrada_nueva_hora);

				entrada_nueva_hora[0] = '\0';
				entrada_matricula[0] = '\0';
				entrada_hora[0] = '\0';
				etapa_edicion = 1;
				opcion_menu = 0;
			}
			else {
				mostrar_error = true;
			}
		}
	}

	if (mostrar_error)
	{
		const char* mensaje_error;
		if (etapa_edicion == 1) {
			mensaje_error = "ERROR: Matricula, hora o reserva no validas.";
		}
		else {
			mensaje_error = "ERROR: Ingrese una nueva hora valida.";
		}
		const char* texto_boton = "ACTUALIZAR RESERVA";
		Vector2 tamano_btn = MeasureTextEx(fuente1, texto_boton, tamano_seccion_titulo, espaciado);
		float ancho_btn = tamano_btn.x + 40;
		float altura_btn = tamano_btn.y + 10;
		float ancho_error = MeasureTextEx(fuente2, mensaje_error, tamano_fuente_datos, espaciado).x;
		DrawTextCenterHorizontal(fuente2, mensaje_error, tamano_fuente_datos, espaciado, y_actual + altura_btn + 20, RED);
	}
}
//**********************************************************************************************************************************************
void EliminarReserva(Font fuente1, Font fuente2, Gestor& gestor)
{
	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_seccion_titulo = alto_ventana * 0.05f;
	float tamano_titulo_principal = alto_ventana * 0.06f;
	float espaciado = 0.0f;
	float altura_linea = tamano_fuente_datos * 1.5f;

	float pos_y_titulo_principal_desc = alto_ventana * 0.3f;
	float inicio_y_campos = pos_y_titulo_principal_desc + (alto_ventana * 0.15f);

	float ancho_campo = ancho_ventana * 0.4f;
	float altura_campo = alto_ventana * 0.07f;

	Vector2 posicion_mouse = GetMousePosition();

	const char* texto_titulo = "ELIMINAR RESERVA";
	float ancho_titulo = MeasureTextEx(fuente1, texto_titulo, tamano_titulo_principal, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, alto_ventana * 0.25f };
	DrawTextEx(fuente1, texto_titulo, pos_titulo, tamano_titulo_principal, espaciado, verde_manzana);

	float x_etiqueta = ancho_ventana * 0.3f;
	float x_campo = ancho_ventana * 0.45f;
	float y_actual = inicio_y_campos;

	DrawTextEx(fuente2, "Ingrese la informacion de la reserva a eliminar:", { x_etiqueta, pos_y_titulo_principal_desc }, tamano_seccion_titulo * 0.8f, espaciado, WHITE);

	Rectangle area_matricula = { x_campo, y_actual, ancho_campo, altura_campo };
	Rectangle area_hora = { x_campo, y_actual + altura_campo + altura_linea, ancho_campo, altura_campo };

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (CheckCollisionPointRec(posicion_mouse, area_matricula))
			campo_activo = 1;
		else if (CheckCollisionPointRec(posicion_mouse, area_hora))
			campo_activo = 2;
		else
			campo_activo = 0;

		mostrar_error = false;
	}

	int tecla = GetCharPressed();
	while (tecla > 0)
	{
		if (tecla >= 32 && tecla <= 125)
		{
			if (campo_activo == 1)
			{
				int len = strlen(entrada_matricula);
				if (len < 15)
				{
					entrada_matricula[len] = (char)tecla;
					entrada_matricula[len + 1] = '\0';
				}
			}
			else if (campo_activo == 2)
			{
				int len = strlen(entrada_hora);
				if (len < 5)
				{
					entrada_hora[len] = (char)tecla;
					entrada_hora[len + 1] = '\0';
				}
			}
		}
		tecla = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		if (campo_activo == 1)
		{
			int len = strlen(entrada_matricula);
			if (len > 0)
				entrada_matricula[len - 1] = '\0';
		}
		else if (campo_activo == 2)
		{
			int len = strlen(entrada_hora);
			if (len > 0)
				entrada_hora[len - 1] = '\0';
		}
	}

	Color color_fondo_campo = WHITE;
	Color color_borde;
	int grosor_borde = 2;
	float altura_texto_simple = MeasureTextEx(fuente2, "A", tamano_fuente_datos, espaciado).y;

	DrawTextEx(fuente2, "MATRICULA:", { x_etiqueta, area_matricula.y }, tamano_fuente_datos, espaciado, arena);
	color_borde = (campo_activo == 1) ? GREEN : LIGHTGRAY;
	DrawRectangleRec(area_matricula, color_borde);
	DrawRectangle(area_matricula.x + grosor_borde, area_matricula.y + grosor_borde, area_matricula.width - 2 * grosor_borde, area_matricula.height - 2 * grosor_borde, color_fondo_campo);
	DrawTextEx(fuente1, entrada_matricula, { area_matricula.x + 10, area_matricula.y + (altura_campo - altura_texto_simple) / 2 }, tamano_fuente_datos, espaciado, BLACK);

	// CAMPO HORA DE INICIO
	DrawTextEx(fuente2, "HORA DE INICIO (HH:MM):", { x_etiqueta, area_hora.y }, tamano_fuente_datos, espaciado, arena);
	color_borde = (campo_activo == 2) ? GREEN : LIGHTGRAY;
	DrawRectangleRec(area_hora, color_borde);
	DrawRectangle(area_hora.x + grosor_borde, area_hora.y + grosor_borde, area_hora.width - 2 * grosor_borde, area_hora.height - 2 * grosor_borde, color_fondo_campo);
	DrawTextEx(fuente1, entrada_hora, { area_hora.x + 10, area_hora.y + (altura_campo - altura_texto_simple) / 2 }, tamano_fuente_datos, espaciado, BLACK);

	y_actual = area_hora.y + altura_campo + altura_linea * 2.0f;

	// --- 3. BOTÓN DE ELIMINAR ---
	const char* texto_boton = "ELIMINAR RESERVA";
	Vector2 tamano_btn = MeasureTextEx(fuente1, texto_boton, tamano_seccion_titulo, espaciado);
	float ancho_btn = tamano_btn.x + 40;
	float altura_btn = tamano_btn.y + 10;

	Rectangle area_btn = { x_campo, y_actual, ancho_btn, altura_btn };

	DrawRectangleRounded(area_btn, 0.5f, 10, RED);
	DrawTextEx(fuente1, texto_boton, { area_btn.x + 20, area_btn.y + 5 }, tamano_seccion_titulo, espaciado, WHITE);

	// --- 4. MANEJO DE LA LÓGICA DE ELIMINACIÓN (se mantiene) ---
	if (CheckCollisionPointRec(posicion_mouse, area_btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		std::string matricula_str = entrada_matricula;
		std::string hora_str = entrada_hora;

		if (matricula_str.empty() || hora_str.empty())
		{
			mostrar_error = true;
		}
		else
		{
			gestor.eliminarReservas(matricula_str, hora_str);

			entrada_matricula[0] = '\0';
			entrada_hora[0] = '\0';
			campo_activo = 0;
			mostrar_error = false;
		}
	}

	// --- 5. VISUALIZACIÓN DE ERRORES (se mantiene) ---
	if (mostrar_error)
	{
		const char* mensaje_error = "ERROR: Complete todos los campos.";
		float ancho_error = MeasureTextEx(fuente2, mensaje_error, tamano_fuente_datos, espaciado).x;
		// Centrar el mensaje de error
		Vector2 pos_error = { (ancho_ventana - ancho_error) / 2, area_btn.y + altura_btn + 20 };
		DrawTextEx(fuente2, mensaje_error, pos_error, tamano_fuente_datos, espaciado, RED);
	}
}
//**********************************************************************************************************************************************
void SalasComputo(Font fuente1, Font fuente2)
{
	Vector2 tamanio_texto;
	float tamanio_fuente = alto_ventana * 0.08f;
	const char* texto_salaab = "SALAS A Y B";
	tamanio_texto = MeasureTextEx(fuente1, texto_salaab, tamanio_fuente, 0);
	BotonTexto btn_salaab = { texto_salaab,{ ancho_ventana * 0.3f, alto_ventana * 0.4f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,verde_pasto };
	if (ManejarBotonTexto(btn_salaab))
	{
		opcion_menu = 41;
	}

	const char* texto_salac = "SALA C";
	tamanio_texto = MeasureTextEx(fuente1, texto_salac, tamanio_fuente, 0);
	BotonTexto btn_salac = { texto_salac,{ ancho_ventana * 0.65f, alto_ventana * 0.4f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,verde_pasto };
	if (ManejarBotonTexto(btn_salac))
		opcion_menu = 42;

	const char* texto_salas = "TODAS LAS SALAS";
	tamanio_texto = MeasureTextEx(fuente1, texto_salas, tamanio_fuente, 0);
	BotonTexto btn_salas = { texto_salas,{ ancho_ventana * 0.425f, alto_ventana * 0.6f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,verde_pasto };
	if (ManejarBotonTexto(btn_salas))
		opcion_menu = 43;
}
//**********************************************************************************************************************************************
void SalasComputoAYB(Font fuente1, Font fuente2, Gestor& gestor)
{
	const vector<Sala>& salas = gestor.obtenerSalas();

	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_seccion_titulo = alto_ventana * 0.05f;
	float altura_linea = tamano_fuente_datos * 1.5f;
	Color color_texto = verde_pasto;
	float espaciado = 0.0f;

	float pos_y_titulo_principal = alto_ventana * 0.25f;
	float inicio_y_contenido_sala = pos_y_titulo_principal + (alto_ventana * 0.08f);
	float tamano_titulo_principal = alto_ventana * 0.06f;

	const char* titulo_texto = "SALAS DE COMPUTO A Y B";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, tamano_titulo_principal, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, pos_y_titulo_principal };
	DrawTextEx(fuente1, titulo_texto, pos_titulo, tamano_titulo_principal, espaciado, verde_manzana);

	float centro_x = ancho_ventana / 2.0f;
	float margen_horizontal = ancho_ventana * 0.05f;

	DrawLineV({ centro_x, inicio_y_contenido_sala - tamano_seccion_titulo * 0.5f }, { centro_x, alto_ventana * 0.9f }, GRAY);

	const Sala& salaA = salas[0];
	float pos_x_salaA = margen_horizontal;
	float y_actual = inicio_y_contenido_sala;

	DrawTextEx(fuente2, "SALA A", { pos_x_salaA, y_actual }, tamano_seccion_titulo, espaciado, verde_pasto);
	y_actual += altura_linea * 1.5f;

	string estadoA = salaA.estaOcupada() ? "ESTADO: OCUPADA" : "ESTADO: DISPONIBLE";
	Color color_estadoA = salaA.estaOcupada() ? RED : verde_lima;
	DrawTextEx(fuente2, estadoA.c_str(), { pos_x_salaA, y_actual }, tamano_fuente_datos, espaciado, color_estadoA);
	y_actual += altura_linea;

	const Sala& salaB = salas[1];
	float pos_x_salaB = centro_x + margen_horizontal;
	y_actual = inicio_y_contenido_sala;

	DrawTextEx(fuente2, "SALA B", { pos_x_salaB, y_actual }, tamano_seccion_titulo, espaciado, verde_pasto);
	y_actual += altura_linea * 1.5f;

	string estadoB = salaB.estaOcupada() ? "ESTADO: OCUPADA" : "ESTADO: DISPONIBLE";
	Color color_estadoB = salaB.estaOcupada() ? RED : verde_lima;
	DrawTextEx(fuente2, estadoB.c_str(), { pos_x_salaB, y_actual }, tamano_fuente_datos, espaciado, color_estadoB);
	y_actual += altura_linea;
}
//**********************************************************************************************************************************************
void SalaComputoC(Font fuente1, Font fuente2, Gestor& gestor)
{
	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_seccion_titulo = alto_ventana * 0.05f;
	float altura_linea = tamano_fuente_datos * 1.5f;
	Color color_texto = verde_pasto;
	float espaciado = 0.0f;

	float pos_y_titulo_principal = alto_ventana * 0.25f;
	float inicio_y_contenido_sala = pos_y_titulo_principal + (alto_ventana * 0.08f);
	float tamano_titulo_principal = alto_ventana * 0.06f;

	const char* titulo_texto = "SALA DE COMPUTO C";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, tamano_titulo_principal, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, pos_y_titulo_principal };
	DrawTextEx(fuente1, titulo_texto, pos_titulo, tamano_titulo_principal, espaciado, verde_manzana);

	float x_centro = ancho_ventana / 2.0f;
	float y_actual = inicio_y_contenido_sala + tamano_seccion_titulo;

	DrawTextCenterHorizontal(fuente2, "Esta sala contiene las 20 computadoras reservables por PC.", tamano_fuente_datos, espaciado, y_actual, WHITE);
	y_actual += altura_linea * 1.5f;

	DrawTextCenterHorizontal(fuente2, "Para ver la disponibilidad detallada de las PC, navega a 'COMPUTADORAS'.", tamano_fuente_datos, espaciado, y_actual, GRAY);
	y_actual += altura_linea * 1.5f;
}
//**********************************************************************************************************************************************
void TodasSalasComputo(Font fuente1, Font fuente2, Gestor& gestor)
{
	const vector<Sala>& salas = gestor.obtenerSalas();
	const vector<Computadora>& computadoras = gestor.obtenerComputadoras();

	float tamano_estado = alto_ventana * 0.05f;
	float tamano_titulo_principal = alto_ventana * 0.06f;
	float tamano_titulo_bloque = alto_ventana * 0.05f;
	float espaciado = 0.0f;

	float pos_y_titulo_principal_ajustado = alto_ventana * 0.30f;
	float inicio_y_bloques = pos_y_titulo_principal_ajustado + (alto_ventana * 0.08f);
	float altura_bloque = alto_ventana * 0.15f;
	float margen_bloque_y = alto_ventana * 0.03f;
	float ancho_bloque = ancho_ventana * 0.6f;

	const char* titulo_texto = "DISPONIBILIDAD GENERAL DE SALAS";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, tamano_titulo_principal, espaciado).x;
	DrawTextCenterHorizontal(fuente1, titulo_texto, tamano_titulo_principal, espaciado, alto_ventana * 0.25f, verde_manzana);
	DrawTextCenterHorizontal(fuente2, "ESTADO DE OCUPACION ACTUAL:", tamano_estado * 0.8f, espaciado, pos_y_titulo_principal_ajustado, WHITE);

	float x_bloque = (ancho_ventana - ancho_bloque) / 2;
	float y_actual = inicio_y_bloques;

	bool estaLibreSalaC = false;
	for (const auto& pc : computadoras) {
		if (!pc.estaOcupada() && !pc.estaEnServicio()) {
			estaLibreSalaC = true;
			break;
		}
	}

	struct InfoSala { const char* titulo; string estado; Color color; };
	vector<InfoSala> info_salas;

	if (salas.size() >= 1) {
		const Sala& salaA = salas[0];
		info_salas.push_back({ "SALA A", salaA.estaOcupada() ? "OCUPADA" : "LIBRE", salaA.estaOcupada() ? RED : verde_lima });
	}
	if (salas.size() >= 2) {
		const Sala& salaB = salas[1];
		info_salas.push_back({ "SALA B", salaB.estaOcupada() ? "OCUPADA" : "LIBRE", salaB.estaOcupada() ? RED : verde_lima });
	}

	info_salas.push_back({ "SALA C (PCS)", estaLibreSalaC ? "LIBRE (PCs Disponibles)" : "OCUPADA", estaLibreSalaC ? verde_lima : RED });

	for (const auto& info : info_salas)
	{
		Rectangle area_bloque = { x_bloque, y_actual, ancho_bloque, altura_bloque };

		DrawRectangleRounded(area_bloque, 0.5f, 10, info.color);

		string texto_combinado = string(info.titulo) + ": [" + info.estado + "]";

		Vector2 tamano_medido = MeasureTextEx(fuente1, texto_combinado.c_str(), tamano_titulo_bloque, espaciado);

		float x_centrado = x_bloque + (ancho_bloque - tamano_medido.x) / 2;
		float y_centrado = y_actual + (altura_bloque - tamano_medido.y) / 2;

		DrawTextEx(fuente1, texto_combinado.c_str(), { x_centrado, y_centrado }, tamano_titulo_bloque, espaciado, WHITE);

		y_actual += altura_bloque + margen_bloque_y;
	}
}
//**********************************************************************************************************************************************
void ListaComputadoras(Font fuente1, Font fuente2, Gestor& gestor)
{
	const vector<Computadora>& computadoras = gestor.obtenerComputadoras();

	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_fuente_titulo_columna = alto_ventana * 0.05f;
	float altura_linea = tamano_fuente_datos * 1.5f;
	Color color_texto = verde_pasto;
	float espaciado = 0.0f;

	float pos_y_titulo_principal = alto_ventana * 0.25f;
	float inicio_y_tabla = pos_y_titulo_principal + (alto_ventana * 0.08f);

	const char* titulo_texto = "LISTA DE COMPUTADORAS (SALA C)";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, alto_ventana * 0.06f, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, pos_y_titulo_principal };
	DrawTextEx(fuente1, titulo_texto, pos_titulo, alto_ventana * 0.06f, espaciado, color_texto);

	float pos_columna_x[] = { ancho_ventana * 0.05f,ancho_ventana * 0.18f, ancho_ventana * 0.52f, ancho_ventana * 0.65f };

	DrawTextEx(fuente2, "NO. PC", { pos_columna_x[0], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "ESTADO / USUARIO", { pos_columna_x[1], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "NO. PC", { pos_columna_x[2], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "ESTADO / USUARIO", { pos_columna_x[3], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);

	DrawLine(pos_columna_x[0], inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, ancho_ventana * 0.95f, inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, WHITE);

	float y_actual = inicio_y_tabla + tamano_fuente_titulo_columna * 1.5f;

	int total_computadoras = computadoras.size();
	int mitad = (total_computadoras + 1) / 2;
	float y_inicial_datos = y_actual;

	for (int i = 0; i < total_computadoras; ++i)
	{
		const auto& pc = computadoras[i];
		string estado_cadena;
		string pc_num_cadena = to_string(pc.getNumeroPC());

		if (pc.estaEnServicio()) {
			estado_cadena = "[FUERA DE SERVICIO]";
		}
		else if (pc.estaOcupada()) {
			estado_cadena = "[OCUPADA] por " + pc.getMatriculaUsuario();
		}
		else {
			estado_cadena = "[DISPONIBLE]";
		}

		float pos_x_pc;
		float pos_x_estado;
		float pos_y_actual;

		if (i < mitad)
		{
			pos_x_pc = pos_columna_x[0];
			pos_x_estado = pos_columna_x[1];
			pos_y_actual = y_inicial_datos + i * altura_linea;
		}
		else
		{
			pos_x_pc = pos_columna_x[2];
			pos_x_estado = pos_columna_x[3];
			pos_y_actual = y_inicial_datos + (i - mitad) * altura_linea;
		}

		DrawTextEx(fuente2, pc_num_cadena.c_str(), { pos_x_pc, pos_y_actual }, tamano_fuente_datos, espaciado, WHITE);

		DrawTextEx(fuente2, estado_cadena.c_str(), { pos_x_estado, pos_y_actual }, tamano_fuente_datos, espaciado, color_texto);
	}
}
//**********************************************************************************************************************************************
void Usuarios(Font fuente1, Font fuente2)
{
	Vector2 tamanio_texto;
	float tamanio_fuente = alto_ventana * 0.08f;
	const char* texto_profesores = "PROFESORES";
	tamanio_texto = MeasureTextEx(fuente1, texto_profesores, tamanio_fuente, 0);
	BotonTexto btn_profesores = { texto_profesores,{ ancho_ventana * 0.3f, alto_ventana * 0.4f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,verde_pasto };
	if (ManejarBotonTexto(btn_profesores))
	{
		opcion_menu = 61;
	}

	const char* texto_alumnos = "ALUMNOS";
	tamanio_texto = MeasureTextEx(fuente1, texto_alumnos, tamanio_fuente, 0);
	BotonTexto btn_alumnos = { texto_alumnos,{ ancho_ventana * 0.65f, alto_ventana * 0.4f, tamanio_texto.x, tamanio_texto.y },fuente1,tamanio_fuente,verde_pasto };
	if (ManejarBotonTexto(btn_alumnos))
		opcion_menu = 62;
}
//**********************************************************************************************************************************************
void ListaProfesores(Font fuente1, Font fuente2, Gestor& gestor)
{
	const vector<Profesor>& profesores = gestor.obtenerProfesores();

	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_fuente_titulo_columna = alto_ventana * 0.05f;
	float altura_linea = tamano_fuente_datos * 1.5f;
	Color color_texto = verde_pasto;
	float espaciado = 0.0f;

	float pos_y_titulo_principal = alto_ventana * 0.25f;

	float inicio_y_tabla = pos_y_titulo_principal + (alto_ventana * 0.08f);

	const char* titulo_texto = "LISTA DE PROFESORES";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, alto_ventana * 0.06f, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, pos_y_titulo_principal };
	DrawTextEx(fuente1, titulo_texto, pos_titulo, alto_ventana * 0.06f, espaciado, color_texto);

	float pos_columna_x[] = { ancho_ventana * 0.05f,ancho_ventana * 0.18f,ancho_ventana * 0.35f,ancho_ventana * 0.55f,ancho_ventana * 0.70f,ancho_ventana * 0.88f };

	DrawTextEx(fuente2, "MATRICULA", { pos_columna_x[0], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "NOMBRE", { pos_columna_x[1], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "APELLIDOS", { pos_columna_x[2], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "FACULTAD", { pos_columna_x[3], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "RESERVA", { pos_columna_x[4], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "SALA", { pos_columna_x[5], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);

	DrawLine(pos_columna_x[0], inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, ancho_ventana * 0.95f, inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, WHITE);

	float y_actual = inicio_y_tabla + tamano_fuente_titulo_columna * 1.5f;

	if (profesores.empty())
	{
		DrawTextEx(fuente2, "No hay profesores registrados.", { pos_columna_x[0], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		return;
	}

	for (const auto& profesor : profesores)
	{
		DrawTextEx(fuente2, profesor.getMatricula().c_str(), { pos_columna_x[0], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		DrawTextEx(fuente2, profesor.getNombre().c_str(), { pos_columna_x[1], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		DrawTextEx(fuente2, profesor.getApellidos().c_str(), { pos_columna_x[2], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		DrawTextEx(fuente2, profesor.getFacultad().c_str(), { pos_columna_x[3], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		string reserva_cadena = profesor.tieneReservaActual() ? "Si" : "No";
		DrawTextEx(fuente2, reserva_cadena.c_str(), { pos_columna_x[4], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		if (profesor.tieneReservaActual())
		{
			string sala_cadena;
			int sala = profesor.getSalaAsignada();

			if (sala == 2)
			{
				sala_cadena = "C (PC " + to_string(profesor.getNumeroPC() + 1) + ")";
			}
			else
			{
				char sala_caracter = (sala == 0 ? 'A' : sala == 1 ? 'B' : '?');
				sala_cadena = string(1, sala_caracter);
			}

			DrawTextEx(fuente2, sala_cadena.c_str(), { pos_columna_x[5], y_actual }, tamano_fuente_datos, 0, color_texto);
		}
		else
		{
			DrawTextEx(fuente2, "-", { pos_columna_x[5], y_actual }, tamano_fuente_datos, 0, color_texto);
		}

		y_actual += altura_linea;
	}
}
//**********************************************************************************************************************************************
void ListaAlumnos(Font fuente1, Font fuente2, Gestor& gestor)
{
	const std::vector<Alumno>& alumnos = gestor.obtenerAlumnos();

	float tamano_fuente_datos = alto_ventana * 0.035f;
	float tamano_fuente_titulo_columna = alto_ventana * 0.05f;
	float altura_linea = tamano_fuente_datos * 1.5f;
	Color color_texto = verde_pasto;
	float espaciado = 0.0f;

	float pos_y_titulo_principal = alto_ventana * 0.25f;

	float inicio_y_tabla = pos_y_titulo_principal + (alto_ventana * 0.08f);

	const char* titulo_texto = "LISTA DE ALUMNOS";
	float ancho_titulo = MeasureTextEx(fuente1, titulo_texto, alto_ventana * 0.06f, espaciado).x;
	Vector2 pos_titulo = { (ancho_ventana - ancho_titulo) / 2, pos_y_titulo_principal };
	DrawTextEx(fuente1, titulo_texto, pos_titulo, alto_ventana * 0.06f, espaciado, color_texto);

	float pos_columna_x[] = { ancho_ventana * 0.05f,ancho_ventana * 0.18f,ancho_ventana * 0.35f,ancho_ventana * 0.53f, ancho_ventana * 0.69f,ancho_ventana * 0.81f,ancho_ventana * 0.90f };

	DrawTextEx(fuente2, "MATRICULA", { pos_columna_x[0], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "NOMBRE", { pos_columna_x[1], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "APELLIDOS", { pos_columna_x[2], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "FACULTAD", { pos_columna_x[3], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "RESERVA", { pos_columna_x[4], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "SALA", { pos_columna_x[5], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);
	DrawTextEx(fuente2, "PC", { pos_columna_x[6], inicio_y_tabla }, tamano_fuente_titulo_columna, espaciado, verde_manzana);

	DrawLine(pos_columna_x[0], inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, ancho_ventana * 0.98f, inicio_y_tabla + tamano_fuente_titulo_columna * 1.2f, WHITE);

	float y_actual = inicio_y_tabla + tamano_fuente_titulo_columna * 1.5f;

	if (alumnos.empty())
	{
		DrawTextEx(fuente2, "No hay alumnos registrados.", { pos_columna_x[0], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		return;
	}

	for (const auto& alumno : alumnos)
	{
		DrawTextEx(fuente2, alumno.getMatricula().c_str(), { pos_columna_x[0], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		DrawTextEx(fuente2, alumno.getNombre().c_str(), { pos_columna_x[1], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		DrawTextEx(fuente2, alumno.getApellidos().c_str(), { pos_columna_x[2], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		DrawTextEx(fuente2, alumno.getFacultad().c_str(), { pos_columna_x[3], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		string reserva_cadena = alumno.tieneReservaActual() ? "Si" : "No";
		DrawTextEx(fuente2, reserva_cadena.c_str(), { pos_columna_x[4], y_actual }, tamano_fuente_datos, espaciado, color_texto);

		if (alumno.tieneReservaActual())
		{
			DrawTextEx(fuente2, "C", { pos_columna_x[5], y_actual }, tamano_fuente_datos, espaciado, color_texto);

			string pc_cadena = to_string(alumno.getNumeroPC() + 1);
			DrawTextEx(fuente2, pc_cadena.c_str(), { pos_columna_x[6], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		}
		else
		{
			DrawTextEx(fuente2, "-", { pos_columna_x[5], y_actual }, tamano_fuente_datos, espaciado, color_texto);
			DrawTextEx(fuente2, "-", { pos_columna_x[6], y_actual }, tamano_fuente_datos, espaciado, color_texto);
		}

		y_actual += altura_linea;
	}
}
//**********************************************************************************************************************************************
void LimpiarCamposEntrada()
{
	entrada_matricula[0] = '\0';
	entrada_hora_inicio[0] = '\0';
	entrada_hora_fin[0] = '\0';
	entrada_pc_num[0] = '\0';
	entrada_hora[0] = '\0';
	entrada_nueva_hora[0] = '\0';
	etapa_edicion = 1;
	campo_activo = 0;
	campo_activo_res = 0;
	mostrar_error = false;
	mostrar_error_res = false;
	reserva_encontrada = false;
	tipo_reserva_seleccionado = 0;
	sala_seleccionada = -1;
	tipo_usuario_detectado = "";
}

bool EsNumero(const char* texto)
{
	for (int i = 0; texto[i] != '\0'; i++) {
		if (!isdigit(texto[i])) {
			return false;
		}
	}
	return true;
}

bool ValidarFormatoHoraSimple(const char* hora)
{
	size_t len = strlen(hora);
	int horas, minutos;

	if (len != 4 && len != 5) return false;

	if (len == 5) {
		if (hora[2] != ':') return false;

		if (!isdigit(hora[0]) || !isdigit(hora[1]) ||
			!isdigit(hora[3]) || !isdigit(hora[4])) {
			return false;
		}

		horas = (hora[0] - '0') * 10 + (hora[1] - '0');
		minutos = (hora[3] - '0') * 10 + (hora[4] - '0');
	}
	else {
		if (hora[1] != ':') return false;

		if (!isdigit(hora[0]) ||
			!isdigit(hora[2]) || !isdigit(hora[3])) {
			return false;
		}

		horas = hora[0] - '0';
		minutos = (hora[2] - '0') * 10 + (hora[3] - '0');
	}

	if (horas < 0 || horas > 23) return false;
	if (minutos < 0 || minutos > 59) return false;

	return true;
}

bool ValidarRangoPC(const char* pc_num)
{
	if (!EsNumero(pc_num)) return false;

	int num = atoi(pc_num);
	return (num >= 1 && num <= 20);
}

void ManejarEntradaMatricula() {
	int tecla = GetCharPressed();
	while (tecla > 0) {
		if (strlen(entrada_matricula) < 15 && (isdigit(tecla) || tecla == ' ' || isalpha(tecla))) {
			entrada_matricula[strlen(entrada_matricula)] = (char)tecla;
			entrada_matricula[strlen(entrada_matricula) + 1] = '\0';
		}
		tecla = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE) && strlen(entrada_matricula) > 0) {
		entrada_matricula[strlen(entrada_matricula) - 1] = '\0';
	}
}

void ManejarEntradaHora(char* entrada_hora) {
	int tecla = GetCharPressed();
	while (tecla > 0) {
		int len = strlen(entrada_hora);

		if (len < 5) {
			// Insertar ':' automáticamente en la posición 2
			if (len == 2 && entrada_hora[len] != ':') {
				entrada_hora[len] = ':';
				entrada_hora[len + 1] = '\0';
				len++;
			}

			// Solo aceptar dígitos
			if (isdigit(tecla)) {
				entrada_hora[len] = (char)tecla;
				entrada_hora[len + 1] = '\0';
			}
		}

		tecla = GetCharPressed();
	}

	// Manejo de Backspace
	if (IsKeyPressed(KEY_BACKSPACE) && strlen(entrada_hora) > 0) {
		int len = strlen(entrada_hora);
		entrada_hora[len - 1] = '\0';

		// Si borramos el tercer carácter y era ':', lo eliminamos también
		if (len == 3 && entrada_hora[2] == ':') {
			entrada_hora[2] = '\0';
		}
	}
}

void ManejarEntradaNumerica(char* entrada, int max_len) {
	int tecla = GetCharPressed();
	while (tecla > 0) {
		if (strlen(entrada) < max_len && isdigit(tecla)) {
			entrada[strlen(entrada)] = (char)tecla;
			entrada[strlen(entrada) + 1] = '\0';
		}
		tecla = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE) && strlen(entrada) > 0) {
		entrada[strlen(entrada) - 1] = '\0';
	}
}