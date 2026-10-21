// Testes automatizados e tabelas comparativas (AVL x Rubro-Negra).
// As mesmas sequencias de dados sao aplicadas nas duas arvores.
// Se alguma condicao falhar, o assert aborta mostrando a linha e a descricao.

// assert precisa estar ativo mesmo em build Release (que define NDEBUG)
#undef NDEBUG
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "arvoreavl.hpp"
#include "arvorerb.hpp"

using namespace std;

/// BST comum (sem balanceamento): so serve de referencia nas tabelas
class BST : public ArvoreBase {
    public:
    BST() : ArvoreBase(nullptr) {}

    bool insert(int value) override {
        Node** atual = &root;
        while (*atual != nullptr) {
            if (value < (*atual)->data) {
                atual = &(*atual)->left;
            } else if (value > (*atual)->data) {
                atual = &(*atual)->right;
            } else {
                return false;
            }
        }
        *atual = new Node(value);
        return true;
    }

    bool remove(int) override { return false; }          // nao usado na referencia
};

// ---------------------------------------------------------------- utilidades

ArvoreBase* criar(int tipo) {
    if (tipo == 1) {
        return new AVL();
    }
    return new RB();
}

string nomeTipo(int tipo) {
    return tipo == 1 ? "AVL" : "Rubro-Negra";
}

vector<int> crescente(int n) {
    vector<int> v(n);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

vector<int> decrescente(int n) {
    auto v = crescente(n);
    std::reverse(v.begin(), v.end());
    return v;
}

vector<int> aleatoria(int n, unsigned semente) {
    auto v = crescente(n);
    std::mt19937 gerador(semente);
    std::shuffle(v.begin(), v.end(), gerador);
    return v;
}

/// Limite teorico de altura (em niveis) para n nos
double limiteAltura(int tipo, int n) {
    if (tipo == 1) {
        return 1.4405 * std::log2(n + 2.0);  // AVL
    }
    return 2.0 * std::log2(n + 1.0);         // Rubro-Negra (LLRB)
}

/// Roda o cenario nas duas arvores; se um assert falhar o programa aborta antes do "OK"
void executar(const string& nome, void (*teste)(int)) {
    for (int tipo = 1; tipo <= 2; tipo++) {
        cout << std::left << std::setw(52) << nome << std::setw(14) << nomeTipo(tipo) << std::flush;
        teste(tipo);
        cout << "OK\n";
    }
}

// ------------------------------------------------------------------ cenarios

void testeVazia(int tipo) {
    ArvoreBase* a = criar(tipo);
    assert(!a->search(5) && "busca em arvore vazia retorna false");
    assert(!a->remove(5) && "remocao em arvore vazia retorna false");
    assert(a->toVector().empty() && "percurso em ordem vazio");
    assert(a->height() == 0 && "altura 0");
    assert(a->size() == 0 && "tamanho 0");

    ostringstream saida;
    auto original = cout.rdbuf(saida.rdbuf());
    a->printTree();
    a->printInOrder();
    cout.rdbuf(original);
    assert(saida.str().find("vazia") != string::npos && "impressao de arvore vazia");
    delete a;
}

void testeUmElemento(int tipo) {
    ArvoreBase* a = criar(tipo);
    assert(a->insert(7) && "inserir 7 retorna true");
    assert(a->search(7) && "buscar 7 encontra");
    assert(!a->search(8) && "buscar 8 nao encontra");
    assert(a->height() == 1 && "altura 1");
    assert(a->toVector() == vector<int>{7} && "em ordem = {7}");
    assert(!a->remove(8) && "remover inexistente retorna false");
    assert(a->size() == 1 && "continua com 1 elemento");
    assert(a->remove(7) && "remover 7 retorna true");
    assert(a->size() == 0 && a->height() == 0 && "arvore volta a ficar vazia");
    assert(a->insert(9) && "reinserir apos esvaziar");
    delete a;
}

void testeDuplicados(int tipo) {
    ArvoreBase* a = criar(tipo);
    assert(a->insert(10) && "primeira insercao de 10");
    assert(!a->insert(10) && "segunda insercao de 10 e recusada");
    for (int v : {5, 15, 5, 15, 10}) {
        a->insert(v);
    }
    assert(a->size() == 3 && "tamanho 3 apos duplicados");
    assert(a->toVector() == vector<int>({5, 10, 15}) && "sem valores repetidos");
    delete a;
}

void testeOrdensDeInsercao(int tipo) {
    int n = 1000;
    vector<vector<int>> conjuntos = {crescente(n), decrescente(n), aleatoria(n, 7)};

    for (const auto& conjunto : conjuntos) {
        ArvoreBase* a = criar(tipo);

        for (int v : conjunto) {
            assert(a->insert(v) && "toda insercao de valor novo retorna true");
        }

        assert(a->size() == n && "tamanho correto");
        assert(a->toVector() == crescente(n) && "percurso em ordem crescente");
        assert(a->height() <= limiteAltura(tipo, n) && "altura dentro do limite teorico");
        delete a;
    }
}

void testeBusca(int tipo) {
    ArvoreBase* a = criar(tipo);
    for (int v : aleatoria(500, 3)) {
        a->insert(v);
    }

    for (int v = 1; v <= 500; v++) {
        assert(a->search(v) && "valor existente e encontrado");
    }
    for (int v : {-5, 0, 501, 600, 100000}) {
        assert(!a->search(v) && "valor inexistente nao e encontrado");
    }
    delete a;
}

/// Remove cada chave de varias arvores diferentes: cobre folha, um filho, dois filhos e raiz
void testeRemocaoConfiguracoes(int tipo) {
    vector<vector<int>> entradas;
    for (int n = 1; n <= 24; n++) {
        entradas.push_back(crescente(n));
    }
    for (unsigned semente = 1; semente <= 30; semente++) {
        entradas.push_back(aleatoria(15, semente));
    }

    for (const auto& entrada : entradas) {
        for (int chave : entrada) {
            ArvoreBase* a = criar(tipo);
            for (int v : entrada) {
                a->insert(v);
            }

            set<int> esperado(entrada.begin(), entrada.end());
            esperado.erase(chave);

            assert(a->remove(chave) && "remocao de chave existente retorna true");
            assert(a->toVector() == vector<int>(esperado.begin(), esperado.end())
                   && "conteudo correto apos a remocao");
            assert(!a->search(chave) && "chave removida nao e mais encontrada");
            assert(!a->remove(chave) && "chave removida nao pode ser removida de novo");
            delete a;
        }
    }
}

void testeRemocaoInexistente(int tipo) {
    ArvoreBase* a = criar(tipo);
    for (int v : aleatoria(100, 5)) {
        a->insert(v);
    }
    auto antes = a->toVector();

    for (int v : {-1, 0, 101, 1000}) {
        assert(!a->remove(v) && "remover inexistente retorna false");
    }
    assert(a->toVector() == antes && "arvore inalterada");
    delete a;
}

void testeRemoverTudo(int tipo) {
    ArvoreBase* a = criar(tipo);
    for (int v : aleatoria(500, 3)) {
        a->insert(v);
    }

    for (int v : aleatoria(500, 11)) {
        assert(a->remove(v) && "toda chave existente e removida");
    }
    assert(a->size() == 0 && a->height() == 0 && "arvore termina vazia");
    delete a;
}

/// Operacoes aleatorias misturadas, comparando com std::set
void testeEstresse(int tipo) {
    ArvoreBase* a = criar(tipo);
    set<int> referencia;
    std::mt19937 gerador(2024);
    std::uniform_int_distribution<int> chave(0, 299);
    std::uniform_int_distribution<int> moeda(0, 1);

    for (int i = 0; i < 4000; i++) {
        int valor = chave(gerador);
        if (moeda(gerador) == 0) {
            bool esperado = referencia.insert(valor).second;
            assert(a->insert(valor) == esperado && "retorno de insert igual ao std::set");
        } else {
            bool esperado = referencia.erase(valor) > 0;
            assert(a->remove(valor) == esperado && "retorno de remove igual ao std::set");
        }
    }

    assert(a->toVector() == vector<int>(referencia.begin(), referencia.end())
           && "conteudo final igual ao std::set");
    delete a;
}

// ---------------------------------------------------------- tabela comparativa

struct Medida {
    int altura;
    int rotacoes;
    int flips;
};

Medida medirInsercao(ArvoreBase& a, const vector<int>& dados) {
    a.resetContadores();
    for (int v : dados) {
        a.insert(v);
    }
    return {a.height(), a.getTotalRotacoes(), a.getTotalFlips()};
}

Medida medirRemocao(ArvoreBase& a, const vector<int>& dados) {
    a.resetContadores();
    for (int v : dados) {
        a.remove(v);
    }
    return {a.height(), a.getTotalRotacoes(), a.getTotalFlips()};
}

void tabelaComparativa() {
    vector<int> tamanhos = {1000, 10000, 100000};
    vector<string> rotulos = {"crescente", "decrescente", "aleatoria"};

    cout << "\n=== TABELA 1: insercao (mesmos dados nas duas arvores) ===\n";
    cout << std::right << setw(12) << "dados" << setw(8) << "N"
         << setw(8) << "h_min" << setw(8) << "BST"
         << setw(8) << "AVL h" << setw(8) << "RB h"
         << setw(10) << "AVL rot" << setw(10) << "RB rot"
         << setw(10) << "RB flips" << "\n";

    ostringstream t2;
    t2 << "\n=== TABELA 2: remocao de metade dos elementos (ordem aleatoria) ===\n";
    t2 << std::right << setw(12) << "dados" << setw(8) << "N"
       << setw(10) << "removidos" << setw(8) << "AVL h" << setw(8) << "RB h"
       << setw(10) << "AVL rot" << setw(10) << "RB rot"
       << setw(10) << "RB flips" << "\n";

    for (int n : tamanhos) {
        vector<vector<int>> conjuntos = {crescente(n), decrescente(n), aleatoria(n, 7)};
        vector<int> remocoes = aleatoria(n, 99);
        remocoes.resize(n / 2);
        int alturaMinima = static_cast<int>(std::ceil(std::log2(n + 1.0)));

        for (size_t i = 0; i < conjuntos.size(); i++) {
            AVL avl;
            RB rb;
            Medida mAvl = medirInsercao(avl, conjuntos[i]);
            Medida mRb = medirInsercao(rb, conjuntos[i]);

            string alturaBst = "-";
            if (n <= 10000) {
                BST bst;
                for (int v : conjuntos[i]) {
                    bst.insert(v);
                }
                alturaBst = to_string(bst.height());
            }

            cout << std::right << setw(12) << rotulos[i] << setw(8) << n
                 << setw(8) << alturaMinima << setw(8) << alturaBst
                 << setw(8) << mAvl.altura << setw(8) << mRb.altura
                 << setw(10) << mAvl.rotacoes << setw(10) << mRb.rotacoes
                 << setw(10) << mRb.flips << "\n";

            Medida rAvl = medirRemocao(avl, remocoes);
            Medida rRb = medirRemocao(rb, remocoes);

            t2 << std::right << setw(12) << rotulos[i] << setw(8) << n
               << setw(10) << remocoes.size() << setw(8) << rAvl.altura
               << setw(8) << rRb.altura << setw(10) << rAvl.rotacoes
               << setw(10) << rRb.rotacoes << setw(10) << rRb.flips << "\n";
        }
    }
    cout << t2.str();
    cout << "\nLegenda: h_min = ceil(log2(N+1)); BST = arvore sem balanceamento ('-' = nao medida);\n"
         << "rotacao dupla (LR/RL) conta 2 rotacoes; alturas em numero de niveis.\n";
}

int main() {
    cout << "=== TESTES AUTOMATIZADOS ===\n";
    executar("arvore vazia", testeVazia);
    executar("arvore com um elemento", testeUmElemento);
    executar("insercao de valores repetidos", testeDuplicados);
    executar("insercao crescente/decrescente/aleatoria", testeOrdensDeInsercao);
    executar("busca de existentes e inexistentes", testeBusca);
    executar("remocao (folha, 1 filho, 2 filhos, raiz)", testeRemocaoConfiguracoes);
    executar("remocao de elementos inexistentes", testeRemocaoInexistente);
    executar("remover todos os elementos", testeRemoverTudo);
    executar("estresse: 4000 operacoes vs std::set", testeEstresse);
    cout << "\nTodos os testes passaram.\n";

    tabelaComparativa();
    return 0;
}