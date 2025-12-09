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

void salvarClientes() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    // Abre em modo binário
    std::ofstream arquivo("data/Clientes.bin", std::ios::binary | std::ios::trunc);
    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Falha ao salvar clientes.bin!\n";
        return;
    }

    for (const auto& c : listaDeClientes) {
        int cod = c.getCodigo();
        std::string nom = c.getNome();
        std::string end = c.getEndereco();
        std::string tel = c.getTelefone();

        // 1. Grava o Código (Inteiro)
        arquivo.write(reinterpret_cast<const char*>(&cod), sizeof(cod));

        // Lambda para gravar strings em binário: [Tamanho][Caracteres]
        auto writeString = [&](const std::string& str) {
            size_t size = str.size();
            arquivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
            arquivo.write(str.c_str(), size);
        };

        writeString(nom);
        writeString(end);
        writeString(tel);
    }
    arquivo.close();
}

// ----------------------------------------------------
// CARREGAR BINÁRIO
// ----------------------------------------------------
void carregarClientes() {
    std::ifstream arquivo("data/Clientes.bin", std::ios::binary);
    if (!arquivo.is_open()) {
        std::cout << "[INFO] Nenhum arquivo binario encontrado." << std::endl;
        return;
    }

    listaDeClientes.clear();

    while (true) {
        int cod;
        // Tenta ler o código. Se falhar, fim do arquivo.
        if (!arquivo.read(reinterpret_cast<char*>(&cod), sizeof(cod))) break;

        // Lambda para ler strings: Lê o tamanho e depois redimensiona a string
        auto readString = [&](std::string& str) {
            size_t size;
            arquivo.read(reinterpret_cast<char*>(&size), sizeof(size));
            str.resize(size);
            arquivo.read(&str[0], size);
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
