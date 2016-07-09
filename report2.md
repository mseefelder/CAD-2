% Computação de Alto Desempenho - Trabalho Prático 2
% Alunos: Bernardo Amorim & Marcos Seefelder; Professor: José Camata; Engenharia de Computação e Informação - UFRJ
% 11/07/2016

## Ambiente computacional:

Processador: Intel® Core™ i7-3610QM CPU @ 2.30GHz × 8 

Memória: 7,7 GiB

# Exercício 1

## Configuração:

O código foi compilado com `tau_cc.sh openmp -O -DSTREAM_ARRAY_SIZE=2988441 stream.c -o stream`.

## Gráfico:

Os gráficos a seguir foram gerados utilizando a aplicação *paraprof*:

![Gráfico de barras do tempo consumido por linha de código para cada thread.](images/stream-3.jpg)
![Gráfico de barras do tempo consumido por linha de código para cada thread.](images/stream-4.jpg)

## Análise



# Exercício 2

## Modificações no código:

No final da rotina `Deallocate3DMatrix` faltava um `free(array)`, o qual adicionamos. Além disso, fizemos algumas alterações na rotina `iso_3dfd_it`, resultando no seguinte código (apenas o código interno está exposto):

```cpp
{
	//declaramos value aqui fora mas zero a cada loop lá dentro
	float value = 0.0;
	//variável auxiliar para utilizar no loop mais interno
	float internalvalue = 0.0;
	//Para não ter que ficar acessando coeff
	const float zerocoeff = coeff[0];

	//coloquei as restrições do if nos loops
	//Inverti a ordem para aproveitar a ordem de indexação do C++
	for (int i = HALF_LENGTH; i<(n1 - HALF_LENGTH); i++)
	   for (int j = HALF_LENGTH; j<(n2 - HALF_LENGTH); j++) {
			for (int k = HALF_LENGTH;k<(n3 - HALF_LENGTH); k++) {
			{
				//continuo zerando o value a cada loop
				value = 0.0;

				//Parte dessa linha ficava após o loop que vem a seguir. 
				//Quebrei ela e faço essa parte logo.
				ptr_next[i][j][k] = ptr_prev[i][j][k]+ptr_prev[i][j][k] - ptr_next[i][j][k];
				value += ptr_prev[i][j][k] * zerocoeff;
				
				for (int ir = 1; ir <= HALF_LENGTH; ir++) {
					internalvalue = 0.0;
					//Faço todas as somas e depois multiplico
					internalvalue += ptr_prev[i+ir][j][k] + ptr_prev[i-ir][j][k]
						+ ptr_prev[i][j+ir][k] + ptr_prev[i][j-ir][k]
						+ ptr_prev[i][j][k+ir] + ptr_prev[i][j][k-ir];
					internalvalue *= coeff[ir];
					value += internalvalue;
				}

				ptr_next[i][j][k] += value*ptr_vel[i][j][k];

			}
		}
	}
}

```

Posteriormente, foram adicionados `#pragma`s do OpenMP, para paralelização dos loops de for presentes nas rotinas `initialize` e `iso_3dfd_it`. O código segue em anexo, em *ex2/wave/*.

## Compilação e problemas:

Vale ressaltar que enfrentamos diversos problemas na hora de compilar o código e em seguida apresentamos como resolvemos os dois principais problemas:

* Primeiramente, os `#pragma`s do OpenMP precisaram ser indentados no início das linhas, caso contrário os parsers no processo de compilação do tau apresentavam erros;

* Além disso, a linha `fprintf(fout, "splot \'%s_%03d.dat\' u 1:2:3 t\"\"\n", rootname, t_step);` na função `write_plane_XY` fazia com que o Opari Parser lançasse uma **falha de segmentação**. Nossa solução foi comentar a parte do código que escreve os arquivos de extensão `.plot` e criá-los em uma aplicação separada, o que não atrapalha o funcionamento e permite conferir se os resultados obtidos são corretos.

No fim, a compilação, usando `Makefile.tau-papi-pdt-openmp-opari`, foi feita com o comando `tau_cc.sh -openmp main.cc -o wave`.


## Resultados:

Para a execução e posterior análise, utilizamos o comando `export TAU_METRICS=GET_TIME_OF_DAY\:PAPI_FP_INS` para definir os eventos capturados. A seguir, apresentamos os resultados para o código em diferentes tipos de configuração.

Testamos com e sem `collapse()` no conjunto de *loops* que representava o maior tempo de processamento, na função `iso_3dfd_it`. Não fizemos uma versão com sincronização explícita, pois utilizamos apenas o `#pragma omp parallel for` e não consideramos que fosse aplicável no caso.

### Sem `collapse()`:

**Tempo de execução**: 5m6.643s

### Com `collapse()` e `schedule(static)`:

**Tempo de execução**: 5m15.586s

### Com `collapse()` e `schedule(dynamic)`:

**Tempo de execução**: 6m5.894s

### Com `collapse()` e `schedule(guided)`:

**Tempo de execução**: 4m35.420s

## Análise e escalabilidade:

O melhor tempo de execução foi obtido **colapsando os 3 *loops* em 1** e utilizando **escalonamento guiado**.

![Distribuição de carga com `collapse()` e `schedule(guided)`, gráfico do tempo de execução para as instruções](images/col-gui.jpg)

Pode-se perceber que a carga está muito balanceada entre as *threads*, sendo as duas únicas rotinas que aparecem destacadas no gráfico o *loop* colapsado na função `iso_3dfd_it` (em vermelho) e a barreira implícita do loop (em azul).

Executamos esse melhor caso com 1,2,3,..,8 *threads* (limite de *cores* do ambiente comptacional), medimos o tempo com a rotina *time* do *Linux* e apresentamos a seguir o gráfico de *speedup*.

![*Speedup* da wave propagation](images/speedup-wave.png)

# Exercício 3



# Exercício 4

