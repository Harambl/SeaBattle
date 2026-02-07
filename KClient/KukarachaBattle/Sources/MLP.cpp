#include "MLP.h"

MLPNetw::MLPNetw(
	ld dt_,
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
	void (*d_lossFunc_)(const Matr<ld>& calcRes, Matr<ld>& result, const Matr<ld>& Ans)) :
			dt {dt_}, lrsCounts {lrsCounts_},
	    		actFunc {actFunc_}, d_actFunc {d_actFunc_},
	    		lastActFunc {lastActFunc_}, d_lastActFunc {d_lastActFunc_},
	    		nonLinFunc {nonLinFunc_}, d_nonLinFunc {d_nonLinFunc_},
	    		resFunc {resFunc_}, d_resFunc {d_resFunc_},
	    		lossFunc {lossFunc_}, d_lossFunc {d_lossFunc_}
{
	genRandW();
	setNullLrsResults();
}

int MLPNetw::getInpSize()
{
	return lrsCounts[0];	
}

void MLPNetw::genRandW()
{
	for(size_t i {0}; i < lrsCounts.size() - 1; ++i) {

		Matr<ld> WM {lrsCounts[i + 1], lrsCounts[i]};
		Matr<ld> d_WM {lrsCounts[i + 1], lrsCounts[i]};
		Matr<ld> A_WM {lrsCounts[i + 1], lrsCounts[i]};
		Matr<ld> d_A_WM {lrsCounts[i + 1], lrsCounts[i]};

		for(ll k {0}; k < WM.size; ++k) {
			WM.set(k, static_cast<ld>(std::rand()) / 1e9);
			d_WM.set(k, 0);
			A_WM.set(k, 0);
			d_A_WM.set(k, 0);
		}

		Weights.push_back(std::move(WM));
		d_Weights.push_back(std::move(d_WM));
		ActWeights.push_back(std::move(A_WM));
		d_ActWeights.push_back(std::move(d_A_WM));

	}	

	for(size_t i {1}; i < lrsCounts.size() - 1; ++i) {
		Matr<ld> shw {lrsCounts[i], 1};
		for(ll k {0}; k < shw.size; ++k) {
			shw.set(k, static_cast<ld>(std::rand()) / 1e9);
		}
		shiftsW.push_back(std::move(shw));
	}
}

void MLPNetw::setNullLrsResults()
{
	for(size_t i {1}; i < lrsCounts.size(); ++i) {
		Matr<ld> LResM0 {lrsCounts[i], 1}, LResM1 {lrsCounts[i], 1}, LResM2 {lrsCounts[i], 1};
		Matr<ld> d_LResM0 {1, lrsCounts[i]}, d_LResM1 {1, lrsCounts[i]};
		for(ll k {0}; k < lrsCounts[i]; ++k) {
			LResM0.set(k, 0); LResM1.set(k, 0); LResM2.set(k, 0);
			d_LResM0.set(k, 0), d_LResM1.set(k, 0);
		}
		lrsResults.push_back({
				.input=std::move(LResM0),
				.afterSh=std::move(LResM1),
				.nlOut=std::move(LResM2)
		});
		d_lrsResults.push_back({
				.d_nlOut=std::move(d_LResM0),
				.d_input=std::move(d_LResM1)
		});
	}	

	d_inpLayer = std::move(Matr<ld>(1, lrsCounts[0]));
}

void MLPNetw::setTiedWeights(ld val){
	for(Matr<ld>& W : Weights) {
		for(ll i {0}; i < W.size; ++i) W.set(i, val);
	}
}
void MLPNetw::setTiedShifts(ld val){
	for(Matr<ld>& Sh : shiftsW) {
		for(ll i {0}; i < Sh.size; ++i) Sh.set(i, val);
	}
}

void MLPNetw::calcRes(const Matr<ld>& InpV)
{	
	Matr<ld>::applyFunc(Weights[0], ActWeights[0], this->actFunc);
	Matr<ld>::mul(ActWeights[0], InpV, lrsResults[0].input);
	Matr<ld>::add(lrsResults[0].input, shiftsW[0], lrsResults[0].afterSh);
	Matr<ld>::applyFunc(lrsResults[0].afterSh, lrsResults[0].nlOut, this->nonLinFunc);

	for(size_t i {1}; i < Weights.size(); ++i) {

		if(i == Weights.size() - 1) {
			Matr<ld>::applyFunc(Weights[i], ActWeights[i], this->lastActFunc);
			Matr<ld>::mul(Weights[i], lrsResults[i - 1].nlOut, lrsResults[i].input);
			Matr<ld>::applyFunc(lrsResults[i].input, lrsResults[i].nlOut, this->resFunc);
		}
		else {
			Matr<ld>::applyFunc(Weights[i], ActWeights[i], this->actFunc);
			Matr<ld>::mul(Weights[i], lrsResults[i - 1].nlOut, lrsResults[i].input);
			Matr<ld>::add(lrsResults[i].input, shiftsW[i], lrsResults[i].afterSh);
			Matr<ld>::applyFunc(lrsResults[i].afterSh, lrsResults[i].nlOut, this->nonLinFunc);
		}

	}	
}

void MLPNetw::calcDeltas(const Matr<ld>& InpV, const Matr<ld>& Ans)
{
	this->d_lossFunc(lrsResults[lrsResults.size() - 1].nlOut,
			d_lrsResults[d_lrsResults.size() - 1].d_nlOut,
			Ans);

	
	for(size_t lInd {d_lrsResults.size() - 1} ;; --lInd) {


		if(lInd == d_lrsResults.size() - 1)
			Matr<ld>::multByFunc(d_lrsResults[lInd].d_nlOut, d_lrsResults[lInd].d_input,
					this->d_resFunc, lrsResults[lInd].nlOut);
		else
			Matr<ld>::multByFunc(d_lrsResults[lInd].d_nlOut, d_lrsResults[lInd].d_input,
					this->d_nonLinFunc, lrsResults[lInd].nlOut);


		for(ll x{0}; x < d_ActWeights[lInd].x_sz; ++x) {
			for(ll y {0}; y < d_ActWeights[lInd].y_sz; ++y) {
				if(lInd >= 1)
					d_ActWeights[lInd].set(x, y,
						d_lrsResults[lInd].d_input.get(x) *
						lrsResults[lInd - 1].nlOut.get(y));		
				else
					d_ActWeights[lInd].set(x, y,
						d_lrsResults[lInd].d_input.get(x) *
						InpV.get(y));		
			}
		}	


		this->d_actFunc(4);
		if(lInd == d_lrsResults.size() - 1)
			Matr<ld>::multByFunc(d_ActWeights[lInd], d_Weights[lInd],
					this->d_lastActFunc, Weights[lInd]);
		else
			Matr<ld>::multByFunc(d_ActWeights[lInd], d_Weights[lInd],
					this->d_actFunc, Weights[lInd]);


		if(lInd >= 1)
			Matr<ld>::mul(d_lrsResults[lInd].d_input,
					ActWeights[lInd], d_lrsResults[lInd - 1].d_nlOut);	
		else
			Matr<ld>::mul(d_lrsResults[lInd].d_input,
					ActWeights[lInd], d_inpLayer);	


		if(lInd == 0) break;

	}

}

void MLPNetw::updateW()
{
	for(size_t i {0}; i < Weights.size(); ++i) {
		Matr<ld>::mul(d_Weights[i], dt, d_Weights[i]);
		Matr<ld>::sub(Weights[i], d_Weights[i], Weights[i]);
	}

	for(size_t i {0}; i < shiftsW.size(); ++i) {
		Matr<ld>::mul(d_lrsResults[i].d_input, dt, d_lrsResults[i].d_input);
		Matr<ld>::sub(shiftsW[i], d_lrsResults[i].d_input, shiftsW[i], ExtraSubOp::TRANSPONE);
	}
}

void MLPNetw::updateW_withCalc(const Matr<ld>& InpV, const Matr<ld>& AnsV)
{
	calcRes(InpV);
	calcDeltas(InpV, AnsV);
	updateW();
}

const Matr<ld>& MLPNetw::getResult()
{
	return lrsResults[lrsResults.size() - 1].nlOut;
}

const Matr<ld>& MLPNetw::getInpDelta()
{
	return d_inpLayer;
}

void MLPNetw::printW()
{
	for(size_t i {0}; i < Weights.size(); ++i) {
		cout << "Weights[" << i << "]:";
		Weights[i].print();
		if(i < shiftsW.size()) {
			cout << "shiftsW[" << i << "]:";
			shiftsW[i].print();		
		}
		cout << endl;
	}
}
