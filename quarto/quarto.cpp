#include "quarto.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <limits>
std::vector<Quarto> listaDeQuarto;

// Construtor
Quarto::Quarto(int num, int hospedes, double diaria, int status)
    : numeroDoQuarto(num), quantidadeDeHospedes(hospedes), valorDaDiaria(diaria), status(status) {}

bool numeroExiste(int num) {
    for (const auto &quarto : listaDeQuarto) {
        if (quarto.getNumeroDoQuarto() == num) return true;
    }
    return false;
}

// ----------------------------------------------------
// CARREGAR BINÁRIO 
// ----------------------------------------------------
void carregarQuarto() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ifstream arquivo("data/Quarto.bin", std::ios::binary);
    
    if (!arquivo.is_open()) {
        std::cout << "[INFO] Nenhum arquivo Quarto.bin encontrado." << std::endl;
        return;
    }

    listaDeQuarto.clear();
    
    Quarto temp(0, 0, 0.0, 0);

    while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Quarto))) {
        listaDeQuarto.push_back(temp);
    }

    arquivo.close();
    std::cout << "[OK] " << listaDeQuarto.size() << " quartos carregados com sucesso.\n";
}

// ----------------------------------------------------
// SALVAR BINÁRIO 
// ----------------------------------------------------
void salvarQuarto() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    std::ofstream arquivo("data/Quarto.bin", std::ios::binary | std::ios::trunc);
    
    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Falha ao abrir arquivo para salvar!\n";
        return;
    }

    for (const auto& q : listaDeQuarto) {
        arquivo.write(reinterpret_cast<const char*>(&q), sizeof(Quarto));
    }

    arquivo.close();
    std::cout << "[OK] Dados salvos em binario.\n";
}

// ----------------------------------------------------
// FUNÇÃO CADASTRAR
// ----------------------------------------------------

bool cadastrarQuarto(int num, int hospedes, double diaria, int status) {
    Quarto novoQuarto(num, hospedes, diaria, status);
    listaDeQuarto.push_back(novoQuarto);
    
    salvarQuarto(); 
    std::cout << "\n - Quarto " << num << " cadastrado com sucesso!" << std::endl;
    return true;
}

// ----------------------------------------------------
// TELA DE CADASTRO DE QUARTO
// ----------------------------------------------------

void quarto() {
    int num, hospedes, status;
    double diaria;

    // Limpa o buffer antes de ler o primeiro número, caso a chamada venha do menu.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n-=-| Cadastro de Quartos |-=-" << std::endl;

    do {
        std::cout << "*Informe o numero do quarto: ";
  
        if (!(std::cin >> num) || num <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERRO] O numero do quarto deve ser um inteiro positivo.\n";
            num = -1; // Força a repetição do loop
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        while (numeroExiste(num)) {
            std::cout << "*Esse quarto ja existe! Digite outro: ";
            std::cin >> num;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
    } while (num <= 0);

    do {
        std::cout << "*Hospedes: "; 
        if (!(std::cin >> hospedes) || hospedes < 1) {
            std::cout << "[ERRO] A capacidade de hospedes deve ser 1 ou mais.\n";
            hospedes = 0; 
            continue;
        }

    } while (hospedes < 1);

    do {
        std::cout << "*Valor diaria (minimo R$ 1.00): "; 
        if (!(std::cin >> diaria) || diaria < 1.0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERRO] O valor da diaria deve ser R$ 1.00 ou mais.\n";
            diaria = 0.0; 
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (diaria < 1.0);
   
    do {
        std::cout << "*Status (1-Disponivel, 2-Ocupado): ";
        if (!(std::cin >> status) || (status != 1 && status != 2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERRO] Status invalido. Escolha 1 ou 2.\n";
            status = 0;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (status != 1 && status != 2);

    cadastrarQuarto(num, hospedes, diaria, status);
}

// ----------------------------------------------------
// LISTAR QUARTOS 
// ----------------------------------------------------
void listarQuartos() {
    std::cout << "\n-=-| Quartos Cadastrados |-=-" << std::endl;
    if (listaDeQuarto.empty()) {
        std::cout << "Nenhum quarto na lista." << std::endl;
        return;
    }

    for (const auto &q : listaDeQuarto) {
        std::string statusStr = (q.getStatus() == 1) ? "Disponivel" : "Ocupado";
        std::cout << "Num: " << q.getNumeroDoQuarto() 
                  << " | Hosp: " << q.getQuantidadeDeHospedes()
                  << " | Diaria: R$" << std::fixed << std::setprecision(2) << q.getValorDaDiaria()
                  << " | Status: " << statusStr << std::endl;
    }
}