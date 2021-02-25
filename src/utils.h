#ifndef UTILS_TEMA3_APD
#define UTILS_TEMA3_APD

#include <errno.h>

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <cstring>
#include <unistd.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <mutex>
#include <algorithm>
#include <stack>

#define MASTER 0
#define HORROR 1
#define COMEDY 2 
#define FANTASY 3
#define SCIENCE_FICTION 4

#define LINE_LENGTH 2100

#define UNDEFINED -1

#define MASTER_THREAD_COUNT 4

#define NC "\e[0m"

#define RED "\e[0;31m"
#define REDB "\e[0;41m"

#define GRN "\e[0;32m"
#define GRNB "\e[0;42m"

#define YLL "\e[0;33m"
#define YLLB "\e[0;43m"

#define CYN "\e[0;36m"
#define CYNB "\e[0;46m"

#define MAG "\e[0;35m"
#define MAGB "\e[0;45m"

using std::cout; using std::endl;

int genreToID(std::string genre)
{
	
	if(genre=="horror")
	{
		return HORROR;
	}
	if(genre=="comedy")
	{
		return COMEDY;
	}
	if(genre=="fantasy")
	{
		return FANTASY;
	}
	if(genre=="science-fiction")
	{
		return SCIENCE_FICTION;
	}

	return UNDEFINED;
}

std::string idToGenre(int id)
{
	switch (id)
	{
	case HORROR:
		return "horror";
	case COMEDY:
		return "comedy";
	case FANTASY:
		return "fantasy";
	case SCIENCE_FICTION:
		return "science-fiction";
	default:
		return "";
	}
	return "";
}

#endif