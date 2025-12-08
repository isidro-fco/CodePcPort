#include "Profesor.h"
#include <iostream>


using std::string;
using std::cout;
using std::endl;


Profesor::Profesor(const string& mat, const string& nom,
    const string& ape, const string& fac)
    : Usuario(mat, nom, ape, fac), numeroPC(-1) {
}

void Profesor::reservarSala(int salaId, const string& horaInicio,
    const string& horaFin) {
    setReservaActual(true);
    setSalaAsignada(salaId);
    cout << "Profesor " << getNombre() << " ha reservado Sala "
        << (salaId == 0 ? 'A' : salaId == 1 ? 'B' : 'C')
        << " de " << horaInicio << " a " << horaFin << endl;
}

void Profesor::reservarComputadora(int pc, const string& horaInicio,
    const string& horaFin) {
    setReservaActual(true);
    setSalaAsignada(2); // Sala C
    numeroPC = pc;
    cout << "Profesor " << getNombre() << " ha reservado Computadora "
        << pc << " en Sala C de " << horaInicio << " a " << horaFin << endl;
}

string Profesor::getTipo() const {
    return "Profesor";
}

int Profesor::getNumeroPC() const {
    return numeroPC;
}

void Profesor::setNumeroPC(int pc) {
    numeroPC = pc;
}