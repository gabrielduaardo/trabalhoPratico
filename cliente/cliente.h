/*  -=-=-=-=| LEGENDA cliente.h |-=-=-=-= 

nom = nome que vai receber do usuario
end = endereço que vai receber do usuario
tel = relefone que vai receber do usuario

   -=-=-=-=| FIM |-=-=-=-=*/

#ifndef CLIENTE_H // Primeira Verificação
#define CLIENTE_H // Definição do Símbolo (Se não estiver definido)

#include <string>
#include <vector>
#include <iostream> // Necessário para std::cout

class Cliente {
private:
    int codigo;
    std::string nome;
    std::string endereco;
    std::string telefone;

public:
    // Apenas a DECLARAÇÃO (assinatura) do Construtor
    Cliente(int cod, const std::string& nom, const std::string& end, const std::string& tel);

    // Métodos Get (Acessores) - Podem ficar aqui, mas o ideal é no .cpp
    int getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getEndereco() const { return endereco; }
    std::string getTelefone() const { return telefone; }

    // void salvar(std::ofstream& ofs) const;
    // void carregar(std::ifstream& ifs);
};

// DECLARAÇÃO da variável global: diz ao compilador que ela existe em outro lugar.
extern std::vector<Cliente> listaDeClientes;

// DECLARAÇÃO das funções: apenas a assinatura.
bool cadastrarCliente(const std::string& nome, const std::string& endereco, const std::string& telefone);
void listarClientes();

#endif // CLIENTE_H Fim do Bloco Condicional