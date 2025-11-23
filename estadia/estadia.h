#ifndef ESTADIA_H
#define ESTADIA_H

#include <string>
#include <vector>
#include <iostream>

class Estadia {
private:
    int codigoDaEstadia;
    std::string dataDeEntrada;
    std::string dataDeSaida;
    int quantidadeDeDiarias;
    int codigoDoCliente; 
    int numeroDoQuarto;

public:
    // Construtor
    Estadia(int codEstadia, const std::string& dataEnt, const std::string& dataSai,
            int diarias, int codCliente, int numQuarto)
        : codigoDaEstadia(codEstadia), dataDeEntrada(dataEnt), dataDeSaida(dataSai),
          quantidadeDeDiarias(diarias), codigoDoCliente(codCliente), numeroDoQuarto(numQuarto) {}

    // Métodos Get
    int getCodigoDaEstadia() const { return codigoDaEstadia; }
    std::string getDataDeEntrada() const { return dataDeEntrada; }
    std::string getDataDeSaida() const { return dataDeSaida; }
    int getQuantidadeDeDiarias() const { return quantidadeDeDiarias; }
    int getCodigoDoCliente() const { return codigoDoCliente; }
    int getNumeroDoQuarto() const { return numeroDoQuarto; }
};

#endif