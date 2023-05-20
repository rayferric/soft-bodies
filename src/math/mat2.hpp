#pragma once

#include <common.hpp>

#include <math/math.hpp>
#include <math/vec2.hpp>

namespace soft_bodies::math {

template<scalar T>
class mat2 {
public:
	static const mat2<T> zero;
	static const mat2<T> identity;

	union {
		class {
		public:
			vec2<T> x, y;
		};
		vec2<T> data[2];
	};

	mat2();

	mat2(T identity);

	mat2(const vec2<T> &x, const vec2<T> &y);

	mat2(T xx, T xy,
		 T yx, T yy);

	mat2(const T *array);

	template<scalar U>
	mat2(const mat2<U> &other);

#pragma region Operators

	template<scalar U>
	friend std::ostream &operator<<(std::ostream &lhs, const mat2<U> &rhs);

	vec2<T> &operator[](size_t index);

	const vec2<T> &operator[](size_t index) const;

	template<scalar U>
	mat2<bool> operator==(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<bool> operator!=(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<bool> operator<=(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<bool> operator>=(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<bool> operator<(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<bool> operator>(const mat2<U> &rhs) const;

	template<boolean U>
	friend mat2<bool> operator!(const mat2<U> &mat);

	mat2<T> operator-() const;

	// Matrix + Matrix

	template<scalar U>
	auto operator+(const mat2<U> &rhs) const;
	
	template<scalar U>
	auto operator-(const mat2<U> &rhs) const;
	
	template<scalar U>
	auto operator*(const mat2<U> &rhs) const;

	template<scalar U>
	mat2<T> &operator+=(const mat2<U> &rhs);

	template<scalar U>
	mat2<T> &operator-=(const mat2<U> &rhs);

	template<scalar U>
	mat2<T> &operator*=(const mat2<U> &rhs);

	// Matrix + Scalar

	template<scalar U>
	auto operator*(U rhs) const;

	template<scalar U>
	auto operator/(U rhs) const;

	template<scalar U>
	mat2<T> &operator*=(U rhs);

	template<scalar U>
	mat2<T> &operator/=(U rhs);

	// Scalar + Matrix

	template<scalar L, scalar R>
	friend auto operator*(L lhs, const mat2<R> &rhs);

	// Matrix + Vector

	template<scalar U>
	auto operator*(const vec2<U> &rhs) const;

#pragma endregion

};

using bmat2 = mat2<bool>;
using imat2 = mat2<int32_t>;
using umat2 = mat2<uint32_t>;
using fmat2 = mat2<float>;
using dmat2 = mat2<double>;

template<scalar L, scalar R>
auto operator*(L lhs, const mat2<R> &rhs);

template<boolean T>
bool all(const mat2<T> &mat);

template<boolean T>
bool any(const mat2<T> &mat);

template<scalar T>
T determinant(const mat2<T> &mat);

template<floating_point T>
mat2<T> inverse(const mat2<T> &mat);

template<scalar A, scalar B,
		scalar Epsilon = decltype(epsilon)>
bool is_approx(const mat2<A> &a, const mat2<B> &b,
		Epsilon epsilon = math::epsilon);

template<scalar T, scalar Epsilon = decltype(epsilon)>
bool is_orthogonal(const mat2<T> &mat,
		Epsilon epsilon = math::epsilon);

template<scalar T, scalar Epsilon = decltype(epsilon)>
bool is_orthonormal(const mat2<T> &mat,
		Epsilon epsilon = math::epsilon);

template<floating_point T>
mat2<T> orthogonalize(const mat2<T> &mat);

template<floating_point T>
mat2<T> orthonormalize(const mat2<T> &mat);

template<scalar T>
mat2<T> transpose(const mat2<T> &mat);

}

#include <math/mat2.inl>
