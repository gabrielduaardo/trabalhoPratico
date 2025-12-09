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

using namespace std;

extern vector<Quarto> listaDeQuarto;
extern vector<Cliente> listaDeClientes;
vector<Estadia> listaDeEstadias;
int proximoCodEstadia = 1;

void salvarEstadias() {
    filesystem::create_directories("data");
    ofstream arquivo("data/Estadia.bin", ios::binary | ios::trunc);
    if (!arquivo.is_open()) return;
    for (const auto& e : listaDeEstadias) {
        int id = e.getCodigoDaEstadia();
        int codC = e.getCodigoDoCliente();
        int numQ = e.getNumeroDoQuarto();
        long dias = e.getQuantidadeDeDiarias();
        int stat = e.getStatus();
        arquivo.write((char*)&id, sizeof(id));
        arquivo.write((char*)&codC, sizeof(codC));
        arquivo.write((char*)&numQ, sizeof(numQ));
        arquivo.write((char*)&dias, sizeof(dias));
        arquivo.write((char*)&stat, sizeof(stat));
        auto gravarString = [&](const string& s) {
            size_t size = s.size();
            arquivo.write((char*)&size, sizeof(size));
            arquivo.write(s.c_str(), size);
        };
        gravarString(e.getDataDeEntrada());
        gravarString(e.getDataDeSaida());
    }
    arquivo.close();
}

void carregarEstadias() {
    ifstream arquivo("data/Estadia.bin", ios::binary);
    if (!arquivo.is_open()) return;
    listaDeEstadias.clear();
    while (true) {
        int id, codC, numQ, stat; long dias;
        if (!arquivo.read((char*)&id, sizeof(id))) break;
        arquivo.read((char*)&codC, sizeof(codC));
        arquivo.read((char*)&numQ, sizeof(numQ));
        arquivo.read((char*)&dias, sizeof(dias));
        arquivo.read((char*)&stat, sizeof(stat));
        auto lerString = [&](string& s) {
            size_t size;
            arquivo.read((char*)&size, sizeof(size));
            s.resize(size);
            arquivo.read(&s[0], size);
        };
        string dE, dS;
        lerString(dE); lerString(dS);
        Estadia e(id, dE, dS, dias, codC, numQ);
        e.setStatus(stat);
        listaDeEstadias.push_back(e);
        if (id >= proximoCodEstadia) proximoCodEstadia = id + 1;
    }
    arquivo.close();
}

bool parseDate(const string &s, int &dia, int &mes, int &ano) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    try {
        dia = stoi(s.substr(0, 2));
        mes = stoi(s.substr(3, 2));
        ano = stoi(s.substr(6, 4));
    } catch (...) { return false; }
    return true;
}

time_t stringParaTimeT(string dataStr) {
    int d, m, a;
    if (!parseDate(dataStr, d, m, a)) return -1;
    tm t = {};
    t.tm_mday = d; t.tm_mon = m - 1; t.tm_year = a - 1900;
    return mktime(&t);
}

// VERIFICA SE O PERÍODO DESEJADO CONFLITA COM RESERVAS EXISTENTES
bool verificarConflito(int numQuarto, time_t tEnt, time_t tSai) {
    for (const auto &e : listaDeEstadias) {
        if (e.getNumeroDoQuarto() == numQuarto && e.getStatus() == 1) {
            time_t resEnt = stringParaTimeT(e.getDataDeEntrada());
            time_t resSai = stringParaTimeT(e.getDataDeSaida());
            
            // Lógica: (Entrada1 < Saida2) E (Saida1 > Entrada2) -> Houve choque
            if (tEnt < resSai && tSai > resEnt) return true;
        }
    }
    return false;
}

Quarto *buscarQuartoPtr(int num) {
    for (auto &q : listaDeQuarto) if (q.getNumeroDoQuarto() == num) return &q;
    return nullptr;
}

Cliente *buscarClientePtr(int cod) {
    for (auto &c : listaDeClientes) if (c.getCodigo() == cod) return &c;
    return nullptr;
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
            cout << "Quantos hospedes: "; cin >> pessoas;
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

void finalizarEstadia() {
    int num; cout << "\nQuarto para checkout: "; cin >> num;
    Quarto *qPtr = buscarQuartoPtr(num);
    if (!qPtr) { cout << "Nao existe!\n"; return; }

    cout << "\n--- Reservas Ativas para o Quarto " << num << " ---\n";
    for (const auto &e : listaDeEstadias) {
        if (e.getNumeroDoQuarto() == num && e.getStatus() == 1) {
            cout << "ID: " << e.getCodigoDaEstadia() << " | " << e.getDataDeEntrada() << " a " << e.getDataDeSaida() << endl;
        }
    }

    int id; cout << "\nID da estadia para finalizar: "; cin >> id;
    for (auto &e : listaDeEstadias) {
        if (e.getCodigoDaEstadia() == id && e.getStatus() == 1) {
            e.setStatus(0);
            cout << "Total: R$ " << e.getQuantidadeDeDiarias() * qPtr->getValorDaDiaria() << endl;
            break;
        }
    }
    salvarEstadias();
}

void listarEstadia() {
    for (const auto &e : listaDeEstadias)
        cout << "Estadia ID: " << e.getCodigoDaEstadia() << " | Quarto: " << e.getNumeroDoQuarto() << " | Status: " << (e.getStatus() == 1 ? "Ativa" : "Finalizada") << endl;
}

void listarEstadiaC() {
    int cod; cout << "Cod Cliente: "; cin >> cod;
    for (const auto &e : listaDeEstadias)
        if (e.getCodigoDoCliente() == cod)
            cout << "Estadia " << e.getCodigoDaEstadia() << " | Quarto " << e.getNumeroDoQuarto() << endl;
}