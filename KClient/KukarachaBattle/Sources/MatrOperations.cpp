#include "Matr.h"

template <typename T>
void Matr<T>::ch_size(long long x_sz_, long long y_sz_)
{
	if(x_sz_ * y_sz_ != size) {
		char ft[] {"cannot convert M[%lld]x[%lld] to M[%lld]x[%lld]"}, err[ERRSIZE];
		sprintf(err, ft, x_sz, y_sz, x_sz_, y_sz_);
		throwError(err);
	}
	x_sz = x_sz_;
	y_sz = y_sz_;
}

template <typename T>
void Matr<T>::concat2V(const vector<Matr<T>>& Matrs, Matr<T>* result)
{
	long long summ_sz {0}, wrInd {0};
	for(size_t i {0}; i < Matrs.size(); ++i) summ_sz += Matrs[i].size;

	if(result->x_sz != summ_sz || result->y_sz != 1) {
		char ft[] {"cannot concat2V *TO* M[%lld]x[%lld]"}, err[ERRSIZE];
		sprintf(err, ft, result->x_sz, result->y_sz);
		result->throwError(err);
	}

	for(size_t i {0}; i < Matrs.size(); ++i) {
		for(long long j {0}; j < Matrs[i].size; ++j) {
			result->set(wrInd++, Matrs[i].get(j));	
		}
	}
}

template <typename T>
void Matr<T>::deconcatV2Ms(const Matr<T>& V, vector<Matr<T>>& result)
{
	long long summ_sz {0}, wrInd {0};
	for(size_t i {0}; i < result.size(); ++i) summ_sz += result[i].size;

	if(V.size != summ_sz) {
		char ft[] {"cannot deconcatV2Ms V[%lld]x[%lld] *TO* vector"}, err[ERRSIZE];
		sprintf(err, ft, V.x_sz, V.y_sz);
		V.throwError(err);
	}

	for(size_t i {0}; i < result.size(); ++i) {
		for(long long j {0}; j < result[i].size; ++j)
			result[i].set(j, V.get(wrInd++));
	}
}

template <typename T>
void Matr<T>::add(const Matr<T>& first, const Matr<T>& second, Matr<T>& result)
{
	if(first.x_sz != second.x_sz || first.y_sz != second.y_sz) {
		char ft[] {"cannot summ M[%lld]x[%lld] with M[%lld]x[%lld]"}, err[ERRSIZE];
		sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz);
		first.throwError(err);
	}
	if(first.x_sz != result.x_sz || first.y_sz != result.y_sz) {
		char ft[] {"cannot summ M[%lld]x[%lld] with M[%lld]x[%lld] to M[%lld][%lld]"}, err[ERRSIZE];
		sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz, result.x_sz, result.y_sz);
		first.throwError(err);
	}
	for(long long i {0}; i < first.size; ++i) {
		result.set(i, first.get(i) + second.get(i));
	}
}
template <typename T>
void Matr<T>::add(const Matr<T>& matr, const T& val, Matr<T>& result)
{
	for(long long i {0}; i < matr.size; ++i) {
		result.set(i, matr.get(i) + val);
	}
}

template <typename T>
void Matr<T>::sub(const Matr<T>& first, const Matr<T>& second, Matr<T>& result, ExtraSubOp extOp)
{
	switch(extOp) {
		case ExtraSubOp::NOTHING:
		case ExtraSubOp::ROTATE:
			if(first.x_sz != second.x_sz || first.y_sz != second.y_sz) {
				char ft[] {
					"cannot sub M[%lld]x[%lld] with M[%lld]x[%lld] with r or without"
				}, err[ERRSIZE];
				sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz);
				first.throwError(err);
			}
			if(first.x_sz != result.x_sz || first.y_sz != result.y_sz) {
				char ft[] {
					"cannot sub M[%lld]x[%lld] with M[%lld]x[%lld] to M[%lld][%lld]"
				}, err[ERRSIZE];
				sprintf(err, ft, first.x_sz, first.y_sz,
						second.x_sz, second.y_sz, result.x_sz, result.y_sz);
				first.throwError(err);
			} break;
		case ExtraSubOp::TRANSPONE:
			if(first.x_sz != second.y_sz || first.y_sz != second.x_sz) {
				char ft[] {
					"cannot sub M[%lld]x[%lld] with M[%lld]x[%lld] and transpone"
				}, err[ERRSIZE];
				sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz);
				first.throwError(err);
			}
			if(first.x_sz != result.x_sz || first.y_sz != result.y_sz) {
				char ft[] {
					"cannot sub M[%lld]x[%lld] with M[%lld]x[%lld] to M[%lld][%lld]"
				}, err[ERRSIZE];
				sprintf(err, ft, first.x_sz, first.y_sz,
						second.x_sz, second.y_sz, result.x_sz, result.y_sz);
				first.throwError(err);
			} break;
	}

	switch(extOp) {
		case ExtraSubOp::ROTATE:
			long long nx, ny;
			for(long long x {0}; x < first.x_sz; ++x) {
				for(long long y {0}; y < first.y_sz; ++y) {
					nx = second.x_sz - x - 1;
					ny = second.y_sz - y - 1;
					result.set(x, y, first.get(x, y) - second.get(nx, ny));
				}
			} break;
		case ExtraSubOp::TRANSPONE:
			for(long long x {0}; x < first.x_sz; ++x) {
				for(long long y {0}; y < first.y_sz; ++y) {
					result.set(x, y, first.get(x, y) - second.get(y, x));
				}
			} break;
		case ExtraSubOp::NOTHING:
			for(long long i {0}; i < first.size; ++i) {
				result.set(i, first.get(i) - second.get(i));
			} break;		
	}
}

template <typename T>
void Matr<T>::mul(const Matr<T>& first, const Matr<T>& second, Matr<T>& result)
{
	if(first.y_sz != second.x_sz) {
		char ft[] {"cannot multiply M[%lld]x[%lld] with M[%lld]x[%lld]"}, err[ERRSIZE];
		sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz);
		first.throwError(err);
	}
	if(first.x_sz != result.x_sz || second.y_sz != result.y_sz) {
		char ft[] {"cannot multiply M[%lld]x[%lld] with M[%lld]x[%lld] to M[%lld][%lld]"}, err[ERRSIZE];
		sprintf(err, ft, first.x_sz, first.y_sz, second.x_sz, second.y_sz, result.x_sz, result.y_sz);
		first.throwError(err);
	}

	// simple matrix multuplication
	
	T sum;
	for(long long x {0}; x < first.x_sz; ++x) {
		for(long long y {0}; y < second.y_sz; ++y) {
			sum = 0;
			for(long long i {0}; i < first.y_sz; ++i) {
				sum += first.get(x, i) * second.get(i, y);
			}
			result.set(x, y, sum);
		}
	}	
}

template <typename T>
void Matr<T>::mul(const Matr<T>& matr, const T& val, Matr<T>& result)
{
	for(long long i {0}; i < matr.size; ++i)
		result.set(i, matr.get(i) * val);
}


template <typename T>
long long Matr<T>::calcConvSizeX(const Matr<T>& matr, const Matr<T>& kernel, long long paddingX, long long strideX)
{
	return (matr.x_sz + 2 * paddingX - kernel.x_sz) / strideX + 1;
}
template <typename T>
long long Matr<T>::calcConvSizeX(long long mx_sz, long long kx_sz, long long paddingX, long long strideX)
{
	return (mx_sz + 2 * paddingX - kx_sz) / strideX + 1;
}
template <typename T>
long long Matr<T>::calcConvSizeY(const Matr<T>& matr, const Matr<T>& kernel, long long paddingY, long long strideY)
{
	return (matr.y_sz + 2 * paddingY - kernel.y_sz) / strideY + 1;
}
template <typename T>
long long Matr<T>::calcConvSizeY(long long my_sz, long long ky_sz, long long paddingY, long long strideY)
{
	return (my_sz + 2 * paddingY - ky_sz) / strideY + 1;
}
template <typename T>
long long Matr<T>::calcFullConvSizeX(const Matr<T>& matr, const Matr<T>& kernel, long long paddingX)
{
	return matr.x_sz + kernel.x_sz - 1 - 2 * paddingX;
}
template <typename T>
long long Matr<T>::calcFullConvSizeY(const Matr<T>& matr, const Matr<T>& kernel, long long paddingY)
{
	return matr.y_sz + kernel.y_sz - 1 - 2 * paddingY;
}


template <typename T>
void Matr<T>::conv(const Matr<T>& matr, const Matr<T>& kernel, Matr<T>& result, long long paddingX,
		long long paddingY, long long strideX, long long strideY)
{
	long long nx_sz {calcConvSizeY(matr, kernel, paddingX, strideX)};
	long long ny_sz {calcConvSizeY(matr, kernel, paddingY, strideY)};

	if(nx_sz <= 0 || ny_sz <= 0) {
		char ft[] {
			"cannot convolve M[%lld]x[%lld] with pX=%lld, pY=%lld, stX=%lld, stY=%lld K[%lld]x[%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, paddingX,
				paddingY, strideX, strideY, kernel.x_sz, kernel.y_sz);
		matr.throwError(err);
	}
	if(nx_sz != result.x_sz || ny_sz != result.y_sz) {
		char ft[] {
			"cannot convolve M[%lld]x[%lld] with \
				pX=%lld, pY=%lld, stX=%lld, stY=%lld K[%lld]x[%lld] *TO* M[%lld][%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, paddingX, paddingY, strideX,
				strideY, kernel.x_sz, kernel.y_sz, result.x_sz, result.y_sz);
		matr.throwError(err);
	}

	T sum;
	long long mx, my, r_kx, r_ky;

	for(long long x {0}; x < nx_sz; ++x) {
		for(long long y {0}; y < ny_sz; ++y) {

			sum = 0;
			for(long long kx {0}; kx < kernel.x_sz; ++kx) {
				for(long long ky {0}; ky < kernel.y_sz; ++ky) {
						
					mx = x * strideX + kx - paddingX;
					my = y * strideY + ky - paddingY;
					r_kx = kernel.x_sz - kx - 1;
					r_ky = kernel.y_sz - ky - 1;

					if(mx < 0 || mx >= matr.x_sz) continue;
					if(my < 0 || my >= matr.y_sz) continue;

					sum += matr.get(mx, my) * kernel.get(r_kx, r_ky);

				}	
			} result.set(x, y, sum);

		}
	}	
}

template <typename T>
void Matr<T>::fullConv(const Matr<T>& matr, const Matr<T>& kernel, Matr<T>& result,
		long long paddingX, long long paddingY)
{
	// strideX = 1, strideY = 1
	// padding = padding + kernel.sz - 1
	long long nx_sz {calcFullConvSizeX(matr, kernel, paddingX)};
	long long ny_sz {calcFullConvSizeY(matr, kernel, paddingY)};
	if(nx_sz <= 0 || ny_sz <= 0) {
		char ft[] {
			"cannot fullConvolve M[%lld]x[%lld] with pX=%lld, pY=%lld, K[%lld]x[%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, paddingX, paddingY, kernel.x_sz, kernel.y_sz);
		matr.throwError(err);
	}
	if(nx_sz != result.x_sz || ny_sz != result.y_sz) {
		char ft[] {
			"cannot convolve M[%lld]x[%lld] with \
				pX=%lld, pY=%lld K[%lld]x[%lld] *TO* M[%lld][%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, paddingX, paddingY,
				kernel.x_sz, kernel.y_sz, result.x_sz, result.y_sz);
		matr.throwError(err);
	}

	T sum;
	long long mx, my, r_kx, r_ky;

	for(long long x {}; x < nx_sz; ++x) {
		for(long long y {0}; y < ny_sz; ++y) {

			sum = 0;
			for(long long kx {0}; kx < kernel.x_sz; ++kx) {
				for(long long ky {0}; ky < kernel.y_sz; ++ky) {
						
					mx = (x + paddingX) + kx - (kernel.x_sz - 1);
					my = (y + paddingY) + ky - (kernel.y_sz - 1);
					r_kx = kernel.x_sz - kx - 1;
					r_ky = kernel.y_sz - ky - 1;

					if(mx < 0 || mx >= matr.x_sz) continue;
					if(my < 0 || my >= matr.y_sz) continue;

					sum += matr.get(mx, my) * kernel.get(r_kx, r_ky);

				}	
			} result.set(x, y, sum);

		}
	}	
}

template <typename T>
void Matr<T>::transp(const Matr<T>& matr, Matr<T>& result)
{
	for(long long x {0}; x < matr.x_sz; ++x) {
		for(long long y {0}; y < matr.y_sz; ++y)
			result.set(y, x, matr.get(x, y));
	}
}

template <typename T>
void Matr<T>::avgPooling(const Matr<T>& matr, Matr<T>& result, long long poolX, long long poolY)
{
	long long nx_sz {matr.x_sz / poolX};
	long long ny_sz {matr.y_sz / poolY};

	if(matr.x_sz % poolX != 0 || matr.y_sz % poolY != 0) {
		char ft[] { "cannot avgPooling M[%lld]x[%lld] with poX=%lld, poY=%lld | not divides |"}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, poolX, poolY);
		matr.throwError(err);
	}
	if(nx_sz <= 0 || ny_sz <= 0) {
		char ft[] { "cannot avgPooling M[%lld]x[%lld] with poX=%lld, poY=%lld"}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, poolX, poolY);
		matr.throwError(err);
	}
	if(nx_sz != result.x_sz || ny_sz != result.y_sz) {
		char ft[] {
			"cannot avgPooling M[%lld]x[%lld] with poX=%lld, poY=%lld *TO* M[%lld][%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, poolX, poolY, result.x_sz, result.y_sz);
		matr.throwError(err);
	}

	T sum;
	long long mx, my;

	for(long long x {0}; x < nx_sz; ++x) {
		for(long long y {0}; y < ny_sz; ++y) {

			sum = 0;
			for(long long kx {0}; kx < poolX; ++kx) {
				for(long long ky {0}; ky < poolY; ++ky) {
						
					mx = x * poolX + kx;
					my = y * poolY + ky;

					if(mx < 0 || mx >= matr.x_sz) continue;
					if(my < 0 || my >= matr.y_sz) continue;

					sum += matr.get(mx, my) / (poolX * poolY);

				}	
			} result.set(x, y, sum);

		}
	}	
}

template <typename T>
void Matr<T>::deAvgPooling(const Matr<T>& pldM, Matr<T>& result, long long poolX, long long poolY)
{
	long long nx_sz {pldM.x_sz * poolX};
	long long ny_sz {pldM.y_sz * poolY};
		
	if(nx_sz != result.x_sz || ny_sz != result.y_sz) {
		char ft[] {
			"cannot deAvgPooling pldM[%lld]x[%lld] with poX=%lld, poY=%lld *TO* M[%lld][%lld]"
		}, err[ERRSIZE];
		sprintf(err, ft, pldM.x_sz, pldM.y_sz, poolX, poolY, result.x_sz, result.y_sz);
		result.throwError(err);
	}

	for(long long x {0}; x < pldM.x_sz; ++x) {
		for(long long y {0}; y < pldM.y_sz; ++y) {
			for(long long px {0}; px < poolX; ++px) {
				for(long long py {0}; py < poolY; ++py) {
					result.set(x * poolX + px, y * poolY + py, pldM.get(x, y));
				}	
			}
		}	
	}
}


template <typename T>
void Matr<T>::applyFunc(const Matr<T>& matr, Matr<T>& result, T (*func)(const T&))
{
	if(matr.x_sz != result.x_sz || matr.y_sz != result.y_sz) {
		char ft[] { "cannot apply function M[%lld]x[%lld] *TO* M[%lld][%lld]" }, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, result.x_sz, result.y_sz);
		matr.throwError(err);
	}
	T res;
	for(long long i {0}; i < matr.size; ++i) {
		res = func(matr.get(i));
		result.set(i, res);	
	}	
}

template <typename T>
void Matr<T>::multByFunc(const Matr<T>& matr, Matr<T>& result,
		T (*func)(const T&), const Matr<T>& FInp, ExtFMulOp extOp)
{
	if(matr.x_sz != result.x_sz || matr.y_sz != result.y_sz) {
		char ft[] { "cannot mult by function M[%lld]x[%lld] *TO* M[%lld][%lld]" }, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, result.x_sz, result.y_sz);
		matr.throwError(err);
	}
	if(matr.size != FInp.size) {
		char ft[] { "cannot mult by function M[%lld]x[%lld] and FuncINPUT M[%lld][%lld]" }, err[ERRSIZE];
		sprintf(err, ft, matr.x_sz, matr.y_sz, FInp.x_sz, FInp.y_sz);
		matr.throwError(err);
	}
	T res;
	switch(extOp) {
		case ExtFMulOp::NOTHING:
			for(long long i {0}; i < matr.size; ++i) {
				res = func(FInp.get(i)) * matr.get(i);
				result.set(i, res);	
			}	
			break;
		case ExtFMulOp::ROTATE:
			for(long long x {0}; x < matr.x_sz; ++x) {
				for(long long y {0}; y < matr.y_sz; ++y) {
					res = func(FInp.get(FInp.x_sz - x - 1, FInp.y_sz - y - 1)) * matr.get(x, y);
					result.set(x, y, res);
				}	
			}
			break;
	}
}



