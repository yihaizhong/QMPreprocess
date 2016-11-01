#pragma once

#include "threadpool/threadpool.h"

using namespace std;
namespace yt{
	int getProcessorCount();

	string getFileName(const std::string &path);

	string getDir(const string &path);

	bool getFiles(const string &pattern_path, vector<string> &vecFile);

	bool getFilesPath(const string &pattern_path, vector<string> &vecFilePath);

	void eraseExtention(string &file, size_t n = 1);

	template<int np = -1, class Fn, class T, class ...OT>
	void multiCall(Fn &&fn, vector<T> & tvec, OT ...ot)
	{
		size_t tcnt = np;
		if (np < 1)
		{
			tcnt = getProcessorCount();
		}
		threadpool::ThreadPool pool((int)tcnt);
		for (auto bg = tvec.begin(); bg != tvec.end(); ++bg)
		{
			T &t = *bg;
			pool.run([&t, &fn, &ot...](){fn(t, ot...); });
		}
		pool.wait();
	}

	template<int np = -1, class Fn, class Arg, class Rel, class ...OT>
	void multiCall(Fn &&fn, vector<Arg> &vecArg, vector<Rel> &vecRel, OT ...ot)
	{
		size_t tcnt = np;
		if (np < 1)
		{
			tcnt = getProcessorCount();
		}
		threadpool::ThreadPool pool((int)tcnt);
		for (size_t i = 0; i < vecArg.size(); ++i)
		{
			Arg &arg = vecArg.at(i);
			Rel &rel = vecRel.at(i);
			pool.run([&arg, &rel, &fn, &ot...](){fn(arg, rel, ot...); });
		}
		pool.wait();
	}

#define TIME_START clock_t t_start,t_end;\
	t_start=clock()

#define TIME_END t_end=clock();\
	cout<<"Elapsed time:"<<double(t_end-t_start)/CLOCKS_PER_SEC<<"s"<<endl
}