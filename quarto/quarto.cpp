#include "quarto.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

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
// CARREGAR BINÁRIO (Atualizado)
// ----------------------------------------------------
void carregarQuarto() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    // Abre o arquivo em modo binário
    std::ifstream arquivo("data/Quarto.bin", std::ios::binary);
    
    if (!arquivo.is_open()) {
        std::cout << "[INFO] Nenhum arquivo Quarto.bin encontrado." << std::endl;
        return;
    }

    listaDeQuarto.clear();
    
    // Objeto temporário para ler os dados
    // Inicializamos com valores padrão pois o read vai sobrescrever a memória
    Quarto temp(0, 0, 0.0, 0);

    // Lê o arquivo bloco por bloco do tamanho da classe Quarto
    while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Quarto))) {
        listaDeQuarto.push_back(temp);
    }

    arquivo.close();
    std::cout << "[OK] " << listaDeQuarto.size() << " quartos carregados com sucesso.\n";
}

// ----------------------------------------------------
// SALVAR BINÁRIO (Atualizado)
// ----------------------------------------------------
void salvarQuarto() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    // Abre o arquivo em modo binário e trunc (sobrescreve)
    std::ofstream arquivo("data/Quarto.bin", std::ios::binary | std::ios::trunc);
    
    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Falha ao abrir arquivo para salvar!\n";
        return;
    }

    // Grava cada objeto da lista diretamente da memória
    for (const auto& q : listaDeQuarto) {
        arquivo.write(reinterpret_cast<const char*>(&q), sizeof(Quarto));
    }

    arquivo.close();
    std::cout << "[OK] Dados salvos em binario.\n";
}

// ----------------------------------------------------
// RESTANTE DAS FUNÇÕES (Mantidas)
// ----------------------------------------------------

bool cadastrarQuarto(int num, int hospedes, double diaria, int status) {
    Quarto novoQuarto(num, hospedes, diaria, status);
    listaDeQuarto.push_back(novoQuarto);
    
    salvarQuarto(); // Salva automaticamente após cadastrar
    std::cout << "\n - Quarto " << num << " cadastrado com sucesso!" << std::endl;
    return true;
}

void quarto() {
    int num, hospedes, status;
    double diaria;

    std::cout << "\n-=-| Cadastro de Quartos |-=-" << std::endl;
    std::cout << "*Informe o numero do quarto: ";
    std::cin >> num;

    while (numeroExiste(num)) {
        std::cout << "*Esse quarto ja existe! Digite outro: ";
        std::cin >> num;
    }

    std::cout << "*Hospedes: "; std::cin >> hospedes;
    std::cout << "*Valor diaria: "; std::cin >> diaria;

    do {
        std::cout << "*Status (1-Disponivel, 2-Ocupado): ";
        std::cin >> status;
    } while (status != 1 && status != 2);

    cadastrarQuarto(num, hospedes, diaria, status);
}

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
                  << " | Diaria: " << q.getValorDaDiaria()
                  << " | Status: " << statusStr << std::endl;
    }
}