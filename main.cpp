// main.cpp
#include <iostream>

// Inclui as DECLARAÇÕES das classes e funções
#include "cliente/cliente.cpp"

#include <chrono> // Para usar std::chrono::seconds
#include <thread> // Para usar std::this_thread::sleep_for

// void delay(int segundos) {
//     // Suspende a execução da thread atual (o seu programa)
//     // pelo número de segundos especificado.
//     std::this_thread::sleep_for(std::chrono::seconds(segundos));
// }
void limparTela()
{
   
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void cliente()
{
    std::string nome, endereco, telefone;

    std::cout << "\n--- Cadastro de Cliente ---" << std::endl;

    std::cout << " Informe o seu nome " << std::endl;
    std::cin >> nome;

    std::cout << " Informe o seu endereco " << std::endl;
    std::cin >> endereco;

    std::cout << " Informe o seu numero de telefone " << std::endl;
    std::cin >> telefone;
    // Cadastro dos clientes
    cadastrarCliente(nome, endereco, telefone);
}

int main()
{
    int i;

    do
    {
        // Apresentação do Menu
        std::cout << "\n===============================" << std::endl;
        std::cout << "         MENU PRINCIPAL        " << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "1. Cadastrar Novo Cliente" << std::endl;
        std::cout << "2. Listar Clientes" << std::endl;
        std::cout << "3. Sair" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Escolha uma opcao: ";

        if (!(std::cin >> i)) {
            // 1. Limpa o estado de erro do cin, permitindo novas operações de I/O.
            std::cin.clear();     
            // 2. Limpa o buffer, jogando fora a letra que causou a falha.
            std::cin.ignore(10000, '\n'); 
            i = 0; 
        }
        switch (i)
        {
        case 1: // Chama a função que coleta dados e cadastra
            cliente();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
             std::cin.ignore(10000, '\n'); // Limpa até 10000 caracteres ou até o Enter residual
            std::cin.get(); // Espera o usuário apertar Enter para continuar

            break;
        case 2: // Lista os clientes cadastrados
            listarClientes();
            std::cout << "\n--- Pressione ENTER para voltar ao menu ---" << std::endl;
             std::cin.ignore(10000, '\n'); // Limpa até 10000 caracteres ou até o Enter residual
            std::cin.get(); // Espera o usuário apertar Enter para continuar
            break;
        case 3: // Saindo
            std::cout << "\nSaindo do sistema. Ate mais!" << std::endl;
            //  delay(2);
            break;
        default:
            std::cout << "\nOpcao invalida. Tente novamente." << std::endl;
            break;
        }
        limparTela();
    } while (i != 3);

    return 0;
}