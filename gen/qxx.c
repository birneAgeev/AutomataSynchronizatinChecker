#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <stdio.h>
#include "icdfaGen.h"
#include "icdfa.h"


int main()
{
	freopen("output.txt", "wt", stdout);
	icdfagen_t generator = icdfaGenInit(10, 2, 0, 0, 1);
	int count = 106000;
	/*for (int i = 0; i < automaton->n_states * automaton->n_symbols; ++i)
	{
	printf("%d", automaton->delta[i]);
	}
	int n;
	scanf("%d", &n);*/

	for (int i = 0; i < count; ++i)
	{
		struct icdfa* automaton = icdfaGenRandom(generator, 0);
		icdfaPretty(automaton);
	}
	//int n;
	//scanf("%d", &n);
	return 0;
}
