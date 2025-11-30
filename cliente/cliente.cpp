#include "cliente.h"
#include "../estadia/estadia.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

// Variável global
std::vector<Cliente> listaDeClientes;

// Construtor
Cliente::Cliente(int cod, const std::string &nom, const std::string &end, const std::string &tel)
    : codigo(cod), nome(nom), endereco(end), telefone(tel) {}

int proximoCodigoCliente = 1; // autoincremento real baseado no CSV

// ----------------------------------------------------
// CARREGAR CSV
// ----------------------------------------------------
void carregarClientes()
{
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ifstream arquivo("data/clientes.csv");
    if (!arquivo.is_open())
    {
        std::cout << "[INFO] Nenhum arquivo clientes.csv encontrado. Criando novo..." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha))
    {
        std::stringstream ss(linha);
        std::string campo;

        int codigo;
        std::string nome, endereco, telefone;

        std::getline(ss, campo, ',');
        codigo = std::stoi(campo);

        std::getline(ss, nome, ',');
        std::getline(ss, endereco, ',');
        std::getline(ss, telefone, ',');

        listaDeClientes.emplace_back(codigo, nome, endereco, telefone);

        if (codigo >= proximoCodigoCliente)
            proximoCodigoCliente = codigo + 1;
    }

    arquivo.close();
    std::cout << "[OK] Clientes carregados. Proximo codigo = " << proximoCodigoCliente << "\n";
}

// ----------------------------------------------------
// SALVAR CSV
// ----------------------------------------------------
void salvarClientes()
{
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ofstream arquivo("data/clientes.csv");
    if (!arquivo.is_open())
    {
        std::cerr << "[ERRO] Falha ao salvar clientes.csv!\n";
        return;
    }

    for (const auto& c : listaDeClientes)
    {
        arquivo << c.getCodigo() << ","
                << c.getNome() << ","
                << c.getEndereco() << ","
                << c.getTelefone() << "\n";
    }

    arquivo.close();
}

// ----------------------------------------------------
// FUNÇÃO CADASTRAR
// ----------------------------------------------------
bool cadastrarCliente(const std::string &nome, const std::string &endereco, const std::string &telefone)
{
    Cliente novoCliente(proximoCodigoCliente, nome, endereco, telefone);

    listaDeClientes.push_back(novoCliente);

    std::cout << "\n - Cliente '" << nome << "' cadastrado com sucesso!\n";
    std::cout << " - Codigo atribuido: " << proximoCodigoCliente << "\n";

    proximoCodigoCliente++;

    salvarClientes(); // salva automaticamente após adicionar

    return true;
}

// ----------------------------------------------------
// TELA PARA DIGITAR CLIENTE
// ----------------------------------------------------
void cliente()
{
    std::string nome, endereco, telefone;

    std::cout << "\n-=-| Cadastro de Cliente |-=-\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "*Informe o nome do cliente: ";
    std::getline(std::cin, nome);

    std::cout << "*Informe o endereco do cliente: ";
    std::getline(std::cin, endereco);

    std::cout << "*Informe o telefone do cliente: ";
    std::getline(std::cin, telefone);

    cadastrarCliente(nome, endereco, telefone);
}

// ----------------------------------------------------
// LISTAR CLIENTES
// ----------------------------------------------------
void listarClientes()
{
    std::cout << "\n-=-| Lista de Clientes |-=-\n";

    if (listaDeClientes.empty())
    {
        std::cout << "*Nenhum cliente cadastrado.*\n";
        return;
    }

    for (auto &c : listaDeClientes)
    {
        std::cout << "Codigo: " << c.getCodigo()
                  << ", Nome: " << c.getNome()
                  << ", Endereco: " << c.getEndereco()
                  << ", Telefone: " << c.getTelefone() << "\n";
    }

    std::cout << "------------------------------------------\n";
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

    bool encontrado = false;
    for (const auto &cliente : listaDeClientes)
    {
        if (cliente.getCodigo() == codigo)
        {
            std::cout << "Codigo: " << cliente.getCodigo()
                      << ", Nome: " << cliente.getNome()
                      << ", Endereco: " << cliente.getEndereco()
                      << ", Telefone: " << cliente.getTelefone() << std::endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
        std::cout << "*Cliente nao encontrado!*";

    std::cout << "\n------------------------------------------" << std::endl;
}
