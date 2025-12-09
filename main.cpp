#include "Gestor.h"
#include "Visual.h"
#include "raylib.h"
#include <iostream>

int main(void)
{
	// CONFIGURACION DE VENTANA
	// Configura la redimencion
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	// Inicia la ventana
	InitWindow(800, 450, "CodePcPort");
	// Definimos el monitor en el cual va a aparecer la ventana.
	int monitor_deseado = 2;
	SetWindowMonitor(monitor_deseado);
	ancho_ventana = GetMonitorWidth(monitor_deseado);
	alto_ventana = GetMonitorHeight(monitor_deseado);
	// Centrar la ventana
	SetWindowPosition((ancho_ventana - 800) / 2, (alto_ventana - 450) / 2);
	// Maximiza la pantalla
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

	InicializarFuentes();
	InicializarImagenes();

	Gestor gestor_principal;

	SetTargetFPS(60);

	static int opcion_menu_anterior = 0;

	while (!WindowShouldClose())
	{
		ancho_ventana = GetScreenWidth();
		alto_ventana = GetScreenHeight();
		pos_mouse = GetMousePosition();

		ConfigurarBotonImagen();

		BeginDrawing();
		ClearBackground(fondo);
		/*
		if (pantalla_devs == true)
		{
			PantallaDesarrolladores(logo_devs, fuente1, fuente2);
			pantalla_devs = false;
		}
		*/


		if (desplegar_menu == true)
		{
			opcion_menu = MenuLateral(logo, fuente1, fuente2);
			if (opcion_menu != 0)
			{
				desplegar_menu = false;
			}
			zona_no_menu = { ancho_ventana / 3.5f,0, ancho_ventana - (ancho_ventana / 3.5f), (float)alto_ventana };
			if (CheckCollisionPointRec(pos_mouse, zona_no_menu))
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					desplegar_menu = false;
				}
			}
			if (ManejarBotonImagen(boton_salida))
			{
				DescargarFuentes();
				DescargarImagenes();
				CloseWindow();
				return 0;
			}
		}
		else
		{
			if (opcion_menu != opcion_menu_anterior)
			{
				LimpiarCamposEntrada();
			}
			opcion_menu_anterior = opcion_menu;
			const float porcentaje_ancho_deseado = 0.30f;
			float ancho_deseado = ancho_ventana * porcentaje_ancho_deseado;
			float escala = ancho_deseado / (float)logo.width;
			DrawTextureEx(logo, { ancho_ventana * 0.35f, alto_ventana * 0.1f }, 0.0f, escala, WHITE);

			switch (opcion_menu)
			{
			case 1:
				CrearReserva(fuente1, fuente2, gestor_principal);
				break;
			case 2:
				EditarReserva(fuente1, fuente2, gestor_principal);
				break;
			case 3:
				EliminarReserva(fuente1, fuente2, gestor_principal);
				break;
			case 4:
				SalasComputo(fuente1, fuente2);
				break;
			case 5:
				ListaComputadoras(fuente1, fuente2, gestor_principal);
				break;
			case 6:
				Usuarios(fuente1, fuente2);
				break;
			case 41:
				SalasComputoAYB(fuente1, fuente2, gestor_principal);
				break;
			case 42:
				SalaComputoC(fuente1, fuente2, gestor_principal);
				break;
			case 43:
				TodasSalasComputo(fuente1, fuente2, gestor_principal);
				break;
			case 61:
				ListaProfesores(fuente1, fuente2, gestor_principal);
				break;
			case 62:
				ListaAlumnos(fuente1, fuente2, gestor_principal);
			}

			if (ManejarBotonImagen(boton_menu))
			{
				desplegar_menu = true;
			}
			if (opcion_menu == 0)
			{
			DrawTextCenterHorizontal(fuente2, "Bienvenido!!!", alto_ventana * 0.3, 0.0f, alto_ventana*0.4, verde_mar);

			}
		}

		Reloj(fuente1);
		EndDrawing();
	}
}
