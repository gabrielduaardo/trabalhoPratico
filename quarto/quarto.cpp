/*
num = numero do quarto
hospedes = quantidade de hospedes
diaria = valor da diaria
status = status do quarto (ocupado, disponivel)
*/

#include "quarto.h"
std::vector<Quarto> listaDeQuarto;

// Construtor
Quarto::Quarto(int num, int hospedes, double diaria, const int status)
    : numeroDoQuarto(num), quantidadeDeHospedes(hospedes), valorDaDiaria(diaria), status(status) {}

bool numeroExiste(int num)
{
    for (const auto &quarto : listaDeQuarto)
    {
        if (quarto.getNumeroDoQuarto() == num)
        {
            return true;
        }
    }
    return false;
}

bool cadastrarQuarto(int num, int hospedes, double diaria, int status)
{
    std::string opcao;

    switch (status)
    {
    case 1:
        opcao = "disponivel";
        break;

    case 2:
        opcao = "ocupado";
        break;

        // case 3:
        //     opcao = "manutencao";
        //     break;

        // case 4:
        //     opcao = "limpeza";
        //     break;

    default:
        std::cout << "Opcao invalida!\n";
        opcao = "desconhecido";
        break;
    }

    Quarto novoQuarto(num, hospedes, diaria, status);

    listaDeQuarto.push_back(novoQuarto);

    std::cout << "\n - Quarto '" << num << "' Cadastrado com sucesso!" << std::endl;
    std::cout << " - Quantidade de hospedes: " << hospedes << std::endl;
    std::cout << " - Valor da diaria: " << diaria << std::endl;
    std::cout << " - Status: " << opcao << std::endl;

    return true;
}

void quarto()
{
    int num, hospedes, status;
    float diaria;

    std::cout << "\n-=-| Cadastro de Quartos |-=-" << std::endl;

    std::cout << "*Informe o numero do quarto:  ";
    std::cin >> num;

    while (numeroExiste(num))
    {
        std::cout << "*Esse quarto ja esta cadastrado! \n Digite outro: ";
        std::cin >> num;
    }

    std::cout << "*Informe a quantidade de hospedes do quarto: ";
    std::cin >> hospedes;

    std::cout << "*Informe o valor da diaria do quarto: ";
    std::cin >> diaria;

    do
    {
        std::cout << "*Informe o status do quarto  ";
        std::cout << " 1 - Disponivel "
                  << " | 2 - Ocupado: ";
        std::cin >> status;
    } while (status != 1 && status != 2);

    cadastrarQuarto(num, hospedes, diaria, status);
}

void listarQuartos()
{

    std::cout << "\n-=-| Lista de Quartos Cadastrados ( " << listaDeQuarto.size() << " ) |-=-" << std::endl;

    if (listaDeQuarto.empty())
    {
        std::cout << "*Nenhum quarto cadastrado.* " << std::endl;
        return;
    }

    for (const auto &quarto : listaDeQuarto)
    {
        std::string opcao;

        switch (quarto.getStatus())
        {
        case 1:
            opcao = "Disponivel";
            break;

        case 2:
            opcao = "Ocupado";
            break;

            // case 3:
            //     opcao = "manutencao";
            //     break;

            // case 4:
            //     opcao = "limpeza";
            //     break;

        default:
            std::cout << "Opcao invalida!\n";
            opcao = "desconhecido";
            break;
        }
        std::cout << "Numero: " << quarto.getNumeroDoQuarto()
                  << ", Quantidade de hospedes: " << quarto.getQuantidadeDeHospedes()
                  << ", Valor da diaria: " << quarto.getValorDaDiaria()
                  << ", Status: " << opcao << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}