/*  -=-=-=-=| LEGENDA funcionario |-=-=-=-=

nom = nome que vai receber do usuario
carg = cargo que vai receber do usuario
tel = relefone que vai receber do usuario
sal = salario
push_back é um método de vetores que adiciona um novo elemento ao final do contêiner
.empty() é uma função membro de contêineres da Standard Template Library (STL), como std::vector, std::string e std::map, que verifica se o contêiner está vazi

   -=-=-=-=| FIM |-=-=-=-=*/

#include "funcionario.h"

std::vector<Funcionario> listaDeFuncionario;

Funcionario::Funcionario(int cod, const std::string &nom, const std::string &tel, const std::string &carg, double sal)
    : codigo(cod), nome(nom), telefone(tel), cargo(carg), salario(sal) {}

void funcionario()
{
    std::string nome, telefone, cargo;
    double salario;

    std::cout << "\n-=-| Cadastro de Funcionario |-=-" << std::endl;

    std::cout << " Informe o NOME " << std::endl;
    std::cin >> nome;

    std::cout << " Informe o NUMERO DE TELEFONE " << std::endl;
    std::cin >> telefone;

    std::cout << " Informe o CARGO " << std::endl;
    std::cin >> cargo;

    std::cout << " Informe o SALARIO " << std::endl;
    std::cin >> salario;

    // Cadastro dos Funcioanrio
    cadastrarFuncionario(nome, telefone, cargo, salario);
}

bool cadastrarFuncionario(const std::string &nome, const std::string &telefone, const std::string &cargo, double salario)
{

    static int proximoCodigo = 1;

    Funcionario novoFuncionario(proximoCodigo, nome, telefone, cargo, salario);

    listaDeFuncionario.push_back(novoFuncionario);
    proximoCodigo++;

    std::cout << "Funcionario '" << nome << "' cadastrado com sucesso!" << std::endl;
    std::cout << "Codigo atribuido: " << novoFuncionario.getCodigo() << std::endl;

    return true;
}

void listarFuncionario()
{
    std::cout << "\n-=-| Lista de Funcionarios Cadastrados ( " << listaDeFuncionario.size() << " ) |-=-" << std::endl;

    if (listaDeFuncionario.empty())
    {
        std::cout << "Nenhum funcionario cadastrado. " << std::endl;
        return;
    }

    for (const auto &funcionario : listaDeFuncionario)
    {
        std::cout << "Codigo: " << funcionario.getCodigo()
                  << ", Nome: " << funcionario.getNome()
                  << ", Telefone: " << funcionario.getTelefone()
                  << ", Cargo: " << funcionario.getCargo()
                  << ", Salario: " << funcionario.getSalario() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}