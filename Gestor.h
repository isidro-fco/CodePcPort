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
    bool reservarSala(const std::string& matricula, const std::string& hora_inicio, const std::string& hora_fin, int sala_id);
    bool reservarPC(const std::string& matricula, const std::string& hora_inicio, const std::string& hora_fin, int pc_num);
    bool editarReserva(const std::string& matricula, const std::string& hora_antigua, const std::string& hora_nueva);
    void agregarReserva(const Reserva& reserva);
    void eliminarReservas(const std::string& matricula, const std::string& hora);
    const vector<Sala>& obtenerSalas() const;
    const vector<Computadora>& obtenerComputadoras() const;
    const vector<Profesor>& obtenerProfesores() const;
    const vector<Alumno>& obtenerAlumnos() const;
};

