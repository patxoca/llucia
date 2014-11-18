// -*- coding: utf-8 -*-

// $Id:$

#ifndef _ARRAY_H_
#define _ARRAY_H_

// Si es defineix la macro DEBUG_ARRAY_H s'afegeix codi que facilita detectar
// i depurar errors originats per punters fora de control. El funcionament és
// el següent:
//
// * per una banda s'afegeix una "tanca" al voltant de la matriu en forma de
//   dos files addicionals, una al començament i l'altra al final. Aquestes
//   files estan inicialitzades amb un valor conegut.
//
// * el mètode 'check_fence()' permet detectar si s'ha sobreescrit el
//   contingut d'alguna d'aquestes files.
//
// Afegir crides a 'check_fence()' en llocs estratégics del programa
// facilitará delimitar l'origen de l'error.


#include <exception>
#include <cstddef> /* NULL */
#include <iostream>
#include <string.h>


struct ArrayException : public std::exception {
};

template< class T=int > class Array {

	int  num_rows;
	int  num_cols;
	T   *data;

	T *allocate(int nrows, int ncols);
	void deallocate();

#ifdef DEBUG_ARRAY_H
	void init_fence(int nrows, int ncols, T* p);
#endif


public:
	typedef enum {
		TRIANGULARIZE,
		DIAGONALIZE,
		NORMALIZE
	} GaussAction;

	// Aplica Gauss a aquesta matriu. En funció del valor de ACTION
	// triangularitza (zeros baix la diagonal), diagonalitza (zeros dalt i
	// baix) o normalitza (diagonalitza i obté uns a la diagonal).
	//
	// Si es pot completar l'operació retorna 1 o -1 en funció del nombre
	// d'intercanvis de files. Si no es pot completar l'operació (apareixen
	// zeros a la diagonal) retorna 0.
	//
	// Important: aquest mètode muta la matriu. Si no es pot triangularitzar
	// el seu contingut queda en un estat indefinit.
	int gauss(GaussAction action=TRIANGULARIZE);

#ifdef DEBUG_ARRAY_H
	bool check_fence() const;
#endif

public:
	// constructors

	// matriu buida
	Array();

	// copia de matriu
	Array(const Array<T> & a);

	// matriu rows x cols inicialitzada amb zeros
	Array(int rows, int cols);

	// matriu rows x cols inicialitzada a partir d'un array C
	Array(int rows, int cols, const T *value);

	~Array();

	// identitat
	static Array<T> identity(int dim);

	// operadors
	Array<T> & operator = (const Array<T> & a);

	// accessors

	int get_num_rows() const;
	int get_num_cols() const;

	T get(int row, int col) const;
	void put(int row, int col, T value);

	// àlgebra lineal

	T det();

	// calcula la inversa de la matriu in situ. Retorna la matriu per
	// possibilitar l'encadenament d'operacions.
	Array<T> inv();
	Array<T> inv(Array<T> & tmp);

	// producte de matrius
	// Array<T> dot(const Array<T> a) const;

	// transposta de la matriu
	// Array<T> t() const;

};

/*
 *  _                 _                           _             _
 * (_)_ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _  ___(_) ___
 * | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` |/ __| |/ _ \
 * | | | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | (__| | (_) |
 * |_|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\___|_|\___/
 *             |_|
 */

template <class T>
Array<T>::Array() {
	num_rows = 0;
	num_cols = 0;
	data = NULL;
}

template <class T>
Array<T>::Array(const Array<T> & a) {
	int size;

	num_rows = a.num_rows;
	num_cols = a.num_cols;
	data = allocate(num_rows, num_cols);
	size = num_rows * num_cols * sizeof(T);
	memcpy(data, a.data, size);
}

template <class T>
Array<T>::Array(int rows, int cols) {
	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);
}

template <class T>
Array<T>::Array(int rows, int cols, const T *value) {
	int size;

	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);
	size = rows * cols * sizeof(T);
	memcpy(data, value, size);
}

template <class T>
Array<T> Array<T>::identity(int dim) {
	Array<T> m(dim, dim);
	int i, j;
	T *dst;

	for (i = 0, dst = m.data; i < dim; i++) {
		for (j = 0; j < dim; j++, dst++) {
			*dst = (i == j) ? 1 : 0;
		}
	}
	return m;
}

template <class T>
Array<T>::~Array() {
	deallocate();
}

template <class T>
Array<T> & Array<T>::operator = (const Array<T> & a) {
	int size;

	if (this != &a) {
		if ((num_rows != a.num_rows) || (num_cols != a.num_cols)) {
			deallocate();
			num_rows = a.num_rows;
			num_cols = a.num_cols;
			data = allocate(num_rows, num_cols);
		}
		size = num_rows * num_cols * sizeof(T);
		memcpy(data, a.data, size);
	}
	return *this;
}

template <class T>
T Array<T>::get(int r, int c) const {
	return data[r * num_cols + c];
}

template <class T>
void Array<T>::put(int row, int col, T value) {
	data[row * num_cols + col] = value;
}

template <class T>
int Array<T>::get_num_rows() const {
	return num_rows;
}

template <class T>
int Array<T>::get_num_cols() const {
	return num_cols;
}

template <class T>
int Array<T>::gauss(GaussAction action) {
	int rows = num_rows;
	int rows_1 = rows - 1;
	int cols = num_cols;
	int sign = 1;
	int delta = cols + 1; // següent element diagonal
	T  *m = data;
	T  *p, *q, *r;

#ifdef DEBUG_ARRAY_H
	if (!check_fence()) {
		std::cout << "gauss:error fence input array" << std::endl;
	}
#endif
	// @TODO: alex 2014-11-07 21:33:51: cal garantir que el nombre de files és
	// major o igual que el de columnes, sino retornar 0
	for (int i = 0; i < rows_1; i++, m += delta) {              // index de la fila/columna del pivot
		if (*m == 0) {                                          // el pivot és 0
			int j;
			q = m;
			p = m + cols;
			for (j = i + 1; (j < rows) &&  (*p == 0); j++, p += cols) {     // busca fila amb pivot != 0
			}
			if (j < rows) {                                     // trobat
				for (int k = i; k < cols; k++) {                // intercanvia
					T tmp = *p;
					*p++ = *q;
					*q++ = tmp;
				}
				sign = -sign;
			} else {
				return 0;
			}
		}
		// en aquest punt *m != 0
		p = m + cols;                                           // fila següent a la del pivot
		for (int j = i + 1; j < rows; j++, p += cols) {         // index de la fila on es fa zero
			if (*p != 0) {                                      // cal pivotar
				T factor = *p / *m;
				r = m;
				q = p;
				for (int k = i; k < cols; k++, q++, r++) {      // pivota
					*q -= factor * *r;
				}
			}
		}
	}

#ifdef DEBUG_ARRAY_H
	if (!check_fence()) {
		std::cout << "gauss:error fence after triangularize" << std::endl;
	}
#endif

	// en aquest punt m apunta a l'element inferior-dret de la
	// diagonal
	if (*m == 0) {
		return 0;
	}

	// en aquest punt la matriu està triangularitzada
	if (action == TRIANGULARIZE) {
		return sign;
	}

	for (int i = rows_1; i > 0; i--, m -= delta) {    // index del pivot
		p = m - cols;
		for (int j = i - 1; j >= 0; j--, p -= cols) { // index de la fila on es fa zero
			T factor = *p / *m;
			*p = 0;
			q = m + rows - i;
			r = p + rows - i;
			for (int k = rows; k < cols; k++) {       // itera sobre els "termes independents"
				*r++ -= factor * *q++;
			}
		}
	}

#ifdef DEBUG_ARRAY_H
	if (!check_fence()) {
		std::cout << "gauss:error fence after diagonalize" << std::endl;
	}
#endif

	// en aquest punt la matriu està diagonalitzada
	if (action == DIAGONALIZE) {
		return sign;
	}

	// m apunta novament a l'element superior-esquerre
	for (int i = 0; i < rows; i++, m += delta) {
		T factor = *m;
		p = m + rows - i;
		for (int j = rows; j < cols; j++) {
			*p++ /= factor;
		}
		*m = 1;
	}

#ifdef DEBUG_ARRAY_H
	if (!check_fence()) {
		std::cout << "gauss:error fence after normalize" << std::endl;
	}
#endif

	return sign;

}

template <class T>
T Array<T>::det() {
	int sign;
	int delta = num_cols + 1;
	T res = static_cast<T>(1);
	T *p = data;

	sign = gauss(TRIANGULARIZE);
	for (int i = 0; i < num_rows; i++, p += delta) {
		res *= *p;
	}
	return res * sign;
}

template <class T>
Array<T> Array<T>::inv() {
	Array<T> m(num_rows, 2 * num_cols);
	return inv(m);
}

template <class T>
Array<T> Array<T>::inv(Array<T> & tmp) {
	int sign;
	T  *src, *dst;

	if (num_rows != num_cols) {
		throw ArrayException();
	}
	if ((tmp.num_rows != num_rows) || (tmp.num_cols != 2 * num_cols)) {
		throw ArrayException();
	}
	src = data;
	dst = tmp.data;
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			*dst++ = *src++;
		}
		for (int j = 0; j < num_cols; j++) {
			*dst++ = (i == j) ? 1 : 0;
		}
	}
	sign = tmp.gauss(NORMALIZE);
	if (sign == 0) {
		throw ArrayException();
	}
	src = tmp.data;
	dst = data;
	for (int i = 0; i < num_rows; i++) {
		src += num_cols; // es posiciona al començament de la sumatriu dreta
		for (int j = 0; j < num_cols; j++) {
			*dst++ = *src++;
		}
	}
	return *this;
}

/* template <class T> */
/* Array<T> Array<T>::dot(const Array<T> a) const { */
/* 	if (num_cols != a.num_rows) { */
/* 		throw ArrayException(); */
/* 	} */
/* 	Array<T> m(num_rows, a.num_cols); */
/* 	for (int i = 0; i < num_rows; i++) { */
/* 		for (int j = 0; j < a.num_cols; j++) { */
/* 			T tmp = static_cast<T>(0); */
/* 			for (int k = 0; k < num_cols; k++) { */
/* 				tmp += data[i][k] * a.data[k][j]; */
/* 			} */
/* 			m.data[i][j] = tmp; */
/* 		} */
/* 	} */
/* 	return m; */
/* } */

/* template <class T> */
/* Array<T> Array<T>::t() const { */
/* 	Array<T> m(num_cols, num_rows); */
/* 	for (int i = 0; i < num_rows; i++) { */
/* 		for (int j = 0; j < num_cols; j++) { */
/* 			m.data[j][i] = data[i][j]; */
/* 		} */
/* 	} */
/* 	return m; */
/* } */

template<class T>
std::ostream & operator << (std::ostream & stream, Array<T> const & value ) {
#ifdef DEBUG_ARRAY_H
	// imprimeix la tanca inicial
	for (int j = 0; j < value.get_num_cols(); j++) {
		stream << value.get(-1, j) << "\t";
	}
	stream << std::endl;
#endif

	for (int i = 0; i < value.get_num_rows(); i++) {
		for (int j = 0; j < value.get_num_cols(); j++) {
			stream << value.get(i, j) << "\t";
		}
		stream << std::endl;
	}

#ifdef DEBUG_ARRAY_H
	// imprimeix la tanca final
	int nr = value.get_num_rows();
	for (int j = 0; j < value.get_num_cols(); j++) {
		stream << value.get(nr, j) << "\t";
	}
	stream << std::endl;
#endif

	return stream;
}

/*
 *             _            _
 *  _ __  _ __(_)_   ____ _| |_
 * | '_ \| '__| \ \ / / _` | __|
 * | |_) | |  | |\ V / (_| | |_
 * | .__/|_|  |_| \_/ \__,_|\__|
 * |_|
 */

template <class T>
T *Array<T>::allocate(int nrows, int ncols) {
#ifdef DEBUG_ARRAY_H
	T *p = (T*)malloc((2 + nrows) * ncols * sizeof(T));
	init_fence(2 + nrows, ncols, p);
	return p + ncols;
#else
	return (T*)malloc(nrows * ncols * sizeof(T));
#endif
}

template <class T>
void Array<T>::deallocate() {
	if (data != NULL) {
#ifdef DEBUG_ARRAY_H
		T *p = data - num_cols;
		free(p);
#else
		free(data);
#endif
		data = NULL;
	}
}

#ifdef DEBUG_ARRAY_H
template <class T>
void Array<T>::init_fence(int nrows, int ncols, T* p) {
	T* q = p;
	for (int i = 0; i < ncols * nrows; i++) {
		*q++ = 42; // the answer to life, the universe and everything
	}
}

template <class T>
bool Array<T>::check_fence() const {
	T *p = data - num_cols;
	T *q = data + num_cols * num_rows;
	for (int i = 0; i < num_cols; i++) {
		if ((*p++ != 42) || (*q++ != 42)) {
			return false;
		}
	}
	return true;
}
#endif

#endif /* _ARRAY_H_ */
