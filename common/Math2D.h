/*
 Copyright (C) 2013 by Anton Gaenko
 Mail anton.gaenko@yahoo.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */



#ifndef MATH2D_H
#define MATH2D_H

#include <vector>
#include <math.h>
#include <limits>

namespace math2d {
  static const float PI = 3.141592;
  static const float FLOAT_COMPARISON_PRECISION = 1e-5;
  static const float DegreesToRadians = PI / 180;
  enum AngleMeasure {Degree, Radians};


  /**
   * Simple array with size and some useful operators.
   * TODO Move out member definitions
  */
  template <typename T, int Size>
  class Array {
  public:
    Array() {}

    virtual T& operator [](const int index) {return a[index];}

    virtual T operator [](const int index) const {return a[index];}

    // if copied array is bigger than copy only part
    // if smaller set all remain elements to 0
    template <int AnySize>Array& operator = (const Array<T, AnySize>& another) {
      for (int i = 0; i < Size; i++) {
        // set all above elements to 0
        a[i] = i < another.getSize() ? another[i] : 0;
      }
      return *this;
    }

    bool operator ==(const Array &another) const {
      for (int i = 0; i < Size; i++) {
        if (another[i] != a[i]) return false;
      }
      return true;
    }

    T *getArrayC() {return a;}

    int getSize() const {return Size;};

  protected:
    T a[Size];
  };

  // TODO make all such declarations at the beginning
  template <class T>
  class Vector3;

  /**
  * Square matrix [N][N].
  * Is can be only Identity or Nil.
  * And all transformations we do by multiplication.
  * TODO Move out member definitions
  */
  template <class T, int Size>
  class SquareMatrix {
  public:
    SquareMatrix(bool isIdentity = false) {
      for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
          _m[i][j] = isIdentity && (i == j) ? 1 : 0;
        }
      }
    }

    // return all elements in one-dimension array row by row
    Array<T, Size * Size> flat() {
      Array<T, Size * Size> arrayToFill;
      for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
          arrayToFill[i * Size + j] = _m[i][j];
        }
      }
      return arrayToFill;
    }

    /**
     * Multiply to other square matrix with the same size using the simplest algorithm with O(n^3).
     * Return new matrix.
     */
    virtual SquareMatrix<T,Size> operator *(const SquareMatrix<T,Size> &on) {
      SquareMatrix r;

      for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
          r._m[i][j] = 0;
          for (int k = 0; k < Size; k++) {
            r._m[i][j] = r._m[i][j] + _m[i][k] * on._m[k][j];
          }
        }
      }

      return r;
    }

    int getSize() {return Size;}

    // to allow * operator in Vector
    friend Vector3<T>;

  protected:
    // matrix[row][column]
    T _m[Size][Size];

  };



  /**
  * Matrix 3x3
  *
  * TODO unknown type Vector3
   */
  template<class T>
  using Matrix33 = SquareMatrix<T, 3>;

  /**
  * Rotate matrix 3x3.
  * User should set angle for transformations.
  * Positive angle specify counter-clockwise rotation.
  */
  template <class T>
  class RotateMatrix33 : public Matrix33<T> {
  public:
    RotateMatrix33(float angle, AngleMeasure in = Radians):Matrix33<T>(true) {
      float angleInRadians = in == Radians ? angle : angle * DegreesToRadians;
      this->_m[0][0] = cosf(angleInRadians);
      this->_m[0][1] = sinf(angleInRadians);
      this->_m[1][0] = -sinf(angleInRadians);
      this->_m[1][1] = cosf(angleInRadians);
    }

  };

  /**
  * Translate matrix 3x3.
  * It add dx to X and dy to Y coordinates.
  * (1,1,1) * TranslateMatrix33(1,1) = (2,2,1)
  */
  template <class T>
  class TranslateMatrix33 : public Matrix33<T> {
  public:
    TranslateMatrix33(float dx, float dy):Matrix33<T>(true) {
      this->_m[2][0] = dx;
      this->_m[2][1] = dy;
    }
  };

  /**
  * Scale matrix 3x3.
  * (1,1,1) * ScaleMatrix33(2) = (1,1,1/2)
  */
  template <class T>
  class ScaleMatrix33 : public Matrix33<T> {
  public:
    ScaleMatrix33(float scale):Matrix33<T>(true) {
      // In homogeneous coordinates X and Y is dividing by this factor X/Z.
      // It's why, here we use reverse value.
      this->_m[2][2] = 1.0 / scale;
    }
  };

  /**
   * Vector-row in homogeneous coordinates
   */
  template <class T>
  class Vector3 {
  public:
    Vector3(T x = 0, T y = 0, T z = 0):_x(x), _y(y), _z(z) {}

    T getX() const {
      return _x;
    }

    T getY() const {
      return _y;
    }

    T getZ() const {
      return _z;
    }

    T operator [](int num) const {
      switch (num) {
        case 0:
          return _x;
        case 1:
          return _y;
        case 2:
          return _z;
          // TODO throw error
        default:
          return 0;
      }
    }

    Vector3<T> operator*(const SquareMatrix<T, 3> &m) {
      return Vector3<T>(
          getX() * m._m[0][0] + getY() * m._m[1][0] + getZ() * m._m[2][0],
          getX() * m._m[0][1] + getY() * m._m[1][1] + getZ() * m._m[2][1],
          getX() * m._m[0][2] + getY() * m._m[1][2] + getZ() * m._m[2][2]
      );
    };

  private:
    static const int size = 3;
    T _x;
    T _y;
    T _z;
  };


  typedef Matrix33<float> Matrix;
  typedef RotateMatrix33<float> RotateMatrix;
  typedef TranslateMatrix33<float> TranslateMatrix;
  typedef ScaleMatrix33<float> ScaleMatrix;
  typedef Array<float, 9> FlatMatrix;
  typedef Vector3<float> Vector;

  bool isEqual(const Vector &v, const Vector &v2);

  /**
  * Geometry is an array of Vector.
  */
  template <class T, int Size>
  class Geometry : public Array<Vector3<T>, Size> {
    public:
    Geometry():Array<Vector3<T>, Size>() {}

    Geometry(const Vector3<T> arrayOfVectors[Size]):Array<Vector3<T>, Size>() {
      for (int i =0; i<Size; i++) {
        this->a[i] = arrayOfVectors[i];
      }
    }

    // it transforms all vectors in geometry
    Geometry operator*(const Matrix33<T> &m) {
      Geometry r;
      for (int i = 0; i<Size; i++) {
        r[i] = this->a[i] * m;
      }
      return r;
    }

    // TODO add rotate(), translate() and scale()
  };


  // Point in 2D space
  // TODO make fields const
  class Point2D {
  public:
    float x;
    float y;
  };

  // RGB color
  // TODO make fields const
  typedef struct colorRGB {
    float r;
    float g;
    float b;
  } ColorRGB;

  // TODO should it be const?
  static const struct colorRGB kBLUE = {0.0, 0.0, 1.0};
  static const struct colorRGB kRED = {1.0, 0.0, 0.0};

  // Viewport POD configuration for internal game objects and graphics recalculation
  typedef struct config {
    const int width;
    const int height;
    // Developer can choose any resolution as default. And all game objects and graphics were calculated for that by designer.
    // We use scale factor for game object's re-calculation to save designed proportion.
    // fe game object with size 10х10px for default resolution (fe 800x480) should be
    // 13х19px for resolution 1024x768 without kept proportions
    // 13х13px (or 15х15 or 19х19 as chose) for resolution 1024x768 with kept proportions
    const float scale;
    const bool isKeptProportion;

  } ViewportConfig;


  /**
  * Matrix 2x2
  */
  class Matrix2 {
  public:
    Matrix2();

    // make this matrix identity
    void setIdentity();

    // return 4 elements in one-dimension array ([0][0],[0][1],[1][0],[1][1])
    void flat(float *arrayToFill);

    Matrix2 operator *(Matrix2 &on);

    // allow to make [][] direct access
    float *operator [](int index);

  protected:
    // matrix[row][column]
    float _m[2][2];
  };

  /**
  * Rotate matrix
  */
  class RotateMatrix2 : public Matrix2 {
  public:
    RotateMatrix2(float angle);
  };

  /**
  * Translate matrix
  */
  class TranslateMatrix2 : public Matrix2 {
  public:
    TranslateMatrix2(float dx, float dy);
  };

  // Rectangle
  class Rectangle {
  private:
    Point2D _topleft;
    Point2D _bottomright;
  public:
    Rectangle(Point2D &topleft, Point2D &bottomright);

    bool isIntersected(Rectangle &r);

    bool isContained(Point2D &p);

    Point2D &getTopLeft();

    Point2D &getBottomRight();
  };
}

#endif /* MATH2D_H */


