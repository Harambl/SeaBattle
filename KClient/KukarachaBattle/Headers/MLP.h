#ifndef MLP_H
#define MLP_H

#include "Matr.h"
#include <vector>

using std::vector;
using std::pair;
using ld = long double;
using ll = long long;

struct MLP_Layer {
	Matr<ld> input;
	Matr<ld> afterSh;	
	Matr<ld> nlOut;
};

struct MLPd_Layer {
	Matr<ld> d_nlOut;		
	Matr<ld> d_input;		
};

class MLPNetw {

	ld dt;
	vector<int> lrsCounts {};	

	vector<Matr<ld>> shiftsW {};

	vector<Matr<ld>> Weights {};
	vector<Matr<ld>> ActWeights {};
	vector<Matr<ld>> d_ActWeights {};
	vector<Matr<ld>> d_Weights {};

	vector<MLP_Layer> lrsResults {};
	vector<MLPd_Layer> d_lrsResults {};
	Matr<ld> d_inpLayer {0, 0};

	ld (*actFunc)(const ld&);
	ld (*d_actFunc)(const ld&);

	ld (*lastActFunc)(const ld&);
	ld (*d_lastActFunc)(const ld&);

	ld (*nonLinFunc)(const ld&);
	ld (*d_nonLinFunc)(const ld&);

	ld (*resFunc)(const ld&);
	ld (*d_resFunc)(const ld&);

	ld (*lossFunc)(const Matr<ld>& calcRes, const Matr<ld>& Ans);
	void (*d_lossFunc)(const Matr<ld>& calcRes, Matr<ld>& result, const Matr<ld>& Ans);

public:
	MLPNetw(ld dt_,
		vector<int> lrsCounts_,		
		ld (*actFunc_)(const ld&),
		ld (*d_actFunc_)(const ld&),
		ld (*lastActFunc_)(const ld&),
		ld (*d_lastActFunc_)(const ld&),
		ld (*nonLinFunc_)(const ld&),
		ld (*d_nonLinFunc_)(const ld&),
		ld (*resFunc_)(const ld&),
		ld (*d_resFunc_)(const ld&),
		ld (*lossFunc_)(const Matr<ld>& calcRes, const Matr<ld>& Ans),
		void (*d_lossFunc_)(const Matr<ld>&, Matr<ld>& result, const Matr<ld>& Ans));

	int getInpSize();
	void genRandW();
	void setNullLrsResults();
	void calcRes(const Matr<ld>& InpV);
	void calcDeltas(const Matr<ld>& InpV, const Matr<ld>& Ans);
	void updateW();
	void updateW_withCalc(const Matr<ld>& InpV, const Matr<ld>& Ans);

	void setTiedWeights(ld val);
	void setTiedShifts(ld val);

	const Matr<ld>& getResult();
	const Matr<ld>& getInpDelta();
	
	void printW();

	~MLPNetw() = default;
};


#endif
