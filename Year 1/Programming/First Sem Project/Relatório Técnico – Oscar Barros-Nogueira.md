# Relatório Técnico – Oscar Barros-Nogueira

### Introdução

Este relatório descreve o programa fornecido, cujo principal objetivo é implementar um
jogo de tabuleiro que suporta modos de jogo entre humanos ou entre um humano e a
máquina. O código é escrito em C, utilizando diversas bibliotecas padrão para
manipulação de arquivos, alocação dinâmica de memória e entrada/saída de dados.

## Decisões Técnicas

**Organização do Código**

O código é estruturado em funções para modularidade e reutilização. Principais
componentes incluem:

- **Funções de Manipulação de Memória:** Alocação dinâmica de arrays uni e
    bidimensionais, incluindo tratamento de erros.
- **Lógica do Jogo:** Funções que implementam a jogabilidade, como adicionar
    elementos ao tabuleiro, verificar condições de vitória, e imprimir o estado atual
    do jogo.
- **Sistema de Salvamento:** Funções para salvar e carregar o estado do jogo a
    partir de arquivos.
- **Interface do Utilizador:** Leitura de entradas, validação, e exibição de
    mensagens interativas.

**Algoritmos e Estruturas de Dados**

1. **Struct Offsets:** Gerencia espaçamentos para imprimir o tabuleiro com
    formatação adequada.
2. **Arrays Dinâmicos:** Utilizados para armazenar estados do jogo de maneira
    escalável.
3. **Algoritmos do Jogo:** Incluem verificações para remoção de moedas e controle
    do turno.


**Ferramentas Utilizadas**

- **Linguagem:** C
- **IDE Utilizada:** VS Code.
- **Bibliotecas:**
    o stdio.h: Para entrada/saída de dados e manipulação de arquivos.
    o stdlib.h: Para alocação dinâmica de memória e controle de processos.
    o string.h: Para manipulação de strings, como cópia e comparação.
    o dirent.h: Para manipulação de diretórios e listagem de arquivos.
    o time.h: Para funções relacionadas ao tempo, como randomização.
    o sys/stat.h: Para criar e verificar permissões de diretórios.
    o ctype.h: Para validações de caracteres, como identificação de dígitos.

## Descrição das Funções

**Funções de Manipulação de Memória**

- **allocateArray** : Aloca dinamicamente um array unidimensional de strings.
- **allocate2DArray** : Aloca dinamicamente uma matriz bidimensional de inteiros,
    verificando erros durante o processo.
- **addElementToArray** : Adiciona um elemento ao array, realocando memória se
    necessário.
- **addElementTo2DArray** : Insere um elemento em uma matriz 2D, expandindo-a
    dinamicamente conforme necessário.

**Funções de Interface do Utilizador**

- **printArray** : Exibe um array unidimensional de strings no terminal.
- **print2DArray** : Exibe uma matriz 2D de inteiros no terminal.
- **printCurrentGame** : Exibe o estado atual do jogo, formatando os dados de
    maneira compreensível para o jogador.
- **printGameHistory** : Mostra o histórico completo do jogo desde o início.


**Funções de Lógica do Jogo**

- **checkAndAddPairs** : Verifica combinações possíveis de pares de moedas em
    uma fila.
- **checkPairsBadNim** : Avalia se uma fila contém combinações desfavoráveis no
    contexto do jogo.
- **AI** : Implementa a lógica do computador para realizar movimentos baseados em
    estratégias predefinidas.

**Funções de Sistema de Salvamento**

- **saveGameArray** : Salva o estado atual do jogo em um arquivo, incluindo
    informações sobre filas e moedas.
- **loadGameArray** : Carrega um estado de jogo salvo, reconstruindo a matriz 2D
    correspondente.
- **renameSave** : Permite renomear arquivos de salvamento, validando nomes e
    evitando caracteres inválidos.
- **reloadSave** : Lista arquivos de salvamento disponíveis, permitindo que o usuário
    escolha qual carregar.
- **reloadAutoSave** : Recupera automaticamente o último estado salvo em caso de
    falha ou saída abrupta.

**Funções Auxiliares**

- **determineAndPrintOffsetSeperator** : Calcula e imprime os separadores
    necessários para formatar a saída do tabuleiro.
- **determineAndPrintFrontOffsetWhitespace** : Determina e imprime o espaço
    em branco à esquerda dos números no tabuleiro.
- **determineAndPrintBackOffsetWhitespace** : Determina e imprime o espaço em
    branco à direita dos números no tabuleiro.


## Estrutura de Arquivos

**Arquivo "initialization.ini"**

- **0x0000:** Indica se o último jogo foi fechado corretamente.
    o 1 : Sim
    o 2 : Não
    o 255 (FF): Novo arquivo de inicialização.
- **0x0001:** Último salvamento carregado.

**Arquivo de Salvamento**

- **0x000:** Modo de jogo.
    o 01 : PvP (Jogador vs Jogador).
    o 02 : PvC (Jogador vs Computador).
- **0x001:** Último jogador.
    o 01 : Jogador 1.
    o 02 : Jogador 2.
    o 03 : Computador.
- **0x002:** Número de linhas do tabuleiro.
- **0x003:** Número de colunas do tabuleiro.
- **0x004:** Início da matriz que representa o estado do jogo.

**Instruções para Compilação e Execução**

1. **Compilação** Utilize um compilador como gcc. Execute os seguintes comandos
    no terminal:
2. gcc -c main.c
3. gcc -o main.o main.o
4. gcc -o main main.c

Esses comandos geram o executável correspondente ao nome especificado e também
um chamado main.

5. **Execução** Para iniciar o programa, execute:
6. ./main


7. **Testando as Funcionalidades**
    o Ao iniciar o programa, escolha o modo de jogo:
       ▪ 1 para Humano vs Humano.
       ▪ 2 para Humano vs Computador.
    o Configure as opções de jogo, como número de filas e randomização das
       moedas.
    o Insira os comandos conforme as instruções exibidas, por exemplo:
       ▪ Para remover moedas: Digite o número da fila e a quantidade de
          moedas a remover.
       ▪ Para sair e salvar: Digite exit.
8. **Arquivos de Salvamento**
    o Os jogos são salvos automaticamente em arquivos com extensão .save.
       Esses podem ser recarregados para continuar o jogo posteriormente.
9. **Debugging e Erros**
    o Mensagens de erro e validações são exibidas no terminal em caso de
       entradas inválidas ou problemas com arquivos.

## Considerações Finais

O programa é flexível e robusto, permitindo interações dinâmicas entre os jogadores e
o sistema. Possui funcionalidades de salva e carrega, bem como modos de jogo
variáveis para maior flexibilidade. No entanto, ainda existem alguns problemas
residuais no código que, embora raros, podem ocorrer em situações específicas.


