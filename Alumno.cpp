#include "Alumno.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

Alumno::Alumno(const string& mat, const string& nom,
    const string& ape, const string& fac)
    : Usuario(mat, nom, ape, fac), numeroPC(-1) {
}

void Alumno::reservarComputadora(int pc, const string& horaInicio,
    const string& horaFin) {
    setReservaActual(true);
    setSalaAsignada(2); // Sala C
    numeroPC = pc;
    cout << "Alumno " << getNombre() << " ha reservado Computadora "
        << pc << " en Sala C de " << horaInicio << " a " << horaFin << endl;
}

string Alumno::getTipo() const {
    return "Alumno";
}

int Alumno::getNumeroPC() const {
    return numeroPC;
}

void Alumno::setNumeroPC(int pc) {
    numeroPC = pc;
}