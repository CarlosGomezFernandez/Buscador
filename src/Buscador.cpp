/******************************************************************************** 
* -Program: SSOOIIGLE Part-II (practica 3 de Sistemas Operativos II)
* -Author/s name: Jesus Baltasar Fernandez, Carlos Gomez Fernandez
* -Release/Creation date: 23/06/2021
* -Class name: Buscador.cpp
********************************************************************************/

#include <iostream>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unistd.h>
#include "../include/colaProtegida.h"
#include "../include/SSOOIIGLE.h"  
#include "../include/semaforoContador.h" 
#include "../include/colores.h" 

#define CLIENTES 50
#define REPLICAS 4

std::condition_variable cv_banco;
std::condition_variable &ref_cv_banco = cv_banco;
std::condition_variable cv_busqueda;

std::mutex mutex_banco;
std::mutex mutex_print;
std::mutex mutex_pago;
std::mutex mutex_buscar;
SemCounter Sem(3);
std::mutex &ref_mutex_pago = mutex_pago;
std::mutex &ref_mutex_buscar = mutex_buscar;

ColaProtegida cp_peticiones_banco;
ColaProtegida &cp_ref_peticiones_banco = cp_peticiones_banco;
ColaProtegida cp_cliente_premium;
ColaProtegida cp_cliente_gratis;
ColaProtegida cp_peticiones;
ColaProtegida cp_peticiones_print;

void sistemaPago();
void peticionCliente();
void servicioBusqueda();
void mostrarResultados(double tiempo, Cliente c);

int main(){
    std::vector<std::thread> vectorClientes;
    std::vector<std::thread> vectorBusquedas;
    int tipo;

    for (int i = 1; i <= CLIENTES; i++){
        tipo = rand() % 3;
        std::queue<std::queue<std::string>> colaClientes;
        Cliente c(i, "", 0, colaClientes);
        switch (tipo){
            case 0:
                c.setTipoCliente("Cuenta gratuita");
                c.setSaldoCliente(10);
                cp_cliente_gratis.push(c);
                break;

            case 1:
                c.setTipoCliente("Cuenta premium con limite de saldo");
                c.setSaldoCliente(10);
                cp_cliente_premium.push(c);
                break;

            case 2:
                c.setTipoCliente("Cuenta premium ilimitada");
                c.setSaldoCliente(1);
                cp_cliente_premium.push(c);
                break;
        }

        vectorClientes.push_back(std::thread(peticionCliente));
    }

    for (int i = 0; i < REPLICAS; i++){
        vectorBusquedas.push_back(std::thread(servicioBusqueda));
    }

    std::thread hiloBanco(sistemaPago);
    hiloBanco.detach();
    std::for_each(vectorClientes.begin(), vectorClientes.end(), std::mem_fn(&std::thread::join));
    std::for_each(vectorBusquedas.begin(), vectorBusquedas.end(), std::mem_fn(&std::thread::detach));

    return EXIT_SUCCESS;
}

void sistemaPago(){

    ref_mutex_buscar.lock();
    ref_mutex_pago.lock();

    while (1){
        std::unique_lock<std::mutex> ul(mutex_banco);
        /* Bloquea el semáforo asociado a la cola de solicitudes del
        sistema de pago, mientras que no haya solicitudes de recarga
        de saldo por parte de los clientes */
        ref_cv_banco.wait(ul, [] { return !cp_ref_peticiones_banco.vacia(); });

        try{
            cp_ref_peticiones_banco.recargaSaldo(500);
            std::cout << BOLDRED << "[BANCO]" << RESET << " ha recargado el saldo al " << BOLDGREEN<< "cliente " << cp_ref_peticiones_banco.front().getIdCliente() << RESET << std::endl;
        }catch (std::exception &e){
            std::cout << BOLDYELLOW << "[EXCEPCION] " << RESET << "Se ha producido la excepcion: " << BOLDYELLOW << e.what() << RESET << " al recargar el saldo del cliente." << std::endl;
        }

        ref_mutex_buscar.unlock();
        ref_mutex_pago.lock();
        cp_ref_peticiones_banco.pop();
        sleep(0.5);
    }
}

void peticionCliente(){

    int prioridad;
    std::queue<std::queue<std::string>> colaClientes;
    prioridad = rand() % 10;
    Cliente aux(0, "", 0, colaClientes);
    if (!cp_cliente_premium.vacia() && !cp_cliente_gratis.vacia())
    {
        if (prioridad > 7)
        {
            aux.setIdCliente(cp_cliente_gratis.front().getIdCliente());
            aux.setTipoCliente(cp_cliente_gratis.front().getTipoCliente());
            aux.setSaldoCliente(cp_cliente_gratis.front().getSaldoCliente());
            cp_cliente_gratis.pop();
            cp_peticiones.push(aux);
        }
        else
        {
            aux.setIdCliente(cp_cliente_premium.front().getIdCliente());
            aux.setTipoCliente(cp_cliente_premium.front().getTipoCliente());
            aux.setSaldoCliente(cp_cliente_premium.front().getSaldoCliente());
            cp_cliente_premium.pop();
            cp_peticiones.push(aux);
        }
    }
    else if (cp_cliente_premium.vacia() && !cp_cliente_gratis.vacia())
    {

        aux.setIdCliente(cp_cliente_gratis.front().getIdCliente());
        aux.setTipoCliente(cp_cliente_gratis.front().getTipoCliente());
        aux.setSaldoCliente(cp_cliente_gratis.front().getSaldoCliente());
        cp_cliente_gratis.pop();
        cp_peticiones.push(aux);
    }
    else
    {

        aux.setIdCliente(cp_cliente_premium.front().getIdCliente());
        aux.setTipoCliente(cp_cliente_premium.front().getTipoCliente());
        aux.setSaldoCliente(cp_cliente_premium.front().getSaldoCliente());
        cp_peticiones.push(aux);
        cp_cliente_premium.pop();
    }
    unsigned tIni = clock();
    while (cp_peticiones_print.vacia())
    {
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    try
    {
        while (cp_peticiones_print.front().getIdCliente() != aux.getIdCliente())
        {

        }
    }
    catch (std::exception &e)
    {
        std::cout << BOLDYELLOW << "[EXCEPCION] " << RESET << "Excepcion: " << BOLDYELLOW << e.what() << RESET << " al comparar la cabecera de la cola cp_peticiones_print con el cliente" << std::endl;
    }

    mutex_print.lock();
    try
    {
        unsigned tFin = clock();
        float tiempo = (double(tFin - tIni) / CLOCKS_PER_SEC);
        mostrarResultados(tiempo, cp_peticiones_print.front());
    }
    catch (std::exception &e)
    {
        std::cout << BOLDYELLOW << "[EXCEPCION] " << RESET << "Excepcion: " << BOLDYELLOW << e.what() << RESET << " al imprimir." << std::endl;
    }

    mutex_print.unlock();
    cp_peticiones_print.pop();
}

void servicioBusqueda(){
    std::mutex mutex_busqueda;
    std::unique_lock<std::mutex> ul_busqueda(mutex_busqueda);
    std::vector<std::string> diccionario;
    int random;

    diccionario.push_back("gente");
    diccionario.push_back("cenizas");
    diccionario.push_back("personas");
    diccionario.push_back("realidad");
    diccionario.push_back("equipos");
    diccionario.push_back("que");
    diccionario.push_back("esfuerzo");
    diccionario.push_back("valor");
    diccionario.push_back("liderazgo");
    diccionario.push_back("pensar");

    while (1){
        /* Bloquea el semáforo asociado a la cola de peticiones de busqueda siempre que no
        haya solicitudes de busqueda por parte de los clientes */
        cv_busqueda.wait(ul_busqueda, [] { return !cp_peticiones.vacia(); });
        random = rand() % 10;
        Sem.wait();
        std::cout << BOLDYELLOW << "[MANAGER] " << RESET << "ha atendido correctamente al " << BOLDGREEN << "cliente " << cp_peticiones.front().getIdCliente() << RESET << std::endl;
        SSOOIIGLE SSOOIIGLE(cp_peticiones.front(), diccionario[random], ref_cv_banco, cp_ref_peticiones_banco, ref_mutex_buscar, ref_mutex_pago);
        cp_peticiones.pop();
        Sem.signal();
        std::thread busqueda(&SSOOIIGLE::buscar, &SSOOIIGLE);
        busqueda.join();
        Cliente c(SSOOIIGLE.getCliente().getIdCliente(), SSOOIIGLE.getCliente().getTipoCliente(), SSOOIIGLE.getCliente().getSaldoCliente(), SSOOIIGLE.getCliente().getColaClientes());
        cp_peticiones_print.push(c);
    }
}

void mostrarResultados(double tiempo, Cliente c){
    
    std::string archivo = "busquedas/Cliente ";
    archivo += std::to_string(c.getIdCliente());

    std::queue<std::queue<std::string>> aux = c.getColaClientes();

    std::ofstream archivoBusquedas(archivo);
    std::cout << BOLDGREEN << "[CLIENTE: " << c.getIdCliente() << "] " << RESET << " imprimiendo busqueda en fichero" << std::endl;

    archivoBusquedas << "ID del cliente: " << c.getIdCliente() <<  std::endl;
    archivoBusquedas << "Tipo de cliente: " << c.getTipoCliente() << std::endl;
    archivoBusquedas << "Tiempo total de la busqueda: " << tiempo << " segundos" << std::endl;
    archivoBusquedas << ""  << std::endl;
    
    while (!aux.empty()){
        std::queue<std::string> listaAuxiliar = aux.front();
        std::chrono::milliseconds(30);
        archivoBusquedas << "Archivo: " << listaAuxiliar.front();
        listaAuxiliar.pop();
        archivoBusquedas << " | en la linea " << listaAuxiliar.front();
        listaAuxiliar.pop();
        archivoBusquedas << " :: ... " << listaAuxiliar.front();
        listaAuxiliar.pop();
        archivoBusquedas << " " << listaAuxiliar.front();
        listaAuxiliar.pop();
        archivoBusquedas << " " << listaAuxiliar.front() << " ... " << std::endl;
        listaAuxiliar.pop();
        aux.pop();
    }
    
    archivoBusquedas.close();
    std::cout << BOLDGREEN << "[CLIENTE: " << c.getIdCliente() << "] " << RESET << " ha terminado de imprimir" << std::endl;
    sleep(0.5);
}