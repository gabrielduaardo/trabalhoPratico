
#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <string>
#include <vector>
#include <iostream>

class Funcionario {
private:
    int codigo;
    std::string nome;
    std::string telefone;
    std::string cargo;
    double salario;

public:
    // Construtor
    Funcionario(int cod, const std::string& nome, const std::string& tel, const std::string& cargo, double sal);

    // Métodos Get
    int getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getTelefone() const { return telefone; }
    std::string getCargo() const { return cargo; }
    double getSalario() const { return salario; }
};

extern std::vector<Funcionario> listaDeFuncionario;

bool cadastrarFuncionario(const std::string &nome, const std::string &telefone, const std::string &cargo, double salario);
void listarFuncionario();

#endif // Fim do bloco Funcionario_h