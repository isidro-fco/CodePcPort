#include "Sala.h"
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;
using std::endl;
using std::setw;

Sala::Sala(int salaId) : id(salaId), ocupada(false) {
	if (salaId == 2) { // Sala C
		inicializarComputadoras(20); // 20 computadoras por ejemplo
	}
}

int Sala::getId() const { return id; }
bool Sala::estaOcupada() const { return ocupada; }

char Sala::getLetraSala() const {
	switch (id) {
	case 0: return 'A';
	case 1: return 'B';
	case 2: return 'C';
	default: return '?';
	}
}

void Sala::inicializarComputadoras(int cantidad) {
	computadoras.resize(cantidad, false); // Todas libres inicialmente
}

void Sala::agregarReserva(const Reserva& reserva) {
	horario.push_back(reserva);
	ocupada = true;
}

void Sala::eliminarReserva(const string& matricula, const string& hora) {
	for (auto it = horario.begin(); it != horario.end(); ++it) {
		if (it->getMatricula() == matricula && it->getHoraInicio() == hora) {
			horario.erase(it);
			if (horario.empty()) {
				ocupada = false;
			}
			cout << "Reserva eliminada para matrícula " << matricula
				<< " a las " << hora << endl;
			return;
		}
	}
	cout << "Reserva no encontrada." << endl;
}

bool Sala::reservarComputadora(int pc, const Reserva& reserva) {
	if (pc < 0 || pc >= computadoras.size()) {
		cout << "Número de computadora inválido." << endl;
		return false;
	}

	if (computadoras[pc]) {
		cout << "Computadora " << pc << " ya está ocupada." << endl;
		return false;
	}

	computadoras[pc] = true;
	horario.push_back(reserva);
	ocupada = true;
	return true;
}

bool Sala::liberarComputadora(int pc) {
	if (pc < 0 || pc >= computadoras.size()) {
		return false;
	}

	computadoras[pc] = false;

	// Verificar si todas las computadoras están libres
	bool todasLibres = true;
	for (bool estado : computadoras) {
		if (estado) {
			todasLibres = false;
			break;
		}
	}

	if (todasLibres) {
		ocupada = false;
	}

	return true;
}

bool Sala::computadoraOcupada(int pc) const {
	if (pc < 0 || pc >= computadoras.size()) {
		return false;
	}
	return computadoras[pc];
}

void Sala::verVistaComputadoras() const {
	if (id != 2) {
		cout << "Esta función solo está disponible para la Sala C." << endl;
		return;
	}

	cout << "\n=== VISTA DETALLADA SALA C (Computadoras) ===" << endl;
	for (int i = 0; i < computadoras.size(); i++) {
		cout << "Computadora " << setw(2) << i + 1 << ": "
			<< (computadoras[i] ? "[OCUPADA] " : "[LIBRE]   ") << endl;
	}
}

void Sala::verVistaGeneral() const {
	cout << "\n=== VISTA GENERAL SALA " << getLetraSala() << " ===" << endl;
	cout << "Estado: " << (ocupada ? "OCUPADA  " : "LIBRE    ") << endl;

	if (!horario.empty()) {
		cout << "Reservas activas:" << endl;
		for (const auto& reserva : horario) {
			cout << "  - Matrícula: " << reserva.getMatricula()
				<< " | Hora: " << reserva.getHoraInicio()
				<< " a " << reserva.getHoraFin() << endl;
		}
	}
}

void Sala::verHorario() const {
	cout << "\n=== HORARIO SALA " << getLetraSala() << " ===" << endl;
	if (horario.empty()) {
		cout << "No hay reservas." << endl;
	}
	else {
		for (const auto& reserva : horario) {
			cout << "• " << reserva.getMatricula() << " : "
				<< reserva.getHoraInicio() << " - "
				<< reserva.getHoraFin() << endl;
		}
	}
}

void Sala::verVistaActual() const {
	cout << "Sala " << getLetraSala() << ": ";
	if (ocupada) {
		cout << "[OCUPADA]";
	}
	else {
		cout << "[LIBRE]";
	}
	cout << endl;
}

bool Sala::intentarEditarReserva(const string& matricula, const string& hora_antigua, const string& hora_nueva)
{
	for (auto& reserva : horario) {
		if (reserva.getMatricula() == matricula && reserva.getHoraInicio() == hora_antigua) {

			reserva.setHoraInicio(hora_nueva);

			cout << "Reserva editada de " << hora_antigua << " a " << hora_nueva
				<< " en Sala " << getLetraSala() << " para " << matricula << endl;
			return true;
		}
	}
	return false;
}
