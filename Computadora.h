#pragma once
#include <string>

using std::string;

class Computadora {
private:
    int numeroPC;
    bool ocupada;
    bool enServicio;
    string matriculaUsuario; // Matrícula del usuario que la tiene reservada

public:
    Computadora(int num);

    // Getters
    int getNumeroPC() const;
    bool estaOcupada() const;
    bool estaEnServicio() const;
    string getMatriculaUsuario() const;

    // Setters
    void setOcupada(bool estado, const string& matricula = "");
    void setEnServicio(bool estado);

    void mostrarEstado() const;
};
