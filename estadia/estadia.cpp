#include "../cliente/cliente.h"
#include "../Quarto/quarto.h"
#include "estadia.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <limits>

using namespace std;

extern vector<Quarto> listaDeQuarto;
extern vector<Cliente> listaDeClientes;

void cliente();
void quarto();

vector<Estadia> listaDeEstadias;
int proximoCodEstadia = 1;

void salvarEstadias()
{
    filesystem::create_directories("data");
    ofstream arquivo("data/Estadia.bin", ios::binary | ios::trunc);
    if (!arquivo.is_open())
        return;
    for (const auto &e : listaDeEstadias)
    {
        int id = e.getCodigoDaEstadia();
        int codC = e.getCodigoDoCliente();
        int numQ = e.getNumeroDoQuarto();
        long dias = e.getQuantidadeDeDiarias();
        int stat = e.getStatus();
        arquivo.write((char *)&id, sizeof(id));
        arquivo.write((char *)&codC, sizeof(codC));
        arquivo.write((char *)&numQ, sizeof(numQ));
        arquivo.write((char *)&dias, sizeof(dias));
        arquivo.write((char *)&stat, sizeof(stat));
        auto gravarString = [&](const string &s)
        {
            size_t size = s.size();
            arquivo.write((char *)&size, sizeof(size));
            arquivo.write(s.c_str(), size);
        };
        gravarString(e.getDataDeEntrada());
        gravarString(e.getDataDeSaida());
    }
    arquivo.close();
}

void carregarEstadias()
{
    ifstream arquivo("data/Estadia.bin", ios::binary);
    if (!arquivo.is_open())
        return;
    listaDeEstadias.clear();
    while (true)
    {
        int id, codC, numQ, stat;
        long dias;
        if (!arquivo.read((char *)&id, sizeof(id)))
            break;
        arquivo.read((char *)&codC, sizeof(codC));
        arquivo.read((char *)&numQ, sizeof(numQ));
        arquivo.read((char *)&dias, sizeof(dias));
        arquivo.read((char *)&stat, sizeof(stat));
        auto lerString = [&](string &s)
        {
            size_t size;
            arquivo.read((char *)&size, sizeof(size));
            s.resize(size);
            arquivo.read(&s[0], size);
        };
        string dE, dS;
        lerString(dE);
        lerString(dS);
        Estadia e(id, dE, dS, dias, codC, numQ);
        e.setStatus(stat);
        listaDeEstadias.push_back(e);
        if (id >= proximoCodEstadia)
            proximoCodEstadia = id + 1;
    }
    arquivo.close();
}

bool parseDate(const string &s, int &dia, int &mes, int &ano)
{
    if (s.size() != 10 || s[2] != '/' || s[5] != '/')
        return false;

    try
    {
        dia = stoi(s.substr(0, 2));
        mes = stoi(s.substr(3, 2));
        ano = stoi(s.substr(6, 4));
    }
    catch (...)
    {
        return false;
    }

    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1)
        return false;

    auto isBissexto = [](int y)
    {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    };

    int diasNoMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2)
    {
        if (isBissexto(ano))
        {
            if (dia > 29)
                return false;
        }
        else
        {
            if (dia > 28)
                return false;
        }
    }
    else
    {
        if (dia > diasNoMes[mes])
            return false;
    }

    return true;
}

time_t stringParaTimeT(string dataStr)
{
    int d, m, a;
    if (!parseDate(dataStr, d, m, a))
        return (time_t)-1;

    tm t = {};
    t.tm_mday = d;
    t.tm_mon = m - 1;
    t.tm_year = a - 1900;

    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    return mktime(&t);
}

Quarto *buscarQuartoPtr(int num)
{
    for (auto &q : listaDeQuarto)
        if (q.getNumeroDoQuarto() == num)
            return &q;
    return nullptr;
}

Cliente *buscarClientePtr(int cod)
{
    for (auto &c : listaDeClientes)
        if (c.getCodigo() == cod)
            return &c;
    return nullptr;
}

bool verificarConflito(int numQuarto, time_t tEnt, time_t tSai)
{
    for (const auto &e : listaDeEstadias)
    {
        if (e.getNumeroDoQuarto() == numQuarto && e.getStatus() == 1)
        {
            time_t resEnt = stringParaTimeT(e.getDataDeEntrada());
            time_t resSai = stringParaTimeT(e.getDataDeSaida());

            if (resEnt == (time_t)-1 || resSai == (time_t)-1)
                continue;

            if (tEnt < resSai && tSai > resEnt)
                return true;
        }
    }
    return false;
}
void cadastrarEstadia() {
    int codC, numQ, pessoas;
    string dEnt, dSai;
    char resp;

    cout << "\n-=-| REGISTRAR NOVA ESTADIA |-=-\n";

    Cliente *cPtr = nullptr;
    do {
        cout << "Codigo do cliente: "; cin >> codC;
        cPtr = buscarClientePtr(codC);
        if (!cPtr) {
            cout << "Cliente nao encontrado. Cadastrar novo? (S/N): "; cin >> resp;
            if (toupper(resp) == 'S') { cliente(); cPtr = buscarClientePtr(codC); }
        }
    } while (!cPtr);

    Quarto *qPtr = nullptr;
    do {
        cout << "Numero do quarto: "; cin >> numQ;
        qPtr = buscarQuartoPtr(numQ);
        if (!qPtr) {
            cout << "Quarto nao cadastrado. Cadastrar agora? (S/N): "; cin >> resp;
            if (toupper(resp) == 'S') { quarto(); qPtr = buscarQuartoPtr(numQ); }
        } else {
    // Se o quarto foi cadastrado como Ocupado, não deixa reservar nunca
    if (qPtr->getStatus() != 1) {
        cout << "[ERRO] O quarto " << numQ << " foi cadastrado como Ocupado e nao pode receber estadias.\n";
        qPtr = nullptr; // força repetir o loop para escolher outro quarto
        continue;
    }

    cout << "Quantos hospedes: ";
    cin >> pessoas;
    if (pessoas > qPtr->getQuantidadeDeHospedes()) {
        cout << "[ERRO] Quarto comporta so " << qPtr->getQuantidadeDeHospedes() << "\n";
        qPtr = nullptr;
    }
}
    } while (!qPtr);

    while (true) {
        cout << "Entrada (DD/MM/AAAA): "; cin >> dEnt;
        cout << "Saida (DD/MM/AAAA): "; cin >> dSai;

        time_t t1 = stringParaTimeT(dEnt);
        time_t t2 = stringParaTimeT(dSai);

        if (t1 == -1 || t2 == -1 || t1 >= t2) {
            cout << "Datas invalidas ou Saida menor que Entrada!\n";
            continue;
        }

        if (verificarConflito(numQ, t1, t2)) {
            cout << "[ERRO] Quarto " << numQ << " ja reservado neste periodo! Escolha outras datas.\n";
            continue;
        }
        break;
    }

    long diarias = (stringParaTimeT(dSai) - stringParaTimeT(dEnt)) / 86400;
    Estadia nova(proximoCodEstadia++, dEnt, dSai, (diarias == 0 ? 1 : (int)diarias), codC, numQ);
    listaDeEstadias.push_back(nova);
    
    salvarEstadias();
    cout << "\nEstadia registrada com sucesso!\n";
}

void liberarQuartoForce()
{
    int numQ;
    cout << "\n-=-| TORNAR QUARTO DISPONIVEL MANUALMENTE |-=-\n";
    cout << "Numero do quarto para tornar Disponivel: ";

    if (!(cin >> numQ))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Entrada invalida para o numero do quarto.\n";
        return;
    }
    Quarto *qPtr = buscarQuartoPtr(numQ);
    if (!qPtr)
    {
        cout << "[ERRO] Quarto " << numQ << " nao encontrado.\n";
        return;
    }
    qPtr->setStatus(1);

    cout << "\nSucesso! O Quarto " << numQ << " foi marcado manualmente como Disponivel.\n";
}

void finalizarEstadia()
{
    int num;
    cout << "\nQuarto para checkout: ";

    if (!(cin >> num))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Entrada invalida para o numero do quarto.\n";
        return;
    }

    Quarto *qPtr = buscarQuartoPtr(num);
    if (!qPtr)
    {
        cout << "[ERRO] Quarto " << num << " nao encontrado.\n";
        return;
    }

    cout << "\n--- Reservas Ativas para o Quarto " << num << " ---\n";

    bool reservaAtivaEncontrada = false;
    for (const auto &e : listaDeEstadias)
    {
        if (e.getNumeroDoQuarto() == num && e.getStatus() == 1)
        {
            cout << "ID: " << e.getCodigoDaEstadia()
                 << " | Entrada: " << e.getDataDeEntrada()
                 << " | Saida: " << e.getDataDeSaida() << endl;
            reservaAtivaEncontrada = true;
        }
    }
    if (!reservaAtivaEncontrada)
    {
        cout << "[AVISO] Nenhuma reserva ativa encontrada para o Quarto " << num << ".\n";
        return;
    }

    int id;
    cout << "\nID da estadia para finalizar: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Entrada invalida para o ID da estadia.\n";
        return;
    }

    bool encontrada = false;
    for (auto &e : listaDeEstadias)
    {

        if (e.getCodigoDaEstadia() == id && e.getNumeroDoQuarto() == num && e.getStatus() == 1)
        {
            e.setStatus(0);
            long diarias = e.getQuantidadeDeDiarias();
            double valorDiaria = qPtr->getValorDaDiaria();

            cout << "------------------------------------------\n";
            Cliente *clienteFinalizado = buscarClientePtr(e.getCodigoDoCliente());
            cout << "Cliente: " << (clienteFinalizado ? clienteFinalizado->getNome() : "CLIENTE NAO ENCONTRADO") << endl;
            cout << "Diarias reservadas: " << diarias << endl;
            cout << "Valor diaria: R$ " << fixed << setprecision(2) << valorDiaria << endl;
            cout << "Valor total da estadia: R$ " << fixed << setprecision(2) << (diarias * valorDiaria) << endl;
            cout << "------------------------------------------\n";
            encontrada = true;
            break;
        }
    }

    if (!encontrada)
    {
        cout << "[ERRO] Estadia ID " << id << " nao esta ativa neste quarto (" << num << ") ou nao existe.\n";
    }
    else
    {
        qPtr->setStatus(1);
        salvarEstadias();
        cout << "Estadia finalizada com sucesso!\n";
    }
}

void listarEstadia()
{
    cout << "\n-=-| Lista de Todas as Estadias |-=-\n";

    if (listaDeEstadias.empty())
    {
        cout << "*Nenhuma estadia cadastrada.*\n";
        return;
    }

    for (const auto &e : listaDeEstadias)
    {
        int codC = e.getCodigoDoCliente();

        Cliente *cPtr = buscarClientePtr(codC);

        string nomeCliente = cPtr ? cPtr->getNome() : "CLIENTE NAO ENCONTRADO";

        cout << "Estadia ID: " << e.getCodigoDaEstadia()
             << " | Cliente: " << codC << " - " << nomeCliente
             << " | Quarto: " << e.getNumeroDoQuarto()
             << " | Entrada: " << e.getDataDeEntrada()
             << " | Saida: " << e.getDataDeSaida()
             << " | Status: " << (e.getStatus() == 1 ? "Ativa" : "Finalizada") << endl;
    }

    cout << "------------------------------------------\n";
}

void listarEstadiaC()
{
    int cod;
    cout << "Cod Cliente: ";
    if (!(cin >> cod))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Entrada invalida.\n";
        return;
    }

    bool encontrada = false;
    for (const auto &e : listaDeEstadias)
    {
        if (e.getCodigoDoCliente() == cod)
        {
            int codC = e.getCodigoDoCliente();

            Cliente *cPtr = buscarClientePtr(codC);

            string nomeCliente = cPtr ? cPtr->getNome() : "CLIENTE NAO ENCONTRADO";

            cout << "Estadia ID: " << e.getCodigoDaEstadia()
                 << " | Cliente: " << codC << " - " << nomeCliente
                 << " | Quarto: " << e.getNumeroDoQuarto()
                 << " | Entrada: " << e.getDataDeEntrada()
                 << " | Saida: " << e.getDataDeSaida()
                 << " | Status: " << (e.getStatus() == 1 ? "Ativa" : "Finalizada") << endl;
            encontrada = true;
        }
    }
    if (!encontrada)
    {
        cout << "*Nenhuma estadia encontrada para o Cliente ID " << cod << ".*\n";
    }
}

bool listarEstadiaC(int cod)
{
    bool encontrada = false;
    for (const auto &e : listaDeEstadias)
    {
        if (e.getCodigoDoCliente() == cod)
        {
            int codC = e.getCodigoDoCliente();

            Cliente *cPtr = buscarClientePtr(codC);

            string nomeCliente = cPtr ? cPtr->getNome() : "CLIENTE NAO ENCONTRADO";

            cout << "Estadia ID: " << e.getCodigoDaEstadia()
                 << " | Cliente: " << codC << " - " << nomeCliente
                 << " | Quarto: " << e.getNumeroDoQuarto()
                 << " | Entrada: " << e.getDataDeEntrada()
                 << " | Saida: " << e.getDataDeSaida()
                 << " | Status: " << (e.getStatus() == 1 ? "Ativa" : "Finalizada") << endl;
            encontrada = true;
        }
    }
    return encontrada;
}