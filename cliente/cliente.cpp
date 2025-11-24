/*  -=-=-=-=| LEGENDA cliente.pp |-=-=-=-=

nom = nome que vai receber do usuario
end = endereço que vai receber do usuario
tel = relefone que vai receber do usuario
push_back é um método de vetores que adiciona um novo elemento ao final do contêiner
.empty() é uma função membro de contêineres da Standard Template Library (STL), como std::vector, std::string e std::map, que verifica se o contêiner está vazi

   -=-=-=-=| FIM |-=-=-=-=*/
#include "cliente.h" // Inclui as declarações que estão em cliente.h
#include "../estadia/estadia.h"

// #extern void estadia();

// DEFINIÇÃO da variável global (aloca o espaço na memória)
std::vector<Cliente> listaDeClientes;

// DEFINIÇÃO do Construtor
Cliente::Cliente(int cod, const std::string &nom, const std::string &end, const std::string &tel)
    : codigo(cod), nome(nom), endereco(end), telefone(tel) {}

// DEFINIÇÃO da função de cadastro

void cliente()
{
    std::string nome, endereco, telefone;
    int hospedes;

    std::cout << "\n-=-| Cadastro de Cliente |-=-" << std::endl;

    std::cout << "*Informe o nome do cliente: ";
    std::cin >> nome;

    std::cout << "*Informe o endereco do cliente: ";
    std::cin >> endereco;

    std::cout << "*Informe o telefone do clinete: ";
    std::cin >> telefone;

    // Cadastro dos clientes
    cadastrarCliente(nome, endereco, telefone);
}

bool cadastrarCliente(const std::string &nome, const std::string &endereco, const std::string &telefone)
{
    static int proximoCodigo = 1;
    Cliente novoCliente(proximoCodigo, nome, endereco, telefone);
    listaDeClientes.push_back(novoCliente);
    proximoCodigo++;

    std::cout << "\n - Cliente '" << nome << "' cadastrado com sucesso!" << std::endl;
    std::cout << " - Codigo atribuido: " << novoCliente.getCodigo() << std::endl;

    return true;
}

// DEFINIÇÃO da função de listagem
void listarClientes()
{
    std::cout << "\n-=-| Lista de Clientes Cadastrados (" << listaDeClientes.size() << ") |-=-" << std::endl;
    if (listaDeClientes.empty())
    {
        std::cout << "*Nenhum cliente cadastrado.*" << std::endl;
        return;
    }
    for (const auto &cliente : listaDeClientes)
    {
        std::cout << "Codigo: " << cliente.getCodigo()
                  << ", Nome: " << cliente.getNome()
                  << ", Endereco: " << cliente.getEndereco()
                  << ", Telefone: " << cliente.getTelefone() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void buscarCliente()
{
    int codigo;

    if (listaDeClientes.empty())
    {
        std::cout << "*Nenhum cliente cadastrado.*" << std::endl;
        return;
    }

    std::cout << "*Informe o codigo do cliente: ";
    std::cin >> codigo;
   
    for (const auto &cliente : listaDeClientes)
    {
        if (cliente.getCodigo() == codigo)
        {
            std::cout << "Codigo: " << cliente.getCodigo()
                      << ", Nome: " << cliente.getNome()
                      << ", Endereco: " << cliente.getEndereco()
                      << ", Telefone: " << cliente.getTelefone() << std::endl;
        }
        else
        {
            std::cout << "*Cliente nao encontrado!*";
        }
    }
    std::cout << "\n------------------------------------------" << std::endl;
}