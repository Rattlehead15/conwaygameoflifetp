#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000

/*   ___________________________________
   /                                    \
  |        SISTEMAS OPERATIVOS I        |
  \____________________________________/

   O=====================================O
  || Lautaro Garavano - Giuliano Regolo ||
  O=====================================O

  1) El deadlock se produce en el caso de que todos los filosofos agarren el tenedor de la derecha,
  impidiendo así que alguno agarre el de la izquierda y comience a comer.

  2) Al haber un filosofo zurdo (en nuestro programa, representado por el id 0), el deadlock no se produce.
  Esto se debe a que cuando todos los filosofos agarran el primer tenedor, el zurdo intentará agarrar el mismo tenedor
  que el filosofo que se encuentra a su izquierda. Por otro lado, el tenedor que se encuentra a la derecha del zurdo estará libre,
  y el filosofo de su derecha podrá empezar a comer sin problemas.
  Una vez que este coma, liberara sus dos tenedores y permitira comer al filosofo de su derecha, y asi sucesivamente.
*/

pthread_mutex_t tenedor[N_FILOSOFOS];

void pensar(int i)
{
  printf("Filosofo %d pensando...\n",i);
  usleep(random() % ESPERA);
}

void comer(int i)
{
  printf("Filosofo %d comiendo...\n",i);
  usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
  pthread_mutex_lock(&tenedor[i ? i : (i+1) % N_FILOSOFOS]);
  pthread_mutex_lock(&tenedor[i ? (i+1)%N_FILOSOFOS : i]);
}

void dejar_tenedores(int i)
{
  pthread_mutex_unlock(&tenedor[i]); /* Deja el tenedor de su derecha */
  pthread_mutex_unlock(&tenedor[(i+1)%N_FILOSOFOS]); /* Deja el tenedor de su izquierda */
}

void *filosofo(void *arg)
{
  int i = (int) arg;
  for (;;)
  {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);
  }
}

int main()
{
  int i;
  pthread_t filo[N_FILOSOFOS];
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_create(&filo[i], NULL, filosofo, (void *)i);
  pthread_join(filo[0], NULL);
  return 0;
}
