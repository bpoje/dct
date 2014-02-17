#pragma once
#include "complex.h"
#include <memory>

template <class T>
class Array2D
{
public:
	Array2D(int height, int width);
	Array2D(const Array2D& other);
	~Array2D();
	int getHeight();
	int getWidth();
	void setAt(int indexHeight, int indexWidth, T value);
	T getAt(int indexHeight, int indexWidth);
	void fill(T value);
	T findMin();
	T findMax();
	Array2D<T> duplicate();

private:
	std::shared_ptr<std::shared_ptr<T>> pArray;
	int arrayHeight;
	int arrayWidth;
};

template <class T>
Array2D<T>::Array2D(int height, int width)
{
	pArray = NULL;

	arrayHeight = height;
	arrayWidth = width;

	if (height != 0 && width != 0)
	{
		pArray = std::shared_ptr<std::shared_ptr<T>>(new std::shared_ptr<T>[height], std::default_delete<std::shared_ptr<T>[]>());
		for (int i=0; i < height; i++)
		{
			pArray.get()[i].reset(new T[width], std::default_delete<T[]>());
		}
	}
}


template <class T>
Array2D<T>::Array2D(const Array2D& other)
{
	arrayHeight = other.arrayHeight;
	arrayWidth = other.arrayWidth;

	pArray = other.pArray;
}


template <class T>
Array2D<T>::~Array2D()
{
	pArray.reset();
}

template <class T>
int Array2D<T>::getHeight()
{
	return arrayHeight;
}

template <class T>
int Array2D<T>::getWidth()
{
	return arrayWidth;
}

template <class T>
void Array2D<T>::setAt(int indexHeight, int indexWidth, T value)
{
	if (indexHeight < arrayHeight && indexWidth < arrayWidth && indexHeight >= 0 && indexWidth >= 0)
		pArray.get()[indexHeight].get()[indexWidth] = value;
	else
		fprintf(stderr, "Wrong index!\n");
}

template <class T>
T Array2D<T>::getAt(int indexHeight, int indexWidth)
{
	if (indexHeight < arrayHeight && indexWidth < arrayWidth && indexHeight >= 0 && indexWidth >= 0)
		return pArray.get()[indexHeight].get()[indexWidth];
	else
		fprintf(stderr, "Wrong index!\n");

	return 0;
}

template <class T>
void Array2D<T>::fill(T value)
{
	for (int i=0; i < arrayHeight; i++)
		for (int j=0; j < arrayWidth; j++)
			pArray.get()[i].get()[j] = value;
}

template <class T>
T Array2D<T>::findMin()
{
	T min = pArray.get()[0].get()[0];

	for (int i=0; i < arrayHeight; i++)
		for (int j=0; j < arrayWidth; j++)
			if (pArray.get()[i].get()[j] < min)
				min = pArray.get()[i].get()[j];

	return min;
}

template <class T>
T Array2D<T>::findMax()
{
	T max = pArray.get()[0].get()[0];

	for (int i=0; i < arrayHeight; i++)
		for (int j=0; j < arrayWidth; j++)
			if (pArray.get()[i].get()[j] > max)
				max = pArray.get()[i].get()[j];

	return max;
}

template <class T>
Array2D<T> Array2D<T>::duplicate()
{
	Array2D<T> newArray(arrayHeight, arrayWidth);

	for (int i = 0; i < newArray.getHeight(); i++)
		for (int j = 0; j < newArray.getWidth(); j++)
			newArray.setAt(i, j, getAt(i,j));

	return newArray;
}

typedef Array2D<complex> cArray2D;
typedef Array2D<double> dArray2D;
