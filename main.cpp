// main.cpp
#include <iostream>

// Inclui as DECLARAÇÕES das classes e funções
#include "cliente/cliente.cpp"
// #include "cliente.h"

int main() {
    int i = 0;
    std::string nome;
    std::string endereco;
    std::string telefone;

    std::cout << " SEJA BEM VINDO! " << std::endl;
    std::cout << " ESCOLHA A OPCAO DESEJADA! " << std::endl;

    while (i < 1 || i > 2) {
        std::cout << "Escolha uma das opcoes abaixo" << std::endl;

        std::cout << " 1 - Realizar o seu cadastro " << std::endl;
        std::cout << " 2 - Para sair " << std::endl;
        std::cin >> i;
    }

    if (i == 1) {
        
        std::cout << " Informe o seu nome " << std::endl;
        std::cin >> nome;

        std::cout << " Informe o seu endereco " << std::endl;
        std::cin >> endereco;

        std::cout << " Informe o seu numero de telefone " << std::endl;
        std::cin >> telefone;
        // Cadastro dos clientes
        cadastrarCliente("Gabriel Silva", "Rua A, 10", "3199991111");
        cadastrarCliente("Mariana Santos", "Av. B, 200", "3198882222");
        cadastrarCliente("Pedro Oliveira", "Travessa C, 5", "3197773333");
        cadastrarCliente(nome, endereco, telefone);

        // Listagem
        listarClientes();
    } else if (i == 2) {
        std::cout << " Saindo... " << std::endl;
    }
    return 0;
}