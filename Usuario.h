#pragma once
#include <string>

using std::string;

class Usuario {
protected:
    string matricula;
    string nombre;
    string apellidos;
    string facultad;
    bool reservaActual;
    int salaAsignada;

public:
    Usuario(const string& mat, const string& nom,
        const string& ape, const string& fac);

    virtual ~Usuario() {}

    // Getters
    string getMatricula() const;
    string getNombre() const;
    string getApellidos() const;
    string getFacultad() const;
    bool tieneReservaActual() const;
    int getSalaAsignada() const;

    // Setters
    void setReservaActual(bool estado);
    void setSalaAsignada(int sala);

    virtual string getTipo() const = 0; // Método virtual puro
};
