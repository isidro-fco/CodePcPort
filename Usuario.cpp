#include "Usuario.h"

Usuario::Usuario(const string& mat, const string& nom,
    const string& ape, const string& fac)
    : matricula(mat), nombre(nom), apellidos(ape), facultad(fac),
    reservaActual(false), salaAsignada(-1) {
}

string Usuario::getMatricula() const { return matricula; }
string Usuario::getNombre() const { return nombre; }
string Usuario::getApellidos() const { return apellidos; }
string Usuario::getFacultad() const { return facultad; }
bool Usuario::tieneReservaActual() const { return reservaActual; }
int Usuario::getSalaAsignada() const { return salaAsignada; }

void Usuario::setReservaActual(bool estado) { reservaActual = estado; }
void Usuario::setSalaAsignada(int sala) { salaAsignada = sala; }