#pragma once
#include "Usuario.h"
#include "Reserva.h"
#include <vector>

using std::string;

class Profesor : public Usuario {
private:
    int numeroPC; // Para reserva en sala C

public:
    Profesor(const string& mat, const string& nom,
        const string& ape, const string& fac);

    void reservarSala(int salaId, const string& horaInicio,
        const string& horaFin);
    void reservarComputadora(int pc, const string& horaInicio,
        const string& horaFin);

    string getTipo() const override;
    int getNumeroPC() const;
    void setNumeroPC(int pc);
};
