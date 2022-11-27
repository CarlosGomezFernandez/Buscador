/******************************************************************************** 
* -Program: SSOOIIGLE Part-II (practica 3 de Sistemas Operativos II)
* -Author/s name: Jesus Baltasar Fernandez, Carlos Gomez Fernandez
* -Release/Creation date: 23/06/2021
* -Class name: colaProtegida.h
********************************************************************************/

#include "../include/cliente.h"

class ColaProtegida{
    private: 
        std::mutex semCola;
        std::queue <Cliente> colaProtegida;
        
    public:
        void push(Cliente c);
        void pop();
        Cliente front();
        bool vacia();
        int tamanio();
        void recargaSaldo(int saldo);
};

void ColaProtegida::push(Cliente c){ 
    std::unique_lock<std::mutex> ul (semCola);
    colaProtegida.push(c);
    ul.unlock();
}

void ColaProtegida::pop() { 
    std::unique_lock<std::mutex> ul (semCola);
    colaProtegida.pop();
    ul.unlock();
}

Cliente ColaProtegida::front() {
    std::unique_lock<std::mutex> ul (semCola);
    Cliente c(colaProtegida.front().getIdCliente(), colaProtegida.front().getTipoCliente(), colaProtegida.front().getSaldoCliente(), colaProtegida.front().getColaClientes());
    ul.unlock();
    return c;
}

bool ColaProtegida::vacia() {
    bool vacia;
    std::unique_lock<std::mutex> ul (semCola);
    vacia = colaProtegida.empty();
    ul.unlock();
    return vacia;
}

int ColaProtegida::tamanio(){
    int tamanio;
    std::unique_lock<std::mutex> ul (semCola);
    tamanio = colaProtegida.size();
    ul.unlock();
    return tamanio;
}

void ColaProtegida::recargaSaldo(int saldo) {
    std::unique_lock<std::mutex> ul (semCola);
    colaProtegida.front().setSaldoCliente(saldo);
    ul.unlock();
}