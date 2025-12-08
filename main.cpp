#include "Gestor.h"
#include "Visual.h"
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

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		ancho_ventana = GetScreenWidth();
		alto_ventana = GetScreenHeight();
		BeginDrawing();
		ClearBackground(fondo);
		/*
		if (pantalla_devs == true)
		{
			PantallaDesarrolladores(logo_devs, fuente1, fuente2);
			pantalla_devs = false;
		}
		*/

		Reloj(fuente1);

		MenuLateral(logo, fuente1, fuente2);

		DrawText(TextFormat("Monitor actual: %d", GetCurrentMonitor()), 190, 300, 20, DARKGRAY);
		EndDrawing();
	}

	DescargarFuentes();
	DescargarImagenes();
	CloseWindow();

    Gestor gestor;
    gestor.mostrarMenu();

    return 0;
}