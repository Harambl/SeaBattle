#ifndef MATR_H
#define MATR_H

#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cstring>
#include <cstdint>

#define ERRSIZE			256
#define LOG			false

using std::cout;
using std::endl;
using std::vector;

enum class ExtraSubOp { NOTHING, ROTATE, TRANSPONE };
enum class ExtFMulOp { NOTHING, ROTATE };


template <typename T>
class Matr {
	T* body {nullptr};
	void log(const char* str) const;
	void _checkBody() const;
	void _checkInd(long long x, long long y) const;
	void _checkInd(long long ind) const;

public:
	long long x_sz {0};
	long long y_sz {0};
	long long size {0};

	Matr(long long size);
	Matr(long long x_sz_, long long y_sz_);
	Matr(T* arr, long long x_sz_, long long y_sz_);

	Matr(const Matr& other);
	Matr(Matr&& other);

	Matr<T> operator=(const Matr<T>& other);
	Matr<T> operator=(Matr<T>&& other);

	void ch_size(long long x_sz, long long y_sz);
	static void concat2V(const vector<Matr<T>>& Matrs, Matr<T>* result);
	static void deconcatV2Ms(const Matr<T>& V, vector<Matr<T>>& result);

	static void add(const Matr<T>& first, const Matr<T>& second, Matr<T>& result); 	// in MatrOperations.cpp
	static void add(const Matr<T>& matr, const T& val, Matr<T>& result); 		// in MatrOperations.cpp
	static void sub(const Matr<T>& first, const Matr<T>& second,
			Matr<T>& result, ExtraSubOp extOp = ExtraSubOp::NOTHING);	// in MatrOperations.cpp
	static void mul(const Matr<T>& first, const Matr<T>& second, Matr<T>& result);	// in MatrOperations.cpp
	static void mul(const Matr<T>& matr, const T& val, Matr<T>& result);		// in MatrOperations.cpp
	
	static long long calcConvSizeX(const Matr<T>& matr, const Matr<T>& kernel,
			long long paddingX, long long strideX = 1);
	static long long calcConvSizeX(long long mx_sz, long long kx_sz,
			long long paddingX, long long strideX = 1);

	static long long calcConvSizeY(const Matr<T>& matr, const Matr<T>& kernel,
			long long paddingY, long long strideY = 1);
	static long long calcConvSizeY(long long my_sz, long long ky_sz,
			long long paddingY, long long strideY = 1);

	static long long calcFullConvSizeX(const Matr<T>& matr, const Matr<T>& kernel, long long paddingX);

	static long long calcFullConvSizeY(const Matr<T>& matr, const Matr<T>& kernel, long long paddingY);
								
	static void conv(
			const Matr<T>& matr,
			const Matr<T>& kernel,
			Matr<T>& result,
			long long paddingX,
			long long paddingY,
			long long strideX = 1,
			long long strideY = 1
	);										 // in MatrOperations.cpp
								
	static void fullConv(
			const Matr<T>& matr,
			const Matr<T>& kernel,
			Matr<T>& result,
			long long paddingX,
			long long paddingY
	);										 // in MatrOperations.cpp
								
	static void transp(const Matr<T>& matr, Matr<T>& result);			 // in MatrOperations.cpp
		
	static void avgPooling(const Matr<T>& matr,
		       	Matr<T>& result, long long poolX, long long poolY);		 // in MatrOperations.cpp
	static void deAvgPooling(const Matr<T>& pldM,
		       	Matr<T>& result, long long poolX, long long poolY);		 // in MatrOperations.cpp

	static void applyFunc(const Matr<T>& matr,
			Matr<T>& result, T (*func)(const T&)); 				// in MatrOperations.cpp
	static void multByFunc(const Matr<T>& matr,
			Matr<T>& result, T (*func)(const T&),
			const Matr<T>& FInp, ExtFMulOp extOp = ExtFMulOp::NOTHING);	// in MatrOperations.cpp
	
	void set(long long x, long long y, const T& val);
	void set(long long ind, const T& val);

	const T& get(long long x_ind, long long y_ind) const;
	const T& get(long long ind) const;

	void print() const;
	void throwError(const char* err) const;

	~Matr();
};

template class Matr<long double>;
template class Matr<float>;

#endif
