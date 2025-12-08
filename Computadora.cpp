#include "Computadora.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

Computadora::Computadora(int num)
    : numeroPC(num), ocupada(false), enServicio(false), matriculaUsuario("") {
}

int Computadora::getNumeroPC() const { return numeroPC; }
bool Computadora::estaOcupada() const { return ocupada; }
bool Computadora::estaEnServicio() const { return enServicio; }
string Computadora::getMatriculaUsuario() const { return matriculaUsuario; }

void Computadora::setOcupada(bool estado, const string& matricula) {
    ocupada = estado;
    if (estado) {
        matriculaUsuario = matricula;
    }
    else {
        matriculaUsuario = "";
    }
}

void Computadora::setEnServicio(bool estado) {
    enServicio = estado;
    if (estado) {
        ocupada = true; // Si está en servicio, no se puede usar
    }
}

void Computadora::mostrarEstado() const {
    cout << "PC " << numeroPC << ": ";
    if (enServicio) {
        cout << "[EN SERVICIO]";
    }
    else if (ocupada) {
        cout << "[OCUPADA] por " << matriculaUsuario;
    }
    else {
        cout << "[DISPONIBLE]";
    }
    cout << endl;
}