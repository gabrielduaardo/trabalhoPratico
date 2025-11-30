#include <fstream>
#include <iostream>
#include <string>
#include "../cliente/cliente.h"
#include "../quarto/quarto.h"
#include "../funcionario/funcionario.h"
#include "../estadia/estadia.h"

using namespace std;

// ==========================================
// =============   SALVAR JSON  =============
// ==========================================

void salvarClientesJSON() {
    ofstream arq("clientes.json");
    arq << "[\n";
    for (size_t i = 0; i < listaDeClientes.size(); i++) {
        const Cliente &c = listaDeClientes[i];
        arq << "  {\"codigo\":" << c.getCodigo()
            << ",\"nome\":\"" << c.getNome()
            << "\",\"endereco\":\"" << c.getEndereco()
            << "\",\"telefone\":\"" << c.getTelefone() << "\"}";
        if (i + 1 < listaDeClientes.size()) arq << ",";
        arq << "\n";
    }
    arq << "]";
    arq.close();
}

void salvarFuncionariosJSON() {
    ofstream arq("funcionarios.json");
    arq << "[\n";
    for (size_t i = 0; i < listaDeFuncionario.size(); i++) {
        const Funcionario &f = listaDeFuncionario[i];
        arq << "  {\"codigo\":" << f.getCodigo()
            << ",\"nome\":\"" << f.getNome()
            << "\",\"telefone\":\"" << f.getTelefone()
            << "\",\"cargo\":\"" << f.getCargo()
            << "\",\"salario\":" << f.getSalario() << "}";
        if (i + 1 < listaDeFuncionario.size()) arq << ",";
        arq << "\n";
    }
    arq << "]";
    arq.close();
}

void salvarQuartosJSON() {
    ofstream arq("quartos.json");
    arq << "[\n";
    for (size_t i = 0; i < listaDeQuartos.size(); i++) {
        const Quarto &q = listaDeQuartos[i];
        arq << "  {\"numero\":" << q.getNumeroDoQuarto()
            << ",\"hospedes\":" << q.getQuantidadeDeHospedes()
            << ",\"valor\":" << q.getValorDaDiaria()
            << ",\"status\":" << q.getStatus() << "}";
        if (i + 1 < listaDeQuartos.size()) arq << ",";
        arq << "\n";
    }
    arq << "]";
    arq.close();
}

void salvarEstadiasJSON() {
    ofstream arq("estadias.json");
    arq << "[\n";
    for (size_t i = 0; i < listaDeEstadias.size(); i++) {
        const Estadia &e = listaDeEstadias[i];
        arq << "  {\"codigo\":" << e.getCodigoDaEstadia()
            << ",\"entrada\":\"" << e.getDataDeEntrada()
            << "\",\"saida\":\"" << e.getDataDeSaida()
            << "\",\"diarias\":" << e.getQuantidadeDeDiarias()
            << ",\"cliente\":" << e.getCodigoDoCliente()
            << ",\"quarto\":" << e.getNumeroDoQuarto()
            << ",\"status\":" << e.getStatus() << "}";
        if (i + 1 < listaDeEstadias.size()) arq << ",";
        arq << "\n";
    }
    arq << "]";
    arq.close();
}


// ==========================================
// ============ CARREGAR JSON ===============
// (simples, feito manualmente)
// ==========================================

string pegarValor(const string &linha, const string &campo) {
    size_t pos = linha.find(campo);
    if (pos == string::npos) return "";
    pos += campo.size();
    size_t fim = linha.find_first_of(",}", pos);
    return linha.substr(pos, fim - pos);
}

void carregarClientesJSON() {
    ifstream arq("clientes.json");
    if (!arq.good()) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.find("{") == string::npos) continue;

        int codigo = stoi(pegarValor(linha, "\"codigo\":"));
        string nome = pegarValor(linha, "\"nome\":\"");
        string endereco = pegarValor(linha, "\"endereco\":\"");
        string telefone = pegarValor(linha, "\"telefone\":\"");

        listaDeClientes.emplace_back(codigo, nome, endereco, telefone);
    }
}

void carregarFuncionariosJSON() {
    ifstream arq("funcionarios.json");
    if (!arq.good()) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.find("{") == string::npos) continue;

        int codigo = stoi(pegarValor(linha, "\"codigo\":"));
        string nome = pegarValor(linha, "\"nome\":\"");
        string telefone = pegarValor(linha, "\"telefone\":\"");
        string cargo = pegarValor(linha, "\"cargo\":\"");
        double salario = stod(pegarValor(linha, "\"salario\":"));

        listaDeFuncionario.emplace_back(codigo, nome, telefone, cargo, salario);
    }
}

void carregarQuartosJSON() {
    ifstream arq("quartos.json");
    if (!arq.good()) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.find("{") == string::npos) continue;

        int numero = stoi(pegarValor(linha, "\"numero\":"));
        int hosp = stoi(pegarValor(linha, "\"hospedes\":"));
        double valor = stod(pegarValor(linha, "\"valor\":"));
        int status = stoi(pegarValor(linha, "\"status\":"));

        listaDeQuartos.emplace_back(numero, hosp, valor, status);
    }
}

void carregarEstadiasJSON() {
    ifstream arq("estadias.json");
    if (!arq.good()) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.find("{") == string::npos) continue;

        int codigo = stoi(pegarValor(linha, "\"codigo\":"));
        string entrada = pegarValor(linha, "\"entrada\":\"");
        string saida = pegarValor(linha, "\"saida\":\"");
        int diarias = stoi(pegarValor(linha, "\"diarias\":"));
        int cliente = stoi(pegarValor(linha, "\"cliente\":"));
        int quarto = stoi(pegarValor(linha, "\"quarto\":"));
        int status = stoi(pegarValor(linha, "\"status\":"));

        Estadia e(codigo, entrada, saida, diarias, cliente, quarto);
        e.setStatus(status);
        listaDeEstadias.push_back(e);
    }
}
