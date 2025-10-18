#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura para representar uma peça do Tetris.
// Cada peça possui um nome (tipo: 'I', 'O', 'T', 'L') e um ID único.
struct Peca {
    char nome;
    int id;
};

// Variável global para controlar o ID único das peças.
// Inicia em 0 e incrementa a cada nova peça gerada.
int next_id = 0;

// Função para gerar uma nova peça automaticamente.
// Seleciona um tipo aleatório entre 'I', 'O', 'T', 'L' e atribui o próximo ID.
// Utiliza rand() para aleatoriedade.
struct Peca gerarPeca() {
    struct Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L'}; // Array com os tipos possíveis de peças.
    int indice = rand() % 4; // Gera um índice aleatório entre 0 e 3.
    nova_peca.nome = tipos[indice];
    nova_peca.id = next_id++; // Atribui e incrementa o ID global.
    return nova_peca;
}

// Função para verificar se a fila está vazia.
// Retorna 1 se vazia (size == 0), caso contrário 0.
int isEmpty(int size) {
    return (size == 0);
}

// Função para verificar se a fila está cheia.
// Retorna 1 se cheia (size == capacity), caso contrário 0.
int isFull(int size, int capacity) {
    return (size == capacity);
}

// Função para inserir uma peça no final da fila (enqueue).
// Utiliza lógica de fila circular: rear = (rear + 1) % capacity.
// Atualiza o tamanho da fila.
void enqueue(struct Peca queue[], int *front, int *rear, int *size, int capacity, struct Peca nova_peca) {
    if (isFull(*size, capacity)) {
        printf("A fila esta cheia. Nao e possivel inserir nova peca.\n");
        return;
    }
    *rear = (*rear + 1) % capacity; // Avança o rear circularmente.
    queue[*rear] = nova_peca; // Insere a peça na posição rear.
    (*size)++; // Incrementa o tamanho.
    printf("Peca inserida: [%c %d]\n", nova_peca.nome, nova_peca.id);
}

// Função para remover uma peça da frente da fila (dequeue).
// Utiliza lógica de fila circular: front = (front + 1) % capacity.
// Atualiza o tamanho da fila e exibe a peça removida.
void dequeue(struct Peca queue[], int *front, int *rear, int *size, int capacity) {
    if (isEmpty(*size)) {
        printf("A fila esta vazia. Nao e possivel jogar peca.\n");
        return;
    }
    struct Peca peca_removida = queue[*front]; // Pega a peça da frente.
    *front = (*front + 1) % capacity; // Avança o front circularmente.
    (*size)--; // Decrementa o tamanho.
    printf("Peca jogada: [%c %d]\n", peca_removida.nome, peca_removida.id);
}

// Função para exibir o estado atual da fila.
// Percorre os elementos de front até front + size - 1, usando módulo para circularidade.
// Formato: [T 0] [O 1] ...
void display(struct Peca queue[], int front, int size, int capacity) {
    if (isEmpty(size)) {
        printf("Fila de pecas vazia.\n");
        return;
    }
    printf("Fila de pecas: ");
    for (int i = 0; i < size; i++) {
        int index = (front + i) % capacity; // Calcula índice circular.
        printf("[%c %d] ", queue[index].nome, queue[index].id);
    }
    printf("\n");
}

// Função principal do programa.
// Inicializa a fila com 5 peças, exibe menu e processa ações até sair.
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com o tempo atual.

    const int capacity = 5; // Capacidade fixa da fila.
    struct Peca queue[5]; // Array para armazenar as peças (fila circular).
    int front = 0; // Índice da frente da fila.
    int rear = -1; // Índice do final da fila (inicia em -1 para vazio).
    int size = 0; // Tamanho atual da fila.

    // Inicializa a fila com 5 peças geradas automaticamente.
    for (int i = 0; i < 5; i++) {
        enqueue(queue, &front, &rear, &size, capacity, gerarPeca());
    }

    int choice; // Variável para armazenar a escolha do usuário.
    do {
        // Exibe o estado atual da fila após cada ação (incluindo inicial).
        display(queue, front, size, capacity);

        // Exibe as opções do menu.
        printf("\nOpcoes de acao:\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca (dequeue)\n");
        printf("2\tInserir nova peca (enqueue)\n");
        printf("0\tSair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice); // Lê a escolha do usuário.

        // Processa a escolha usando switch para controle de fluxo.
        switch (choice) {
            case 1:
                dequeue(queue, &front, &rear, &size, capacity);
                break;
            case 2:
                enqueue(queue, &front, &rear, &size, capacity, gerarPeca());
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 0); // Loop continua até escolher 0.

    return 0;
}