#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "arvoreavl.hpp"
#include "arvorerb.hpp"


ArvoreBase* criarArvore(const std::string& estrutura) {
    if (estrutura == "--avl") {
        return new AVL();
    }
    return new RB();
}

// Lote base: ids de 0 a n-1 (o vetor ja chega com o tamanho certo)
void inserir_pedidos(std::vector<int>& pedidos) {
    for (size_t i {0}; i < pedidos.size(); ++i) {
        pedidos[i] = i + 1;
    }
}

// Mesma seed = mesma ordem em toda execucao
void shuffle_pedidos(std::vector<int>& pedidos, int seed) {
    std::mt19937 gerador(seed);
    shuffle(pedidos.begin(), pedidos.end(), gerador);
}

void imprimirMetricas(ArvoreBase* arvore, const std::string& etapa) {
    std::string motivo;
    std::cout << "-> " << etapa << ": altura = " << arvore->height()
         << ", rotacoes = " << arvore->getTotalRotacoes();
    if (arvore->usaCor()) {
        std::cout << ", trocas de cor = " << arvore->getTotalFlips();
    }
    std::cout << " (acumulados)\n";
}

// Remove metade dos pedidos, em ordem aleatoria, e mostra o custo so dessa fase
void removerMetade(ArvoreBase* arvore, const std::vector<int>& lotes, int seed) {
    std::vector<int> remocoes = lotes;
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

    std::cout << "Remocao em massa: " << removidos << " encomendas removidas, "
         << arvore->getTotalRotacoes() - rotacoes_antes << " rotacoes";
    if (arvore->usaCor()) {
        std::cout << ", " << arvore->getTotalFlips() - flips_antes << " trocas de cor";
    }
    std::cout << "\n";
    imprimirMetricas(arvore, "Apos remocao em massa");
}

void executarCenario(ArvoreBase* arvore, const std::vector<int>& lotes,
                     const std::string& nome_cenario, bool detalhado, int seed) {
    std::cout << "\n=== CENARIO: " << nome_cenario << " (" << lotes.size() << " encomendas) ===\n";

    if (lotes.empty()) {
        std::cout << "Lote vazio, nada a fazer.\n";
        return;
    }

    for (int id : lotes) {
        if (!arvore->insert(id)) {
            std::cout << "AVISO: Encomenda " << id << " ja existe no sistema!\n";
        } else if (detalhado) {
            std::cout << "Encomenda " << id << " registada com sucesso.\n";
        }
    }
    imprimirMetricas(arvore, "Apos insercao");

    if (detalhado) {
        arvore->printTree();
        std::cout << "Em ordem: ";
        arvore->printInOrder();
    }

    int pacote_teste = lotes[lotes.size() / 2];
    bool encontrado = arvore->search(pacote_teste);
    bool removido = arvore->remove(pacote_teste);

    if (detalhado) {
        std::cout << "Busca da encomenda " << pacote_teste << ": "
             << (encontrado ? "ENCONTRADA" : "NAO ENCONTRADA") << "\n";
        std::cout << "Remocao da encomenda " << pacote_teste << ": "
             << (removido ? "REMOVIDA" : "NAO REMOVIDA") << "\n";
        arvore->printTree();
    }
    imprimirMetricas(arvore, "Apos remocao de 1 encomenda");

    removerMetade(arvore, lotes, seed);
}

// Mostra a arvore antes e depois de inserir um valor e o custo dessa insercao
void inserirMostrando(ArvoreBase& arvore, int valor) {
    std::cout << "Antes de inserir " << valor << ":\n";
    arvore.printTree();
    arvore.resetContadores();
    arvore.insert(valor);
    std::cout << "Depois de inserir " << valor << " (rotacoes: " << arvore.getTotalRotacoes();
    if (arvore.usaCor()) {
        std::cout << ", trocas de cor: " << arvore.getTotalFlips();
    }
    std::cout << "):\n";
    arvore.printTree();
}

// Cenarios pequenos e fixos, pensados para os slides
void demonstracao() {
    std::cout << "\n============================================\n";
    std::cout << "   DEMONSTRACAO VISUAL: AVL vs LLRB\n";
    std::cout << "============================================\n";

    AVL avl;
    RB rb;

    // 1. INSERCAO
    std::cout << "\n[1] INSERINDO PACOTES: 20, 10, 30, 5, 15, 25, 40\n";
    std::vector<int> pacotes = {20, 10, 30, 5, 15, 25, 40};
    for (int p : pacotes) {
        avl.insert(p);
        rb.insert(p);
    }
    
    std::cout << "\n--- Estrutura AVL ---\n";
    avl.printTree();
    std::cout << "\n--- Estrutura LLRB ---\n";
    rb.printTree();

    // 2. BUSCA
    std::cout << "\n[2] OPERACOES DE BUSCA\n";
    std::cout << "-> Buscando pacote 15 (Existente):\n";
    std::cout << "   AVL: " << (avl.search(15) ? "ENCONTRADO" : "FALHOU") << "\n";
    std::cout << "   LLRB: " << (rb.search(15) ? "ENCONTRADO" : "FALHOU") << "\n";

    std::cout << "-> Buscando pacote 99 (Inexistente):\n";
    std::cout << "   AVL: " << (avl.search(99) ? "ENCONTRADO" : "NAO ENCONTRADO") << "\n";
    std::cout << "   LLRB: " << (rb.search(99) ? "ENCONTRADO" : "NAO ENCONTRADO") << "\n";

    // 3. REMOCAO
    std::cout << "\n[3] REMOCAO DO PACOTE 20 (Nó raiz com 2 filhos)\n";
    avl.remove(20);
    rb.remove(20);

    std::cout << "\n--- AVL Apos Remocao do 20 ---\n";
    avl.printTree();
    std::cout << "\n--- LLRB Apos Remocao do 20 ---\n";
    rb.printTree();
}

void mostrarUso() {
    std::cout << "Uso: ./amazon_logistics <--avl | --llrb> [qtd]\n";
    std::cout << "     ./amazon_logistics --demo\n";
    std::cout << "  qtd   quantidade de encomendas (padrao: 10)\n";
    std::cout << "  seed  semente do embaralhamento (padrao: 42)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "ERRO: Estrutura nao informada.\n";
        mostrarUso();
        return 1;
    }

    std::string estrutura = argv[1];

    if (estrutura == "--demo") {
        demonstracao();
        return 0;
    }

    if (estrutura != "--avl" and estrutura != "--llrb") {
        std::cout << "ERRO: Estrutura desconhecida.\n";
        mostrarUso();
        return 1;
    }

    int qtd = 10;
    int seed = 42;
    try {
        if (argc > 2) {
            qtd = std::stoi(argv[2]);
        }
    } catch (const std::exception&) {
        std::cout << "ERRO: seed deve ser numeros inteiros.\n";
        mostrarUso();
        return 1;
    }

    if (qtd < 1) {
        std::cout << "ERRO: qtd deve ser pelo menos 1.\n";
        return 1;
    }

    std::cout << ">>> INICIANDO SISTEMA COM ARVORE "
         << (estrutura == "--avl" ? "AVL" : "RUBRO-NEGRA")
         << " (qtd = " << qtd << ", seed = " << seed << ") <<<\n";

    // Com poucas encomendas mostra a arvore inteira; com muitas, so as metricas
    bool detalhado = qtd <= 20;

    // Gerar Lote Base de Encomendas
    std::vector<int> lote_base(qtd);
    inserir_pedidos(lote_base);
    shuffle_pedidos(lote_base, seed);

    ArvoreBase* arvore = criarArvore(estrutura);

    // Cenario 1: Nao ordenado
    executarCenario(arvore, lote_base, "Pico de Vendas (Nao Ordenado)", detalhado, seed);
    delete arvore;

    // Cenario 2: Crescente
    arvore = criarArvore(estrutura);
    std::vector<int> lote_crescente = lote_base;
    std::sort(lote_crescente.begin(), lote_crescente.end());
    executarCenario(arvore, lote_crescente, "Falha de Lote (Crescente)", detalhado, seed);
    delete arvore;

    // Cenario 3: Decrescente
    arvore = criarArvore(estrutura);
    std::vector<int> lote_decrescente = lote_base;
    std::sort(lote_decrescente.begin(), lote_decrescente.end(), std::greater<int>());
    executarCenario(arvore, lote_decrescente, "Falha de Lote Invertido (Decrescente)", detalhado, seed);
    delete arvore;

    return 0;
}