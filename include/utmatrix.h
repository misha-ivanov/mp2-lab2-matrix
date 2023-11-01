// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
// Верхнетреугольная матрица - реализация на основе шаблона вектора

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
  ValType *pVector;
  int Size;       // размер вектора
  int StartIndex; // индекс первого элемента вектора
public:
  TVector(int s = 10, int si = 0);
  TVector(const TVector &v);                // конструктор копирования
  ~TVector();
  int GetSize()      { return Size;       } // размер вектора
  int GetStartIndex(){ return StartIndex; } // индекс первого элемента
  ValType& operator[](int pos);             // доступ
  bool operator==(const TVector &v) const;  // сравнение
  bool operator!=(const TVector &v) const;  // сравнение
  TVector& operator=(const TVector &v);     // присваивание

  // скалярные операции
  TVector  operator+(const ValType &val);   // прибавить скаляр
  TVector  operator-(const ValType &val);   // вычесть скаляр
  TVector  operator*(const ValType &val);   // умножить на скаляр

  // векторные операции
  TVector  operator+(TVector &v);     // сложение
  TVector  operator-(TVector &v);     // вычитание
  ValType  operator*(TVector &v);     // скалярное произведение

  // ввод-вывод
  friend istream& operator>>(istream &in, TVector &v)
  {
    for (int i = 0; i < v.Size; i++)
      in >> v.pVector[i];
    return in;
  }
  friend ostream& operator<<(ostream &out, TVector &v)
  {
    for (int i = 0; i < v.Size + v.StartIndex; i++)
      out << v[i] << ' ';
    return out;
  }
};

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
    if (s < 0 || s > MAX_VECTOR_SIZE)
        throw "out_of_range_in_Vector_Size";
    if (si < 0)
        throw "out_of_rangein_StartIndex";
    Size = s;
    StartIndex = si;
    pVector = new ValType[Size];
} /*-------------------------------------------------------------------------*/

template <class ValType> //конструктор копирования
TVector<ValType>::TVector(const TVector<ValType> &v)
{
    pVector = nullptr;
    Size = v.Size;
    StartIndex = v.StartIndex;
    delete[]pVector;
    pVector = new ValType[Size];
    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];
} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>::~TVector()
{
    delete[] pVector;
} /*-------------------------------------------------------------------------*/

template <class ValType> // доступ
ValType& TVector<ValType>::operator[](int pos)
{
    ValType tmp = 0;
    if (pos < 0 || pos > MAX_VECTOR_SIZE || pos - StartIndex >= Size)
        throw "out_of_range_in_operator[]";
    if (pos < StartIndex)
        return tmp;
    return pVector[pos-StartIndex];
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator==(const TVector &v) const
{
    if (Size == v.Size && StartIndex == v.StartIndex) {
        for (int i = 0; i < Size; i++)
            if (pVector[i] != v.pVector[i])
                return false;
        return true;
    }
    return false;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator!=(const TVector &v) const
{
    return !(*this == v);
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TVector<ValType>& TVector<ValType>::operator=(const TVector &v)
{
    if (*this == v)
        return *this;

    Size = v.Size;
    StartIndex = v.StartIndex;
    delete[]pVector;
    pVector = new ValType[Size];
    for (int i = 0; i < Size; i++)
        pVector[i] = v.pVector[i];
    return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // прибавить скаляр
TVector<ValType> TVector<ValType>::operator+(const ValType &val)
{
    TVector<ValType> tmp(Size+StartIndex, 0);
    for (int i = 0; i < tmp.Size; i++)
        tmp[i] = (*this)[i] + ValType(val);
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычесть скаляр
TVector<ValType> TVector<ValType>::operator-(const ValType &val)
{
    TVector<ValType> tmp(Size + StartIndex, 0);
    for (int i = 0; i < tmp.Size; i++)
        tmp[i] = (*this)[i] - ValType(val);
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // умножить на скаляр
TVector<ValType> TVector<ValType>::operator*(const ValType &val)
{
    TVector<ValType> tmp(Size, StartIndex);
    for (int i = 0; i < tmp.Size; i++) 
        tmp.pVector[i] = pVector[i] * ValType(val);
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TVector<ValType> TVector<ValType>::operator+(TVector<ValType> &v)
{
    if (StartIndex + Size != v.StartIndex + v.Size)
        throw "not_euqal_sizes_in_Vector_operator+";
    TVector<ValType> tmp(std::max(Size, v.Size), min(StartIndex, v.StartIndex));
    for (int i = tmp.StartIndex; i < tmp.Size + tmp.StartIndex; i++) {
        tmp[i] = (*this)[i] + v[i];
    }
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TVector<ValType> TVector<ValType>::operator-(TVector<ValType> &v)
{
    if (StartIndex + Size != v.StartIndex + v.Size)
        throw "not_euqal_sizes_in_Vector_operator-";
    TVector<ValType> tmp(std::max(Size, v.Size), min(StartIndex, v.StartIndex));
    for (int i = tmp.StartIndex; i < tmp.Size + tmp.StartIndex; i++) {
        tmp[i] = (*this)[i] - v[i];
    }
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // скалярное произведение
ValType TVector<ValType>::operator*(TVector<ValType> &v)
{
    if (StartIndex + Size != v.StartIndex + v.Size)
        throw "not_euqal_sizes_in_Vector_operator*";
    ValType tmp = 0;
    for (int i = max(StartIndex, v.StartIndex); i < min(Size, v.Size) + max(StartIndex, v.StartIndex); i++)
        tmp = tmp + (*this)[i] * v[i];
    return tmp;
} /*-------------------------------------------------------------------------*/


// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType> >
{
public:
  TMatrix(int s = 10);                           
  TMatrix(const TMatrix &mt);                    // копирование
  TMatrix(const TVector<TVector<ValType> > &mt); // преобразование типа
  bool operator==(const TMatrix &mt) const;      // сравнение
  bool operator!=(const TMatrix &mt) const;      // сравнение
  TMatrix& operator= (const TMatrix &mt);        // присваивание
  TMatrix  operator+ (const TMatrix &mt);        // сложение
  TMatrix  operator- (const TMatrix &mt);        // вычитание

  // ввод / вывод
  friend istream& operator>>(istream &in, TMatrix &mt)
  {
    for (int i = 0; i < mt.Size; i++)
      in >> mt.pVector[i];
    return in;
  }
  friend ostream & operator<<( ostream &out, const TMatrix &mt)
  {
    for (int i = 0; i < mt.Size; i++)
      out << mt.pVector[i] << endl;
    return out;
  }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s): TVector<TVector<ValType> >(s)
{
    if (s < 0 || s > MAX_MATRIX_SIZE)
        throw "out_of_range_in_Matrix_Size";
    for (int i = 0; i < s; i++) {
        TVector<ValType> v(Size - i, i);
        pVector[i] = v;
    }
} /*-------------------------------------------------------------------------*/

template <class ValType> // конструктор копирования
TMatrix<ValType>::TMatrix(const TMatrix<ValType> &mt):TVector<TVector<ValType>>(mt) 
{
   for (int i = 0; i < Size; i++)
       pVector[i] = mt.pVector[i];
}

template <class ValType> // конструктор преобразования типа
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType> > &mt):
  TVector<TVector<ValType> >(mt) 
{
}

template <class ValType> // сравнение
bool TMatrix<ValType>::operator==(const TMatrix<ValType> &mt) const
{
    if (this == &mt)
        return true;
    if (Size != mt.Size)
        return false;
    for (int i = 0; i < Size; i++)
        if (pVector[i] != mt.pVector[i])
            return false;
    return true;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TMatrix<ValType>::operator!=(const TMatrix<ValType> &mt) const
{
    return !(*this == mt);
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType> &mt)
{
    if (*this == mt)
        return *this;

    if (Size != mt.Size) {
        Size = mt.Size;
        TVector<ValType>* tmp = new TVector<ValType>[Size];
        delete[]pVector;
        pVector = tmp;
    }

    for (int i = 0; i < Size; i++)
        pVector[i] = mt.pVector[i];
    return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType> &mt)
{
    if (Size != mt.Size)
        throw "not_euqal_sizes_in_Matrix_operator+";
    TMatrix<ValType > tmp(Size);
    for (int i = 0; i < Size; i++)
    {
        tmp.pVector[i] = pVector[i] + mt.pVector[i];
    }
    return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType> &mt)
{
    if (Size != mt.Size)
        throw "not_euqal_sizes_in_Matrix_operator-";
    TMatrix<ValType > tmp(Size);
    for (int i = 0; i < Size; i++)
    {
        tmp.pVector[i] = pVector[i] - mt.pVector[i];
    }
    return tmp;
} /*-------------------------------------------------------------------------*/

// TVector О3 Л2 П4 С6
// TMatrix О2 Л2 П3 С3
#endif
