#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "arvoreavl.hpp"
#include "arvorerb.hpp"

using namespace std;

ArvoreBase* criarArvore(const string& estrutura) {
    if (estrutura == "--avl") {
        return new AVL();
    }
    return new RB();
}

// Lote base: ids de 0 a n-1 (o vetor ja chega com o tamanho certo)
void inserir_pedidos(vector<int>& pedidos) {
    for (size_t i {0}; i < pedidos.size(); ++i) {
        pedidos[i] = i;
    }
}

// Mesma seed = mesma ordem em toda execucao
void shuffle_pedidos(vector<int>& pedidos, int seed) {
    std::mt19937 gerador(seed);
    shuffle(pedidos.begin(), pedidos.end(), gerador);
}

void imprimirMetricas(ArvoreBase* arvore, const string& etapa) {
    string motivo;
    cout << "-> " << etapa << ": altura = " << arvore->height()
         << ", rotacoes = " << arvore->getTotalRotacoes();
    if (arvore->usaCor()) {
        cout << ", trocas de cor = " << arvore->getTotalFlips();
    }
    cout << " (acumulados)\n";
}

// Remove metade dos pedidos, em ordem aleatoria, e mostra o custo so dessa fase
void removerMetade(ArvoreBase* arvore, const vector<int>& lotes, int seed) {
    vector<int> remocoes = lotes;
    shuffle_pedidos(remocoes, seed);
    remocoes.resize(remocoes.size() / 2);

    int rotacoes_antes = arvore->getTotalRotacoes();
    int flips_antes = arvore->getTotalFlips();

    int removidos = 0;
    for (int id : remocoes) {
        if (arvore->remove(id)) {
            removidos++;
        }
    }

    cout << "Remocao em massa: " << removidos << " encomendas removidas, "
         << arvore->getTotalRotacoes() - rotacoes_antes << " rotacoes";
    if (arvore->usaCor()) {
        cout << ", " << arvore->getTotalFlips() - flips_antes << " trocas de cor";
    }
    cout << "\n";
    imprimirMetricas(arvore, "Apos remocao em massa");
}

void executarCenario(ArvoreBase* arvore, const vector<int>& lotes,
                     const string& nome_cenario, bool detalhado, int seed) {
    cout << "\n=== CENARIO: " << nome_cenario << " (" << lotes.size() << " encomendas) ===\n";

    if (lotes.empty()) {
        cout << "Lote vazio, nada a fazer.\n";
        return;
    }

    for (int id : lotes) {
        if (!arvore->insert(id)) {
            cout << "AVISO: Encomenda " << id << " ja existe no sistema!\n";
        } else if (detalhado) {
            cout << "Encomenda " << id << " registada com sucesso.\n";
        }
    }
    imprimirMetricas(arvore, "Apos insercao");

    if (detalhado) {
        arvore->printTree();
        cout << "Em ordem: ";
        arvore->printInOrder();
    }

    int pacote_teste = lotes[lotes.size() / 2];
    bool encontrado = arvore->search(pacote_teste);
    bool removido = arvore->remove(pacote_teste);

    if (detalhado) {
        cout << "Busca da encomenda " << pacote_teste << ": "
             << (encontrado ? "ENCONTRADA" : "NAO ENCONTRADA") << "\n";
        cout << "Remocao da encomenda " << pacote_teste << ": "
             << (removido ? "REMOVIDA" : "NAO REMOVIDA") << "\n";
        arvore->printTree();
    }
    imprimirMetricas(arvore, "Apos remocao de 1 encomenda");

    removerMetade(arvore, lotes, seed);
}

// Mostra a arvore antes e depois de inserir um valor e o custo dessa insercao
void inserirMostrando(ArvoreBase& arvore, int valor) {
    cout << "Antes de inserir " << valor << ":\n";
    arvore.printTree();
    arvore.resetContadores();
    arvore.insert(valor);
    cout << "Depois de inserir " << valor << " (rotacoes: " << arvore.getTotalRotacoes();
    if (arvore.usaCor()) {
        cout << ", trocas de cor: " << arvore.getTotalFlips();
    }
    cout << "):\n";
    arvore.printTree();
}

// Cenarios pequenos e fixos, pensados para os slides
void demonstracao() {
    cout << "\n##### AVL: rotacao simples (caso LL) #####\n";
    AVL a1;
    a1.insert(30);
    a1.insert(20);
    inserirMostrando(a1, 10);

    cout << "\n##### AVL: rotacao dupla (caso LR) #####\n";
    AVL a2;
    a2.insert(30);
    a2.insert(10);
    inserirMostrando(a2, 20);

    cout << "\n##### Rubro-Negra: rotacao (vermelho a direita) #####\n";
    RB r1;
    r1.insert(10);
    inserirMostrando(r1, 20);

    cout << "\n##### Rubro-Negra: troca de cores (flipColors) #####\n";
    inserirMostrando(r1, 30);

    cout << "\n##### Insercao de 1 a 7 e remocao do 4 (no com dois filhos) #####\n";
    AVL a3;
    RB r3;
    for (int v = 1; v <= 7; v++) {
        a3.insert(v);
        r3.insert(v);
    }
    cout << "AVL:\n";
    a3.printTree();
    cout << "Rubro-Negra:\n";
    r3.printTree();

    a3.remove(4);
    r3.remove(4);
    cout << "AVL apos remover 4:\n";
    a3.printTree();
    cout << "Rubro-Negra apos remover 4:\n";
    r3.printTree();

    cout << "\n##### Valor repetido e remocao de inexistente #####\n";
    cout << "Inserir 5 de novo (AVL): " << (a3.insert(5) ? "inserido" : "recusado (duplicado)") << "\n";
    cout << "Remover 99 (RB): " << (r3.remove(99) ? "removido" : "nao encontrado") << "\n";
    cout << "Buscar 99 (RB): " << (r3.search(99) ? "encontrado" : "nao encontrado") << "\n";
}

void mostrarUso() {
    cout << "Uso: ./amazon_logistics <--avl | --rb> [qtd] [seed]\n";
    cout << "     ./amazon_logistics --demo\n";
    cout << "  qtd   quantidade de encomendas (padrao: 10)\n";
    cout << "  seed  semente do embaralhamento (padrao: 42)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "ERRO: Estrutura nao informada.\n";
        mostrarUso();
        return 1;
    }

    string estrutura = argv[1];

    if (estrutura == "--demo") {
        demonstracao();
        return 0;
    }

    if (estrutura != "--avl" and estrutura != "--rb") {
        cout << "ERRO: Estrutura desconhecida.\n";
        mostrarUso();
        return 1;
    }

    int qtd = 10;
    int seed = 42;
    try {
        if (argc > 2) {
            qtd = std::stoi(argv[2]);
        }
        if (argc > 3) {
            seed = std::stoi(argv[3]);
        }
    } catch (const std::exception&) {
        cout << "ERRO: qtd e seed devem ser numeros inteiros.\n";
        mostrarUso();
        return 1;
    }

    if (qtd < 1) {
        cout << "ERRO: qtd deve ser pelo menos 1.\n";
        return 1;
    }

    cout << ">>> INICIANDO SISTEMA COM ARVORE "
         << (estrutura == "--avl" ? "AVL" : "RUBRO-NEGRA")
         << " (qtd = " << qtd << ", seed = " << seed << ") <<<\n";

    // Com poucas encomendas mostra a arvore inteira; com muitas, so as metricas
    bool detalhado = qtd <= 20;

    // Gerar Lote Base de Encomendas
    vector<int> lote_base(qtd);
    inserir_pedidos(lote_base);
    shuffle_pedidos(lote_base, seed);

    ArvoreBase* arvore = criarArvore(estrutura);

    // Cenario 1: Nao ordenado
    executarCenario(arvore, lote_base, "Pico de Vendas (Nao Ordenado)", detalhado, seed);
    delete arvore;

    // Cenario 2: Crescente
    arvore = criarArvore(estrutura);
    vector<int> lote_crescente = lote_base;
    std::sort(lote_crescente.begin(), lote_crescente.end());
    executarCenario(arvore, lote_crescente, "Falha de Lote (Crescente)", detalhado, seed);
    delete arvore;

    // Cenario 3: Decrescente
    arvore = criarArvore(estrutura);
    vector<int> lote_decrescente = lote_base;
    std::sort(lote_decrescente.begin(), lote_decrescente.end(), std::greater<int>());
    executarCenario(arvore, lote_decrescente, "Falha de Lote Invertido (Decrescente)", detalhado, seed);
    delete arvore;

    return 0;
}