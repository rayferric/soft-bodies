#include <util/util.hpp>

namespace soft_bodies::math {

template<scalar T>
const mat2<T> mat2<T>::zero(0);

template<scalar T>
const mat2<T> mat2<T>::identity(1);

template<scalar T>
mat2<T>::mat2() : mat2(1) {}

template<scalar T>
mat2<T>::mat2(T identity) :
		x(identity, 0),
		y(0, identity) {}

template<scalar T>
mat2<T>::mat2(
		const vec2<T> &x,
		const vec2<T> &y) :
		x(x), y(y) {}

template<scalar T>
mat2<T>::mat2(
		T xx, T xy,
		T yx, T yy) :
		x(xx, xy),
		y(yx, yy) {}

template<scalar T>
mat2<T>::mat2(const T *array) :
	x(array[0], array[1]),
	y(array[2], array[3]) {}

template<scalar T>
template<scalar U>
mat2<T>::mat2(const mat2<U> &other) :
		mat2(other.x, other.y) {}

#pragma region Operators

template<scalar U>
std::ostream &operator<<(std::ostream &lhs, const mat2<U> &rhs) {
	std::stringstream lines[2], final_ss;

	for (size_t i = 0; i < 2; i++) {
		size_t max_len = 0;
		for (size_t j = 0; j < 2; j++) {
			size_t len = util::to_string(rhs[i][j]).size();
			if (len > max_len)
				max_len = len;
		}

		for (size_t j = 0; j < 2; j++) {
			lines[j] << std::setw(max_len) << util::to_string(rhs[i][j]);
			if (i != 1)
				lines[j] << " | ";
		}
	}

	for (size_t i = 0; i < 2; i++) {
		if (i != 1) lines[i] << '\n';
		final_ss << lines[i].str();
	}

	return lhs << final_ss.str();
}


template<scalar T>
vec2<T> &mat2<T>::operator[](size_t index) {
	return data[index];
}

template<scalar T>
const vec2<T> &mat2<T>::operator[](size_t index) const {
	return data[index];
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator==(const mat2<U> &rhs) const {
	return mat2<bool>(x == rhs.x, y == rhs.y);
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator!=(const mat2<U> &rhs) const {
	return mat2<bool>(x != rhs.x, y != rhs.y);
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator<=(const mat2<U> &rhs) const {
	return mat2<bool>(x <= rhs.x, y <= rhs.y);
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator>=(const mat2<U> &rhs) const {
	return mat2<bool>(x >= rhs.x, y >= rhs.y);
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator<(const mat2<U> &rhs) const {
	return mat2<bool>(x < rhs.x, y < rhs.y);
}

template<scalar T>
template<scalar U>
mat2<bool> mat2<T>::operator>(const mat2<U> &rhs) const {
	return mat2<bool>(x > rhs.x, y > rhs.y);
}

template<boolean U>
mat2<bool> operator!(const mat2<U> &mat) {
	return mat2<bool>(!mat.x, !mat.y);
}

template<scalar T>
mat2<T> mat2<T>::operator-() const {
	return mat2<T>(-x, -y);
}

// Matrix + Matrix

template<scalar T>
template<scalar U>
auto mat2<T>::operator+(const mat2<U> &rhs) const {
	return mat2(x + rhs.x, y + rhs.y);
}

template<scalar T>
template<scalar U>
auto mat2<T>::operator-(const mat2<U> &rhs) const {
	return mat2(x - rhs.x, y - rhs.y);
}

template<scalar T>
template<scalar U>
auto mat2<T>::operator*(const mat2<U> &rhs) const {
	return mat2(
		x * rhs.x.x + y * rhs.x.y,
		x * rhs.y.x + y * rhs.y.y
	);
}

template<scalar T>
template<scalar U>
mat2<T> &mat2<T>::operator+=(const mat2<U> &rhs) {
	return *this = *this + rhs;
}

template<scalar T>
template<scalar U>
mat2<T> &mat2<T>::operator-=(const mat2<U> &rhs) {
	return *this = *this - rhs;
}

template<scalar T>
template<scalar U>
mat2<T> &mat2<T>::operator*=(const mat2<U> &rhs) {
	return *this = *this * rhs;
}

// Matrix + Scalar

template<scalar T>
template<scalar U>
auto mat2<T>::operator*(U rhs) const {
	return mat2(
		x.x * rhs, x.y * rhs,
		y.x * rhs, y.y * rhs
	);
}

template<scalar T>
template<scalar U>
auto mat2<T>::operator/(U rhs) const {
	return mat2(
		x.x / rhs, x.y / rhs,
		y.x / rhs, y.y / rhs
	);
}

template<scalar T>
template<scalar U>
mat2<T> &mat2<T>::operator*=(U rhs) {
	return *this = *this * rhs;
}

template<scalar T>
template<scalar U>
mat2<T> &mat2<T>::operator/=(U rhs) {
	return *this = *this / rhs;
}

// Scalar + Matrix

template<scalar L, scalar R>
auto operator*(L lhs, const mat2<R> &rhs) {
	return mat2(
		lhs * rhs.x.x, lhs * rhs.x.y,
		lhs * rhs.y.x, lhs * rhs.y.y
	);
}

// Matrix + Vector

template<scalar T>
template<scalar U>
auto mat2<T>::operator*(const vec2<U> &rhs) const {
	mat2<T> t = transpose(*this);
	return vec2(dot(t.x, rhs), dot(t.y, rhs));
}

#pragma endregion

template<boolean T>
bool all(const mat2<T> &mat) {
	return all(mat.x) && all(mat.y);
}

template<boolean T>
bool any(const mat2<T> &mat) {
	return any(mat.x) || any(mat.y);
}

template<scalar T>
T determinant(const mat2<T> &mat) {
	return mat.x.x * mat.y.y - mat.y.x * mat.x.y;
}

template<floating_point T>
mat2<T> inverse(const mat2<T> &mat) {
	T det = determinant(mat);
	SOFT_BODIES_DEVELOPMENT_ASSERT(det != 0, "Matrix is singular.")

	return mat2<T>(
		mat.y.y, -mat.x.y,
		-mat.y.x, mat.x.x
	) * (1 / det);
}

template<scalar A, scalar B, scalar Epsilon>
bool is_approx(const mat2<A> &a, const mat2<B> &b, Epsilon epsilon) {
	return is_approx(a.x, b.x, epsilon) &&
		   is_approx(a.y, b.y, epsilon);
}

template<scalar T, scalar Epsilon>
bool is_orthogonal(const mat2<T> &mat, Epsilon epsilon) {
	return is_approx(dot(mat.x, mat.y), 0, epsilon);
}

template<scalar T, scalar Epsilon>
bool is_orthonormal(const mat2<T> &mat, Epsilon epsilon) {
	return is_orthogonal(mat, epsilon) &&
			is_normalized(mat.x, epsilon) &&
			is_normalized(mat.y, epsilon);
}

template<floating_point T>
mat2<T> orthogonalize(const mat2<T> &mat) {
	vec2<T> x = mat.x;

	vec2<T> y = mat.y
			- proj(x, mat.y);

	return mat2(x, y);
}

template<floating_point T>
mat2<T> orthonormalize(const mat2<T> &mat) {
	// auto ortho = orthogonalize(mat);

	// ortho.x = normalize(ortho.x);
	// ortho.y = normalize(ortho.y);

	// return ortho;

	vec2<T> x = normalize(mat.x);

	vec2<T> y = normalize(mat.y
			- dot(x, mat.y) * x);

	return mat2(x, y);
}

template<scalar T>
mat2<T> transpose(const mat2<T> &mat) {
	return mat2(
		mat.x.x, mat.y.x,
		mat.x.y, mat.y.y
	);
}

}
