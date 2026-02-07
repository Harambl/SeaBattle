#include "MLP.h"
#include "CNN.h"
#include "Canvas.h"
#include "Structs.h"
#include <math.h>
#include <QThread>
#include <QString>
#include <QApplication>
#include <QMainWindow>

using std::cin;


ld CNNactFunc__(const ld& x)
{
	return x;
	//return (x > 0) ? x : 0.1 * x;	
}
ld d_CNNactFunc__(const ld& x)
{
	return 1;
	//return (x > 0) ? 1 : 0.1;
}
ld MLPactFunc__(const ld& x)
{
	return (x > 0) ? x : 0 * x;	
}
ld d_MLPactFunc__(const ld& x)
{
	return (x > 0) ? 1 : 0;
}


ld lastActFunc__(const ld& x)
{
	return (x > 0) ? x : 0;	
}
ld d_lastActFunc__(const ld& x)
{
	return (x > 0) ? 1 : 0;
}


ld nonLinearFunc__(const ld& x)
{
	//return x;
	return (x > 0) ? pow(x, 5/4) : -pow(-x, 5/4);
}
ld d_nonLinearFunc__(const ld& x)
{
	//return 1;
	return (x > 0) ? 5/4*pow(x, 1/4) : 5/4*pow(-x, 1/4);
}


ld resFunc__(const ld& x)
{
	//cout << x << endl;
	return (atan(x / 1000000) * 2) / 3.1415926535;
}
ld d_resFunc__(const ld& x)
{
	return 2 / (3.1415926525 * (1 + x * x)) / 1000000;
}


ld lossFunc__(const Matr<ld>& calcRes, const Matr<ld>& Ans)
{
	ld sum {0};
	for(ll i {0}; i < calcRes.size; ++i)
		sum += (calcRes.get(i) - Ans.get(i)) * (calcRes.get(i) - Ans.get(i));
	return sum;
}
void d_lossFuns__(const Matr<ld>& calcRes, Matr<ld>& result, const Matr<ld>& Ans)
{
	if(result.x_sz != calcRes.y_sz || result.y_sz != calcRes.x_sz) result.throwError("d_loss_ r calcRes");
	if(Ans.x_sz != calcRes.x_sz || Ans.y_sz != calcRes.y_sz) Ans.throwError("d_loss_ a calcRes");
	for(ll i {0}; i < calcRes.size; ++i) {
		result.set(i, 2 * (calcRes.get(i) - Ans.get(i)));	
	}
}


void init_and_teach_MLP_CNN(MLPNetw** pMLP, CNNetw** pCNN)
{
	int iters = 100;
	long double dt = 0.00002;

	*pMLP = new MLPNetw {
		dt * 1000000,
		{81, 10, 1},
		MLPactFunc__, d_MLPactFunc__,
		lastActFunc__, d_lastActFunc__,
		nonLinearFunc__, d_nonLinearFunc__,
		resFunc__, d_resFunc__,
		lossFunc__, d_lossFuns__
	};

	//(*pMLP)->setTiedWeights(1);
	//(*pMLP)->setTiedShifts(0);

	*pCNN = new CNNetw {
		dt,	 	  // learning speed
		128, 128,	  // input size
		0,	    	  // padding
		1,	    	  // crdsCount
		{2, 2, 2},	  // kernel sizes
		{2, 3, 2, 1}, 	  // pooling params
		*pMLP,
		CNNactFunc__, d_CNNactFunc__
	};

	return;
	QString name {};
	unsigned char* rawMatr;
	RGBpixel* PixMatr;

	Matr<ld> InpM {128, 128};
	Matr<ld> AnsV {1, 1};

	for(int i {0}; i < iters; ++i) {
		for(int num {1}; num < 4; ++num) {
			for(int d {0}; d < 2; ++d) {

				name = QString("./Images/%1image%2.png").arg(d).arg(num);
				QImage img {name};
				img.convertToFormat(QImage::Format_ARGB32);
				rawMatr = img.bits();
				PixMatr = reinterpret_cast<RGBpixel*>(rawMatr);

				for(int ind {0}; ind < InpM.size; ++ind) {
					InpM.set(ind, static_cast<ld>(239 - PixMatr[ind].r));
				}

				//for(int ind {0}; ind < AnsV.size; ++ind) {
					//AnsV.set(ind, static_cast<ld>(((ind == d) ? 1 : 0)));
				//}

				AnsV.set(0, ((d == 1) ? 1 : 0));

				(*pCNN)->updateK_withCalc(InpM, AnsV);

				//_CNN.printK();
				//_CNN.calcRes(InpM);
				//AnsV.print();
				//_CNN.getRes().print();

			}
		}	
	}

	//_CNN.printK();
	//_CNN.calcRes(InpM);
	//AnsV.print();
	//_CNN.getRes().print();
	//return 0;

	for(int i {0}; i < 10 * iters; ++i) {
		for(int d {0}; d < 2; ++d) {
			for(int num {1}; num < 4; ++num) {

				name = QString("./Images/%1image%2.png").arg(d).arg(num);
				QImage img {name};
				img.convertToFormat(QImage::Format_ARGB32);
				rawMatr = img.bits();
				PixMatr = reinterpret_cast<RGBpixel*>(rawMatr);

				for(int ind {0}; ind < InpM.size; ++ind) {
					InpM.set(ind, static_cast<ld>(239 - PixMatr[ind].r));
				}

				//for(int ind {0}; ind < AnsV.size; ++ind) {
					//AnsV.set(ind, static_cast<ld>(((ind == d) ? 1 : 0)));
				//}
				
				AnsV.set(0, ((d == 1) ? 1 : 0));

				(*pCNN)->updateMLP_withCalc(InpM, AnsV);

			}
		}	
		//if(i % (iters / 10) == 0) cout << i << "/" << iters * 10 << endl;
	}

	//_CNN.printW();
	//_CNN.calcRes(InpM);
	//AnsV.print();
	//_CNN.getRes().print();
	

#if(0)
	for(int d {0}; d <2; ++d) {
		for(int num {1}; num < 4; ++num) {

			name = QString("./Images/%1image%2.png").arg(d).arg(num);
			QImage img {name};
			img.convertToFormat(QImage::Format_ARGB32);
			rawMatr = img.bits();
			PixMatr = reinterpret_cast<RGBpixel*>(rawMatr);

			for(int ind {0}; ind < InpM.size; ++ind) {
				InpM.set(ind, static_cast<ld>(239 - PixMatr[ind].r));
			}

			//for(int ind {0}; ind < AnsV.size; ++ind) {
				//AnsV.set(ind, static_cast<ld>(((ind == d) ? 1 : 0)));
			//}
			AnsV.set(0, ((d == 1) ? 1 : 0));

			_CNN.calcRes(InpM);
			cout << "ANS:" << endl;
			AnsV.print();
			_CNN.getRes().print();

		}
	}	

	//_CNN.printK();

	for(;;) {
		QApplication app(argc, argv);

		QMainWindow MW;
		MW.resize(128, 128);

		Canvas CV(&MW);

		MW.show();
		app.exec();

		QImage img (CV.size(), QImage::Format_ARGB32);
		QPainter painter(&img);
		CV.render(&painter);
		//img.convertToFormat(QImage::Format_ARGB32);
		rawMatr = img.bits();
		PixMatr = reinterpret_cast<RGBpixel*>(rawMatr);

		for(int ind {0}; ind < InpM.size; ++ind) {
			InpM.set(ind, static_cast<ld>(239 - PixMatr[ind].r));
		}

		//for(int ind {0}; ind < AnsV.size; ++ind) {
			//AnsV.set(ind, static_cast<ld>(((ind == d) ? 1 : 0)));
		//}

		_CNN.calcRes(InpM);
		cout << "ANS TO IMAGE:" << endl;
		//_CNN.getRes().print();
	
	}

#endif

}
