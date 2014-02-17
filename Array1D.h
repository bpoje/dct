#pragma once
#include "complex.h"
#include <memory>
#include <stdio.h>

template <class T>
class Array1D
{
public:
	Array1D(int size);
	Array1D(const Array1D& other);
	~Array1D();
	int getLength();
	void setAt(int index, T value);
	T getAt(int index);
	Array1D<T> duplicate();

private:
	std::shared_ptr<T> pArray;
	int arraySize;
};

template <class T>
Array1D<T>::Array1D(int size)
{
	pArray = NULL;

	arraySize = size;

	if (size != 0)
		pArray = std::shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

template <class T>
Array1D<T>::Array1D(const Array1D& other)
{
	arraySize = other.arraySize;
	pArray = other.pArray;
}

template <class T>
Array1D<T>::~Array1D()
{
	pArray.reset();
}

template <class T>
int Array1D<T>::getLength()
{
	return arraySize;
}

template <class T>
void Array1D<T>::setAt(int index, T value)
{
	if (index >= 0 && index < arraySize)
		pArray.get()[index] = value;
	else
		fprintf(stderr, "Wrong index!\n");
}

template <class T>
T Array1D<T>::getAt(int index)
{
	if (index >= 0 && index < arraySize)
		return pArray.get()[index];
	else
		fprintf(stderr, "Wrong index!\n");

	return 0;
}

template <class T>
Array1D<T> Array1D<T>::duplicate()
{
	Array1D<T> newArray(arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		T value = getAt(i);
		newArray.setAt(i, value);
	}

	return newArray;
}

typedef Array1D<complex> cArray1D;
typedef Array1D<double> dArray1D;
