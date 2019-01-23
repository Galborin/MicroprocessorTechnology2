/*
 * process_sections.c
 *
 *  Created on: 21.01.2019
 *      Author: piotr
 */

#define SAMPLING_FREQ 	8000
#define SECTION			8960
#define STEP 			85
#define BLOCK_SIZE 	  	256
#define BASE_SIZE		512
#define CEP				19
#define NCEPVEC			65

#include "stdio.h"
#include "arm_math.h"


/* input[NCEPVEC][CEP] -> input[CEP][NCEPVEC]
 * reference[ref_ncepvec][CEP] -> reference[CEP][ref_ncepvec]
 * */
void classify(uint16_t ref_ncepvec,float32_t reference[CEP][ref_ncepvec],float32_t input[CEP][NCEPVEC])
{
	uint8_t Q = 0.2 * ref_ncepvec;// 0.2 * liczba vektorow cepstralnych wzorca (moze ich byc wiecej niz w inpucie)  ;
	float32_t d[NCEPVEC][ref_ncepvec]; //macierz odleglosci
	float32_t g[NCEPVEC][ref_ncepvec]; //macierz zakumulowana
	for(uint16_t ver = 0; ver< NCEPVEC;++ver)
	{
		for(uint16_t hor = 0;hor<ref_ncepvec;++hor)
		{
			d[ver][hor] = 999; //Inf
		}
	}
	float32_t tg = (ref_ncepvec - Q)/(NCEPVEC - Q);
	int down[NCEPVEC];
	int up[NCEPVEC];
	uint16_t temp; //na potrzeby obliczen
	for(uint8_t i = 0;i<NCEPVEC;++i)
	{
		temp = (tg*i)-(Q*tg);//floor
		if(temp<1)
			{down[i] =1;}
		else
			{down[i]=temp;}
		temp = (tg*i)+Q;
		temp = temp + (temp!=((tg*i)+Q));//ceil
		if (temp>ref_ncepvec)
			{up[i] = ref_ncepvec;}
		else
			{up[i] = temp;}

			/*liczymy macierz odleglosci*/
			float32_t sum;
			for(uint16_t j = down[i];j<up[i];++j)
			{
				sum = 0;
				for(uint8_t di = 0;di<CEP;++di)
				{
					sum = sum + (input[di][i] - reference[di][j]); //kolejne cepstra z wektorow
				}
				d[i][j]=sqrt(sum*sum); //distance
			}

	}

	for(uint16_t ver = 0; ver< NCEPVEC;++ver)
	{
		for(uint16_t hor = 0;hor<ref_ncepvec;++hor)
		{
			g[ver][hor] = d[ver][hor]; //g=d
		}
	}

	for(uint16_t i = 1;i<NCEPVEC;++i)
	{
		g[i][1] = g[i - 1][1] + d[i][1]; //akumulacja pierwszej kolumny
	}

	for(uint16_t i = 1;i<ref_ncepvec;++i)
	{
		g[1][i] = g[1][i-1] - d[1][i]; //akumulacja pierwszego wiersza
	}

	float32_t dd = 0; //temporary
	float32_t moveup;
	float32_t movediag;
	float32_t moveright;
	for(uint16_t i = 1; i<NCEPVEC;++i)
	{
		uint16_t j = 1;
		if(down[i]>1)
			{j = down[i];}

		for(;j<up[i];++j)
		{
			dd = d[i][j];
			moveup = g[i - 1][j] + dd;
			g[i][j] = moveup;
			movediag = g[i - 1][j-1] + 2*dd;
			if(moveup>movediag)
				{g[i][j] = movediag;}
			moveright = g[i][j-1] + dd;
			if(g[i][j]>moveright)
				{g[i][j] = moveright;}
			 //the lowest value
		}
	}
}



