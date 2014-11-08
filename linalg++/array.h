// -*- coding: utf-8 -*-

// $Id:$

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <vector>


template< class T=int > class Array {

	int  num_rows;
	int  num_cols;
	T   **data;

	T **allocate(int nrows, int ncols);

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

	// matriu rows x cols inicialitzada amb zeros
	Array(int rows, int cols);

	// matriu rows x cols inicialitzada a partir d'un array C
	Array(int rows, int cols, const T *value);

	// matriu inicialitza a partir d'un vector de vectors
	Array(std::vector< std::vector< T > > value);

	// matriu creada concatenant dos matrius
	Array(const Array a1, const Array a2);

	// identitat
	Array(int dim);

	~Array();

	// accessors

	int get_num_rows();
	int get_num_cols();

	T get(int row, int col);

	// àlgebra lineal

	T det();

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
Array<T>::Array(int rows, int cols) {
	T *dst;

	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);

	{
		int i;
		T **d;

		for (i = 0, d = data; i < rows; i++, d++) {
			dst = *d;
			for (int j = 0; j < cols; j++) {
				*dst++ = static_cast<T>(0);
			}
		}
	}
}

template <class T>
Array<T>::Array(int rows, int cols, const T *value) {
	const T *src;
	T *dst;

	num_rows = rows;
	num_cols = cols;
	data = allocate(rows, cols);

	src = value;
	{
		int i;
		T **d;

		for (i = 0, d = data; i < rows; i++, d++) {
			dst = *d;
			for (int j = 0; j < cols; j++) {
				*dst++ = *src++;
			}
		}
	}
}

template <class T>
Array<T>::Array(std::vector< std::vector< T > > value) {
	T *dst;

	// @TODO: alex 2014-11-07 19:46:42: açò assumeix que value te dades i que
	// els vectors fila son tots de la mateixa fila. Cal garantir-ho.
	num_rows = value.size();
	num_cols = value[0].size();
	data = allocate(num_rows, num_cols);

	{
		int i;
		T **d;

		for (i = 0, d = data; i < num_rows; i++, d++) {
			dst = *d;
			std::vector<T> fila = value[i];
			for (int j = 0; j < num_cols; j++) {
				*dst++ = fila[j];
			}
		}
	}
}

template <class T>
Array<T>::Array(const Array a1, const Array a2) {
	T *dst, *src;

	// @TODO: alex 2014-11-07 20:33:04: cal garanti que a1 i a2 tinguin el
	// mateix nombre de files.
	num_rows = a1.num_rows;
	num_cols = a1.num_cols + a2.num_cols;
	data = allocate(num_rows, num_cols);

	{
		int i;
		T **d;
		T **s1, **s2;

		for (i = 0, d = data, s1 = a1.data, s2 = a2.data; i < num_rows; i++, d++, s1++, s2++) {
			dst = *d;
			src = *s1;
			for (int j = 0; j < a1.num_cols; j++) {
				*dst++ = *src++;
			}
			src = *s2;
			for (int j = 0; j < a2.num_cols; j++) {
				*dst++ = *src++;
			}
		}
	}
}

template <class T>
Array<T>::Array(int dim) {
	T *dst;

	num_rows = dim;
	num_cols = dim;
	data = allocate(dim, dim);

	{
		int i;
		T **d;

		for (i = 0, d = data; i < dim; i++, d++) {
			dst = *d;
			for (int j = 0; j < dim; j++) {
				*dst++ = (i == j) ? 1 : 0;
			}
		}
	}
}

template <class T>
Array<T>::~Array() {
	//delete data;
}

template <class T>
T Array<T>::get(int r, int c) {
	return data[r][c];
}

template <class T>
int Array<T>::get_num_rows() {
	return num_rows;
}

template <class T>
int Array<T>::get_num_cols() {
	return num_cols;
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

#endif /* _ARRAY_H_ */
