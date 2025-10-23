#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // para usar sleep()
#include <time.h>   // para gerar números aleatórios

//---------------------------------------------------------------------
// Definição da estrutura do mapa (struct)
typedef struct mapa {
    char floresta[50];
    char lago[50];
    char campo[50];
    char caverna[50];
    char montanha[50];
    char pantano[50];
    char cabana[50];
    char vilarejo[50];
    char cemiterio[50];
    char ruinas[50];
} Mapa;

// Estrutura para itens do inventário
typedef struct {
    char nome[30];
    int quantidade;
    int efeito; // valor de cura ou dano
} Item;

// Estrutura para o personagem
typedef struct {
    char nome[50];
    int vida;
    int vidaMaxima;
    int sanidade;
    int fome;
    int sede;
    Item inventario[10];
    int numItens;
    int localizacao; // 0=floresta, 1=lago, etc.
} Personagem;

//---------------------------------------------------------------------
// Variáveis globais
int dano[3][4] = {
    {10, 20, 25, 30}, // Desafios leves
    {13, 25, 35, 40}, // Desafios médios
    {20, 30, 45, 50}, // Desafios pesados
};

// Função para limpar a tela
void limparTela() {
    printf("\033[H\033[J");
}

// Função para adicionar item ao inventário
void adicionarItem(Personagem *p, char nome[30], int quantidade, int efeito) {
    if (p->numItens < 10) {
        strcpy(p->inventario[p->numItens].nome, nome);
        p->inventario[p->numItens].quantidade = quantidade;
        p->inventario[p->numItens].efeito = efeito;
        p->numItens++;
        printf("🎒 Você encontrou: %s x%d\n", nome, quantidade);
    } else {
        printf("🎒 Seu inventário está cheio!\n");
    }
}

// Função para usar item
void usarItem(Personagem *p) {
    if (p->numItens == 0) {
        printf("🎒 Você não tem itens no inventário.\n");
        return;
    }
    
    printf("\n🎒 Seu inventário:\n");
    for (int i = 0; i < p->numItens; i++) {
        printf("%d - %s x%d\n", i+1, p->inventario[i].nome, p->inventario[i].quantidade);
    }
    
    printf("Escolha um item para usar (0 para cancelar): ");
    int escolha;
    scanf("%d", &escolha);
    
    if (escolha > 0 && escolha <= p->numItens) {
        escolha--; // Ajustar para índice do array
        printf("Você usou: %s\n", p->inventario[escolha].nome);
        
        // Aplicar efeito do item
        if (strstr(p->inventario[escolha].nome, "Poção") != NULL) {
            p->vida += p->inventario[escolha].efeito;
            if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            printf("❤️ Sua vida aumentou em %d pontos!\n", p->inventario[escolha].efeito);
        } else if (strstr(p->inventario[escolha].nome, "Comida") != NULL) {
            p->fome += p->inventario[escolha].efeito;
            if (p->fome > 100) p->fome = 100;
            printf("🍖 Sua fome diminuiu em %d pontos!\n", p->inventario[escolha].efeito);
        } else if (strstr(p->inventario[escolha].nome, "Água") != NULL) {
            p->sede += p->inventario[escolha].efeito;
            if (p->sede > 100) p->sede = 100;
            printf("💧 Sua sede diminuiu em %d pontos!\n", p->inventario[escolha].efeito);
        }
        
        // Reduzir quantidade ou remover item
        p->inventario[escolha].quantidade--;
        if (p->inventario[escolha].quantidade <= 0) {
            // Remover item do inventário
            for (int i = escolha; i < p->numItens - 1; i++) {
                p->inventario[i] = p->inventario[i + 1];
            }
            p->numItens--;
        }
    }
}

// Função para exibir status do personagem
void exibirStatus(Personagem p) {
    printf("\n╔════════════════════════════════╗\n");
    printf("║         STATUS DO PERSONAGEM      ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║ Nome: %-25s ║\n", p.nome);
    printf("║ ❤️ Vida: %d/%d %-18s ║\n", p.vida, p.vidaMaxima, "");
    printf("║ 🧠 Sanidade: %d/100 %-15s ║\n", p.sanidade, "");
    printf("║ 🍖 Fome: %d/100 %-17s ║\n", p.fome, "");
    printf("║ 💧 Sede: %d/100 %-17s ║\n", p.sede, "");
    printf("║ 🎒 Itens: %-22d ║\n", p.numItens);
    printf("╚════════════════════════════════╝\n");
}

// Função para exibir animação de digitação
void digitar(char texto[]) {
    for (int i = 0; texto[i] != '\0'; i++) {
        printf("%c", texto[i]);
        fflush(stdout);
        usleep(30000); // Pequena pausa entre caracteres
    }
    printf("\n");
}

// Função para eventos aleatórios na floresta
void eventoFloresta(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🌲 Você encontra pegadas estranhas na terra úmida.\n");
            printf("Elas parecem humanas, mas são muito grandes...\n");
            printf("O que você faz?\n");
            printf("1 - Seguir as pegadas\n");
            printf("2 - Ignorar e seguir em frente\n");
            printf("3 - Tentar esconder-se\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue as pegadas e encontra uma cabana abandonada.\n");
                printf("Dentro, você encontra uma poção de cura!\n");
                adicionarItem(p, "Poção de Cura", 1, 30);
            } else if (escolha == 2) {
                printf("Você continua seu caminho, mas sente que está sendo observado.\n");
                p->sanidade -= 5;
            } else {
                printf("Você se esconde atrás de uma árvore grossa.\n");
                printf("Após alguns minutos, você ouve passos pesados passando perto de você.\n");
                printf("🤫 Você escapou por pouco!\n");
            }
            break;
            
        case 1:
            printf("🦉 Uma coruja pousa em um galho acima de você.\n");
            printf("Ela parece observar com inteligência incomum.\n");
            printf("De repente, ela fala: 'Cuidado com as sombras que dançam sem música.'\n");
            printf("Antes que você possa reagir, ela voa para longe.\n");
            p->sanidade -= 10;
            break;
            
        case 2:
            printf("🍄 Você encontra cogumelos brilhantes no chão da floresta.\n");
            printf("Eles parecem mágicos e emitem uma luz suave.\n");
            printf("O que você faz?\n");
            printf("1 - Coletar os cogumelos\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Comer um cogumelo\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você coleta cuidadosamente os cogumelos.\n");
                adicionarItem(p, "Cogumelo Mágico", 3, 15);
            } else if (escolha == 2) {
                printf("Você decide não arriscar e continua seu caminho.\n");
            } else {
                printf("Você come um cogumelo e sente uma estranha sensação.\n");
                printf("Por um momento, você consegue ver as cores das emoções das árvores.\n");
                p->sanidade -= 15;
                p->vida += 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 3:
            printf("🌧️ Começa a chover repentinamente.\n");
            printf("A chuva é estranhamente fria e escura.\n");
            printf("Você precisa encontrar abrigo rapidamente.\n");
            printf("O que você faz?\n");
            printf("1 - Procurar abrigo sob árvores densas\n");
            printf("2 - Continuar caminhando na chuva\n");
            printf("3 - Tentar construir um abrigo improvisado\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você encontra um grupo de árvores densas que oferecem alguma proteção.\n");
                printf("Enquanto espera a chuva passar, você encontra um pequeno baú escondido.\n");
                adicionarItem(p, "Comida Enlatada", 2, 20);
            } else if (escolha == 2) {
                printf("Você continua na chuva e começa a sentir frio.\n");
                p->vida -= 10;
                p->sede += 20;
            } else {
                printf("Você constrói um abrigo básico com galhos e folhas.\n");
                printf("Não é perfeito, mas te mantém relativamente seco.\n");
                p->sede += 10;
            }
            break;
            
        case 4:
            printf("🌳 Você encontra uma árvore antiga com runas estranhas gravadas no tronco.\n");
            printf("Quando você toca as runas, visões de um passado distante inundam sua mente.\n");
            printf("Você vê pessoas vestidas de maneira estranha realizando um ritual...\n");
            printf("De repente, as visões param e você se sente exausto.\n");
            p->sanidade -= 20;
            p->vida -= 5;
            printf("Mas algo mudou em você... você sente que entende melhor este lugar.\n");
            break;
    }
}

// Função para eventos aleatórios no lago
void eventoLago(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("💧 A água do lago é estranhamente clara e convidativa.\n");
            printf("Você sente uma vontade irresistível de mergulhar.\n");
            printf("O que você faz?\n");
            printf("1 - Mergulhar nas águas\n");
            printf("2 - Apenas beber um pouco da água\n");
            printf("3 - Ignorar e contornar o lago\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você mergulha nas águas frias.\n");
                printf("Sob a superfície, você vê ruínas submersas e criaturas que não deveriam existir.\n");
                printf("Quando emerge, você sente que mudou de alguma forma.\n");
                p->sanidade -= 25;
                p->sede = 100;
                adicionarItem(p, "Artefato Estranho", 1, 0);
            } else if (escolha == 2) {
                printf("Você bebe um pouco da água e sente-se revigorado.\n");
                p->sede = 100;
                p->vida += 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            } else {
                printf("Você decide não arriscar e contorna o lago.\n");
                printf("Enquanto caminha, você sente olhos observando você das profundezas.\n");
                p->sanidade -= 5;
            }
            break;
            
        case 1:
            printf("🐟 Você vê peixes brilhando nadando perto da superfície.\n");
            printf("Eles parecem mágicos e quase parecem sorrir para você.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar pescar\n");
            printf("2 - Apenas observar\n");
            printf("3 - Tentar comunicar-se com eles\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você consegue capturar um dos peixes brilhantes.\n");
                printf("Quando o toca, ele se transforma em uma poção de cura!\n");
                adicionarItem(p, "Poção Peixe-dourado", 1, 40);
            } else if (escolha == 2) {
                printf("Você observa os peixes por um tempo.\n");
                printf("Eles parecem dançar uma coreografia antiga.\n");
                p->sanidade += 5;
            } else {
                printf("Você tenta comunicar-se com os peixes.\n");
                printf("Para sua surpresa, eles respondem com imagens em sua mente.\n");
                printf("Você vê visões de um mundo subaquático com cidades e criaturas incríveis.\n");
                p->sanidade -= 15;
                p->sede += 10;
            }
            break;
            
        case 2:
            printf("🚤 Você encontra um pequeno barco abandonado na margem do lago.\n");
            printf("Parece antigo, mas ainda em condições de uso.\n");
            printf("O que você faz?\n");
            printf("1 - Usar o barco para explorar o lago\n");
            printf("2 - Ignorar o barco e continuar a pé\n");
            printf("3 - Revistar o barco em busca de itens úteis\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você rema para o centro do lago.\n");
                printf("Lá, você encontra uma pequena ilha com uma estátua misteriosa.\n");
                printf("Ao tocar a estátua, você sente um fluxo de energia.\n");
                p->vida = p->vidaMaxima;
                p->sanidade += 10;
            } else if (escolha == 2) {
                printf("Você decide não arriscar e continua a pé.\n");
                printf("Enquanto caminha, você ouve um som vindo do barco, como se alguém estivesse rindo.\n");
                p->sanidade -= 5;
            } else {
                printf("Você revista o barco e encontra alguns itens úteis.\n");
                adicionarItem(p, "Comida Seca", 2, 15);
                adicionarItem(p, "Garrafa de Água", 1, 30);
            }
            break;
            
        case 3:
            printf("🌊 A superfície do lago começa a ondular estranhamente, sem vento.\n");
            printf("Formações parecem surgir e desaparecer na água.\n");
            printf("De repente, uma figura emerge do lago - parece humana, mas feita de água.\n");
            printf("A figura estende uma mão líquida para você.\n");
            printf("O que você faz?\n");
            printf("1 - Aceitar o toque da figura\n");
            printf("2 - Recuar e fugir\n");
            printf("3 - Tentar comunicar-se com a figura\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você aceita o toque da figura de água.\n");
                printf("Sensações estranhas percorrem seu corpo - você sente o fluxo do tempo e da vida.\n");
                p->vida += 20;
                p->sanidade -= 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            } else if (escolha == 2) {
                printf("Você recua e foge da figura de água.\n");
                printf("Quando olha para trás, a figura desapareceu nas profundezas do lago.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta comunicar-se com a figura.\n");
                printf("Ela responde com imagens de uma civilização antiga que vivia sob as águas.\n");
                printf("Você aprende sobre os segredos deste lugar.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Conhecimento Antigo", 1, 0);
            }
            break;
            
        case 4:
            printf("🌅 O sol começa a se pôr sobre o lago, pintando o céu com cores incríveis.\n");
            printf("A paisagem é tão bonita que você quase esquece os perigos deste lugar.\n");
            printf("Enquanto observa o pôr do sol, você encontra uma pequena cabana à beira do lago.\n");
            printf("Parece abandonada, mas talvez haja algo útil lá dentro.\n");
            printf("O que você faz?\n");
            printf("1 - Explorar a cabana\n");
            printf("2 - Continuar pelo lago\n");
            printf("3 - Acampar aqui e explorar a cabana amanhã\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você entra na cabana e encontra diários antigos.\n");
                printf("Eles falam sobre criaturas que emergem do lago durante a noite.\n");
                printf("Você também encontra alguns suprimentos.\n");
                adicionarItem(p, "Lanterna", 1, 0);
                adicionarItem(p, "Comida Enlatada", 1, 20);
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você decide continuar pelo lago enquanto ainda há luz.\n");
                printf("Enquanto caminha, você ouve sons estranhos vindo da direção da cabana.\n");
                p->sanidade -= 5;
            } else {
                printf("Você decide acampar e explorar a cabana amanhã.\n");
                printf("Durante a noite, você ouve barulhos estranhos vindos da cabana.\n");
                printf("Pela manhã, você encontra pegadas misteriosas ao redor de seu acampamento.\n");
                p->sanidade -= 15;
                p->fome -= 10;
                p->sede -= 10;
            }
            break;
    }
}

// Função para eventos aleatórios na caverna
void eventoCaverna(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🦇 Morcegos saem voando de uma passagem escura quando você se aproxima.\n");
            printf("Eles parecem maiores que o normal e seus olhos brilham com uma luz vermelha estranha.\n");
            printf("O que você faz?\n");
            printf("1 - Seguir pela passagem de onde vieram os morcegos\n");
            printf("2 - Procurar outra passagem\n");
            printf("3 - Acender uma tocha e explorar cuidadosamente\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue pela passagem escura.\n");
                printf("Após alguns metros, você encontra uma câmara com cristais brilhantes.\n");
                printf("Eles emitem uma luz suave e parecem pulsar com energia.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Cristal Energético", 2, 20);
            } else if (escolha == 2) {
                printf("Você procura outra passagem e encontra um túnel inclinado.\n");
                printf("Ao descer, você chega a um pequeno lago subterrâneo.\n");
                p->sede = 100;
            } else {
                printf("Você acende uma tocha e explora cuidadosamente.\n");
                printf("Com a luz, você percebe que as paredes estão cobertas de pinturas antigas.\n");
                printf("Elas parecem contar uma história sobre uma civilização que vivia aqui.\n");
                p->sanidade -= 5;
                adicionarItem(p, "Tocha", 3, 0);
            }
            break;
            
        case 1:
            printf("👂 Você ouve sussurros vindos das profundezas da caverna.\n");
            printf("Parecem estar chamando seu nome, mas em uma língua que você não conhece.\n");
            printf("O que você faz?\n");
            printf("1 - Seguir em direção aos sussurros\n");
            printf("2 - Ignorar e continuar em outra direção\n");
            printf("3 - Tentar responder aos sussurros\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue em direção aos sussurros.\n");
                printf("Eles ficam mais altos à medida que você avança.\n");
                printf("Finalmente, você chega a uma câmara onde os sussurros parecem vir de todos os lados.\n");
                printf("No centro da câmara, há um altar com um livro estranho.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Livro Sombrio", 1, 0);
            } else if (escolha == 2) {
                printf("Você decide ignorar os sussurros e continuar em outra direção.\n");
                printf("Mesmo assim, os sussurros parecem seguir você, como se estivessem em sua mente.\n");
                p->sanidade -= 10;
            } else {
                printf("Você tenta responder aos sussurros na mesma língua estranha.\n");
                printf("Para sua surpresa, eles respondem!\n");
                printf("Você entende fragmentos de uma história antiga sobre seres que vivem nas sombras.\n");
                p->sanidade -= 15;
                p->vida += 5;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 2:
            printf("💎 Você encontra veios de minerais brilhantes nas paredes da caverna.\n");
            printf("Eles emitem uma luz fraca e parecem valiosos.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar extrair os minerais\n");
            printf("2 - Apenas observar e continuar\n");
            printf("3 - Tocar nos minerais\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta extrair os minerais com uma pedra.\n");
                printf("Consegue extrair alguns pedaços que brilham no escuro.\n");
                adicionarItem(p, "Mineral Luminoso", 3, 0);
            } else if (escolha == 2) {
                printf("Você apenas observa os minerais e continua seu caminho.\n");
                printf("Enquanto se afasta, você sente uma estranha sensação de perda.\n");
            } else {
                printf("Você toca nos minerais e sente uma descarga de energia.\n");
                printf("Visões de um futuro possível passam por sua mente.\n");
                p->sanidade -= 10;
                p->vida += 15;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 3:
            printf("🕸️ Você encontra teias enormes bloqueando uma passagem.\n");
            printf("Elas são mais grossas que cordas e parecem recentes.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar cortar as teias\n");
            printf("2 - Procurar outra passagem\n");
            printf("3 - Queimar as teias\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta cortar as teias com uma pedra afiada.\n");
                printf("Elas são surpreendentemente resistentes, mas você consegue abrir um caminho.\n");
                printf("Do outro lado, você encontra restos de criaturas desconhecidas.\n");
                p->sanidade -= 5;
            } else if (escolha == 2) {
                printf("Você procura outra passagem e encontra um túnel estreito.\n");
                printf("Ao passar por ele, você ouve sons de algo se movendo atrás de você.\n");
                p->sanidade -= 10;
            } else {
                printf("Você usa uma tocha para queimar as teias.\n");
                printf("Elas queimam rapidamente, revelando uma câmara com ovos estranhos.\n");
                printf("Você decide não ficar para ver o que eclode deles.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Tocha", 2, 0);
            }
            break;
            
        case 4:
            printf("🗿 Você encontra estátuas antigas em uma câmara da caverna.\n");
            printf("Elas representam figuras humanoides com características que não são humanas.\n");
            printf("Seus olhos parecem seguir você enquanto você se move.\n");
            printf("O que você faz?\n");
            printf("1 - Examinar as estátuas cuidadosamente\n");
            printf("2 - Ignorar e continuar explorando\n");
            printf("3 - Tentar mover as estátuas\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você examina as estátuas e encontra inscrições em uma língua desconhecida.\n");
                printf("Quando você toca as inscrições, as estátuas parecem ganhar vida por um momento.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Fragmento de Conhecimento", 1, 0);
            } else if (escolha == 2) {
                printf("Você decide ignorar as estátuas e continuar explorando.\n");
                printf("Mesmo assim, você sente os olhos delas em suas costas.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta mover uma das estátuas.\n");
                printf("Ela é surpreendentemente leve e se move facilmente, revelando uma passagem secreta.\n");
                printf("A passagem leva a uma câmara com tesouros antigos.\n");
                adicionarItem(p, "Artefato Antigo", 1, 0);
                adicionarItem(p, "Poção de Cura", 2, 30);
                p->sanidade -= 10;
            }
            break;
    }
}

// Função para eventos aleatórios na montanha
void eventoMontanha(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🦅 Uma águia gigante sobrevoa a montanha e parece observar você.\n");
            printf("Ela é maior que qualquer ave que você já viu.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar se esconder\n");
            printf("2 - Acenar para a águia\n");
            printf("3 - Continuar subindo, ignorando a águia\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você se esconde atrás de algumas rochas.\n");
                printf("A águia pousa perto e parece procurar por algo.\n");
                printf("Após alguns minutos, ela voa para longe, deixando cair uma pena brilhante.\n");
                adicionarItem(p, "Pena Mágica", 1, 0);
            } else if (escolha == 2) {
                printf("Você acena para a águia.\n");
                printf("Para sua surpresa, ela responde com um grasnido que parece quase uma palavra.\n");
                printf("Ela então voa em círculos acima de você antes de desaparecer nas nuvens.\n");
                p->sanidade -= 10;
            } else {
                printf("Você continua subindo, ignorando a águia.\n");
                printf("Ela segue você por um tempo antes de desaparecer.\n");
                printf("Mais tarde, você encontra um ninho com ovos estranhos.\n");
                p->sanidade -= 5;
                adicionarItem(p, "Ovo Misterioso", 2, 0);
            }
            break;
            
        case 1:
            printf("🌨️ Uma neblina espessa começa a descer a montanha.\n");
            printf("Ela é tão densa que você mal consegue ver seus próprios pés.\n");
            printf("O que você faz?\n");
            printf("1 - Procurar abrigo\n");
            printf("2 - Continuar subindo mesmo com a neblina\n");
            printf("3 - Tentar descer a montanha\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você procura abrigo e encontra uma pequena gruta na montanha.\n");
                printf("Dentro, você encontra sinais de que alguém já esteve aqui antes.\n");
                adicionarItem(p, "Comida Desidratada", 1, 15);
                adicionarItem(p, "Mapa Antigo", 1, 0);
            } else if (escolha == 2) {
                printf("Você continua subindo mesmo com a neblina densa.\n");
                printf("É difícil caminhar sem ver para onde vai, e você tropeça várias vezes.\n");
                p->vida -= 15;
                p->sanidade -= 10;
            } else {
                printf("Você tenta descer a montanha na neblina.\n");
                printf("É um caminho perigoso, e você escorrega várias vezes.\n");
                p->vida -= 10;
                p->sanidade -= 5;
            }
            break;
            
        case 2:
            printf("🏔️ Você encontra uma trilha estreita que leva a um pico mais alto.\n");
            printf("Do alto, você pode ver toda a ilha misteriosa.\n");
            printf("O que você faz?\n");
            printf("1 - Seguir a trilha até o pico\n");
            printf("2 - Ignorar a trilha e continuar pelo caminho atual\n");
            printf("3 - Procurar por atalhos para o pico\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue a trilha até o pico.\n");
                printf("A vista é incrível - você pode ver todos os locais da ilha.\n");
                printf("No topo, você encontra um altar antigo com oferendas recentes.\n");
                p->sanidade += 10;
                adicionarItem(p, "Amuleto da Montanha", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora a trilha e continua pelo caminho atual.\n");
                printf("Enquanto caminha, você sente que perdeu algo importante.\n");
                p->sanidade -= 5;
            } else {
                printf("Você procura por atalhos para o pico.\n");
                printf("Encontra um caminho perigoso, mas mais rápido.\n");
                printf("Ao chegar ao topo, você vê figuras estranhas dançando ao redor do altar.\n");
                p->sanidade -= 20;
                p->vida += 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 3:
            printf("🌨️ Começa a nevar na montanha.\n");
            printf("Os flocos são grandes e brilhantes, quase mágicos.\n");
            printf("O que você faz?\n");
            printf("1 - Procurar abrigo imediatamente\n");
            printf("2 - Continuar explorando apesar da neve\n");
            printf("3 - Tentar coletar a neve mágica\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você procura abrigo e encontra uma pequena cabana de montanheses.\n");
                printf("Dentro, há suprimentos e um diário com anotações estranhas.\n");
                adicionarItem(p, "Comida Quente", 1, 25);
                adicionarItem(p, "Diário do Montanhês", 1, 0);
            } else if (escolha == 2) {
                printf("Você continua explorando apesar da neve.\n");
                printf("A neve é fria, mas parece ter propriedades estranhas.\n");
                p->vida -= 10;
                p->sanidade -= 5;
            } else {
                printf("Você tenta coletar a neve mágica.\n");
                printf("Quando toca os flocos, eles brilham e se transformam em cristais.\n");
                adicionarItem(p, "Cristal de Neve", 3, 0);
                p->sanidade -= 10;
            }
            break;
            
        case 4:
            printf("🗻 Você encontra formações rochosas estranhas na montanha.\n");
            printf("Elas parecem ter sido esculpidas por mãos antigas.\n");
            printf("O que você faz?\n");
            printf("1 - Examinar as formações rochosas\n");
            printf("2 - Ignorar e continuar subindo\n");
            printf("3 - Tentar escalar as formações\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você examina as formações rochosas e encontra símbolos antigos.\n");
                printf("Eles parecem contar uma história sobre deuses que viviam na montanha.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Tábua de Runas", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora as formações e continua subindo.\n");
                printf("Enquanto caminha, você sente que está sendo observado pelas rochas.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta escalar as formações rochosas.\n");
                printf("É difícil, mas você consegue chegar ao topo de uma delas.\n");
                printf("De lá, você vê uma caverna secreta que não era visível de baixo.\n");
                adicionarItem(p, "Cristal da Montanha", 2, 0);
                p->sanidade -= 15;
            }
            break;
    }
}

// Função para eventos aleatórios no pantano
void eventoPantano(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🐸 Você ouve cantos de sapos estranhos vindo de todas as direções.\n");
            printf("Eles não soam como sapos normais - parecem formar uma melodia sinistra.\n");
            printf("O que você faz?\n");
            printf("1 - Seguir em direção aos cantos\n");
            printf("2 - Tentar abafar o som e seguir em outra direção\n");
            printf("3 - Tentar imitar os cantos\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue em direção aos cantos dos sapos.\n");
                printf("Eles ficam mais altos à medida que você avança.\n");
                printf("Finalmente, você chega a um pequeno lago onde os sapos parecem dançar.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Pó de Sapo Mágico", 2, 0);
            } else if (escolha == 2) {
                printf("Você tenta abafar o som e segue em outra direção.\n");
                printf("Mesmo assim, os cantos parecem seguir você, como se estivessem em sua mente.\n");
                p->sanidade -= 10;
            } else {
                printf("Você tenta imitar os cantos dos sapos.\n");
                printf("Para sua surpresa, eles respondem!\n");
                printf("Os sapos se aproximam e deixam um pequeno objeto brilhante antes de saltar para a água.\n");
                p->sanidade -= 5;
                adicionarItem(p, "Amuleto do Pantano", 1, 0);
            }
            break;
            
        case 1:
            printf("🌿 Você encontra plantas estranhas que brilham no escuro do pantano.\n");
            printf("Elas emitem uma luz verde e parecem pulsar com vida.\n");
            printf("O que você faz?\n");
            printf("1 - Coletar algumas plantas\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Tentar comer uma das plantas\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você coleta algumas das plantas brilhantes.\n");
                printf("Elas continuam brilhando em suas mãos.\n");
                adicionarItem(p, "Planta Luminosa", 3, 0);
            } else if (escolha == 2) {
                printf("Você ignora as plantas e continua seu caminho.\n");
                printf("Enquanto se afasta, você sente uma estranha saudade delas.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta comer uma das plantas.\n");
                printf("Ela tem um gosto estranho, mas você sente uma onda de energia.\n");
                p->vida += 15;
                p->sanidade -= 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 2:
            printf("👻 Você vê figuras sombrias se movendo entre as árvores do pantano.\n");
            printf("Elas parecem etéreas e não fazem som ao se mover.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar se esconder\n");
            printf("2 - Aproximar-se das figuras\n");
            printf("3 - Ignorar e continuar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você se esconde atrás de árvores grossas e observa as figuras.\n");
                printf("Elas parecem procurar por algo antes de desaparecerem na névoa.\n");
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você se aproxima das figuras sombrias.\n");
                printf("Quando chega perto, elas se viram e você vê rostos humanos expressando tristeza.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Lágrima Espectral", 1, 0);
            } else {
                printf("Você ignora as figuras e continua seu caminho.\n");
                printf("Mesmo assim, você sente os olhos delas em suas costas.\n");
                p->sanidade -= 5;
            }
            break;
            
        case 3:
            printf("🐊 Você vê olhos brilhando na água escura do pantano.\n");
            printf("Eles parecem seguir seus movimentos.\n");
            printf("O que você faz?\n");
            printf("1 - Afastar-se da água\n");
            printf("2 - Tentar comunicar-se com a criatura\n");
            printf("3 - Jogar algo na água\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você se afasta da água e os olhos desaparecem.\n");
                printf("Mais tarde, você encontra pegadas de algo grande na lama.\n");
                p->sanidade -= 5;
            } else if (escolha == 2) {
                printf("Você tenta comunicar-se com a criatura na água.\n");
                printf("Ela responde com sons baixos que parecem formar palavras em uma língua antiga.\n");
                p->sanidade -= 15;
                p->sede += 10;
            } else {
                printf("Você joga uma pedra na água.\n");
                printf("A criatura emerge - é um crocodilo enorme, mas com olhos inteligentes.\n");
                printf("Ele observa você por um momento antes de mergulhar novamente.\n");
                p->sanidade -= 10;
                p->vida -= 5;
            }
            break;
            
        case 4:
            printf("🏚️ Você encontra ruínas antigas no meio do pantano.\n");
            printf("Elas parecem ser de um templo ou estrutura cerimonial.\n");
            printf("O que você faz?\n");
            printf("1 - Explorar as ruínas\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Procurar por entradas subterrâneas\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você explora as ruínas e encontra inscrições estranhas.\n");
                printf("Elas falam sobre rituais realizados para apaziguar as entidades do pantano.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Fragmento de Ritual", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora as ruínas e continua seu caminho.\n");
                printf("Enquanto se afasta, você ouve sussurros vindos das ruínas.\n");
                p->sanidade -= 5;
            } else {
                printf("Você procura por entradas subterrâneas nas ruínas.\n");
                printf("Encontra uma escada que leva a uma câmara subterrânea.\n");
                printf("Lá, você encontra oferendas recentes e símbolos estranhos.\n");
                adicionarItem(p, "Artefato do Templo", 1, 0);
                p->sanidade -= 15;
            }
            break;
    }
}

// Função para eventos aleatórios na cabana
void eventoCabana(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🚪 A porta da cabana range ao abrir, como se não fosse usada há muito tempo.\n");
            printf("Dentro, você encontra móveis cobertos de poeira e teias de aranha.\n");
            printf("O que você faz?\n");
            printf("1 - Explorar cuidadosamente cada cômodo\n");
            printf("2 - Procurar por alimentos ou suprimentos\n");
            printf("3 - Procurar por documentos ou diários\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você explora cada cômodo cuidadosamente.\n");
                printf("No quarto, você encontra uma caixa de música estranha.\n");
                printf("Quando você a abre, ela toca uma melodia que parece familiar de alguma forma.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Caixa de Música", 1, 0);
            } else if (escolha == 2) {
                printf("Você procura por alimentos ou suprimentos.\n");
                printf("Na cozinha, encontra comida enlatada e água engarrafada.\n");
                adicionarItem(p, "Comida Enlatada", 3, 20);
                adicionarItem(p, "Garrafa de Água", 2, 30);
            } else {
                printf("Você procura por documentos ou diários.\n");
                printf("Encontra um diário com anotações sobre criaturas estranhas na ilha.\n");
                printf("As últimas páginas estão manchadas e parecem ter sido escritas com pressa.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Diário do Habitante", 1, 0);
            }
            break;
            
        case 1:
            printf("🖼️ Você encontra fotografias antigas na parede da cabana.\n");
            printf("Elas mostram pessoas sorrindo, mas há algo estranho em seus olhos.\n");
            printf("O que você faz?\n");
            printf("1 - Examinar as fotografias cuidadosamente\n");
            printf("2 - Ignorar e continuar explorando\n");
            printf("3 - Levar algumas fotografias\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você examina as fotografias cuidadosamente.\n");
                printf("Nota que em todas elas, há uma figura sombria no fundo que parece observar.\n");
                p->sanidade -= 15;
            } else if (escolha == 2) {
                printf("Você ignora as fotografias e continua explorando.\n");
                printf("Mesmo assim, você sente os olhos das fotos seguindo você.\n");
                p->sanidade -= 5;
            } else {
                printf("Você decide levar algumas fotografias.\n");
                printf("Quando as tira da parede, você nota que as pessoas nas fotos parecem se mover.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Fotografia Misteriosa", 2, 0);
            }
            break;
            
        case 2:
            printf("🔮 Você encontra um objeto estranho em uma prateleira.\n");
            printf("Parece uma bola de cristal, mas dentro dela há algo se movendo.\n");
            printf("O que você faz?\n");
            printf("1 - Pegar o objeto\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Tentar limpar o objeto\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você pega o objeto e sente uma estranha sensação.\n");
                printf("Visões de eventos passados e futuros inundam sua mente.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Bola de Cristal", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora o objeto e continua explorando.\n");
                printf("Mesmo assim, você sente uma estranha atração por ele.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta limpar o objeto.\n");
                printf("Quando o toca, ele brilha e você ouve sussurros em uma língua desconhecida.\n");
                p->sanidade -= 15;
                p->vida += 10;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
            
        case 3:
            printf("📻 Você encontra um rádio antigo em um canto da cabana.\n");
            printf("Parece estar funcionando, apesar da idade.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar ligar o rádio\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Levar o rádio\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta ligar o rádio.\n");
                printf("Ele funciona, mas em vez de música, você ouve vozes sussurrando seu nome.\n");
                p->sanidade -= 15;
            } else if (escolha == 2) {
                printf("Você ignora o rádio e continua explorando.\n");
                printf("Mesmo assim, você sente que está perdendo algo importante.\n");
                p->sanidade -= 5;
            } else {
                printf("Você decide levar o rádio.\n");
                printf("Quando o pega, ele liga sozinho e toca uma melodia estranha.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Rádio Misterioso", 1, 0);
            }
            break;
            
        case 4:
            printf("🕰️ Você encontra um relógio de parede parado.\n");
            printf("Ele marca uma hora estranha - 13:13.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar ajustar o relógio\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Tentar fazer o relógio funcionar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta ajustar o relógio.\n");
                printf("Quando move os ponteiros, você sente uma estranha sensação de déjà vu.\n");
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você ignora o relógio e continua explorando.\n");
                printf("Mesmo assim, você sente que o tempo está se comportando de forma estranha.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta fazer o relógio funcionar.\n");
                printf("Quando ele começa a funcionar, você ouve batidas que parecem vir de todos os lados.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Pêndulo do Tempo", 1, 0);
            }
            break;
    }
}

// Função para eventos aleatórios no vilarejo
void eventoVilarejo(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🏘️ Você entra no vilarejo abandonado.\n");
            printf("As casas estão em ruínas, mas parece que alguém ainda vive aqui.\n");
            printf("O que você faz?\n");
            printf("1 - Explorar as casas uma por uma\n");
            printf("2 - Procurar por sinais de vida recente\n");
            printf("3 - Procurar por suprimentos\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você explora as casas uma por uma.\n");
                printf("Na última casa, você encontra um altar estranho com oferendas recentes.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Amuleto do Vilarejo", 1, 0);
            } else if (escolha == 2) {
                printf("Você procura por sinais de vida recente.\n");
                printf("Encontra pegadas frescas e fumaça saindo de uma chaminé.\n");
                printf("Quando se aproxima, a fumaça desaparece misteriosamente.\n");
                p->sanidade -= 15;
            } else {
                printf("Você procura por suprimentos nas casas abandonadas.\n");
                printf("Encontra comida enlatada, água e alguns medicamentos.\n");
                adicionarItem(p, "Comida Enlatada", 2, 20);
                adicionarItem(p, "Garrafa de Água", 2, 30);
                adicionarItem(p, "Medicamentos", 1, 25);
            }
            break;
            
        case 1:
            printf("👥 Você vê figuras sombrias se movendo entre as casas do vilarejo.\n");
            printf("Elas parecem humanas, mas há algo estranho em seus movimentos.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar se esconder e observar\n");
            printf("2 - Aproximar-se das figuras\n");
            printf("3 - Ignorar e continuar explorando\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você se esconde e observa as figuras.\n");
                printf("Elas parecem realizar um ritual estranho no centro do vilarejo.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Símbolo Ritualístico", 1, 0);
            } else if (escolha == 2) {
                printf("Você se aproxima das figuras.\n");
                printf("Quando chega perto, elas se viram e você vê que não têm rostos.\n");
                p->sanidade -= 25;
                p->vida -= 10;
            } else {
                printf("Você ignora as figuras e continua explorando.\n");
                printf("Mesmo assim, você sente os olhos delas em suas costas.\n");
                p->sanidade -= 10;
            }
            break;
            
        case 2:
            printf("🏪 Você encontra uma loja abandonada no vilarejo.\n");
            printf("A porta está entreaberta e parece haver algo útil dentro.\n");
            printf("O que você faz?\n");
            printf("1 - Entrar na loja\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Olhar pela janela antes de entrar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você entra na loja.\n");
                printf("Dentro, encontra produtos estranhos que não parecem ser deste mundo.\n");
                adicionarItem(p, "Poção Misteriosa", 2, 20);
                adicionarItem(p, "Mapa Estrelado", 1, 0);
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você ignora a loja e continua explorando.\n");
                printf("Enquanto se afasta, você ouve um som vindo de dentro da loja.\n");
                p->sanidade -= 5;
            } else {
                printf("Você olha pela janela antes de entrar.\n");
                printf("Vê figuras sombrias se movendo dentro da loja.\n");
                printf("Quando pisca, elas desapareceram.\n");
                p->sanidade -= 15;
            }
            break;
            
        case 3:
            printf("⛪ Você encontra uma pequena igreja no centro do vilarejo.\n");
            printf("As portas estão abertas e parece haver luz dentro.\n");
            printf("O que você faz?\n");
            printf("1 - Entrar na igreja\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Observar de longe antes de decidir\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você entra na igreja.\n");
                printf("Dentro, encontra velas acesas e símbolos estranhos nas paredes.\n");
                printf("No altar, há um livro aberto com escrituras desconhecidas.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Livro Sagrado", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora a igreja e continua explorando.\n");
                printf("Mesmo assim, você sente uma estranha paz vindo da direção da igreja.\n");
                p->sanidade += 5;
            } else {
                printf("Você observa de longe antes de decidir.\n");
                printf("Vê figuras vestidas de branco entrando e saindo da igreja.\n");
                printf("Quando se aproxima, não há ninguém.\n");
                p->sanidade -= 10;
            }
            break;
            
        case 4:
            printf("🎪 Você encontra um circo abandonado na periferia do vilarejo.\n");
            printf("A lona principal está rasgada, mas ainda parece haver algo dentro.\n");
            printf("O que você faz?\n");
            printf("1 - Entrar na lona principal\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Explorar as barracas ao redor\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você entra na lona principal.\n");
                printf("Dentro, encontra palhaços com sorrisos pintados que parecem muito reais.\n");
                printf("Quando você se vira para sair, eles desapareceram.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Máscara de Palhaço", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora o circo e continua explorando.\n");
                printf("Enquanto se afasta, você ouve uma música de circo estranha.\n");
                p->sanidade -= 10;
            } else {
                printf("Você explora as barracas ao redor.\n");
                printf("Em uma delas, encontra espelhos que refletem versões distorcidas de você.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Espelho Mágico", 1, 0);
            }
            break;
    }
}

// Função para eventos aleatórios no cemitério
void eventoCemiterio(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🪦 Você entra no cemitério antigo.\n");
            printf("As lápides estão cobertas de musgo e algumas estão quebradas.\n");
            printf("O que você faz?\n");
            printf("1 - Ler as inscrições nas lápides\n");
            printf("2 - Procurar por túmulos recentes\n");
            printf("3 - Procurar por itens úteis\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você lê as inscrições nas lápides.\n");
                printf("Elas contêm nomes estranhos e datas que não fazem sentido.\n");
                printf("Uma das lápides tem seu nome gravado nela.\n");
                p->sanidade -= 20;
            } else if (escolha == 2) {
                printf("Você procura por túmulos recentes.\n");
                printf("Encontra um túmulo com terra fresca e flores murchas.\n");
                printf("Quando se aproxima, você ouve um som vindo de dentro do túmulo.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Flor Murcha", 1, 0);
            } else {
                printf("Você procura por itens úteis no cemitério.\n");
                printf("Encontra um medalhão brilhante perto de uma lápide.\n");
                adicionarItem(p, "Medalhão Misterioso", 1, 0);
                p->sanidade -= 10;
            }
            break;
            
        case 1:
            printf("👻 Você vê figuras etéreas flutuando entre as lápides.\n");
            printf("Elas parecem não notar sua presença.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar comunicar-se com as figuras\n");
            printf("2 - Esconder-se e observar\n");
            printf("3 - Ignorar e continuar explorando\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta comunicar-se com as figuras.\n");
                printf("Elas se viram e você vê rostos de pessoas que parecem familiar.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Lembrança Espectral", 1, 0);
            } else if (escolha == 2) {
                printf("Você se esconde e observa as figuras.\n");
                printf("Elas parecem realizar uma cerimônia estranha em torno de uma lápide.\n");
                p->sanidade -= 15;
            } else {
                printf("Você ignora as figuras e continua explorando.\n");
                printf("Mesmo assim, você sente o toque frio delas em sua pele.\n");
                p->sanidade -= 10;
            }
            break;
            
        case 2:
            printf("🌳 Você encontra uma árvore antiga no centro do cemitério.\n");
            printf("Seus galhos parecem formar mãos que se estendem para você.\n");
            printf("O que você faz?\n");
            printf("1 - Tocar na árvore\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Procurar por algo sob a árvore\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você toca na árvore.\n");
                printf("Sensações de vidas passadas inundam sua mente.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Folha da Árvore", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora a árvore e continua explorando.\n");
                printf("Mesmo assim, você sente os olhos da árvore em suas costas.\n");
                p->sanidade -= 5;
            } else {
                printf("Você procura por algo sob a árvore.\n");
                printf("Encontra uma caixa enterrada com objetos antigos.\n");
                adicionarItem(p, "Diário Antigo", 1, 0);
                adicionarItem(p, "Fotografia Desbotada", 1, 0);
                p->sanidade -= 10;
            }
            break;
            
        case 3:
            printf("⚰️ Você encontra um mausoléu aberto no cemitério.\n");
            printf("A porta de pedra está entreaberta e parece haver escuridão dentro.\n");
            printf("O que você faz?\n");
            printf("1 - Entrar no mausoléu\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Olhar dentro antes de decidir\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você entra no mausoléu.\n");
                printf("Dentro, encontra caixões abertos e símbolos estranhos nas paredes.\n");
                printf("Um dos caixões parece ter sido usado recentemente.\n");
                p->sanidade -= 20;
                adicionarItem(p, "Símbolo Mortuário", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora o mausoléu e continua explorando.\n");
                printf("Enquanto se afasta, você ouve um som vindo de dentro do mausoléu.\n");
                p->sanidade -= 10;
            } else {
                printf("Você olha dentro antes de decidir.\n");
                printf("Vê figuras sombrias se movendo no escuro do mausoléu.\n");
                printf("Quando pisca, elas desapareceram.\n");
                p->sanidade -= 15;
            }
            break;
            
        case 4:
            printf("🕯️ Você encontra velas acesas espalhadas pelo cemitério.\n");
            printf("Elas parecem formar um caminho que leva a um local específico.\n");
            printf("O que você faz?\n");
            printf("1 - Seguir o caminho de velas\n");
            printf("2 - Apagar algumas velas\n");
            printf("3 - Ignorar e continuar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você segue o caminho de velas.\n");
                printf("Elas levam a uma clareira onde há um círculo de pedras.\n");
                printf("No centro, há um altar com oferendas recentes.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Vela Mágica", 3, 0);
            } else if (escolha == 2) {
                printf("Você tenta apagar algumas velas.\n");
                printf("Quando sopra em uma, ela brilha intensamente antes de se apagar.\n");
                printf("Você ouve sussurros vindos de todas as direções.\n");
                p->sanidade -= 20;
            } else {
                printf("Você ignora as velas e continua explorando.\n");
                printf("Mesmo assim, você sente uma estranha atração pelo caminho que elas formam.\n");
                p->sanidade -= 5;
            }
            break;
    }
}

// Função para eventos aleatórios nas ruínas
void eventoRuinas(Personagem *p, Mapa m) {
    int evento = rand() % 5;
    
    switch(evento) {
        case 0:
            printf("🏛️ Você entra nas ruínas antigas.\n");
            printf("Elas parecem ser de uma civilização muito avançada.\n");
            printf("O que você faz?\n");
            printf("1 - Explorar as ruínas cuidadosamente\n");
            printf("2 - Procurar por inscrições ou símbolos\n");
            printf("3 - Procurar por tecnologia ou artefatos\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você explora as ruínas cuidadosamente.\n");
                printf("Encontra salas com dispositivos estranhos que ainda parecem funcionar.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Dispositivo Antigo", 1, 0);
            } else if (escolha == 2) {
                printf("Você procura por inscrições ou símbolos.\n");
                printf("Encontra escritos em uma língua que parece mudar enquanto você lê.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Tábua de Escrita", 1, 0);
            } else {
                printf("Você procura por tecnologia ou artefatos.\n");
                printf("Encontra um dispositivo que parece ser uma fonte de energia.\n");
                adicionarItem(p, "Fonte de Energia", 1, 0);
                p->sanidade -= 10;
            }
            break;
            
        case 1:
            printf("🤖 Você encontra figuras mecânicas entre as ruínas.\n");
            printf("Elas parecem estar em modo de espera, mas seus olhos brilham quando você se aproxima.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar ativar uma das figuras\n");
            printf("2 - Ignorar e continuar explorando\n");
            printf("3 - Tentar desmontar uma das figuras\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta ativar uma das figuras.\n");
                printf("Ela se move e seus olhos brilham intensamente.\n");
                printf("Ela aponta para uma direção específica antes de desligar novamente.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Peça Mecânica", 2, 0);
            } else if (escolha == 2) {
                printf("Você ignora as figuras e continua explorando.\n");
                printf("Mesmo assim, você sente os olhos delas seguindo seus movimentos.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta desmontar uma das figuras.\n");
                printf("Quando toca nela, ela emite um som estranho e seus olhos brilham.\n");
                p->sanidade -= 10;
                adicionarItem(p, "Núcleo de Energia", 1, 0);
            }
            break;
            
        case 2:
            printf("🔮 Você encontra cristais estranhos nas ruínas.\n");
            printf("Eles brilham com uma luz interna e parecem conter energia.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar coletar alguns cristais\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Tentar usar os cristais\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta coletar alguns cristais.\n");
                printf("Quando os toca, eles brilham intensamente e você sente uma descarga de energia.\n");
                adicionarItem(p, "Cristal de Energia", 3, 0);
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você ignora os cristais e continua explorando.\n");
                printf("Mesmo assim, você sente uma estranha atração por eles.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta usar os cristais.\n");
                printf("Eles projetam imagens de uma civilização antiga e avançada.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Projetor de Imagens", 1, 0);
            }
            break;
            
        case 3:
            printf("📡 Você encontra uma torre de comunicação nas ruínas.\n");
            printf("Ela parece estar funcionando, emitindo sinais regulares.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar usar a torre para se comunicar\n");
            printf("2 - Ignorar e continuar\n");
            printf("3 - Tentar desligar a torre\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta usar a torre para se comunicar.\n");
                printf("Ela responde com sinais que parecem formar uma mensagem.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Gravador de Sinais", 1, 0);
            } else if (escolha == 2) {
                printf("Você ignora a torre e continua explorando.\n");
                printf("Mesmo assim, você sente que está perdendo uma oportunidade importante.\n");
                p->sanidade -= 5;
            } else {
                printf("Você tenta desligar a torre.\n");
                printf("Quando toca nos controles, você ouve vozes em sua mente.\n");
                p->sanidade -= 20;
                p->vida -= 5;
            }
            break;
            
        case 4:
            printf("🚪 Você encontra uma porta selada nas ruínas.\n");
            printf("Ela parece ser feita de um material desconhecido e não há maçanetas visíveis.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar abrir a porta\n");
            printf("2 - Procurar por mecanismos de abertura\n");
            printf("3 - Ignorar e continuar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta abrir a porta com força.\n");
                printf("Ela não se move, mas você ouve um som vindo do outro lado.\n");
                p->sanidade -= 10;
            } else if (escolha == 2) {
                printf("Você procura por mecanismos de abertura.\n");
                printf("Encontra painéis que respondem ao seu toque, revelando símbolos estranhos.\n");
                p->sanidade -= 15;
                adicionarItem(p, "Chave de Símbolos", 1, 0);
            } else {
                printf("Você ignora a porta e continua explorando.\n");
                printf("Mesmo assim, você sente que algo importante está atrás dela.\n");
                p->sanidade -= 5;
            }
            break;
    }
}

// Função para explorar o local atual
void explorarLocal(Personagem *p, Mapa m) {
    switch(p->localizacao) {
        case 0: // Floresta
            eventoFloresta(p, m);
            break;
        case 1: // Lago
            eventoLago(p, m);
            break;
        case 2: // Campo
            printf("🌾 Você está no campo aberto. O vento sopra suavemente, trazendo o cheiro de flores silvestres.\n");
            printf("Ao longe, você pode ver as silhuetas de outros locais.\n");
            printf("O que você faz?\n");
            printf("1 - Procurar por animais ou plantas\n");
            printf("2 - Seguir em direção a um local distante\n");
            printf("3 - Descansar e observar o céu\n");
            
            int escolha;
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você procura por animais ou plantas.\n");
                printf("Encontra ervas medicinais e pegadas de criaturas desconhecidas.\n");
                adicionarItem(p, "Ervas Medicinais", 2, 15);
                p->sanidade -= 5;
            } else if (escolha == 2) {
                printf("Você segue em direção a um local distante.\n");
                printf("Após caminhar por um tempo, você chega a um novo lugar.\n");
                p->localizacao = rand() % 10;
                p->fome -= 10;
                p->sede -= 10;
            } else {
                printf("Você descansa e observa o céu.\n");
                printf("As nuvens formam figuras estranhas que parecem se mover por conta própria.\n");
                p->sanidade -= 10;
                p->vida += 5;
                if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            }
            break;
        case 3: // Caverna
            eventoCaverna(p, m);
            break;
        case 4: // Montanha
            eventoMontanha(p, m);
            break;
        case 5: // Pantano
            eventoPantano(p, m);
            break;
        case 6: // Cabana
            eventoCabana(p, m);
            break;
        case 7: // Vilarejo
            eventoVilarejo(p, m);
            break;
        case 8: // Cemitério
            eventoCemiterio(p, m);
            break;
        case 9: // Ruínas
            eventoRuinas(p, m);
            break;
    }
}

// Função para viajar para outro local
void viajar(Personagem *p, Mapa m) {
    printf("\n🗺️ Você decide viajar para outro local.\n");
    printf("Para onde você quer ir?\n");
    
    // Mostrar opções de locais disponíveis
    printf("0 - %s\n", m.floresta);
    printf("1 - %s\n", m.lago);
    printf("2 - %s\n", m.campo);
    printf("3 - %s\n", m.caverna);
    printf("4 - %s\n", m.montanha);
    printf("5 - %s\n", m.pantano);
    printf("6 - %s\n", m.cabana);
    printf("7 - %s\n", m.vilarejo);
    printf("8 - %s\n", m.cemiterio);
    printf("9 - %s\n", m.ruinas);
    
    printf("> ");
    int escolha;
    scanf("%d", &escolha);
    
    if (escolha >= 0 && escolha < 10) {
        printf("\n🚶 Você começa sua jornada para %s...\n", 
            escolha == 0 ? m.floresta :
            escolha == 1 ? m.lago :
            escolha == 2 ? m.campo :
            escolha == 3 ? m.caverna :
            escolha == 4 ? m.montanha :
            escolha == 5 ? m.pantano :
            escolha == 6 ? m.cabana :
            escolha == 7 ? m.vilarejo :
            escolha == 8 ? m.cemiterio :
            m.ruinas);
        
        sleep(2);
        
        // Evento aleatório durante a viagem
        int evento = rand() % 3;
        if (evento == 0) {
            printf("🌲 Durante sua viagem, você encontra um viajante misterioso.\n");
            printf("Ele está envolto em um manto escuro e não mostra o rosto.\n");
            printf("Ele estende uma mão e oferece um objeto estranho.\n");
            printf("O que você faz?\n");
            printf("1 - Aceitar o objeto\n");
            printf("2 - Recusar educadamente\n");
            printf("3 - Ignorar o viajante e continuar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você aceita o objeto do viajante.\n");
                printf("É um pequeno amuleto que parece pulsar com energia.\n");
                adicionarItem(p, "Amuleto do Viajante", 1, 0);
                p->sanidade -= 5;
            } else if (escolha == 2) {
                printf("Você recusa educadamente o objeto.\n");
                printf("O viajante assente e desaparece na névoa.\n");
                p->sanidade -= 3;
            } else {
                printf("Você ignora o viajante e continua seu caminho.\n");
                printf("Quando olha para trás, ele desapareceu.\n");
                p->sanidade -= 2;
            }
        } else if (evento == 1) {
            printf("🌧️ Durante sua viagem, o tempo muda repentinamente.\n");
            printf("Uma tempestade se forma e você precisa encontrar abrigo.\n");
            printf("O que você faz?\n");
            printf("1 - Procurar abrigo imediatamente\n");
            printf("2 - Continuar apesar da tempestade\n");
            printf("3 - Tentar esperar a tempestade passar\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você procura abrigo e encontra uma pequena caverna.\n");
                printf("Dentro, você encontra alguns suprimentos deixados por viajantes anteriores.\n");
                adicionarItem(p, "Comida Seca", 1, 15);
                adicionarItem(p, "Garrafa de Água", 1, 20);
            } else if (escolha == 2) {
                printf("Você continua apesar da tempestade.\n");
                printf("É difícil caminhar contra o vento e a chuva, mas você persiste.\n");
                p->vida -= 10;
                p->sanidade -= 5;
            } else {
                printf("Você tenta esperar a tempestade passar.\n");
                printf("Ela dura mais do que você esperava, e você sente o frio penetrante.\n");
                p->vida -= 5;
                p->sede -= 10;
            }
        } else {
            printf("🦌 Durante sua viagem, você avista animais selvagens.\n");
            printf("Eles parecem diferentes dos animais que você conhece.\n");
            printf("O que você faz?\n");
            printf("1 - Tentar caçar um dos animais\n");
            printf("2 - Observar de longe\n");
            printf("3 - Tentar se aproximar com cuidado\n");
            
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                printf("Você tenta caçar um dos animais.\n");
                printf("Eles são surpreendentemente rápidos e inteligentes.\n");
                printf("Você consegue capturar um pequeno animal.\n");
                adicionarItem(p, "Carne Fresca", 1, 25);
                p->sanidade -= 5;
            } else if (escolha == 2) {
                printf("Você observa de longe.\n");
                printf("Os animais parecem realizar comportamentos estranhos, quase rituais.\n");
                p->sanidade -= 3;
            } else {
                printf("Você tenta se aproximar com cuidado.\n");
                printf("Os animais percebem sua presença e fogem rapidamente.\n");
                p->sanidade -= 2;
            }
        }
        
        // Chegada ao novo local
        p->localizacao = escolha;
        p->fome -= 10;
        p->sede -= 10;
        
        printf("\n📍 Você chegou a %s.\n", 
            p->localizacao == 0 ? m.floresta :
            p->localizacao == 1 ? m.lago :
            p->localizacao == 2 ? m.campo :
            p->localizacao == 3 ? m.caverna :
            p->localizacao == 4 ? m.montanha :
            p->localizacao == 5 ? m.pantano :
            p->localizacao == 6 ? m.cabana :
            p->localizacao == 7 ? m.vilarejo :
            p->localizacao == 8 ? m.cemiterio :
            m.ruinas);
    } else {
        printf("❌ Local inválido. Você continua onde está.\n");
    }
}

// Função para o menu de ações
void menuAcoes(Personagem *p, Mapa m) {
    printf("\nO que você quer fazer?\n");
    printf("1 - Explorar o local atual\n");
    printf("2 - Viajar para outro local\n");
    printf("3 - Usar item do inventário\n");
    printf("4 - Descansar\n");
    printf("5 - Ver status\n");
    printf("> ");
    
    int escolha;
    scanf("%d", &escolha);
    
    switch(escolha) {
        case 1:
            explorarLocal(p, m);
            break;
        case 2:
            viajar(p, m);
            break;
        case 3:
            usarItem(p);
            break;
        case 4:
            printf("\n😴 Você decide descansar.\n");
            printf("Encontra um local seguro e se deita para descansar.\n");
            sleep(2);
            printf("Seu corpo se recupera lentamente...\n");
            p->vida += 10;
            if (p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            p->sanidade += 5;
            if (p->sanidade > 100) p->sanidade = 100;
            p->fome -= 5;
            p->sede -= 5;
            break;
        case 5:
            exibirStatus(*p);
            break;
        default:
            printf("❌ Ação inválida. Você perde tempo hesitando.\n");
            p->sanidade -= 2;
            break;
    }
}

// Função para verificar condições de sobrevivência
int verificarSobrevivencia(Personagem *p) {
    if (p->vida <= 0) {
        printf("\n💀 Você sente o corpo falhar... o mundo escurece.\n");
        printf("GAME OVER — a ilha levou mais uma alma.\n");
        return 0;
    }
    
    if (p->sanidade <= 0) {
        printf("\n🤯 Sua mente se quebra... você começa a rir sem parar.\n");
        printf("GAME OVER — você se tornou mais um dos loucos da ilha.\n");
        return 0;
    }
    
    if (p->fome <= 0) {
        printf("\n🍽️ A fome se torna insuportável... seu corpo enfraquece.\n");
        printf("GAME OVER — você sucumbiu à fome.\n");
        return 0;
    }
    
    if (p->sede <= 0) {
        printf("\n💧 A sede se torna insuportável... sua garganta se fecha.\n");
        printf("GAME OVER — você sucumbiu à sede.\n");
        return 0;
    }
    
    return 1;
}

// Função para atualizar status do personagem
void atualizarStatus(Personagem *p) {
    // Reduzir fome e sede a cada turno
    p->fome -= 5;
    p->sede -= 7;
    
    // Verificar efeitos de fome e sede
    if (p->fome < 30) {
        printf("🍽️ Você está com muita fome.\n");
        p->vida -= 5;
    }
    
    if (p->sede < 30) {
        printf("💧 Você está com muita sede.\n");
        p->vida -= 5;
    }
    
    // Efeitos de sanidade baixa
    if (p->sanidade < 30) {
        printf("🧠 Sua sanidade está abalada. Você começa a ver coisas...\n");
        if (rand() % 2 == 0) {
            printf("Você ouve sussurros vindos de lugar nenhum.\n");
            p->sanidade -= 5;
        }
    }
}

//---------------------------------------------------------------------
int main() {
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    Mapa mapa1; // variável do tipo Mapa

    // strcpy(destino, "origem");
    strcpy(mapa1.floresta, "Floresta Escura");
    strcpy(mapa1.lago, "Lago Misterioso");
    strcpy(mapa1.campo, "Campo Aberto");
    strcpy(mapa1.caverna, "Caverna Escura");
    strcpy(mapa1.montanha, "Montanha Alta");
    strcpy(mapa1.pantano, "Pântano Sombrio");
    strcpy(mapa1.cabana, "Cabana Esquecida");
    strcpy(mapa1.vilarejo, "Vilarejo Abandonado");
    strcpy(mapa1.cemiterio, "Cemitério Antigo");
    strcpy(mapa1.ruinas, "Ruínas Antigas");

    // Criar personagem
    Personagem jogador;
    jogador.vida = 100;
    jogador.vidaMaxima = 100;
    jogador.sanidade = 100;
    jogador.fome = 100;
    jogador.sede = 100;
    jogador.numItens = 0;
    jogador.localizacao = 0; // Começa na floresta
    
    printf("🌑 INICIALIZANDO...\n");
    sleep(2);
    limparTela();
    
    printf("🌲 Você acorda em uma %s...\n", mapa1.floresta);
    sleep(2);
    printf("O ar está pesado... e o som de algo rastejando se aproxima.\n");
    sleep(2);
    printf("Onde estou!? Isso... Onde está minha cama, meu quarto, minha casa!!??\n");
    sleep(2);
    printf("Tem alguém aí?!\n");
    sleep(3);
    
    // Adicionar itens iniciais
    adicionarItem(&jogador, "Pedaço de Pão", 1, 10);
    adicionarItem(&jogador, "Garrafa de Água", 1, 20);
    adicionarItem(&jogador, "Fósforo", 1, 0);
    
    int turno = 1;
    
    // Loop principal do jogo
    while (turno < 30) { // Limite de turnos aumentado
        printf("\n╔════════════════════════════════╗\n");
        printf("║           TURNO %d              ║\n", turno);
        printf("╚════════════════════════════════╝\n");
        
        // Mostrar localização atual
        printf("\n📍 Local atual: %s\n", 
            jogador.localizacao == 0 ? mapa1.floresta :
            jogador.localizacao == 1 ? mapa1.lago :
            jogador.localizacao == 2 ? mapa1.campo :
            jogador.localizacao == 3 ? mapa1.caverna :
            jogador.localizacao == 4 ? mapa1.montanha :
            jogador.localizacao == 5 ? mapa1.pantano :
            jogador.localizacao == 6 ? mapa1.cabana :
            jogador.localizacao == 7 ? mapa1.vilarejo :
            jogador.localizacao == 8 ? mapa1.cemiterio :
            mapa1.ruinas);
        
        // Exibir status básico
        printf("❤️ Vida: %d/%d | 🧠 Sanidade: %d/100 | 🍖 Fome: %d/100 | 💧 Sede: %d/100\n", 
            jogador.vida, jogador.vidaMaxima, jogador.sanidade, jogador.fome, jogador.sede);
        
        // Menu de ações
        menuAcoes(&jogador, mapa1);
        
        // Atualizar status
        atualizarStatus(&jogador);
        
        // Verificar condições de sobrevivência
        if (!verificarSobrevivencia(&jogador)) {
            break;
        }
        
        // Eventos especiais a cada 5 turnos
        if (turno % 5 == 0) {
            printf("\n⚠️ O ar fica mais pesado... algo está mudando na ilha.\n");
            sleep(2);
            
            int eventoEspecial = rand() % 3;
            if (eventoEspecial == 0) {
                printf("🌙 A lua parece brilhar mais intensamente, iluminando caminhos antes ocultos.\n");
                printf("Você sente que novos segredos foram revelados na ilha.\n");
                jogador.sanidade -= 5;
            } else if (eventoEspecial == 1) {
                printf("🌫️ Uma névoa espessa cobre a ilha, mudando a paisagem.\n");
                printf("Quando a névoa se dissipa, alguns locais parecem diferentes.\n");
                jogador.sanidade -= 10;
            } else {
                printf("👂 Você ouve um som distante, como um chamado.\n");
                printf("Parece vir de um lugar específico na ilha.\n");
                jogador.sanidade -= 7;
            }
        }
        
        // Aumentar dificuldade gradualmente
        if (turno > 10 && turno % 3 == 0) {
            printf("\n⚠️ Você sente que a ilha está se tornando mais perigosa.\n");
            jogador.sanidade -= 5;
        }
        
        turno++;
        sleep(1);
    }

    if (jogador.vida > 0 && jogador.sanidade > 0 && jogador.fome > 0 && jogador.sede > 0) {
        printf("\n🌅 Você sobreviveu a esta noite...\n");
        sleep(2);
        printf("Mas o amanhecer só revela novos horrores...\n");
        sleep(2);
        printf("Sua jornada nesta ilha misteriosa continua...\n");
        sleep(2);
        printf("FIM DE JOGO - Por enquanto...\n");
    }

    return 0;
}