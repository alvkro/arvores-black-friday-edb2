# Amazon Engineering Challenge: Árvores Balanceadas (AVL e Rubro-Negra)

**Autor:** Alvaro Barbosa Filho — 20250028947
**Disciplina:** IMD0039 - Estruturas de Dados Básicas II (Trabalho da Unidade II)

Protótipo em C++ para o sistema de logística da Black Friday: registra, localiza e
remove identificadores de encomendas usando uma **árvore AVL** ou uma **árvore
Rubro-Negra**, escolhida pelo usuário na execução. Os algoritmos de balanceamento
foram implementados do zero (sem bibliotecas prontas).

## Arquivos

| Arquivo | Descrição |
|---|---|
| `arvorebase.hpp` | `Node` e a classe abstrata `ArvoreBase` (busca, em ordem, impressão, altura, contadores) |
| `arvoreavl.hpp` | Árvore AVL |
| `arvorerb.hpp` | Árvore Rubro-Negra (variante Left-Leaning) |
| `main.cpp` | Programa principal (cenários, métricas e demonstração) |
| `testes.cpp` | Testes automatizados e tabelas comparativas |
| `Makefile` | Compilação |

## Compilação e execução

Requisitos: compilador C++17 (`g++` 7+ ou equivalente). `make` é opcional.

```bash
mkdir build
cd build
cmake ..
make      # Gera os executáveis teste e amazon_logistics
```


### Uso

```bash
./amazon_logistics <--avl | --rb> [qtd]
./amazon_logistics --demo
./testes
```

| Argumento | Descrição | Padrão |
|---|---|---|
| `--avl` / `--rb` | Estrutura utilizada | obrigatório |
| `qtd` | Quantidade de encomendas | 10 |
| `--demo` | Cenários pequenos e fixos: inserção, remoção e busca + ilustração das árvores AVL e LLRB | — |

Exemplos:

```bash
./amazon_logistics --avl            # 10 encomendas, mostra a árvore inteira
./amazon_logistics --rb 5000        # 5000 encomendas, mostra só as métricas
```


## Funcionalidades

- **Inserção, busca e remoção** em ambas as árvores.
- **Percurso em ordem** (valores em ordem crescente).
- **Impressão da estrutura**, deitada (filho direito em cima). Na Rubro-Negra cada nó mostra a cor, `(R)` ou `(B)`:

```
     30(B)
20(B)
     10(B)
```

- **Três cenários por execução:** dados não ordenados, crescentes e decrescentes.
- **Métricas:** altura, rotações e (na RB) trocas de cor, depois da inserção, depois da remoção de uma encomenda e depois da remoção de metade dos elementos em ordem aleatória.
- **Verificação das propriedades** (`validar()`) após cada etapa.
- Com `qtd <= 20` imprime a árvore completa; com valores maiores, só as métricas.

## Decisões de implementação

- **Valor repetido:** a inserção é **recusada** (`insert` retorna `false`), a árvore não muda e o programa avisa.
- **Remoção e busca de inexistente:** `remove` retorna `false` e nada é alterado; `search` retorna `false`.
- **Rubro-Negra = LLRB (Left-Leaning Red-Black, Sedgewick):** equivalente a uma árvore 2-3. Propriedades: raiz preta; nenhum vermelho com filho vermelho; mesma altura preta em todos os caminhos; link vermelho só à esquerda. Não precisa de ponteiro para o pai.
- **Contagem de rotações:** cada rotação simples conta 1; uma rotação dupla (LR/RL) conta 2. As trocas de cor (`flipColors`) da RB são contadas à parte.
- **Alturas** em número de níveis (um nó = altura 1; árvore vazia = 0).
- **Verificação de duplicado/inexistente** é feita com uma busca antes da operação (custo extra O(log n), sem mudar a complexidade).
- **Memória:** o destrutor da base libera todos os nós.

### Propriedades verificadas por `validar()`

- **AVL:** ordem da BST, altura armazenada correta em todos os nós e fator de balanceamento entre -1 e 1.
- **Rubro-Negra:** ordem da BST, raiz preta, nenhum vermelho com filho vermelho, nenhum link vermelho à direita e mesma altura preta em todos os caminhos.

## Testes (`./testes`)

As mesmas sequências são aplicadas nas duas árvores, com verificação das propriedades **após cada operação**:

- árvore vazia e árvore com um elemento;
- inserção de valores repetidos;
- inserção crescente, decrescente e aleatória (1000 elementos);
- busca de elementos existentes e inexistentes;
- remoção de folha, nó com um filho, nó com dois filhos e raiz (cada chave removida de várias árvores diferentes);
- remoção de inexistentes e remoção de todos os elementos;
- estresse: 4000 operações aleatórias comparadas com `std::set`.

Em seguida imprime tabelas comparativas de altura e rotações (N = 1.000, 10.000 e 100.000), com uma BST sem balanceamento como referência. Os testes usam `assert`: se alguma condição falhar, o programa aborta mostrando a linha e a descrição da condição; se tudo passar, imprime "Todos os testes passaram." e retorna `0`.

## Complexidade

| Operação | AVL | Rubro-Negra (LLRB) |
|---|---|---|
| Busca | O(log n) | O(log n) |
| Inserção | O(log n) | O(log n) |
| Remoção | O(log n) | O(log n) |
| Altura máxima | ≈ 1,44 · log₂ n | ≈ 2 · log₂ n |

## Referências

- Sedgewick, R.; Wayne, K. *Algorithms*, 4th ed. (Left-Leaning Red-Black BSTs).
- Cormen et al. *Introduction to Algorithms* (Árvores AVL e Rubro-Negras).
- Material da disciplina IMD0039.