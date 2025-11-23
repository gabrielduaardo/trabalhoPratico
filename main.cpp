// main.cpp
#include <iostream>

// Inclui as DECLARAÇÕES das classes e funções
#include "cliente/cliente.cpp"
#include "funcionario/funcionario.cpp"
#include "quarto/quarto.cpp"
#include "estadia/estadia.cpp"
#include "funcoes/limparTela.cpp"

int main()
{
    int opcao;
    do
    {   
        limparTela();
        // Apresentação do Menu
        std::cout << "\n===============================" << std::endl;
        std::cout << "         MENU PRINCIPAL        " << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "1. Cadastrar Novo Cliente" << std::endl;
        std::cout << "2. Listar Clientes" << std::endl;
        std::cout << "3. Cadastrar Novo Funcionario" << std::endl;
        std::cout << "4. Listar Funcionario" << std::endl;
        std::cout << "5. Cadastrar Novo Quarto" << std::endl;
        std::cout << "6. Listar Quartos" << std::endl;
        std::cout << "7. Cadastrar Estadia" << std::endl;
        std::cout << "8. Finalizar Estadia" << std::endl;
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
            std::cout << "\nSaindo do sistema. Ate mais!" << std::endl;
            break;

        case 1: // Chama a função que coleta dados e cadastra
            cliente();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n'); // Limpa até 10000 caracteres ou até o Enter residual
            std::cin.get();               // Espera o usuário apertar Enter para continuar
            break;

        case 2: // Lista os clientes cadastrados
            listarClientes();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n'); 
            std::cin.get();              
            break;

        case 3: // Cadastrar Funcionario
            funcionario();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 4: // Lista os clientes cadastrados
            listarFuncionario();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n'); 
            std::cin.get();               
            break;

        case 5: // Cadastrar Funcionario
            quarto();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;

        case 6: // Lista os clientes cadastrados
            listarQuartos();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n'); 
            std::cin.get();               
            break;

        case 7: // Lista os clientes cadastrados
            cadastrarEstadia();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.get();              
            break;

           case 8: // Lista os clientes cadastrados
            finalizarEstadia();
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