#include "CNN.h"

CNNetw::CNNetw(ld dt_, ll inpX_, ll inpY_, ll padding_, int crdCount_,
		vector<int> krnlSizes_, vector<ll> poolPrms_,
		MLPNetw* MLP_, ld (*actFunc_)(const ld&), ld (*d_actFunc_)(const ld&)) :
	krnlSizes {krnlSizes_},
	poolPrms {poolPrms_}, MLP{MLP_},
	actFunc {actFunc_}, d_actFunc {d_actFunc_}
{
	dt = dt_; inpX = inpX_; inpY = inpY_;
	padding = padding_; crdCount = crdCount_;
	genRandK();	
	setNullResults();

	cnnOutV = new Matr<ld>({lrsResults[lrsResults.size() - 1].pooled[0].size * crdCount, 1});
	d_cnnOutV = new Matr<ld>({lrsResults[lrsResults.size() - 1].pooled[0].size * crdCount, 1});

	// lrsResults.size() = krnlSizes.size() + 1
	assert(lrsResults.size() == krnlSizes.size() + 1);
	if(MLP->getInpSize() != lrsResults[lrsResults.size() - 1].pooled[0].size * crdCount) {
		char s[ERRSIZE];
		const char* ft = "No connection between MLP input: M[%lld]x[%lld] and CNN out: M[%lld]x[%lld]";
		sprintf(s, ft, MLP->getInpSize(), 1L, lrsResults[lrsResults.size() - 1].pooled[0].size * crdCount, 1L);
		throw std::runtime_error(s);
	}

	puts("LAYERS SIZES: ");
	for(size_t i {1}; i < lrsResults.size(); ++i) {
		cout << "--- Layer " << i << '\n';
		for(int j {0}; j < crdCount; ++j) {
			cout << "Convolved input: x=" << lrsResults[i].cnvdInp[j].x_sz << " y=";
		        cout << lrsResults[i].cnvdInp[j].y_sz << "\t";
			cout << "Pooled output: x=" << lrsResults[i].pooled[j].x_sz << " y=";
			cout << lrsResults[i].pooled[j].y_sz << '\n';	
		}
	} cout << endl;
}

ld CNNetw::abs(const ld& x)
{
	return ((x > 0) ? x : -x);
}
ld CNNetw::d_abs(const ld& x) 
{
	return ((x > 0) ? 1 : -1);	
}

void CNNetw::genRandK()
{
	for(size_t i {0}; i < krnlSizes.size(); ++i) {

		vector<Matr<ld>> clKernels {};
		vector<Matr<ld>> d_clKernels {};
		vector<Matr<ld>> clAKernels {};
		vector<Matr<ld>> d_clAKernels {};

		for(int j {0}; j < crdCount; ++j) {
			Matr<ld> K {krnlSizes[i], krnlSizes[i]};
			Matr<ld> d_K {krnlSizes[i], krnlSizes[i]};
			Matr<ld> AK {krnlSizes[i], krnlSizes[i]};
			Matr<ld> d_AK {krnlSizes[i], krnlSizes[i]};
			for(ll k {0}; k < K.size; ++k) {
				K.set(k, static_cast<ld>(std::rand()) / 1e9);	
				d_K.set(k, 0);
				AK.set(k, 0);
				d_AK.set(k, 0);
			}
			clKernels.push_back(std::move(K));
			d_clKernels.push_back(std::move(d_K));
			clAKernels.push_back(std::move(AK));
			d_clAKernels.push_back(std::move(d_AK));
		}

		Kernels.push_back(clKernels);
		d_Kernels.push_back(d_clKernels);
		AKernels.push_back(clAKernels);
		d_AKernels.push_back(d_clAKernels);
	}	
}

void CNNetw::setNullResults()
{
	ll cvdIx_sz, cvdIy_sz;
	ll pldx_sz, pldy_sz;
	
	for(size_t i {0}; i < krnlSizes.size() + 1; ++i) {

		vector<Matr<ld>> cnvdInp_ {};
		vector<Matr<ld>> absed_ {};
		vector<Matr<ld>> pooled_ {};

		vector<Matr<ld>> d_beg_ {};
		vector<Matr<ld>> d_mid_ {};
		vector<Matr<ld>> d_out_ {};

		if(i == 0) {

			Matr<ld> cnvdIM {inpX, inpY};
			Matr<ld> d_cnvdIM {inpX, inpY};
			cnvdInp_.push_back(std::move(cnvdIM));
			d_beg_.push_back(std::move(d_cnvdIM));

			Matr<ld> absedM {inpX, inpY};
			Matr<ld> d_absedM {inpX, inpY};
			absed_.push_back(std::move(absedM));
			d_mid_.push_back(std::move(d_absedM));

			pldx_sz = inpX / poolPrms[0];
			pldy_sz = inpY / poolPrms[0];
			Matr<ld> pldM {pldx_sz, pldy_sz};
			Matr<ld> d_pldM {pldx_sz, pldy_sz};
			pooled_.push_back(std::move(pldM));
			d_out_.push_back(std::move(d_pldM));

		} else {

			cvdIx_sz = Matr<ld>::calcConvSizeX(pldx_sz, krnlSizes[i - 1], (i == 1) ? padding : 0);
			cvdIy_sz = Matr<ld>::calcConvSizeY(pldy_sz, krnlSizes[i - 1], (i == 1) ? padding : 0);
			pldx_sz = cvdIx_sz / poolPrms[i];
			pldy_sz = cvdIy_sz / poolPrms[i];

			for(int cCnt {0}; cCnt < crdCount; ++cCnt) {

				Matr<ld> cvdM {cvdIx_sz, cvdIy_sz};
				Matr<ld> d_cvdM {cvdIx_sz, cvdIy_sz};
				cnvdInp_.push_back(std::move(cvdM));
				d_beg_.push_back(std::move(d_cvdM));

				Matr<ld> absM {cvdIx_sz, cvdIy_sz};
				Matr<ld> d_absM {cvdIx_sz, cvdIy_sz};
				absed_.push_back(std::move(absM));
				d_mid_.push_back(std::move(d_absM));

				Matr<ld> pldM {pldx_sz, pldy_sz};
				Matr<ld> d_pldM {pldx_sz, pldy_sz};
				pooled_.push_back(std::move(pldM));
				d_out_.push_back(std::move(d_pldM));

			}

		}
				
		lrsResults.push_back({
			.cnvdInp=cnvdInp_,
			.absed=absed_,
			.pooled=pooled_
		});
		d_lrsResults.push_back({
			.d_beg=d_beg_,
			.d_mid=d_mid_,
			.d_out=d_out_
		});

		for(auto m : cnvdInp_) cout << "cnvdInp: " << m.x_sz << " " << m.y_sz << endl;
		for(auto m : pooled_) cout << "pooled: " << m.x_sz << " " << m.y_sz << endl;
		for(auto m : d_mid_) cout << "d_mid: " << m.x_sz << " " << m.y_sz << endl;
		for(auto m : d_out_) cout << "d_out: " << m.x_sz << " " << m.y_sz << endl;
		cout << endl;
	}	

}

void CNNetw::calcRes(const Matr<ld>& inpM)
{
	if(inpM.x_sz != inpX || inpM.y_sz != inpY) throw std::runtime_error("invalid inp sizes");
	
	Matr<ld>::applyFunc(inpM, lrsResults[0].absed[0], abs);
	Matr<ld>::avgPooling(lrsResults[0].absed[0], lrsResults[0].pooled[0], poolPrms[0], poolPrms[0]);
	for(int i {0}; i < crdCount; ++i) {
		Matr<ld>::applyFunc(Kernels[0][i], AKernels[0][i], this->actFunc);
		Matr<ld>::conv(lrsResults[0].pooled[0], AKernels[0][i], lrsResults[1].cnvdInp[i], padding, padding);	
	}
	
	for(size_t lInd {1}; lInd < lrsResults.size(); ++lInd) {
		for(int i {0}; i < crdCount; ++i) {

			Matr<ld>::applyFunc(lrsResults[lInd].cnvdInp[i],
					lrsResults[lInd].absed[i], abs);

			Matr<ld>::avgPooling(lrsResults[lInd].absed[i], 
					lrsResults[lInd].pooled[i], poolPrms[lInd], poolPrms[lInd]);

			if(lInd < lrsResults.size() - 1) {
				Matr<ld>::applyFunc(Kernels[lInd][i], AKernels[lInd][i], this->actFunc);
				Matr<ld>::conv(lrsResults[lInd].pooled[i],
						AKernels[lInd][i], lrsResults[lInd + 1].cnvdInp[i], 0, 0);		
			}
		}
	}

	Matr<ld>::concat2V(lrsResults[lrsResults.size() - 1].pooled, cnnOutV);
	MLP->calcRes(*cnnOutV);
}


void CNNetw::calcDeltas(const Matr<ld>& InpM, const Matr<ld>& AnsV)
{
	MLP->calcDeltas(*cnnOutV, AnsV);

	Matr<ld>::deconcatV2Ms(MLP->getInpDelta(), d_lrsResults[d_lrsResults.size() - 1].d_out);
	//Matr<ld>::deconcatV2Ms(DELTA, d_lrsResults[d_lrsResults.size() - 1].d_out); 

	// PoolPrms.size() = d_lrsResults.size() = d_Kernels.size() + 1

	size_t endInd {d_lrsResults.size() - 1};

	for(int i {0}; i < crdCount; ++i) {
		Matr<ld>::deAvgPooling(d_lrsResults[endInd].d_out[i],
				d_lrsResults[endInd].d_mid[i], poolPrms[endInd], poolPrms[endInd]);
		Matr<ld>::mul(d_lrsResults[endInd].d_mid[i], poolPrms[endInd] * poolPrms[endInd],
				d_lrsResults[endInd].d_mid[i]);
		Matr<ld>::multByFunc(d_lrsResults[endInd].d_mid[i], d_lrsResults[endInd].d_beg[i],
				d_abs, lrsResults[endInd].cnvdInp[i]);
	}

	for(size_t lInd {endInd - 1}; lInd >= 1; --lInd) {
		for(int i {0}; i < crdCount; ++i) {

			Matr<ld>::conv(lrsResults[lInd].pooled[i], d_lrsResults[lInd + 1].d_beg[i],
					d_AKernels[lInd][i], 0, 0);
			Matr<ld>::multByFunc(d_AKernels[lInd][i], d_Kernels[lInd][i],
					this->d_actFunc, Kernels[lInd][i], ExtFMulOp::ROTATE);	

			Matr<ld>::fullConv(AKernels[lInd][i], d_lrsResults[lInd + 1].d_beg[i],
					d_lrsResults[lInd].d_out[i], 0, 0);

			Matr<ld>::deAvgPooling(d_lrsResults[lInd].d_out[i],
					d_lrsResults[lInd].d_mid[i], poolPrms[lInd], poolPrms[lInd]);
			Matr<ld>::mul(d_lrsResults[lInd].d_mid[i], poolPrms[lInd] * poolPrms[lInd],
					d_lrsResults[lInd].d_mid[i]);

			Matr<ld>::multByFunc(d_lrsResults[lInd].d_mid[i], d_lrsResults[lInd].d_beg[i],
					d_abs, lrsResults[lInd].cnvdInp[i]);

		}		
	}

	for(int i {0}; i < crdCount; ++i) {
		Matr<ld>::conv(lrsResults[0].pooled[0], d_lrsResults[1].d_beg[i],
				d_AKernels[0][i], padding, padding);
		Matr<ld>::multByFunc(d_AKernels[0][i], d_Kernels[0][i],
				this->d_actFunc, Kernels[0][i], ExtFMulOp::ROTATE);	

		//cout << "dKERnels" << endl;
		//lrsResults[0].pooled[0].print();
		//d_lrsResults[1].d_mid[i].print();
		//d_AKernels[0][i].print();
		//d_Kernels[0][i].print();
	}
}

void CNNetw::updateK()
{
	for(size_t lInd {0}; lInd < Kernels.size(); ++lInd) {
		for(int i {0}; i < crdCount; ++i) {
			Matr<ld>::mul(d_Kernels[lInd][i], dt, d_Kernels[lInd][i]);
			Matr<ld>::sub(Kernels[lInd][i], d_Kernels[lInd][i], Kernels[lInd][i], ExtraSubOp::ROTATE);
		}
	}
}

void CNNetw::updateMLP_withCalc(const Matr<ld>& InpM, const Matr<ld>& AnsV)
{
	calcRes(InpM);
	calcDeltas(InpM, AnsV);
	MLP->updateW();
}

void CNNetw::updateK_withCalc(const Matr<ld>& InpM, const Matr<ld>& AnsV)
{
	calcRes(InpM);
	calcDeltas(InpM, AnsV);
	updateK();
}

void CNNetw::printK()
{
	for(size_t i {0}; i < Kernels.size(); ++i) {
		cout << "--- Layer " << i << endl;
		for(Matr<ld> K : Kernels[i]) K.print();
	}	
}

void CNNetw::printW()
{
	MLP->printW();
}

void CNNetw::printResults()
{
	for(int i {0}; i < lrsResults.size(); ++i) {
		cout << "-- Layer Result " << i << endl;
		lrsResults[i].cnvdInp[0].print();
		lrsResults[i].pooled[0].print();	
	}	
}

const Matr<ld>& CNNetw::getRes()
{
	return MLP->getResult();	
}

CNNetw::~CNNetw()
{
	delete cnnOutV;	
	delete d_cnnOutV;	
}
