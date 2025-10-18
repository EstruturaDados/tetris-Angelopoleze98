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

// --- Funções para a Fila Circular ---

// Função para verificar se a fila está vazia.
// Retorna 1 se vazia (size == 0), caso contrário 0.
int queueIsEmpty(int size) {
    return (size == 0);
}

// Função para verificar se a fila está cheia.
// Retorna 1 se cheia (size == capacity), caso contrário 0.
int queueIsFull(int size, int capacity) {
    return (size == capacity);
}

// Função para inserir uma peça no final da fila (enqueue).
// Utiliza lógica de fila circular: rear = (rear + 1) % capacity.
// Atualiza o tamanho da fila e não imprime nada.
void enqueue(struct Peca queue[], int *front, int *rear, int *size, int capacity, struct Peca nova_peca) {
    if (queueIsFull(*size, capacity)) {
        printf("A fila esta cheia. Nao e possivel inserir nova peca.\n");
        return;
    }
    *rear = (*rear + 1) % capacity; // Avança o rear circularmente.
    queue[*rear] = nova_peca; // Insere a peça na posição rear.
    (*size)++; // Incrementa o tamanho.
}

// Função para remover e retornar a peça da frente da fila sem imprimir.
// Utiliza lógica de fila circular: front = (front + 1) % capacity.
// Atualiza o tamanho da fila. Retorna a peça removida ou inválida se vazia.
struct Peca removeFront(struct Peca queue[], int *front, int *rear, int *size, int capacity) {
    struct Peca peca_removida;
    if (queueIsEmpty(*size)) {
        // Caso de erro: retorna uma peça inválida (nome ' ' e id -1).
        peca_removida.nome = ' ';
        peca_removida.id = -1;
        return peca_removida;
    }
    peca_removida = queue[*front]; // Pega a peça da frente.
    *front = (*front + 1) % capacity; // Avança o front circularmente.
    (*size)--; // Decrementa o tamanho.
    return peca_removida;
}

// Função para exibir o estado atual da fila.
// Percorre os elementos de front até front + size - 1, usando módulo para circularidade.
// Formato: [I 0] [L 1] ...
void displayQueue(struct Peca queue[], int front, int size, int capacity) {
    if (queueIsEmpty(size)) {
        printf("Fila de pecas vazia.\n");
        return;
    }
    printf("Fila de pecas\t");
    for (int i = 0; i < size; i++) {
        int index = (front + i) % capacity; // Calcula índice circular.
        printf("[%c %d] ", queue[index].nome, queue[index].id);
    }
    printf("\n");
}

// --- Funções para a Pilha Linear ---

// Função para verificar se a pilha está vazia.
// Retorna 1 se vazia (top == -1), caso contrário 0.
int stackIsEmpty(int top) {
    return (top == -1);
}

// Função para verificar se a pilha está cheia.
// Retorna 1 se cheia (top == capacity - 1), caso contrário 0.
int stackIsFull(int top, int capacity) {
    return (top == capacity - 1);
}

// Função para inserir uma peça no topo da pilha (push).
// Incrementa top e insere na posição top.
void push(struct Peca stack[], int *top, int capacity, struct Peca peca) {
    if (stackIsFull(*top, capacity)) {
        printf("A pilha de reserva esta cheia. Nao e possivel reservar peca.\n");
        return;
    }
    (*top)++; // Incrementa o topo.
    stack[*top] = peca; // Insere a peça no topo.
}

// Função para remover e retornar a peça do topo da pilha (pop).
// Decrementa top e retorna a peça removida ou inválida se vazia.
struct Peca pop(struct Peca stack[], int *top) {
    struct Peca peca_removida;
    if (stackIsEmpty(*top)) {
        // Caso de erro: retorna uma peça inválida (nome ' ' e id -1).
        peca_removida.nome = ' ';
        peca_removida.id = -1;
        return peca_removida;
    }
    peca_removida = stack[*top]; // Pega a peça do topo.
    (*top)--; // Decrementa o topo.
    return peca_removida;
}

// Função para exibir o estado atual da pilha.
// Exibe do topo para a base: for i = top downto 0.
void displayStack(struct Peca stack[], int top) {
    if (stackIsEmpty(top)) {
        printf("Pilha de reserva vazia.\n");
        return;
    }
    printf("Pilha de reserva\t(Topo -> base): ");
    for (int i = top; i >= 0; i--) {
        printf("[%c %d] ", stack[i].nome, stack[i].id);
    }
    printf("\n");
}

// --- Funções Avançadas de Troca ---

// Função para trocar a peça da frente da fila com o topo da pilha.
// Acessa diretamente os índices e troca se a pilha não estiver vazia.
void swapFrontTop(struct Peca queue[], int front, int queue_capacity, struct Peca stack[], int top) {
    if (stackIsEmpty(top)) {
        printf("A pilha de reserva esta vazia. Nao e possivel trocar.\n");
        return;
    }
    int q_idx = front % queue_capacity; // Índice da frente da fila.
    struct Peca temp = queue[q_idx];
    queue[q_idx] = stack[top];
    stack[top] = temp;
    printf("Troca realizada: peça da frente da fila com o topo da pilha.\n");
}

// Função para trocar os 3 primeiros da fila com as 3 peças da pilha.
// Copia temporariamente, inverte a ordem da pilha ao colocar na fila, e copia direta da fila para pilha.
// Isso mantém a lógica de ordem observada no exemplo (reversão da pilha ao transferir).
void swapThree(struct Peca queue[], int front, int queue_size, int queue_capacity, struct Peca stack[], int top) {
    if (queue_size < 3 || top + 1 < 3) {
        printf("Nao ha pecas suficientes para a troca multipla (necessario 3 em cada).\n");
        return;
    }
    // Armazena os 3 primeiros da fila.
    struct Peca temp_q[3];
    for (int i = 0; i < 3; i++) {
        int idx = (front + i) % queue_capacity;
        temp_q[i] = queue[idx];
    }
    // Armazena a pilha (base em [0], topo em [2]).
    struct Peca temp_s[3];
    for (int i = 0; i < 3; i++) {
        temp_s[i] = stack[i];
    }
    // Coloca a pilha invertida nos 3 primeiros da fila (top vira primeiro, base vira terceiro).
    for (int i = 0; i < 3; i++) {
        int idx = (front + i) % queue_capacity;
        queue[idx] = temp_s[3 - 1 - i];
    }
    // Coloca os 3 da fila diretamente na pilha (primeiro vira base, terceiro vira topo).
    for (int i = 0; i < 3; i++) {
        stack[i] = temp_q[i];
    }
    printf("Acao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Função principal do programa.
// Inicializa a fila com 5 peças e a pilha vazia, exibe menu e processa ações até sair.
// Mantém a fila cheia adicionando novas peças após remoções.
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com o tempo atual.

    // Configuração da fila circular.
    const int queue_capacity = 5; // Capacidade fixa da fila.
    struct Peca queue[5]; // Array para armazenar as peças da fila.
    int queue_front = 0; // Índice da frente da fila.
    int queue_rear = -1; // Índice do final da fila (inicia em -1 para vazio).
    int queue_size = 0; // Tamanho atual da fila.

    // Configuração da pilha linear.
    const int stack_capacity = 3; // Capacidade fixa da pilha.
    struct Peca stack[3]; // Array para armazenar as peças da pilha.
    int stack_top = -1; // Índice do topo da pilha (inicia em -1 para vazio).

    // Inicializa a fila com 5 peças geradas automaticamente.
    for (int i = 0; i < 5; i++) {
        enqueue(queue, &queue_front, &queue_rear, &queue_size, queue_capacity, gerarPeca());
    }

    int choice; // Variável para armazenar a escolha do usuário.
    do {
        // Exibe o estado atual da fila e da pilha após cada ação (incluindo inicial).
        printf("Estado atual:\n");
        displayQueue(queue, queue_front, queue_size, queue_capacity);
        displayStack(stack, stack_top);

        // Exibe as opções do menu.
        printf("\nOpcoes disponiveis:\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca da frente da fila\n");
        printf("2\tEnviar peca da fila para a pilha de reserva\n");
        printf("3\tUsar peca da pilha de reserva\n");
        printf("4\tTrocar peca da frente da fila com o topo da pilha\n");
        printf("5\tTrocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0\tSair\n");
        printf("Opcao escolhida: ");
        scanf("%d", &choice); // Lê a escolha do usuário.

        // Processa a escolha usando switch para controle de fluxo.
        switch (choice) {
            case 1: { // Jogar peça: remove da fila e adiciona nova.
                struct Peca peca = removeFront(queue, &queue_front, &queue_rear, &queue_size, queue_capacity);
                if (peca.id != -1) {
                    printf("Peca jogada: [%c %d]\n", peca.nome, peca.id);
                    enqueue(queue, &queue_front, &queue_rear, &queue_size, queue_capacity, gerarPeca());
                } else {
                    printf("A fila esta vazia. Nao e possivel jogar peca.\n");
                }
                break;
            }
            case 2: { // Reservar peça: remove da fila, insere na pilha (se possível) e adiciona nova na fila.
                if (stackIsFull(stack_top, stack_capacity)) {
                    printf("A pilha de reserva esta cheia. Nao e possivel reservar peca.\n");
                    break;
                }
                struct Peca peca = removeFront(queue, &queue_front, &queue_rear, &queue_size, queue_capacity);
                if (peca.id != -1) {
                    push(stack, &stack_top, stack_capacity, peca);
                    printf("Peca reservada: [%c %d]\n", peca.nome, peca.id);
                    enqueue(queue, &queue_front, &queue_rear, &queue_size, queue_capacity, gerarPeca());
                } else {
                    printf("A fila esta vazia. Nao e possivel reservar peca.\n");
                }
                break;
            }
            case 3: { // Usar peça reservada: remove da pilha.
                struct Peca peca = pop(stack, &stack_top);
                if (peca.id != -1) {
                    printf("Peca usada da reserva: [%c %d]\n", peca.nome, peca.id);
                } else {
                    printf("A pilha de reserva esta vazia. Nao e possivel usar peca reservada.\n");
                }
                break;
            }
            case 4: { // Trocar frente da fila com topo da pilha.
                swapFrontTop(queue, queue_front, queue_capacity, stack, stack_top);
                break;
            }
            case 5: { // Trocar os 3 primeiros da fila com as 3 da pilha.
                swapThree(queue, queue_front, queue_size, queue_capacity, stack, stack_top);
                break;
            }
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 0); // Loop continua até escolher 0.

    return 0;
}
