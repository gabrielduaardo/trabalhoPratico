#include "../cliente/cliente.h"
#include "../Quarto/quarto.h"
#include "estadia.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// --- VARIAVEIS GLOBAIS ---
extern vector<Quarto> listaDeQuarto;
extern vector<Cliente> listaDeClientes;
vector<Estadia> listaDeEstadias;
int proximoCodEstadia = 1;

// =======================================================
// FUNCAO: VALIDAR DATA MANUALMENTE (FUNCIONA NO MINGW)
// =======================================================

bool parseDate(const string &s, int &dia, int &mes, int &ano)
{
    if (s.size() != 10) return false;
    if (s[2] != '/' || s[5] != '/') return false;

    try {
        dia = stoi(s.substr(0, 2));
        mes = stoi(s.substr(3, 2));
        ano = stoi(s.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (ano < 1900 || ano > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    return true;
}

// =======================================================
// CALCULAR DIARIAS SEM GET_TIME (FUNCIONA EM QUALQUER G++)
// =======================================================

long int calcularDiarias(const string &dataEntradaStr, const string &dataSaidaStr)
{
    int d1, m1, y1;
    int d2, m2, y2;

    if (!parseDate(dataEntradaStr, d1, m1, y1)) return -1;
    if (!parseDate(dataSaidaStr, d2, m2, y2)) return -1;

    tm entrada = {};
    tm saida = {};

    entrada.tm_mday = d1;
    entrada.tm_mon = m1 - 1;
    entrada.tm_year = y1 - 1900;

    saida.tm_mday = d2;
    saida.tm_mon = m2 - 1;
    saida.tm_year = y2 - 1900;

    time_t t1 = mktime(&entrada);
    time_t t2 = mktime(&saida);

    if (t1 == -1 || t2 == -1) return -1;

    double diff = difftime(t2, t1);

    long diarias = diff / 86400;

    if (diarias < 0) return -1;
    if (diarias == 0) return 1;

    return diarias;
}

// =======================================================
// BUSCAR ESTADIA ATIVA
// =======================================================

Estadia *buscarEstadiaAtiva(int numQuarto)
{
    for (int i = listaDeEstadias.size() - 1; i >= 0; i--)
    {
        if (listaDeEstadias[i].getNumeroDoQuarto() == numQuarto &&
            listaDeEstadias[i].getStatus() == 1) // 1 = ativa
        {
            return &listaDeEstadias[i];
        }
    }
    return nullptr;
}

// =======================================================
// BUSCAR QUARTO
// =======================================================

Quarto *buscarQuarto(int numQuarto)
{
    auto it = find_if(listaDeQuarto.begin(), listaDeQuarto.end(),
                      [numQuarto](Quarto &q)
                      { return q.getNumeroDoQuarto() == numQuarto; });

    return (it != listaDeQuarto.end()) ? &(*it) : nullptr;
}

// =======================================================
// BUSCAR CLIENTE
// =======================================================

Cliente *buscarCliente(int codCliente)
{
    auto it = find_if(listaDeClientes.begin(), listaDeClientes.end(),
                      [codCliente](const Cliente &c)
                      { return c.getCodigo() == codCliente; });

    return (it != listaDeClientes.end()) ? &(*it) : nullptr;
}

// =======================================================
// CADASTRAR ESTADIA (CHECK-IN)
// =======================================================

void cadastrarEstadia()
{
    int codCliente, numQuarto;
    string dataEntrada, dataSaida;
    long int diarias;
    char resposta;

    cout << "\n-=-| REGISTRAR NOVA ESTADIA |-=-\n";

    Cliente *ClientePtr = nullptr;
    Quarto *QuartoPtr = nullptr;

    // CLIENTE
    do
    {
        cout << "Codigo do cliente: ";
        cin >> codCliente;

        ClientePtr = buscarCliente(codCliente);

        if (!ClientePtr)
        {
            cout << "Cliente nao encontrado. Cadastrar? (S/N): ";
            cin >> resposta;

            if (toupper(resposta) == 'S')
                cliente();
        }

    } while (!ClientePtr);

    // QUARTO
    do
    {
        cout << "\nNumero do quarto: ";
        cin >> numQuarto;

        QuartoPtr = buscarQuarto(numQuarto);

        if (!QuartoPtr)
        {
            cout << "Quarto nao encontrado. Cadastrar? (S/N): ";
            cin >> resposta;

            if (toupper(resposta) == 'S')
                quarto();
        }
        else if (QuartoPtr->getStatus() != 1)
        {
            cout << "Quarto esta ocupado!\n";
            QuartoPtr = nullptr;
        }

    } while (!QuartoPtr);

    // DATAS
    cout << "\nData de entrada (DD/MM/AAAA): ";
    cin >> dataEntrada;

    cout << "Data de saida (DD/MM/AAAA): ";
    cin >> dataSaida;

    diarias = calcularDiarias(dataEntrada, dataSaida);

    if (diarias <= 0)
    {
        cout << "Erro: datas invalidas!\n";
        return;
    }

    // CADASTRAR
    Estadia nova(
        proximoCodEstadia++,
        dataEntrada,
        dataSaida,
        diarias,
        ClientePtr->getCodigo(),
        QuartoPtr->getNumeroDoQuarto());

    listaDeEstadias.push_back(nova);

    QuartoPtr->setStatus(2);

    cout << "\nEstadia cadastrada com sucesso!\n";
}

// =======================================================
// LISTAR TODAS AS ESTADIAS
// =======================================================

void listarEstadia()
{
    cout << "\n-=-| Lista de Estadias |-=-\n";

    if (listaDeEstadias.empty())
    {
        cout << "*Nenhuma estadia cadastrada.*\n";
        return;
    }

    for (const auto &e : listaDeEstadias)
    {
        cout << "Estadia " << e.getCodigoDoCliente()
             << " | Cliente: " << e.getCodigoDoCliente()
             << " | Quarto: " << e.getNumeroDoQuarto()
             << " | Entrada: " << e.getDataDeEntrada()
             << " | Saida: " << e.getDataDeSaida()
             << " | Diarias: " << e.getQuantidadeDeDiarias()
             << endl;
    }
}

// =======================================================
// LISTAR ESTADIAS POR CLIENTE
// =======================================================

void listarEstadiaC()
{
    int codigoCliente;

    cout << "\nCodigo do cliente: ";
    cin >> codigoCliente;

    Cliente *cliente = buscarCliente(codigoCliente);

    if (!cliente)
    {
        cout << "Cliente nao encontrado!\n";
        return;
    }

    cout << "\nEstadias de: " << cliente->getNome() << "\n";

    for (auto &e : listaDeEstadias)
    {
        if (e.getCodigoDoCliente() == codigoCliente)
        {
            cout << "- Estadia " << e.getCodigoDaEstadia()
                 << " | Quarto " << e.getNumeroDoQuarto()
                 << " | Entrada: " << e.getDataDeEntrada()
                 << " | Diarias: " << e.getQuantidadeDeDiarias()
                 << "\n";
        }
    }
}

// =======================================================
// FINALIZAR ESTADIA (CHECK-OUT)
// =======================================================

void finalizarEstadia()
{
    int numQuarto;

    cout << "\nNumero do quarto para checkout: ";
    cin >> numQuarto;

    Quarto *QuartoPtr = buscarQuarto(numQuarto);
    Estadia *estadia = buscarEstadiaAtiva(numQuarto);

    if (!QuartoPtr)
    {
        cout << "Quarto nao encontrado!\n";
        return;
    }

    if (QuartoPtr->getStatus() != 2)
    {
        cout << "Quarto nao esta ocupado!\n";
        return;
    }

    QuartoPtr->setStatus(1);

    cout << "\nCHECKOUT CONCLUIDO!\nQuarto liberado.\n";

    if (estadia)
    {
        double valorTotal = estadia->getQuantidadeDeDiarias() * QuartoPtr->getValorDaDiaria();

        cout << "\nResumo da Conta:\n";
        cout << "Estadia: " << estadia->getCodigoDaEstadia() << "\n";
        cout << "Diarias: " << estadia->getQuantidadeDeDiarias() << "\n";
        cout << "Valor total: R$ " << valorTotal << "\n";
    }
}
