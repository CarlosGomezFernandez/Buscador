/******************************************************************************** 
* -Program: SSOOIIGLE Part-II (practica 3 de Sistemas Operativos II)
* -Author/s name: Jesus Baltasar Fernandez, Carlos Gomez Fernandez
* -Release/Creation date: 23/06/2021
* -Class name: SSOOIIGLE.h
********************************************************************************/

#include <fstream>
#include <sstream>
#include <list>
#include "../include/colores.h"

class SSOOIIGLE{
    private:
        std::mutex mutex_cola;
        Cliente cliente;
        std::string palabraABuscar;
        std::condition_variable &ref_cv_banco;
        ColaProtegida &cp_ref_peticiones_banco;
        std::mutex &ref_mutex_buscar;
        std::mutex &ref_mutex_pago;
    public:
        SSOOIIGLE(Cliente cliente, std::string palabraABuscar, std::condition_variable &ref_cv_banco, ColaProtegida &cp_ref_peticiones_banco, std::mutex &ref_mutex_buscar, std::mutex &ref_mutex_pago);
        std::string pasarMinusculas(std::string palabra);
        int contarLineas(char *fichero);
        Cliente getCliente();
        void buscarPalabra(std::string palabraABuscar, std::string fichero);
        void buscar();
        void controlBusqueda();
};

/* Constructor de la clase SSOOIIGLE empleando lista de inicializadores */
SSOOIIGLE::SSOOIIGLE(Cliente c, std::string p, std::condition_variable &b, ColaProtegida &p_pb, std::mutex &p_sb, std::mutex &p_ssp) : cliente(c), palabraABuscar(p), ref_cv_banco(b), cp_ref_peticiones_banco(p_pb), ref_mutex_buscar(p_sb), ref_mutex_pago(p_ssp) {}

std::string SSOOIIGLE::pasarMinusculas(std::string palabra){
    
    for(int i=0; i<palabra.length(); i++){
        palabra[i]=std::tolower(palabra[i]);
    }
    return palabra;
}

int SSOOIIGLE::contarLineas(char *fichero){

    int lineas = 0;
    std::fstream archivo;
    std::string linea;

    archivo.open(fichero, std::ios::in);
    if (!archivo.is_open())
    {
        fprintf(stderr, "[ERROR] Error en la apertura del archivo");
        exit(EXIT_FAILURE);
    }
    while (getline(archivo, linea))
    {
        lineas++;
    }
    archivo.close();
    return lineas;
}

Cliente SSOOIIGLE::getCliente(){
    return cliente;
}

void SSOOIIGLE::buscarPalabra(std::string palabraABuscar, std::string fichero){
    std::vector<std::list<std::string> *> vectorBusqueda;
    std::string cadenaPalabra, palabra;
    std::ifstream archivo;
    std::string palabraAnterior;

    int lineas = 0;

    archivo.open(fichero);

    while (!archivo.eof())
    {
        while (getline(archivo, cadenaPalabra))
        {
            lineas++;
            std::string palabraFinal = pasarMinusculas(cadenaPalabra);
            std::istringstream p(palabraFinal);

            while (!p.eof() && cliente.getSaldoCliente() > 0)
            {
                std::string palabra;
                p >> palabra;
                if (palabra == palabraABuscar)
                {
                    
                    std::string palabraSiguiente;
                    p >> palabraSiguiente;

                    std::string numeroLinea = std::__cxx11::to_string(lineas);
                    std::queue<std::string> colaHiloBuscador;

                    colaHiloBuscador.push(fichero);
                    colaHiloBuscador.push(numeroLinea);
                    colaHiloBuscador.push(palabraAnterior);
                    colaHiloBuscador.push(palabra);
                    colaHiloBuscador.push(palabraSiguiente);

                    mutex_cola.lock();

                    cliente.push(colaHiloBuscador);

                    mutex_cola.unlock();
                    controlBusqueda();
                    if (palabraSiguiente == palabra && cliente.getSaldoCliente() > 0)
                    {
                        p >> palabraSiguiente;
                        palabraAnterior = palabra;
                        numeroLinea = std::__cxx11::to_string(lineas);
                        std::queue<std::string> colaHiloBuscador;
            
                        colaHiloBuscador.push(fichero);
                        colaHiloBuscador.push(numeroLinea);
                        colaHiloBuscador.push(palabraAnterior);
                        colaHiloBuscador.push(palabra);
                        colaHiloBuscador.push(palabraSiguiente);

                        mutex_cola.lock();
                        cliente.push(colaHiloBuscador);
                        mutex_cola.unlock();
                        controlBusqueda();
                    }
                }
                palabraAnterior = palabra;
            }
        }
    }
}

void SSOOIIGLE::controlBusqueda(){

    if (cliente.getTipoCliente() != "Cuenta premium ilimitada")
    {
        cliente.setSaldoCliente(cliente.getSaldoCliente() - 1);
    }
    if (cliente.getSaldoCliente() == 0 && cliente.getTipoCliente() == "Cuenta premium con limite de saldo")
    {
        std::cout << BOLDGREEN << "[CLIENTE: " << cliente.getIdCliente() << "]" << RESET << " necesita recargar su saldo" << std::endl;
        cp_ref_peticiones_banco.push(cliente);
        ref_cv_banco.notify_one();
        ref_mutex_buscar.lock();
        cliente.setSaldoCliente(cp_ref_peticiones_banco.front().getSaldoCliente());
        ref_mutex_pago.unlock();
    }
}

void SSOOIIGLE::buscar(){

    buscarPalabra(palabraABuscar, "libros/17 LEYES DEL TRABAJO EN EQUIPO.txt");
    buscarPalabra(palabraABuscar, "libros/El Oro Y La Ceniza - Abecassis Eliette .txt");
    buscarPalabra(palabraABuscar, "libros/SEAMOS PERSONAS DE INFLUENCIA - JOHN MAXWELL.txt");
    buscarPalabra(palabraABuscar, "libros/VIVE TU SUEÑO - JOHN MAXWELL.txt");
}