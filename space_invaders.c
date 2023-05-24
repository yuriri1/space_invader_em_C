#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <pthread.h>
#include <conio.h>

#define NUM_NAVES 5

typedef struct nave_t
{
	int thread_id;
	int y;
	int x;
} nave_t;

pthread_t thread_foguete;
pthread_t thread_nave[NUM_NAVES];
pthread_t thread_canhao;
pthread_t thread_tecla;
pthread_mutex_t mutex;

nave_t naves[NUM_NAVES];
int naves_ativa = NUM_NAVES;

char tecla;
int dificuldade;
char resultado[10];
int velocidade_naves = 0;
int estado_jogo = 1;
int nave_atingida = -1;
int foguete_ativo = 0;
int fogueteX = 0;
int fogueteY = 0;

time_t tempo_i;
int tempo_f = 60;

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void foguete_horizontal(int x, int y)
{
	gotoxy(x, y);
	printf("<");
	gotoxy(x + 1, y);
	printf("___");
}
void apaga_foguete_horizontal(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
	gotoxy(x + 1, y);
	printf("   ");
}
void foguete(int x, int y)
{
	pthread_mutex_lock(&mutex);
	gotoxy(x, y);
	printf("/\\");
	gotoxy(x, y + 1);
	printf("||");
	gotoxy(x - 1, y + 2);
	printf("/||\\");
	pthread_mutex_unlock(&mutex);
}
void apaga_foguete(int x, int y)
{
	pthread_mutex_lock(&mutex);
	gotoxy(x, y);
	printf("  ");
	gotoxy(x, y + 1);
	printf("  ");
	gotoxy(x - 1, y + 2);
	printf("    ");
	pthread_mutex_unlock(&mutex);
}
void explode_foguete(int x, int y)
{
	pthread_mutex_lock(&mutex);
	gotoxy(x, y);
	printf("*");
	Sleep(50);
	printf(" ");
	gotoxy(x, y - 1);
	printf("O");
	gotoxy(x - 1, y);
	printf("O O");
	gotoxy(x, y + 1);
	printf("O");
	Sleep(50);
	gotoxy(x, y - 1);
	printf(" ");
	gotoxy(x - 1, y);
	printf("   ");
	gotoxy(x, y + 1);
	printf(" ");
	gotoxy(x, y - 2);
	printf("o");
	gotoxy(x - 2, y);
	printf("o   o");
	gotoxy(x, y + 2);
	printf("o");
	Sleep(50);
	gotoxy(x, y - 2);
	printf(" ");
	gotoxy(x - 2, y);
	printf("     ");
	gotoxy(x, y + 2);
	printf(" ");
	pthread_mutex_unlock(&mutex);
}
void nave(int x, int y)
{
	pthread_mutex_lock(&mutex);
	gotoxy(x, y);
	printf("/");
	gotoxy(x + 1, y - 1);
	printf("___");
	gotoxy(x + 4, y);
	printf("\\");
	gotoxy(x, y + 1);
	printf("v-V-v");
	gotoxy(x, y + 1);
	printf("V-v-V");
	pthread_mutex_unlock(&mutex);
}
void apaga_nave(int x, int y)
{
	pthread_mutex_lock(&mutex);
	gotoxy(x, y);
	printf(" ");
	gotoxy(x + 1, y - 1);
	printf("   ");
	gotoxy(x + 4, y);
	printf(" ");
	gotoxy(x, y + 1);
	printf("     ");
	gotoxy(x, y + 1);
	printf("     ");
	pthread_mutex_unlock(&mutex);
}

void canhao(int x, int y)
{
	gotoxy(x, y);
	printf("+-+");
	gotoxy(x, y + 1);
	printf("| |");
	gotoxy(x, y + 2);
	printf("| |");
	gotoxy(x, y + 3);
	printf("+-+");
}

void explode_nave(int x, int y)
{
	explode_foguete(x, y);
	Sleep(200);
	apaga_nave(x, y);
	Sleep(200);
}

void set_dificuldade(int i)
{
	if (i == 1)
	{
		velocidade_naves = 80;
	}
	else if (i == 2)
	{
		velocidade_naves = 50;
	}
	else if (i == 3)
	{
		velocidade_naves = 20;
	}
}

void inicia_jogo()
{
	printf("*******************************\n");
	printf("*                                     *\n");
	printf("*  SELECIONE A DIFICULDADE      *\n");
	printf("*                                     *\n");
	printf("*  1 - FACIL                        *\n");
	printf("*  2 - MEDIO                        *\n");
	printf("*  3 - DIFICIL                      *\n");
	printf("*                                     *\n");
	printf("*******************************\n");
	scanf("%d", &dificuldade);
	while (dificuldade < 1 || dificuldade > 3)
	{
		scanf("%d", &dificuldade);
	}
	set_dificuldade(dificuldade);
	system("cls");
}

void finalizacao()
{
	gotoxy(0, 0);
	printf("**************************\n");
	printf("*                               *\n");
	printf("*        VOCE %s         *\n", resultado);
	printf("*                               *\n");
	printf("**************************\n");
}

void *desenha_nave(void *arg)
{
	nave_t *nave_obj = (nave_t *)arg;

	int y = nave_obj->y;

	while (estado_jogo == 1)
	{
		for (int x = 0; x <= 79; x++)
		{
			nave_obj->x = x;
			nave(x, y);
			Sleep(velocidade_naves);
			apaga_nave(x, y);
		}
		if (nave_atingida == nave_obj->thread_id)
		{
			nave_atingida = -1;
			naves_ativa--;
			break;
		}
	}
}

void *lanca_foguete(void *arg)
{
	int x = 40;
	int y = 37;

	while (y > 0 && nave_atingida == -1)
	{
		fogueteX = x;
		fogueteY = y;

		foguete(x, y);
		Sleep(30);
		apaga_foguete(x, y);

		for (int i = 0; i < NUM_NAVES; i++)
		{
			if (fogueteX >= naves[i].x && fogueteX <= naves[i].x + 4 && fogueteY >= naves[i].y && fogueteY <= naves[i].y + 1)
			{
				nave_atingida = i;
				break;
			}
		}

		if (nave_atingida != -1)
		{
			explode_nave(x, y);
			break;
		}

		y--;
	}

	foguete_ativo = 0;
}

void *set_tecla(void *arg)
{
	while (1)
	{
		tecla = getch();
	}
}

void *canhao_jogador(void *arg)
{

	while (1)
	{
		if (naves_ativa == 0)
		{
			estado_jogo = 0;
			strcpy(resultado, "GANHOU");
			break;
		}
		else if (difftime(time(NULL), tempo_i) >= tempo_f)
		{
			estado_jogo = 0;
			strcpy(resultado, "PERDEU");
			break;
		}
		else if (naves_ativa > 0)
		{
			if (tecla == ' ' && foguete_ativo == 0)
			{
				foguete_ativo = 1;
				pthread_create(&thread_foguete, NULL, lanca_foguete, NULL);
				tecla = '1';
			}
			else if (tecla == 's')
			{
				estado_jogo = 0;
				pthread_join(thread_foguete, NULL);
				break;
			}
		}
	}
}

int main()
{
	int i;

	srand(time(NULL));

	system("cls");

	inicia_jogo();

	pthread_mutex_init(&mutex, NULL);

	canhao(40, 40);

	tempo_i = time(NULL);

	pthread_create(&thread_canhao, NULL, canhao_jogador, NULL);
	pthread_create(&thread_tecla, NULL, set_tecla, NULL);

	for (i = 0; i < NUM_NAVES; i++)
	{
		naves[i].y = rand() % 35;
		naves[i].thread_id = i;
		pthread_create(&thread_nave[i], NULL, desenha_nave, &naves[i]);
		Sleep(400);
	}

	while (1)
	{
		if (estado_jogo == 0 || naves_ativa == 0 || difftime(time(NULL), tempo_i) >= tempo_f)
		{
			break;
		}
	}

	for (i = 0; i < NUM_NAVES; i++)
	{
		pthread_join(thread_nave[i], NULL);
	}

	pthread_join(thread_canhao, NULL);

	pthread_mutex_destroy(&mutex);
	Sleep(1000);
	system("cls");

	finalizacao();

	return 0;
}