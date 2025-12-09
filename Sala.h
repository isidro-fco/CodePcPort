#pragma once

#include <string>
#include <vector>
#include "Reserva.h"

using std::vector;
using std::string;

class Sala {
private:
    int id; // 0: A, 1: B, 2: C
    bool ocupada;
    vector<Reserva> horario;
    vector<bool> computadoras; // Solo para Sala C (true = ocupada)

public:
    Sala(int salaId);

    // Getters
    int getId() const;
    bool estaOcupada() const;
    char getLetraSala() const;

    // Métodos para salas A y B
    void agregarReserva(const Reserva& reserva);
    void eliminarReserva(const string& matricula, const string& hora);

    // Métodos para Sala C
    void inicializarComputadoras(int cantidad);
    bool reservarComputadora(int pc, const Reserva& reserva);
    bool liberarComputadora(int pc);
    bool computadoraOcupada(int pc) const;
    void verVistaComputadoras() const;
    void verVistaGeneral() const;

    // Visualización
    void verHorario() const;
    void verVistaActual() const;


    const vector<Reserva>& getHorario() const { return horario; }
    bool intentarEditarReserva(const string& matricula, const string& hora_antigua, const string& hora_nueva);
};
