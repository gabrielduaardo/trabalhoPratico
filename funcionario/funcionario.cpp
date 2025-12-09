#include "funcionario.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

std::vector<Funcionario> listaDeFuncionario;
static int proximoCodigo = 1;

Funcionario::Funcionario(int cod, const std::string &nom, const std::string &tel, const std::string &carg, double sal)
    : codigo(cod), nome(nom), telefone(tel), cargo(carg), salario(sal) {}

// ----------------------------------------------------
// SALVAR EM BINÁRIO REAL
// ----------------------------------------------------
void salvarFuncionario() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ofstream arquivo("data/Funcionario.bin", std::ios::binary);
    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Falha ao abrir Funcionario.bin para escrita!\n";
        return;
    }

    for (const auto& f : listaDeFuncionario) {
        // 1. Salvar dados numéricos simples
        int cod = f.getCodigo();
        double sal = f.getSalario();
        arquivo.write(reinterpret_cast<const char*>(&cod), sizeof(cod));
        arquivo.write(reinterpret_cast<const char*>(&sal), sizeof(sal));

        // 2. Lambda para salvar strings (Tamanho + Caracteres)
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

// ----------------------------------------------------
// CARREGAR DE BINÁRIO REAL
// ----------------------------------------------------
void carregarFuncionario() {
    std::ifstream arquivo("data/Funcionario.bin", std::ios::binary);
    if (!arquivo.is_open()) return;

    listaDeFuncionario.clear();

    while (arquivo.peek() != EOF) {
        int cod;
        double sal;

        // 1. Ler dados numéricos
        arquivo.read(reinterpret_cast<char*>(&cod), sizeof(cod));
        arquivo.read(reinterpret_cast<char*>(&sal), sizeof(sal));

        // 2. Lambda para ler strings (Lê o tamanho e depois os caracteres)
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
    salvarFuncionario(); // Salva automaticamente
    return true;
}

void funcionario() {
    std::string nome, telefone, cargo;
    double salario;

    std::cout << "\n-=-| Cadastro de Funcionario |-=-" << std::endl;
    std::cout << "*Informe o nome: "; std::cin >> nome;
    std::cout << "*Informe o telefone: "; std::cin >> telefone;
    std::cout << "*Informe o cargo: "; std::cin >> cargo;
    std::cout << "*Informe o salario: "; std::cin >> salario;

    cadastrarFuncionario(nome, telefone, cargo, salario);
}

void listarFuncionario() {
    std::cout << "\n-=-| Lista de Funcionarios |-=-" << std::endl;
    if (listaDeFuncionario.empty()) {
        std::cout << "*Vazio*" << std::endl;
        return;
    }
    for (const auto &f : listaDeFuncionario) {
        std::cout << "[" << f.getCodigo() << "] " << f.getNome() << " - " << f.getCargo() << " | R$ " << f.getSalario() << std::endl;
    }
}