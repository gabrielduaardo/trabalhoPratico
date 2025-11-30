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
    int status; // 1 = ativa, 0 = finalizada

public:
    // Construtor
    Estadia(int codEstadia, const std::string& dataEnt, const std::string& dataSai,
            int diarias, int codCliente, int numQuarto)
        : codigoDaEstadia(codEstadia),
          dataDeEntrada(dataEnt),
          dataDeSaida(dataSai),
          quantidadeDeDiarias(diarias),
          codigoDoCliente(codCliente),
          numeroDoQuarto(numQuarto),
          status(1) {} // Nova estadia comeca ativa

    // Gets
    int getCodigoDaEstadia() const { return codigoDaEstadia; }
    std::string getDataDeEntrada() const { return dataDeEntrada; }
    std::string getDataDeSaida() const { return dataDeSaida; }
    int getQuantidadeDeDiarias() const { return quantidadeDeDiarias; }
    int getCodigoDoCliente() const { return codigoDoCliente; }
    int getNumeroDoQuarto() const { return numeroDoQuarto; }
    int getStatus() const { return status; }

    // Sets
    void setDataDeSaida(const std::string& novaData) { dataDeSaida = novaData; }
    void setQuantidadeDeDiarias(int q) { quantidadeDeDiarias = q; }
    void setStatus(int s) { status = s; }
};

Estadia* buscarEstadiaAtiva(int numQuarto);

#endif
