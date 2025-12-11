#include "cliente.h"
#include "../estadia/estadia.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <algorithm>

std::vector<Cliente> listaDeClientes;

Cliente::Cliente(int cod, const std::string &nom, const std::string &end, const std::string &tel)
    : codigo(cod), nome(nom), endereco(end), telefone(tel) {}

int proximoCodigoCliente = 1;

extern bool listarEstadiaC(int cod);

void salvarClientes()
{
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ofstream arquivo("data/Clientes.bin", std::ios::binary | std::ios::trunc);
    if (!arquivo.is_open())
    {
        std::cerr << "[ERRO] Falha ao salvar clientes.bin!\n";
        return;
    }

    for (const auto &c : listaDeClientes)
    {
        int cod = c.getCodigo();
        std::string nom = c.getNome();
        std::string end = c.getEndereco();
        std::string tel = c.getTelefone();

        arquivo.write(reinterpret_cast<const char *>(&cod), sizeof(cod));

        auto writeString = [&](const std::string &str)
        {
            size_t size = str.size();
            arquivo.write(reinterpret_cast<const char *>(&size), sizeof(size));
            arquivo.write(str.c_str(), size);
        };

        writeString(nom);
        writeString(end);
        writeString(tel);
    }
    arquivo.close();
}

void carregarClientes()
{
    std::ifstream arquivo("data/Clientes.bin", std::ios::binary);
    if (!arquivo.is_open())
    {
        std::cout << "[INFO] Nenhum arquivo binario encontrado." << std::endl;
        return;
    }

    listaDeClientes.clear();

    while (true)
    {
        int cod;
        if (!arquivo.read(reinterpret_cast<char *>(&cod), sizeof(cod)))
            break;

        auto readString = [&](std::string &str)
        {
            size_t size;
            if (!arquivo.read(reinterpret_cast<char *>(&size), sizeof(size)))
                return;
            str.resize(size);
            if (!arquivo.read(&str[0], size))
                return;
        };

        std::string nom, end, tel;
        readString(nom);
        readString(end);
        readString(tel);

        listaDeClientes.emplace_back(cod, nom, end, tel);

        if (cod >= proximoCodigoCliente)
            proximoCodigoCliente = cod + 1;
    }

    arquivo.close();
    std::cout << "[OK] Clientes carregados do binario. Proximo cod: " << proximoCodigoCliente << "\n";
}

bool cadastrarCliente(const std::string &nome, const std::string &endereco, const std::string &telefone)
{
    Cliente novoCliente(proximoCodigoCliente, nome, endereco, telefone);

    listaDeClientes.push_back(novoCliente);

    std::cout << "\n - Cliente '" << nome << "' cadastrado com sucesso!\n";
    std::cout << " - Codigo atribuido: " << proximoCodigoCliente << "\n";

    proximoCodigoCliente++;

    salvarClientes();

    return true;
}

void cliente()
{
    std::string nome, endereco, telefone;

    std::cout << "\n-=-| Cadastro de Cliente |-=-\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    auto isBlank = [](const std::string &s) {
        return s.empty() || std::all_of(s.begin(), s.end(), ::isspace);
    };

    do {
        std::cout << "*Informe o nome do cliente: ";
        std::getline(std::cin, nome);
        if (isBlank(nome)) {
            std::cout << "[ERRO] Nome nao pode ser vazio.\n";
        }
    } while (isBlank(nome));

    do {
        std::cout << "*Informe o endereco do cliente: ";
        std::getline(std::cin, endereco);
        if (isBlank(endereco)) {
            std::cout << "[ERRO] Endereco nao pode ser vazio.\n";
        }
    } while (isBlank(endereco));

    do {
        std::cout << "*Informe o telefone do cliente: ";
        std::getline(std::cin, telefone);
        if (isBlank(telefone)) {
            std::cout << "[ERRO] Telefone nao pode ser vazio.\n";
        }
    } while (isBlank(telefone));

    cadastrarCliente(nome, endereco, telefone);
}

void listarClientes()
{
    std::cout << "\n-=-| Lista de Clientes |-=-\n";

    if (listaDeClientes.empty())
    {
        std::cout << "*Nenhum cliente cadastrado.*\n";
        return;
    }

    for (const auto &c : listaDeClientes)
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
    
    if (!(std::cin >> codigo))
    {
        std::cout << "[ERRO] Entrada invalida. Tente novamente.\n";
        return;
    }

    bool encontrado = false;
    for (const auto &cliente : listaDeClientes)
    {
        if (cliente.getCodigo() == codigo)
        {
            std::cout << "\n-=-| DETALHES DO CLIENTE |-=-\n";
            std::cout << "Codigo: " << cliente.getCodigo() << std::endl;
            std::cout << "Nome: " << cliente.getNome() << std::endl;
            std::cout << "Endereco: " << cliente.getEndereco() << std::endl;
            std::cout << "Telefone: " << cliente.getTelefone() << std::endl;
            std::cout << "------------------------------------------\n";

            std::cout << "-=-| HISTORICO DE ESTADIAS |-=-\n";
            if (listarEstadiaC(codigo))
            {
                 std::cout << "------------------------------------------\n";
            }
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
        std::cout << "*Cliente nao encontrado!*";
}