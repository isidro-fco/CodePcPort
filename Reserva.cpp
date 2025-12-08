#include "Reserva.h"

Reserva::Reserva(const string& mat, const string& inicio,
    const string& fin, int sala, int pc,
    const string& tipo)
    : matricula(mat), horaInicio(inicio), horaFin(fin),
    salaId(sala), numeroPC(pc), tipoUsuario(tipo) {
}

string Reserva::getMatricula() const { return matricula; }
string Reserva::getHoraInicio() const { return horaInicio; }
string Reserva::getHoraFin() const { return horaFin; }
int Reserva::getSalaId() const { return salaId; }
int Reserva::getNumeroPC() const { return numeroPC; }
string Reserva::getTipoUsuario() const { return tipoUsuario; }

void Reserva::setHoraInicio(const string& inicio) { horaInicio = inicio; }
void Reserva::setHoraFin(const string& fin) { horaFin = fin; }
void Reserva::setSalaId(int sala) { salaId = sala; }
void Reserva::setNumeroPC(int pc) { numeroPC = pc; }