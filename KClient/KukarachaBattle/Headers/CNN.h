#ifndef CNN_H
#define CNN_H

#include <vector>
#include "Matr.h"
#include "MLP.h"

using std::vector;

struct CNN_Layer {
	vector<Matr<ld>> cnvdInp;
	vector<Matr<ld>> absed;
	vector<Matr<ld>> pooled;
};

struct CNNd_Layer {
	vector<Matr<ld>> d_beg;
	vector<Matr<ld>> d_mid;
	vector<Matr<ld>> d_out;	
};

class CNNetw {
	
	ld dt;
	int crdCount;
	ll padding;
	vector<int> krnlSizes;

	ll inpX;
	ll inpY;

	vector<vector<Matr<ld>>> Kernels {};
	vector<vector<Matr<ld>>> d_Kernels {};
	vector<vector<Matr<ld>>> AKernels {};
	vector<vector<Matr<ld>>> d_AKernels {};

	vector<ll> poolPrms {};

	vector<CNN_Layer> lrsResults {};
	vector<CNNd_Layer> d_lrsResults {};
	
	Matr<ld>* cnnOutV;
	Matr<ld>* d_cnnOutV;
	MLPNetw* MLP;

	ld (*actFunc)(const ld&);
	ld (*d_actFunc)(const ld&);
	static ld abs(const ld& x);
	static ld d_abs(const ld& x);

public:
	CNNetw(ld dt_, ll inpX_, ll inpY_, ll padding_, int crdCount_,
			vector<int> krnlSizes, vector<ll> poolPrms_, MLPNetw* MLP_,
			ld (*actFunc_)(const ld&), ld (*d_actFunc_)(const ld&));

	void genRandK();
	void setNullResults();
	void calcRes(const Matr<ld>& inpM);
	void calcDeltas(const Matr<ld>& InpM, const Matr<ld>& AnsV);
	//void calcDeltas(const Matr<ld>& InpM, const Matr<ld>& AnsV, const Matr<ld>& DELTA);
	void updateK();
	void updateMLP_withCalc(const Matr<ld>& InpM, const Matr<ld>& AnsV);
	void updateK_withCalc(const Matr<ld>& InpM, const Matr<ld>& AnsV);
	//void updateAll_withCalc(const Matr<ld>& InpM, const Matr<ld>& AnsV, const Matr<ld>& DELTA);
	void printK();
	void printW();
	void printResults();

	const Matr<ld>& getRes();

	~CNNetw();

};

#endif
