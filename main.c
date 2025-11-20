#include <iostream>
#include <string>

class Cliente {
private:
    int codigo;
    std::string nome;
    std::string endereco;
    std::string telefone;

public:
    // Construtor
    Cliente(int cod, const std::string& n, const std::string& end, const std::string& tel)
        : codigo(cod), nome(n), endereco(end), telefone(tel) {}

    // Métodos Get (Acessores)
    int getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getEndereco() const { return endereco; }
    std::string getTelefone() const { return telefone; }

    // (Poderia incluir métodos Set para modificação de dados, se necessário)
};


class Funcionario {
private:
    int codigo;
    std::string nome;
    std::string telefone;
    std::string cargo;
    double salario;

public:
    // Construtor
    Funcionario(int cod, const std::string& n, const std::string& tel, const std::string& c, double sal)
        : codigo(cod), nome(n), telefone(tel), cargo(c), salario(sal) {}

    // Métodos Get
    int getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getTelefone() const { return telefone; }
    std::string getCargo() const { return cargo; }
    double getSalario() const { return salario; }
};


class Quarto {
private:
    int numeroDoQuarto;
    int quantidadeDeHospedes;
    double valorDaDiaria;
    std::string status;

public:
    // Construtor
    Quarto(int num, int hospedes, double valor, const std::string& stat)
        : numeroDoQuarto(num), quantidadeDeHospedes(hospedes), valorDaDiaria(valor), status(stat) {}

    // Métodos Get
    int getNumeroDoQuarto() const { return numeroDoQuarto; }
    int getQuantidadeDeHospedes() const { return quantidadeDeHospedes; }
    double getValorDaDiaria() const { return valorDaDiaria; }
    std::string getStatus() const { return status; }

    // Método para mudar o status
    void setStatus(const std::string& novoStatus) { status = novoStatus; }
};

class Estadia {
private:
    int codigoDaEstadia;
    std::string dataDeEntrada;
    std::string dataDeSaida;
    int quantidadeDeDiarias;
    int codigoDoCliente; // Chave estrangeira para Cliente
    int numeroDoQuarto;  // Chave estrangeira para Quarto

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


int main (void) {

}