#include "Gestor.h"
#include "Visual.h"
#include "raylib.h"
#include <iostream>
#include <limits>

using std::cout;
using std::cin;
using std::string;
using std::endl;

Gestor::Gestor() {
	// Inicializar salas
	salas.push_back(Sala(0)); // Sala A
	salas.push_back(Sala(1)); // Sala B
	salas.push_back(Sala(2)); // Sala C

	// Inicializar computadoras (para Sala C)
	for (int i = 1; i <= 20; i++) {
		computadoras.push_back(Computadora(i));
	}

	cargarDatosEjemplo();
}

void Gestor::cargarDatosEjemplo() {
	// Profesores de ejemplo
	profesores.push_back(Profesor("192131", "Juan", "Perez", "Ingenieria"));
	profesores.push_back(Profesor("192312", "Maria", "Gomez", "Ciencias"));
	profesores.push_back(Profesor("190120", "Carlos", "Lopez", "Arquitectura"));

	// Alumnos de ejemplo
	alumnos.push_back(Alumno("391831", "Ana", "Martinez", "Ingenieria"));
	alumnos.push_back(Alumno("318382", "Luis", "Rodriguez", "Ciencias"));
	alumnos.push_back(Alumno("391830", "Sofia", "Hernandez", "Arquitectura"));
}

void Gestor::mostrarMenu() {
	int opcion;

	do {
		cout << "\n=================================" << endl;
		cout << "     SISTEMA CodePCPort" << endl;
		cout << "=================================" << endl;
		cout << "1. Ver salas" << endl;
		cout << "2. Ver computadoras" << endl;
		cout << "3. Reservar" << endl;
		cout << "4. Eliminar reserva" << endl;
		cout << "5. Editar reserva" << endl;
		cout << "6. Ver usuarios" << endl;
		cout << "0. Salir" << endl;
		cout << "=================================" << endl;
		cout << "Seleccione una opcion: ";
		cin >> opcion;

		switch (opcion) {
		case 1: verSalas(); break;
		case 2: verComputadoras(); break;
		case 3: reservar(); break;
		case 4: eliminarReserva(); break;
		case 5: editarReserva(); break;
		case 6: verUsuarios(); break;
		case 0: cout << "Saliendo del sistema..." << endl; break;
		default: cout << "Opcion invalida." << endl;
		}
	} while (opcion != 0);
}

void Gestor::verSalas() {
	int opcion;
	cout << "\n=== VISUALIZAR SALAS ===" << endl;
	cout << "1. Ver salas A y B (vista general)" << endl;
	cout << "2. Ver sala C" << endl;
	cout << "3. Ver todas las salas" << endl;
	cout << "Seleccione: ";
	cin >> opcion;

	switch (opcion) {
	case 1:
		cout << "\n--- SALAS A Y B ---" << endl;
		salas[0].verVistaActual();
		salas[0].verHorario();
		cout << endl;
		salas[1].verVistaActual();
		salas[1].verHorario();
		break;
	case 2:
		cout << "\n--- SALA C ---" << endl;
		cout << "1. Vista por computadoras" << endl;
		cout << "2. Vista general" << endl;
		cout << "Seleccione: ";
		cin >> opcion;
		if (opcion == 1) {
			salas[2].verVistaComputadoras();
		}
		else {
			salas[2].verVistaGeneral();
		}
		salas[2].verHorario();
		break;
	case 3:
		for (auto& sala : salas) {
			sala.verVistaActual();
		}
		break;
	default:
		cout << "Opcion invalida." << endl;
	}
}

void Gestor::verComputadoras() {
	cout << "\n=== LISTADO DE COMPUTADORAS ===" << endl;
	for (const auto& pc : computadoras) {
		cout << "PC " << pc.getNumeroPC() << ": ";
		if (pc.estaEnServicio()) {
			cout << "[EN SERVICIO]";
		}
		else if (pc.estaOcupada()) {
			cout << "[OCUPADA] por " << pc.getMatriculaUsuario();
		}
		else {
			cout << "[DISPONIBLE]";
		}
		cout << endl;
	}
}

void Gestor::reservar() {
	string matricula, tipo, horaInicio, horaFin;
	int opcion, salaId, pcNum;

	cout << "\n=== RESERVAR ===" << endl;
	cout << "Matricula: ";
	cin >> matricula;

	Usuario* usuario = buscarUsuario(matricula);
	if (!usuario) {
		cout << "Usuario no encontrado." << endl;
		return;
	}

	tipo = usuario->getTipo();

	if (tipo == "Profesor") {
		cout << "1. Reservar sala (A o B)" << endl;
		cout << "2. Reservar computadora (Sala C)" << endl;
		cout << "Seleccione: ";
		cin >> opcion;

		cout << "Hora inicio (HH:MM): ";
		cin >> horaInicio;
		cout << "Hora fin (HH:MM): ";
		cin >> horaFin;

		if (opcion == 1) {
			cout << "Seleccione sala (1=A, 2=B): ";
			cin >> salaId;
			salaId--; // Convertir a indice 0 o 1

			if (salaId >= 0 && salaId <= 1) {
				if (verificarDisponibilidad(salaId, horaInicio)) {
					Reserva reserva(matricula, horaInicio, horaFin, salaId, -1, tipo);
					salas[salaId].agregarReserva(reserva);
					usuario->setReservaActual(true);
					usuario->setSalaAsignada(salaId);
					cout << "Reserva exitosa!" << endl;
				}
				else {
					cout << "La sala no esta disponible en ese horario." << endl;
				}
			}
		}
		else if (opcion == 2) {
			cout << "Numero de computadora (1-20): ";
			cin >> pcNum;
			pcNum--; // Convertir a indice base 0

			if (pcNum >= 0 && pcNum < computadoras.size()) {
				if (!computadoras[pcNum].estaOcupada() && !computadoras[pcNum].estaEnServicio()) {
					Reserva reserva(matricula, horaInicio, horaFin, 2, pcNum, tipo);
					if (salas[2].reservarComputadora(pcNum, reserva)) {
						computadoras[pcNum].setOcupada(true, matricula);
						usuario->setReservaActual(true);
						usuario->setSalaAsignada(2);
						if (Profesor* prof = dynamic_cast<Profesor*>(usuario)) {
							prof->setNumeroPC(pcNum);
						}
						cout << "Reserva exitosa!" << endl;
					}
				}
				else {
					cout << "La computadora no esta disponible." << endl;
				}
			}
		}
	}
	else if (tipo == "Alumno") {
		cout << "Hora inicio (HH:MM): ";
		cin >> horaInicio;
		cout << "Hora fin (HH:MM): ";
		cin >> horaFin;

		cout << "Numero de computadora (1-20): ";
		cin >> pcNum;
		pcNum--;

		if (pcNum >= 0 && pcNum < computadoras.size()) {
			if (!computadoras[pcNum].estaOcupada() && !computadoras[pcNum].estaEnServicio()) {
				Reserva reserva(matricula, horaInicio, horaFin, 2, pcNum, tipo);
				if (salas[2].reservarComputadora(pcNum, reserva)) {
					computadoras[pcNum].setOcupada(true, matricula);
					usuario->setReservaActual(true);
					usuario->setSalaAsignada(2);
					if (Alumno* alum = dynamic_cast<Alumno*>(usuario)) {
						alum->setNumeroPC(pcNum);
					}
					cout << "Reserva exitosa!" << endl;
				}
			}
			else {
				cout << "La computadora no esta disponible." << endl;
			}
		}
	}
}

void Gestor::eliminarReserva() {
	string matricula, hora;

	cout << "\n=== ELIMINAR RESERVA ===" << endl;
	cout << "Matricula: ";
	cin >> matricula;
	cout << "Hora de inicio (HH:MM): ";
	cin >> hora;

	// Buscar en todas las salas
	for (auto& sala : salas) {
		sala.eliminarReserva(matricula, hora);
	}

	// Liberar computadora si corresponde
	for (auto& pc : computadoras) {
		if (pc.getMatriculaUsuario() == matricula) {
			pc.setOcupada(false);
		}
	}

	// Actualizar estado del usuario
	Usuario* usuario = buscarUsuario(matricula);
	if (usuario) {
		usuario->setReservaActual(false);
		usuario->setSalaAsignada(-1);
	}
}

void Gestor::editarReserva() {
	string matricula, horaVieja, horaNueva;
	int opcion;

	cout << "\n=== EDITAR RESERVA ===" << endl;
	cout << "Matricula: ";
	cin >> matricula;
	cout << "Hora actual (HH:MM): ";
	cin >> horaVieja;

	// Por simplicidad, aqui solo editamos la hora
	// En una implementacion completa, buscariamos la reserva y la modificariamos
	cout << "Nueva hora inicio (HH:MM): ";
	cin >> horaNueva;

	cout << "Reserva editada (simulacion)." << endl;
	cout << "Nota: En una implementacion completa, se buscaria la reserva" << endl;
	cout << "y se modificarian los campos correspondientes." << endl;
}

void Gestor::verUsuarios() {
	int opcion;
	cout << "\n=== VER USUARIOS ===" << endl;
	cout << "1. Ver profesores" << endl;
	cout << "2. Ver alumnos" << endl;
	cout << "Seleccione: ";
	cin >> opcion;

	if (opcion == 1) {
		verListaProfesores();
	}
	else if (opcion == 2) {
		verListaAlumnos();
	}
	else {
		cout << "Opcion invalida." << endl;
	}
}

void Gestor::verListaProfesores() const {
	cout << "\n=== LISTA DE PROFESORES ===" << endl;
	cout << "Matricula | Nombre | Apellidos | Facultad | Reserva Actual | Sala" << endl;
	cout << "----------------------------------------------------------------" << endl;

	for (const auto& prof : profesores) {
		cout << prof.getMatricula() << " | "
			<< prof.getNombre() << " | "
			<< prof.getApellidos() << " | "
			<< prof.getFacultad() << " | "
			<< (prof.tieneReservaActual() ? "Si" : "No") << " | ";

		if (prof.tieneReservaActual()) {
			int sala = prof.getSalaAsignada();
			if (sala == 2) {
				cout << "C (PC " << prof.getNumeroPC() + 1 << ")";
			}
			else {
				cout << (sala == 0 ? 'A' : sala == 1 ? 'B' : '?');
			}
		}
		else {
			cout << "-";
		}
		cout << endl;
	}
}

void Gestor::verListaAlumnos() const {
	cout << "\n=== LISTA DE ALUMNOS ===" << endl;
	cout << "Matricula | Nombre | Apellidos | Facultad | Reserva Actual | Sala | Computadora" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	for (const auto& alum : alumnos) {
		cout << alum.getMatricula() << " | "
			<< alum.getNombre() << " | "
			<< alum.getApellidos() << " | "
			<< alum.getFacultad() << " | "
			<< (alum.tieneReservaActual() ? "Si" : "No") << " | ";

		if (alum.tieneReservaActual()) {
			cout << "C | PC " << alum.getNumeroPC() + 1;
		}
		else {
			cout << "- | -";
		}
		cout << endl;
	}
}

Usuario* Gestor::buscarUsuario(const string& matricula) {
	// Buscar en profesores
	for (auto& prof : profesores) {
		if (prof.getMatricula() == matricula) {
			return &prof;
		}
	}

	// Buscar en alumnos
	for (auto& alum : alumnos) {
		if (alum.getMatricula() == matricula) {
			return &alum;
		}
	}

	return nullptr;
}

bool Gestor::verificarDisponibilidad(int salaId, const string& hora, int pc) const {
	// Implementacion simplificada
	// En una implementacion real, se verificarian los horarios especificos
	return !salas[salaId].estaOcupada();
}

void Gestor::agregarReserva(const Reserva& reserva) {
	reservas.push_back(reserva);
}

bool Gestor::reservarSala(const std::string& matricula, const std::string& hora_inicio, const std::string& hora_fin, int sala_id)
{
	if (sala_id < 0 || sala_id > 1) {
		std::cerr << "ERROR: Sala ID invalido." << std::endl;
		return false;
	}

	Usuario* usuario = buscarUsuario(matricula);
	if (!usuario || usuario->getTipo() != "Profesor") {
		std::cerr << "ERROR: Usuario no encontrado o no es Profesor." << std::endl;
		return false;
	}

	if (verificarDisponibilidad(sala_id, hora_inicio)) {

		Reserva reserva(matricula, hora_inicio, hora_fin, sala_id, -1, usuario->getTipo());
		salas[sala_id].agregarReserva(reserva);

		usuario->setReservaActual(true);
		usuario->setSalaAsignada(sala_id);

		return true;
	}
	else {
		return false;
	}
}

bool Gestor::reservarPC(const std::string& matricula, const std::string& hora_inicio, const std::string& hora_fin, int pc_num)
{
	const int SALA_C_ID = 2;

	if (pc_num < 0 || pc_num >= computadoras.size()) {
		std::cerr << "ERROR: Numero de PC invalido." << std::endl;
		return false;
	}

	Usuario* usuario = buscarUsuario(matricula);
	if (!usuario) {
		std::cerr << "ERROR: Usuario no encontrado." << std::endl;
		return false;
	}

	if (!computadoras[pc_num].estaOcupada() && !computadoras[pc_num].estaEnServicio()) {

		Reserva reserva(matricula, hora_inicio, hora_fin, SALA_C_ID, pc_num, usuario->getTipo());

		if (salas[SALA_C_ID].reservarComputadora(pc_num, reserva)) {

			// 5. Actualizar estado
			computadoras[pc_num].setOcupada(true, matricula);
			usuario->setReservaActual(true);
			usuario->setSalaAsignada(SALA_C_ID);

			if (Profesor* prof = dynamic_cast<Profesor*>(usuario)) {
				prof->setNumeroPC(pc_num);
			}
			else if (Alumno* alum = dynamic_cast<Alumno*>(usuario)) {
				alum->setNumeroPC(pc_num);
			}

			return true;
		}
	}
	return false;
}

bool Gestor::editarReserva(const std::string& matricula, const std::string& hora_antigua, const std::string& hora_nueva)
{
	if (matricula.empty() || hora_antigua.empty() || hora_nueva.empty()) {
		std::cerr << "ERROR: Todos los campos de edición deben estar llenos." << std::endl;
		return false;
	}

	for (Sala& sala : salas)
	{
		if (sala.intentarEditarReserva(matricula, hora_antigua, hora_nueva))
		{
			return true;
		}
	}

	return false;

}

void Gestor::eliminarReservas(const std::string& matricula, const std::string& hora)
{
	// Buscar en todas las salas
	for (auto& sala : salas) {
		sala.eliminarReserva(matricula, hora);
	}

	// Liberar computadora si corresponde
	for (auto& pc : computadoras) {
		if (pc.getMatriculaUsuario() == matricula) {
			pc.setOcupada(false);
		}
	}

	// Actualizar estado del usuario
	Usuario* usuario = buscarUsuario(matricula);
	if (usuario) {
		usuario->setReservaActual(false);
		usuario->setSalaAsignada(-1);
	}
}

const vector<Sala>& Gestor::obtenerSalas() const
{
	return salas;
}

const vector<Computadora>& Gestor::obtenerComputadoras() const
{
	return computadoras;
}

const vector<Profesor>& Gestor::obtenerProfesores() const
{
	return profesores;
}

const vector<Alumno>& Gestor::obtenerAlumnos() const
{
	return alumnos;
}