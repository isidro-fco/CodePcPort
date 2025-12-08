#pragma once
#include "Usuario.h"

using std::string;

class Alumno : public Usuario {
private:
    int numeroPC;

public:
    Alumno(const string& mat, const string& nom,
        const string& ape, const string& fac);

    void reservarComputadora(int pc, const string& horaInicio,
        const string& horaFin);

    string getTipo() const override;
    int getNumeroPC() const;
    void setNumeroPC(int pc);
};
