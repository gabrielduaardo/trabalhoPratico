// main.cpp
#include <iostream>

// Inclui as DECLARAÇÕES das classes e funções
// #include "cliente/cliente.h"
// #include "funcionario/funcionario.h"
// #include "quarto/quarto.h"
// #include "estadia/estadia.h"
// codigo para rodar o arquivo com tudo .h  g++ main.cpp cliente/cliente.cpp quarto/quarto.cpp estadia/estadia.cpp funcionario/funcionario.cpp -o hotel_app

#include "funcoes/database.cpp"
#include "cliente/cliente.cpp"
#include "funcionario/funcionario.cpp"
#include "quarto/quarto.cpp"
#include "estadia/estadia.cpp"
#include "funcoes/limparTela.cpp"

int main()
{
    carregarClientes();
    carregarFuncionario();
    carregarQuarto();
    carregarEstadias();

    int opcao;
    do
    {
        limparTela();
        // Apresentação do Menu
        std::cout << "\n===============================" << std::endl;
        std::cout << "        MENU PRINCIPAL         " << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "CLIENTES & FUNCIONARIOS" << std::endl;
        std::cout << "1. Cadastrar Novo Cliente" << std::endl;
        std::cout << "2. Listar Clientes" << std::endl;
        std::cout << "3. Buscar Cliente " << std::endl;
        std::cout << "4. Cadastrar Novo Funcionario" << std::endl;
        std::cout << "5. Listar Funcionario" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "QUARTOS & ESTADIAS" << std::endl;
        std::cout << "6. Cadastrar Novo Quarto" << std::endl;
        std::cout << "7. Listar Quartos" << std::endl;
        std::cout << "8. Liberar Quarto Manualmente" << std::endl;
        std::cout << "9. Cadastrar Estadia" << std::endl;
        std::cout << "10. Finalizar Estadia (Checkout)" << std::endl;
        std::cout << "11. Listar Todas as Estadias" << std::endl;
        std::cout << "12. Listar Estadias por Cliente (Busca)" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "0. Sair e Salvar Dados" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Escolha uma opcao: ";

        if (!(std::cin >> opcao))
        {
            // 1. Limpa o estado de erro do cin, permitindo novas operações de I/O.
            std::cin.clear();
            // 2. Limpa o buffer, jogando fora a letra que causou a falha.
            std::cin.ignore(10000, '\n');
            opcao = 100;
        }
        switch (opcao)
        {
        case 0: // Saindo
            std::cout << "\nSalvando dados e saindo do sistema. Ate mais!" << std::endl;
            salvarClientes();
            salvarFuncionario();
            salvarQuarto();
            salvarEstadias();
            break;

        case 1: // Chama a função que coleta dados e cadastra
            cliente();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n'); // Limpa até 10000 caracteres ou até o Enter residual
            std::cin.get();               // Espera o usuário apertar Enter para continuar
            salvarClientes();
            break;

        case 2: 
            listarClientes();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 3: 
            buscarCliente();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 4: // Cadastrar Funcionario
            funcionario();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            salvarFuncionario();
            break;

        case 5: 
            listarFuncionario();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 6: // Cadastrar Funcionario
            quarto();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            salvarQuarto();
            break;

        case 7: 
            listarQuartos();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 8: 
            liberarQuartoForce();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 9: 
            cadastrarEstadia();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 10: 
            finalizarEstadia();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 11: 
            listarEstadia();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 12: 
            listarEstadiaC();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        default:
            std::cout << "\nOpcao invalida. Tente novamente." << std::endl;
            break;
        }
    } while (opcao != 0);

    return 0;
}