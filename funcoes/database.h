#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include "../cliente/cliente.h"
#include "../funcionario/funcionario.h"
#include "../quarto/quarto.h"
#include "../estadia/estadia.h"

// ====== LISTAS GLOBAIS ======
extern std::vector<Cliente> listaDeClientes;
extern std::vector<Funcionario> listaDeFuncionario;
extern std::vector<Quarto> listaDeQuarto;
extern std::vector<Estadia> listaDeEstadias;

// ====== FUNÇÕES ======
void salvarClientesJSON();
// void salvarFuncionariosJSON();
// void salvarQuartosJSON();
// void salvarEstadiasJSON();

void carregarClientesJSON();
// void carregarFuncionariosJSON();
// void carregarQuartosJSON();
// void carregarEstadiasJSON();

#endif
