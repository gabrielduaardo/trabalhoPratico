#include "funcionario.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

std::vector<Funcionario> listaDeFuncionario;
static int proximoCodigo = 1;

Funcionario::Funcionario(int cod, const std::string &nom, const std::string &tel, const std::string &carg, double sal)
    : codigo(cod), nome(nom), telefone(tel), cargo(carg), salario(sal) {}

void salvarFuncionario() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ofstream arquivo("data/Funcionario.bin", std::ios::binary);
    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Falha ao abrir Funcionario.bin para escrita!\n";
        return;
    }

    for (const auto& f : listaDeFuncionario) {
        int cod = f.getCodigo();
        double sal = f.getSalario();
        arquivo.write(reinterpret_cast<const char*>(&cod), sizeof(cod));
        arquivo.write(reinterpret_cast<const char*>(&sal), sizeof(sal));

        auto salvarStr = [&](const std::string& str) {
            size_t tam = str.size();
            arquivo.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
            arquivo.write(str.c_str(), tam);
        };

        salvarStr(f.getNome());
        salvarStr(f.getTelefone());
        salvarStr(f.getCargo());
    }

    arquivo.close();
}

void carregarFuncionario() {
    std::ifstream arquivo("data/Funcionario.bin", std::ios::binary);
    if (!arquivo.is_open()) return;

    listaDeFuncionario.clear();

    while (arquivo.peek() != EOF) {
        int cod;
        double sal;

        arquivo.read(reinterpret_cast<char*>(&cod), sizeof(cod));
        arquivo.read(reinterpret_cast<char*>(&sal), sizeof(sal));

        auto lerStr = [&](std::string& str) {
            size_t tam;
            arquivo.read(reinterpret_cast<char*>(&tam), sizeof(tam));
            str.resize(tam);
            arquivo.read(&str[0], tam);
        };

        std::string nome, tel, cargo;
        lerStr(nome);
        lerStr(tel);
        lerStr(cargo);

        listaDeFuncionario.emplace_back(cod, nome, tel, cargo, sal);

        if (cod >= proximoCodigo) proximoCodigo = cod + 1;
    }

    arquivo.close();
}

bool cadastrarFuncionario(const std::string &nome, const std::string &telefone, const std::string &cargo, double salario) {
    Funcionario novoFuncionario(proximoCodigo++, nome, telefone, cargo, salario);
    listaDeFuncionario.push_back(novoFuncionario);

    std::cout << "\n - Funcionario '" << nome << "' cadastrado com sucesso!" << std::endl;
    salvarFuncionario();
    return true;
}

void funcionario() {
    std::string nome, telefone, cargo;
    double salario;

    std::cout << "\n-=-| Cadastro de Funcionario |-=-" << std::endl;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto isBlank = [](const std::string &s) {
        return s.empty() || std::all_of(s.begin(), s.end(), ::isspace);
    };

    do {
        std::cout << "*Informe o nome: ";
        std::getline(std::cin, nome);
        if (isBlank(nome)) {
            std::cout << "[ERRO] Nome nao pode ser vazio.\n";
        }
    } while (isBlank(nome));

    do {
        std::cout << "*Informe o telefone: ";
        std::getline(std::cin, telefone);
        if (isBlank(telefone)) {
            std::cout << "[ERRO] Telefone nao pode ser vazio.\n";
        }
    } while (isBlank(telefone));
    
    do {
        std::cout << "*Informe o cargo: ";
        std::getline(std::cin, cargo);
        if (isBlank(cargo)) {
            std::cout << "[ERRO] Cargo nao pode ser vazio.\n";
        }
    } while (isBlank(cargo));

    while (true) {
        std::cout << "*Informe o salario: ";
        if (!(std::cin >> salario)) {
            std::cout << "[ERRO] Salario invalido. Use apenas numeros.\n";
        } else {
            
            if (salario < 0) {
                std::cout << "[ERRO] Salario nao pode ser menor que zero.\n";
            } else {
                break; 
            }
        }
    }

    cadastrarFuncionario(nome, telefone, cargo, salario);
}

void listarFuncionario() {
    std::cout << "\n-=-| Lista de Funcionarios |-=-" << std::endl;
    if (listaDeFuncionario.empty()) {
        std::cout << "Nenhum funcionario cadastrado" << std::endl;
        return;
    }
    
    for (const auto &f : listaDeFuncionario) {
        std::cout << "Codigo: " << f.getCodigo()
                  << ", Nome: " << f.getNome()
                  << ", Cargo: " << f.getCargo()
                  << ", Telefone: " << f.getTelefone()
                  << ", Salario: R$ " << std::fixed << std::setprecision(2) << f.getSalario() << std::endl;
    }
    std::cout << "------------------------------------------\n";
}