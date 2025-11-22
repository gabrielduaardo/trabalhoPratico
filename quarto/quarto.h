#ifndef QUARTO_H
#define QUARTO_H

#include <string>
#include <vector>
#include <iostream>

class Quarto {
private:
    int numeroDoQuarto, status, quantidadeDeHospedes;
    double valorDaDiaria; 

public:
    // Construtor
    Quarto(int num, int hospedes, double valor, const int status);


    // Métodos Get
    int getNumeroDoQuarto() const { return numeroDoQuarto; }
    int getQuantidadeDeHospedes() const { return quantidadeDeHospedes; }
    double getValorDaDiaria() const { return valorDaDiaria; }
    int getStatus() const { return status; }

    // Método para mudar o status
    void setStatus(const int novoStatus) { status = novoStatus; }
};

#endif