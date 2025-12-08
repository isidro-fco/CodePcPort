#pragma once
#include <string>

using std::string;

class Reserva {
private:
    string matricula;
    string horaInicio;
    string horaFin;
    int salaId; // 0: Sala A, 1: Sala B, 2: Sala C
    int numeroPC; // Solo para Sala C, -1 para otras
    string tipoUsuario; // "profesor" o "alumno"

public:
    Reserva(const string& mat, const string& inicio,
        const string& fin, int sala, int pc = -1,
        const string& tipo = "");

    // Getters
    string getMatricula() const;
    string getHoraInicio() const;
    string getHoraFin() const;
    int getSalaId() const;
    int getNumeroPC() const;
    string getTipoUsuario() const;

    // Setters
    void setHoraInicio(const string& inicio);
    void setHoraFin(const string& fin);
    void setSalaId(int sala);
    void setNumeroPC(int pc);
};
