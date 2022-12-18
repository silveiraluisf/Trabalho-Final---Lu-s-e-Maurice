#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define WORD_SIZE 16
#define WORD_COUNT 100

int countOccurrences(char *word);

int main(int argc, char** argv) {
    struct timeval t1, t2;
	MPI_Init(NULL, NULL);// Initialize the MPI environment
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);// Find out rank, size
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// We are assuming at least 2 processes for this task
	if (world_size < 2) {
		fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
    
    gettimeofday(&t1, NULL);
    
	char keywords[WORD_COUNT][WORD_SIZE] = {
        "death", "day", "night", "fear", "sea", "whale", "ship", "joy",
        "king", "dispair", "tomorrow", "yesterday", "time", "home", "love", "snake", 
        "man", "woman", "god", "tired", "me", "life", "rose", "book", 
        "danger", "death", "day", "night", "fear", "sea", "whale", "ship", "joy",
        "king", "dispair", "tomorrow", "yesterday", "time", "home", "love", "snake", 
        "man", "woman", "god", "tired", "me", "life", "rose", "book", 
        "danger", "death", "day", "night", "fear", "sea", "whale", "ship", "joy",
        "king", "dispair", "tomorrow", "yesterday", "time", "home", "love", "snake", 
        "man", "woman", "god", "tired", "me", "life", "rose", "book", 
        "danger", "death", "day", "night", "fear", "sea", "whale", "ship", "joy",
        "king", "dispair", "tomorrow", "yesterday", "time", "home", "love", "snake", 
        "man", "woman", "god", "tired", "me", "life", "rose", "book", 
        "danger",  
    };
    char recebedor[WORD_COUNT][WORD_SIZE];
   
	if (world_rank == 0) {
		MPI_Send(&keywords, (WORD_COUNT * WORD_SIZE), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	} 
    else {
		MPI_Recv(&recebedor, (WORD_COUNT * WORD_SIZE), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < WORD_COUNT; i++)
            printf("Palavra: %s | Numero de vezes = %d.\n", recebedor[i], countOccurrences(recebedor[i]));
	}

    gettimeofday(&t2, NULL);
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
    printf("Tempo total de execucao = %f\n" , t_total);

	MPI_Finalize();
}

int countOccurrences(char *word) {
    FILE *fptr = fopen("livro/mobdick.txt", "rb");

    /* Tratamento de excessao, caso nao cosiga ler o arquivo */
    if (fptr == NULL)
    {
        printf("Não foi possivel abrir o arquivo.\n");

        exit(EXIT_FAILURE);
    }
    
    char str[BUFFER_SIZE];
    char *pos;

    int index;
    
    int count = 0;

    /* le as linhas do arquivo ate o final dele */
        while ((fgets(str, BUFFER_SIZE, fptr)) != NULL)
        {
            index = 0;

            /* encontra a proxima ocorrencia da palavra em str */
            while ((pos = strstr(str + index, word)) != NULL)
                {
                    index = (pos - str) + 1;
                    count++;
                }
        }

    return count;

    fclose(fptr);
}
