//-----------------------------------------------------------------------
// CAngle class
// This class is template and permits you to perform basic operations over 
// with. It works with radians.
//-----------------------------------------------------------------------
#pragma once

#ifndef __ANGLE_H__
#define __ANGLE_H__

// includes base
#include "Base.h"
#include "MathTypes.h"
#include "MathUtils.h"

template<typename T>
class CAngle
{
public:
  CAngle() : m_Value(0.0f) {}
  CAngle(T value) : m_Value(value) {}
  CAngle(const CAngle& other) : m_Value(other.m_Value) {}
  ~CAngle() {};

  inline void SetRad(T value);
  inline void SetDeg(T value);

  inline T GetRad() const;
  inline T GetDeg() const;

  void Canonize();
  inline T GetCanonical() const;

  // Operators
  inline CAngle<T>  operator+  (const CAngle<T>& other) const;
  inline CAngle<T>& operator+= (const CAngle<T>& other);
  
  inline CAngle<T>  operator+  (T value) const;
  inline CAngle<T>& operator+= (T value);

  inline CAngle<T>  operator-  (const CAngle<T>& other) const;
  inline CAngle<T>& operator-= (const CAngle<T>& other);

  inline CAngle<T>  operator-  (T value) const;
  inline CAngle<T>& operator-= (T value);

  inline CAngle<T>  operator*  (T value) const;
  inline CAngle<T>& operator*= (T value);

  inline CAngle<T>  operator/  (T value) const;
  inline CAngle<T>& operator/= (T value);

  inline bool operator ==       (const CAngle<T>& other) const;
  inline bool operator !=       (const CAngle<T>& other) const;
  inline bool IsEqualEpsilon    (const CAngle<T>& other, const T Epsilo = Epsilon<T>()) const;
  inline bool IsNotEqualEpsilon (const CAngle<T>& other, const T Epsilo = Epsilon<T>()) const;

  CAngle<T> GetDistance (const CAngle<T>& other) const;
  CAngle<T> GetCanonicalDistance (const CAngle<T>& other) const;
  inline CAngle<T> GetLerp (const CAngle<T>& other, const T t) const;
  inline CAngle<T> GetCanonicalLerp (const CAngle<T>& other, const T t) const;

private:

  T m_Value;
};

//------------------------------------------------
/// Extern operators
//------------------------------------------------

template<typename T> 
inline CAngle<T> operator+ (const T value, const CAngle<T>& other);

template<typename T> 
inline CAngle<T> operator- (const T value, const CAngle<T>& other);

//------------------------------------------------
// Inline functions
//------------------------------------------------
#include "Angle.inl"

//------------------------------------------------
/// Angle types
//------------------------------------------------
typedef CAngle<float>  CAnglef;
typedef CAngle<double> CAngled;

#endif
