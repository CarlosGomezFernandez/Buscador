/******************************************************************************** 
* -Program: SSOOIIGLE Part-II (practica 3 de Sistemas Operativos II)
* -Author/s name: Jesus Baltasar Fernandez, Carlos Gomez Fernandez
* -Release/Creation date: 23/06/2021
* -Class name: cliente.h
********************************************************************************/

class Cliente{
    private: 
        int idCliente;
        std::string tipoCliente;
        int saldoCliente;
        std::queue<std::queue<std::string>> colaClientes;
   
    public:
        Cliente(int idCliente, std::string tipoCliente, int saldoCliente, std::queue<std::queue<std::string>> colaClientes);

        int getIdCliente();
        std::string getTipoCliente();
        int getSaldoCliente();
        std::queue <std::queue <std::string>> getColaClientes();
        
        void setIdCliente(int id);
        void setTipoCliente(std::string tipo);  
        void setSaldoCliente(int saldo);   
        
        void push(std::queue<std::string> colaC);
        std::queue<std::string> pop();
        bool vacia();
};

/* Constructor de la clase cliente empleando lista de inicializadores */
Cliente::Cliente(int id, std::string tipo,int saldo, std::queue < std::queue <std::string>> colaC) : idCliente(id), tipoCliente(tipo), saldoCliente(saldo), colaClientes(colaC) {}

/* Getters de la clase Cliente */
int Cliente::getIdCliente(){ 
    return idCliente;
}

std::string Cliente::getTipoCliente(){
    return tipoCliente;
}

int Cliente::getSaldoCliente(){
    return saldoCliente;
}

std::queue <std::queue <std::string>> Cliente::getColaClientes(){
    return colaClientes;
}

/* Setters de la clase Cliente */
void Cliente::setIdCliente(int id){
    idCliente = id;
}

void Cliente::setTipoCliente(std::string tipo){
    tipoCliente = tipo;
}

void Cliente::setSaldoCliente(int saldo){
    saldoCliente = saldo;
}

/* Añade colas a la cola del cliente. */
void Cliente::push(std::queue<std::string> colaC){
    colaClientes.push(colaC);  
}

/* Saca la cola que encabeza la cola del cliente. */
std::queue<std::string> Cliente::pop(){
    std::queue<std::string> cabecera = colaClientes.front();
    colaClientes.pop();
    return cabecera;  
}

/* Comprueba si la cola del cliente está vacia. */
bool Cliente::vacia(){
    return colaClientes.empty();
}