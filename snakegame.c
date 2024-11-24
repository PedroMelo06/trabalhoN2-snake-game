#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep
#include <time.h> // to include time function
#include <conio.h> // to include getch
#include <windows.h> // forçar tamanho da janela
#include <locale.h> //necessário para usar setlocale
 
char jogador[20]; //nome do jogador

// Key code
enum
{
    KEY_ESC     = 27,
    KEY_ENTER     = 13,
    ARROW_UP    = 256 + 72,
    ARROW_DOWN  = 256 + 80,
    ARROW_LEFT  = 256 + 75,
    ARROW_RIGHT = 256 + 77
};

char menus[5][1000] = {
    "Jogar cobrinha",
    "Ver recordes",
    "Como jogar?",
    "Sair do jogo"};

// Definindo structs para Cobra e Ponto
typedef struct {
    int x;
    int y;
} Posicao;

typedef struct {
    Posicao corpo[38 * 13]; // posições do corpo da cobra
    int tamanho;
} Cobra;

typedef struct {
    Posicao ponto;
} Ponto;

// Funções do código
static int get_code(void);
void comoJogar(void);
void jogarCobrinha(int nivel);
void verRecordes(void);
void generateMenu(int choice);
void generateMenu(int choice){
    int count;
    printf("\n\n\n");
    for (count=0;count<4;count++){
        if(choice==count){
            printf ("\t[*] %s\n",menus[count]);
        }else{
            printf ("\t[ ] %s\n",menus[count]);
        }
    }

    SMALL_RECT windowSize = {0 , 0 , 39 , 12}; // Mudar valores
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);

    int ch;
    while (( ch = get_code()) != KEY_ESC ){
        switch (ch)
        {
        case ARROW_UP:
            system("cls");
            if(choice>0){
                choice=choice-1;
            }
            generateMenu(choice);
            break;
        case ARROW_DOWN:
            system("cls");
            if(choice<3){
                choice=choice+1;
            }
            generateMenu(choice);
            break;
        case ARROW_LEFT:
            printf("LEFT\n");
            break;
        case ARROW_RIGHT:
            printf("RIGHT\n");
            break;
        case KEY_ENTER:
            switch(choice){
                case 0:
                    jogarCobrinha(1);
                    break;
                case 1:
                    verRecordes();
                    break;
                case 2:
                    comoJogar();
                    break;
                case 3:
                    printf("Saindo....");
                    exit(0);
                    break;
            }
            break;
        }
    }
}

void irColunaLinha(int coluna, int linha)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD) {
        coluna,linha
    });
}

void carregarJogo(){
    system("cls");
    printf("Nome do jogador?\n");
    scanf("%[^\n]s", &jogador);
    system("cls");
    printf("Aguarde o jogo ser carregado...\n");
    char load[11] = "carregando";
    for (int i = 0; i < 11; ++i)
    {
        printf("%c ",load[i]);
        Sleep(((rand() % 100) + 350));
    }
    printf("...\nCarregamento concluido\n");
    sleep(1);
    printf("Iniciando...");
    system("cls");
}

void mostrarLimitesMatriz(void){
    system("cls");
    int matriz[12][39];

    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 39; ++j)
        {
            if((j==0) || (i==0) || (i==11) || (j==38)){
                printf("+");
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }    
}

void mostrarGameOver(int pontos){
    FILE *arquivo = fopen("recordes.txt", "a"); // append
    if(arquivo!=NULL){
        fprintf(arquivo, "\n %03dP - %s", pontos,jogador);
        fclose(arquivo);
    }else{
        printf("Nenhum recorde registrado!\n");
    }

    Sleep(600);
    mostrarLimitesMatriz();
    irColunaLinha(4,4);
    printf("==============================");
    irColunaLinha(10,5);
    printf("G A M E  O V E R");
    irColunaLinha(4,6);
    printf("==============================");
    getch();
    system("cls");
    generateMenu(0);    
}

void jogarCobrinha(int nivel){
    Cobra cobra; // Inicializando a cobra com a struct
    Ponto ponto; // Inicializando o ponto
    int PontoPosicaoX, PontoPosicaoY; // Posicoes do ponto
    int i, tamanhoCobra = 15; // Tamanho inicial da cobra
    int pontos = 1; // Pontuação do jogador
    int ch; // código da tecla pressionada
    int velocidade = 300;  // velocidade do jogo
    char teclou; // tecla pressionada
    carregarJogo();
    mostrarLimitesMatriz();

    // Gerar a posição do primeiro ponto
    ponto.ponto.x = (rand() % 37) + 1;
    ponto.ponto.y = (rand() % 10) + 1;

    // Gerar a posição da cobra de forma aleatória
    cobra.corpo[0].x = (rand() % 37) + 1;
    cobra.corpo[0].y = (rand() % 10) + 1;
    irColunaLinha(cobra.corpo[0].x, cobra.corpo[0].y);
    printf("%c",'H');

    ch = get_code(); // Aguardar uma tecla para iniciar
    while (ch != KEY_ESC){
        
        // Limpar instruções
        for (int i = 0; i < 37; ++i)
        {
            irColunaLinha(i,12);
            printf(" ");
        }

        // Verificar se a cobrinha bateu na parede
        if(cobra.corpo[0].y == 0 || cobra.corpo[0].y == 12 || cobra.corpo[0].x == 0 || cobra.corpo[0].x == 38)
        {
            mostrarGameOver(pontos);
        }

        // Acompanhar os movimentos da cobra
        for(i = tamanhoCobra; i > 0; i--){
            cobra.corpo[i] = cobra.corpo[i-1];
        }

        // Apagar rastro
        irColunaLinha(cobra.corpo[tamanhoCobra].x, cobra.corpo[tamanhoCobra].y);
        printf(" ");

        if(kbhit()){
            ch = get_code();
        }

        // Verificar o código da tecla pressionada
        switch (ch){
            case ARROW_UP:
                cobra.corpo[0].y--;
                break;
            case ARROW_DOWN:
                cobra.corpo[0].y++;
                break;
            case ARROW_LEFT:
                cobra.corpo[0].x--;
                break;
            case ARROW_RIGHT:
                cobra.corpo[0].x++;
                break;
            default:
                ch = get_code();
                break;
        }

        // Verificar se a cobrinha comeu o ponto
        if(ponto.ponto.x == cobra.corpo[0].x && ponto.ponto.y == cobra.corpo[0].y){   
            tamanhoCobra++; // aumentar o tamanho da cobra
            pontos++; // aumentar os pontos
            // Gerar o próximo ponto
            ponto.ponto.x = (rand() % 37) + 1;
            ponto.ponto.y = (rand() % 10) + 1;
        }

        // Imprimir a cobrinha
        irColunaLinha(cobra.corpo[0].x, cobra.corpo[0].y);
        printf("%c",'H'); 

        // Atualizar o placar
        irColunaLinha(0, 12);
        printf("Pontos: %d - %s", pontos, jogador);

        Sleep(300);

        // Verificar se a cobra bateu nela mesma
        for(i = 1; i < tamanhoCobra; i++) {
            if(cobra.corpo[0].x == cobra.corpo[i].x && cobra.corpo[0].y == cobra.corpo[i].y){
                mostrarGameOver(pontos);
            }
        }

        // Imprimir o ponto
        irColunaLinha(ponto.ponto.x, ponto.ponto.y);
        printf("%c",'c');
    }

    getch();
    system("cls");
    generateMenu(0);        
}

void verRecordes(void){
    system("cls");
    char texto_str[20];
    FILE *arquivo = fopen("recordes.txt", "r");

    if(arquivo != NULL){
        printf("Listando:\n");
        while(fgets(texto_str, sizeof(texto_str), arquivo)){
            printf("%s", texto_str);
        }
        fclose(arquivo);
    } else {
        printf("Nenhum recorde registrado\n");
    }

    getch();
    system("cls");
    generateMenu(0);
}

void comoJogar(void){
    system("cls");
    printf("Use as setas para mover a cobra.\n");
    printf("Coma os pontos para aumentar o tamanho da cobra.\n");
    printf("Se a cobra bater nas paredes ou nela mesma, o jogo acaba!\n");
    getch();
    system("cls");
    generateMenu(0);
}

int get_code(void){
    int ch;
    ch = getch();
    if(ch == 224) // código para setas
        ch = getch() + 256;
    return ch;
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    generateMenu(0);
    return 0;
}
