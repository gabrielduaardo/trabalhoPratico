#include "../cliente/cliente.h"
#include "../Quarto/Quarto.h"
#include "estadia.h" // Seus includes das classes
#include <iostream>
#include <vector>
#include <algorithm>

extern std::vector<Quarto> listaDeQuarto;
extern std::vector<Cliente> listaDeClientes;
std::vector<Estadia> listaDeEstadias;

// 🚨 NOVO NOME: Declarações das funções de cadastro que estão em outros arquivos
extern void cliente(); // Função para cadastrar o Cliente
extern void quarto();  // Função para cadastrar o Quarto

int proximoCodEstadia = 1;

// Função para buscar um Quarto pelo número
Quarto *buscarQuarto(int numQuarto)
{
    auto it = std::find_if(listaDeQuarto.begin(), listaDeQuarto.end(),
                           [numQuarto](Quarto &q)
                           { return q.getNumeroDoQuarto() == numQuarto; });

    if (it != listaDeQuarto.end())
        return &(*it);
    return nullptr;
}

Cliente *buscarCliente(int codCliente)
{
    auto it = std::find_if(listaDeClientes.begin(), listaDeClientes.end(),
                           [codCliente](const Cliente &c)
                           { return c.getCodigo() == codCliente; });

    if (it != listaDeClientes.end())
        return &(*it);
    return nullptr;
}

// Função para buscar uma Estadia Ativa pelo número do Quarto
Estadia *buscarEstadiaAtiva(int numQuarto)
{
    // Para simplificar, consideramos a estadia a mais recente para aquele Quarto
    auto it = std::find_if(listaDeEstadias.rbegin(), listaDeEstadias.rend(), // Busca reversa para pegar a mais recente
                           [numQuarto](const Estadia &e)
                           {
                               return e.getNumeroDoQuarto() == numQuarto;
                           });

    if (it != listaDeEstadias.rend())
    {
        return &(*it);
    }
    return nullptr;
}

// --- 1. REGISTRAR ESTADIA (CHECK-IN) ---

void cadastrarEstadia()
{
    int codCliente;
    int numQuarto;
    std::string dataEntrada, dataSaida;
    int diarias;
    char resposta = ' ';

    std::cout << "\n-=-| REGISTRAR NOVA ESTADIA |-=-" << std::endl;

    Cliente *Cliente = nullptr;
    Quarto *Quarto = nullptr;

    // --- A. VALIDAÇÃO E BUSCA DO CLIENTE ---
    do
    {
        std::cout << "\n*Digite o codigo do cliente: ";
        if (!(std::cin >> codCliente))
            return;

        Cliente = buscarCliente(codCliente);

        if (Cliente == nullptr)
        {
            std::cout << " *ERRO: Cliente de codigo " << codCliente << " nao encontrado.*" << std::endl;
            std::cout << "*Deseja cadastrar um novo cliente (S/N)? ";
            std::cin >> resposta;

            if (std::toupper(resposta) == 'S')
            {
                cliente(); // ⬅️ CHAMADA RENOMEADA
                std::cout << "\nCliente cadastrado! Digite o codigo dele novamente para confirmar a estadia." << std::endl;
                // Resposta 'S' irá fazer o loop rodar novamente para o usuário digitar o novo código.
            }
            else if (std::toupper(resposta) != 'N')
            {
                std::cout << "*Opcao invalida. Digite S ou N.*" << std::endl;
            }
        }
    } while (Cliente == nullptr && std::toupper(resposta) != 'N');

    if (Cliente == nullptr)
    {
        std::cout << "Operacao de cliente cancelada." << std::endl;
        return;
    }

    // --- B. VALIDAÇÃO E BUSCA DO QUARTO ---
    resposta = ' '; // Reseta a resposta
    do
    {
        std::cout << "\n*Digite o numero do Quarto: ";
        if (!(std::cin >> numQuarto))
            return;

        Quarto = buscarQuarto(numQuarto);

        if (Quarto == nullptr)
        {
            std::cout << "*ERRO: Quarto numero " << numQuarto << " nao encontrado.*" << std::endl;
            std::cout << "*Deseja cadastrar um novo quarto (S/N)? ";
            std::cin >> resposta;

            if (std::toupper(resposta) == 'S')
            {
                quarto(); // ⬅️ CHAMADA RENOMEADA
                std::cout << "\nQuarto cadastrado! Digite o numero dele novamente para confirmar a estadia." << std::endl;
            }
            else if (std::toupper(resposta) != 'N')
            {
                std::cout << "*Opcao invalida. Digite S ou N.*" << std::endl;
            }
        }
        else if (Quarto->getStatus() != 1)
        { // 1 = Disponível
            // Validação de Status
            std::cout << " ERRO: Quarto " << numQuarto << " NAO esta disponivel (Status: " << Quarto->getStatus() << ")." << std::endl;
            std::cout << "Por favor, digite outro numero de Quarto." << std::endl;
            Quarto = nullptr; // Força o loop a continuar
        }

    } while (Quarto == nullptr && std::toupper(resposta) != 'N');

    if (Quarto == nullptr)
    {
        std::cout << "Operacao de Quarto cancelada." << std::endl;
        return;
    }

    // --- C. INPUT DE DETALHES DA ESTADIA ---
    std::cout << "\n--- DETALHES DA ESTADIA ---" << std::endl;
    std::cout << "Cliente: " << Cliente->getNome() << " | Quarto: " << Quarto->getNumeroDoQuarto() << std::endl;

    std::cout << "*Digite a quantidade de diarias: ";
    if (!(std::cin >> diarias))
        return;
    std::cout << "*Digite a data de entrada (DD/MM/AAAA): ";
    std::cin >> dataEntrada;
    std::cout << "*Digite a data de saida (DD/MM/AAAA): ";
    std::cin >> dataSaida;

    // --- D. AÇÃO: CRIAR ESTADIA E ATUALIZAR QUARTO ---
    Estadia novaEstadia(proximoCodEstadia++, dataEntrada, dataSaida, diarias,
                        Cliente->getCodigo(), Quarto->getNumeroDoQuarto());

    listaDeEstadias.push_back(novaEstadia);
    Quarto->setStatus(2); // 2 = Ocupado

    std::cout << "\n - Estadia '" << novaEstadia.getCodigoDaEstadia() << "' cadastrada com sucesso!" << std::endl;
    std::cout << " - Cliente: " << Cliente->getNome() << std::endl;
    std::cout << " - Quarto " << Quarto->getNumeroDoQuarto() << "." << std::endl;
}

// --- 2. FINALIZAR ESTADIA (CHECK-OUT) ---
void finalizarEstadia()
{
    int numQuarto;

    std::cout << "\n-=-| FINALIZAR ESTADIA (CHECKOUT) |-=-" << std::endl;
    std::cout << "*Digite o numero do Quarto a ser liberado: ";
    if (!(std::cin >> numQuarto))
        return;

    Quarto *Quarto = buscarQuarto(numQuarto);
    Estadia *estadia = buscarEstadiaAtiva(numQuarto);

    if (Quarto == nullptr)
    {
        std::cout << "*ERRO: Quarto " << numQuarto << " nao encontrado.*" << std::endl;
        return;
    }

    if (Quarto->getStatus() != 2) // 2 = Ocupado
    {
        std::cout << "*ERRO: Quarto " << numQuarto << " nao esta ocupado. Status atual: " << Quarto->getStatus() << ".*" << std::endl;
        return;
    }

    // Libera o Quarto
    Quarto->setStatus(1); // 1 = Disponivel

    std::cout << "\n CHECKOUT CONCLUIDO!" << std::endl;
    std::cout << "Quarto " << Quarto->getNumeroDoQuarto() << " marcado como **DISPONIVEL**." << std::endl;

    if (estadia)
    {
        double valorDiaria = Quarto->getValorDaDiaria();
        int diarias = estadia->getQuantidadeDeDiarias();
        double valorTotal = (double)diarias * valorDiaria;

        std::cout << "-=-| RESUMO DA CONTA |-=-" << std::endl;
        std::cout << " - Estadia Cod: " << estadia->getCodigoDaEstadia() << std::endl;
        std::cout << " - Diarias: " << diarias << " x R$ " << valorDiaria << std::endl;
        std::cout << "**VALOR TOTAL A PAGAR: R$ " << valorTotal << "**" << std::endl;
    }
    else
    {
        std::cout << "Quarto liberado. Nao foi encontrado registro de estadia ativa." << std::endl;
    }
}