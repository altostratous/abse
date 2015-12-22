#ifndef ABSE_MULTITHREAD
#define ABSE_MULTITHREAD 1
#include<pthread.h>
#include<cstdlib>
#include<vector>

#include "file.h"

using namespace disk;

struct iterateargs
{
	file* f;
	string o;
	analysis* p;
	int fileidstart;
};

void *singlefile(void *argv)
{
	struct iterateargs* args = (struct iterateargs*) argv;
	// cout<<args->fileidstart<< "\n";
	args->f->iterate(*args->p, args->o, args->fileidstart);
}

void fasterate(file& f, string outdir, analysis& anal, int threadnum)
{
	int step = f.getFilesCount() / (threadnum) + 1;
	pthread_t tid[1000];
	struct iterateargs args[1000];
	vector<string>filenames = f.getFilenames();
	vector<string>::iterator i = filenames.begin();
	for(int k = 0; k < threadnum; k++)
	{
		vector<string> chunk;
		for(int j = 0; j < step && i != filenames.end(); j++)
		{
			chunk.push_back(*i);
			++i;
		}// = (struct iterateargs*)malloc(sizeof(struct iterateargs));
		args[k].f = new file(chunk);
		args[k].o = outdir;
		args[k].p = &anal;
		args[k].fileidstart = k * step;
		pthread_create(&tid[k], NULL, singlefile, (void*)(&args[k]));
	}
	for(int k = 0; k < threadnum; k++)
		pthread_join(tid[k], NULL);
}
#endif
