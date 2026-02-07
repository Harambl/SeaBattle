#include "Matr.h"
#include "MatrOperations.cpp"


// speialized constructors

template <typename T>
Matr<T>::Matr(long long size_) : size{size_}
{
	body = new T[size];
	if(!body) throw std::bad_alloc();
}
template <typename T>
Matr<T>::Matr(long long x_sz_, long long y_sz_) : Matr(x_sz_ * y_sz_)
{
	x_sz = x_sz_;
	y_sz = y_sz_;	
	log("allocated");
}
template <typename T>
Matr<T>::Matr(T* arr, long long x_sz_, long long y_sz_) : Matr(x_sz_, y_sz_)
{
	memcpy(body, arr, sizeof(T) * x_sz_ * y_sz_);
	log("copied from array");
}

// standard constructors

template <typename T>
Matr<T>::Matr(const Matr& other) : Matr(other.x_sz, other.y_sz)
{	
	memcpy(body, other.body, sizeof(T) * other.size);
	log("copied from matrix");
}
template <typename T>
Matr<T>::Matr(Matr&& other) : x_sz{other.x_sz}, y_sz{other.y_sz}, size{other.size}
{
	body = other.body;
	other.body = nullptr;	

	other.x_sz = 0;
	other.y_sz = 0;
	other.size = 0;

	log("init from moved");
}

// assignments

template <typename T>
Matr<T> Matr<T>::operator=(const Matr<T>& other)
{
	if(x_sz != other.x_sz || y_sz != other.y_sz) {
		if(body) delete [] body;
		body = new T[other.size];
		x_sz = other.x_sz;
		y_sz = other.y_sz;
		size = other.size;
	}
	memcpy(body, other.body, sizeof(T) * other.size);
	log("assigned by copy");
	return *this;
}
template <typename T>
Matr<T> Matr<T>::operator=(Matr<T>&& other)
{
	if(body) delete [] body;

	body = other.body;
	other.body = nullptr;

	x_sz = other.x_sz;
	y_sz = other.y_sz;
	size = other.size;

	other.x_sz = 0;
	other.y_sz = 0;
	other.size = 0;

	log("assigned by move");
	return *this;
}

// logs && errors && checks

template <typename T>
void Matr<T>::log(const char* str) const
{
	#if(LOG)
	printf("Matr[%lld]x[%lld]: (%s)\n", x_sz, y_sz, str);
	#endif
}
template <typename T>
void Matr<T>::throwError(const char* err) const
{
	char error[ERRSIZE];
	sprintf(error, "(!) \t Matr[%lld]x[%lld]: (%s)", x_sz, y_sz, err);
	throw std::runtime_error(error);
}
template <typename T>
void Matr<T>::_checkInd(long long x, long long y) const
{
	if((x < 0 || x >= x_sz) || (y < 0 || y >= y_sz)) {
		char ft[] {"index %c=%lld is out of range"}, err[ERRSIZE];
		if(x < 0 || x >= x_sz)  sprintf(err, ft, 'X', x);
		else 			sprintf(err, ft, 'Y', y);
		throwError(err);
	}
}
template <typename T>
void Matr<T>::_checkInd(long long ind) const
{
	if(ind >= size) {
		char ft[] {"index %lld is out of len=%lld"}, err[ERRSIZE];
		sprintf(err, ft, ind, size);
		throwError(err);
	}
}
template <typename T>
void Matr<T>::_checkBody() const
{
	if(!body) throwError("body = nullptr");
}

// getters && setters

template <typename T>
const T& Matr<T>::get(long long x, long long y) const
{
	_checkBody();
	_checkInd(x, y);
	return body[x * y_sz + y];	
}
template <typename T>
const T& Matr<T>::get(long long ind) const
{
	_checkBody();
	_checkInd(ind);
	return body[ind];	
}
template <typename T>
void Matr<T>::set(long long x, long long y, const T& val)
{
	_checkBody();
	_checkInd(x, y);
	body[x * y_sz + y] = val;		
}
template <typename T>
void Matr<T>::set(long long ind, const T& val)
{
	_checkBody();
	_checkInd(ind);
	body[ind] = val;
}

// print function

template <typename T>
void Matr<T>::print() const
{
	for(long long i {0}; i < size; ++i){
		if(i % y_sz == 0) cout << "\t|\n|\t";
		cout << body[i] << " ";
	} cout << "\t|" << endl;
}
template <>
void Matr<uint8_t>::print() const
{
	cout << "\n|";
	for(long long i {0}; i < size; ++i){
		if(i % y_sz == 0 && i > 0) cout << "|\n|";
		printf("%4d ", body[i]);
	} cout << "|\n" << endl;
}
template <>
void Matr<long double>::print() const
{
	cout << "\n|";
	for(long long i {0}; i < size; ++i){
		if(i % y_sz == 0 && i > 0) cout << "|\n|";
		printf("%7.3Lf ", body[i]);
	} cout << "|\n" << endl;
}

// destructor

template <typename T>
Matr<T>::~Matr()
{
	if(body) delete [] body;		
	log("deleted");
}
