// -*- coding: utf-8 -*-

// $Id:$

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <exception>
#include <cstddef> /* NULL */
#include <string.h>


struct ArrayException : public std::exception {
};

template< class T=int > class Array {

	int  num_rows;
	int  num_cols;
	T   **data;

	T **allocate(int nrows, int ncols);
	void deallocate();

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

	int get_num_rows();
	int get_num_cols();

	T get(int row, int col);
	void put(int row, int col, T value);

	// àlgebra lineal

	T det();

	// calcula la inversa de la matriu in situ. Retorna la matriu per
	// possibilitar l'encadenament d'operacions.
	Array<T> inv();
	Array<T> inv(Array<T> & tmp);

	// producte de matrius
	Array<T> dot(const Array<T> a) const;

	// transposta de la matriu
	Array<T> t() const;

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
	int i, size, nr;
	T **dst, **src;

	num_rows = a.num_rows;
	num_cols = a.num_cols;
	data = allocate(num_rows, num_cols);

	size = num_cols * sizeof(T);
	nr = num_rows;
	for (i = 0, dst = data, src = a.data; i < nr; i++, dst++, src++) {
		memcpy(*dst, *src, size);
	}
}

template <class T>
Array<T>::Array(int rows, int cols) {
	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);
}

template <class T>
Array<T>::Array(int rows, int cols, const T *value) {
	const T *src;
	T **dst;
	int i, size;

	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);

	src = value;
	size = cols * sizeof(T);
	for (i = 0, dst = data; i < rows; i++, dst++, src += cols) {
		memcpy(*dst, src, size);
	}
}

template <class T>
Array<T> Array<T>::identity(int dim) {
	Array<T> m(dim, dim);
	int i;
	T **dst;

	for (i = 0, dst = m.data; i < dim; i++, dst++) {
		(*dst)[i] = 1;
	}
	return m;
}

template <class T>
Array<T>::~Array() {
	deallocate();
}

template <class T>
Array<T> & Array<T>::operator = (const Array<T> & a) {
	int i, j;
	T **dst, **src;
	int size;
	int nr;

	if (this != &a) {
		if ((num_rows != a.num_rows) || (num_cols != a.num_cols)) {
			deallocate();
			num_rows = a.num_rows;
			num_cols = a.num_cols;
			data = allocate(num_rows, num_cols);
		}
		size = num_cols * sizeof(T);
		nr = num_rows;
		for (i = 0, dst = data, src = a.data; i < nr; i++, dst++, src++) {
			memcpy(*dst, *src, size);
		}
	}
	return *this;
}

template <class T>
T Array<T>::get(int r, int c) {
	return data[r][c];
}

template <class T>
void Array<T>::put(int row, int col, T value) {
	data[row][col] = value;
}

template <class T>
int Array<T>::get_num_rows() {
	return num_rows;
}

template <class T>
int Array<T>::get_num_cols() {
	return num_cols;
}


template <class T>
int Array<T>::gauss(GaussAction action) {
	int rows = num_rows;
	int cols = num_cols;
	int sign = 1;
	T **m = data;

	// @TODO: alex 2014-11-07 21:33:51: cal garantir que el nombre de files és
	// major o igual que el de columnes, sino retornar 0
	for (int i = 0; i < rows - 1; i++) {             // index de la fila/columna del pivot
		if (m[i][i] == 0) {
			for (int j = i + 1; j < rows; j++) {     // busca fila amb pivot != 0
				if (m[j][i] != 0) {                  // intercanvia
					T* tmp = m[j];
					m[j] = m[i];
					m[i] = tmp;
					sign = -sign;
					break;
				}
			}
		}
		if (m[i][i] == 0) { // no es pot continuar
			return 0;
		}
		for (int j = i + 1; j < rows; j++) {     // index de la fila on es fa zero
			if (m[j][i] != 0) {                  // cal pivotar
				T factor = m[j][i] / m[i][i];
				for (int k = i; k < cols; k++) { // pivota
					m[j][k] -= factor * m[i][k];
				}
			}
		}
	}
	if (m[rows - 1][rows - 1] == 0) {
		return 0;
	}

	// en aquest punt la matriu està triangularitzada
	if (action == TRIANGULARIZE) {
		return sign;
	}

	for (int i = rows - 1; i > 0; i--) {   // index del pivot
		for (int j = i - 1; j >= 0; j--) { // indes de la fila on es fa zero
			T factor = m[j][i] / m[i][i];
			for (int k = i; k < cols; k++) {
				m[j][k] -= factor * m[i][k];
			}
		}
	}

	// en aquest punt la matriu està diagonalitzada
	if (action == DIAGONALIZE) {
		return sign;
	}

	for (int i = 0; i < rows; i++) {
		T factor = m[i][i];
		for (int j = rows; j < cols; j++) {
			m[i][j] /= factor;
		}
		m[i][i] = 1;
	}

	return sign;

}

template <class T>
T Array<T>::det() {
	int sign;
	T res = static_cast<T>(1);

	sign = gauss(TRIANGULARIZE);
	for (int i = 0; i < num_rows; i++) {
		res *= data[i][i];
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

	if (num_rows != num_cols) {
		throw ArrayException();
	}
	if ((tmp.num_rows != num_rows) || (tmp.num_cols != 2 * num_cols)) {
		throw ArrayException();
	}
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			tmp.data[i][j] = data[i][j];
			tmp.data[i][j + num_cols] = (i == j) ? 1 : 0;
		}
	}
	sign = tmp.gauss(NORMALIZE);
	if (sign == 0) {
		throw ArrayException();
	}
	for (int i = 0; i < num_rows; i++) {
		for (int j = num_cols; j < 2 * num_cols; j++) {
			data[i][j - num_cols] = tmp.data[i][j];
		}
	}
	return *this;
}

template <class T>
Array<T> Array<T>::dot(const Array<T> a) const {
	if (num_cols != a.num_rows) {
		throw ArrayException();
	}
	Array<T> m(num_rows, a.num_cols);
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < a.num_cols; j++) {
			T tmp = static_cast<T>(0);
			for (int k = 0; k < num_cols; k++) {
				tmp += data[i][k] * a.data[k][j];
			}
			m.data[i][j] = tmp;
		}
	}
	return m;
}

template <class T>
Array<T> Array<T>::t() const {
	Array<T> m(num_cols, num_rows);
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			m.data[j][i] = data[i][j];
		}
	}
	return m;
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
T **Array<T>::allocate(int nrows, int ncols) {
	T **result;

	result = new T*[nrows];
	for (int i = 0; i < nrows; i++) {
		result[i] = new T[ncols];
	}
	return result;
}

template <class T>
void Array<T>::deallocate() {
	if (data != NULL) {
		for (int i = 0; i < num_rows; i++) {
			delete [] data[i];
		}
		delete [] data;
		data = NULL;
	}
}

#endif /* _ARRAY_H_ */
