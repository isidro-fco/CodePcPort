#pragma once
#include <vector>
#include <string>
#include "Usuario.h"
#include "Profesor.h"
#include "Alumno.h"
#include "Sala.h"
#include "Computadora.h"
#include "Reserva.h"

using std::string;
using std::vector;

class Gestor {
private:
    vector<Profesor> profesores;
    vector<Alumno> alumnos;
    vector<Sala> salas;
    vector<Computadora> computadoras;
    vector<Reserva> reservas;

    void cargarDatosEjemplo();

public:
    Gestor();

    // Métodos principales
    void mostrarMenu();
    void verSalas();
    void verComputadoras();
    void reservar();
    void eliminarReserva();
    void editarReserva();
    void verUsuarios();

    // Métodos auxiliares
    void verListaProfesores() const;
    void verListaAlumnos() const;
    Usuario* buscarUsuario(const string& matricula);
    bool verificarDisponibilidad(int salaId, const string& hora, int pc = -1) const;
    void agregarReserva(const Reserva& reserva);
};

