#ifndef ESTADIA_H
#define ESTADIA_H

#include <string>
#include <vector>

class Estadia {
private:
    int codigoDaEstadia, codigoDoCliente, numeroDoQuarto, status, quantidadeDeDiarias;
    std::string dataDeEntrada, dataDeSaida;

public:
    Estadia(int cod, std::string ent, std::string sai, int dias, int cli, int qua)
        : codigoDaEstadia(cod), dataDeEntrada(ent), dataDeSaida(sai), 
          quantidadeDeDiarias(dias), codigoDoCliente(cli), numeroDoQuarto(qua), status(1) {}

    int getCodigoDaEstadia() const { return codigoDaEstadia; }
    int getCodigoDoCliente() const { return codigoDoCliente; }
    int getNumeroDoQuarto() const { return numeroDoQuarto; }
    int getQuantidadeDeDiarias() const { return quantidadeDeDiarias; }
    int getStatus() const { return status; }
    std::string getDataDeEntrada() const { return dataDeEntrada; }
    std::string getDataDeSaida() const { return dataDeSaida; }
    void setStatus(int s) { status = s; }
};

// Protótipos das funções globais
void cadastrarEstadia();
void finalizarEstadia();
void listarEstadia();
void listarEstadiaC();
void carregarEstadias();
void salvarEstadias();

#endif