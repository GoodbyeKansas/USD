//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// vec.template.h file to make changes.

#ifndef GF_{{ UPPER(VEC)[2:] }}_H
#define GF_{{ UPPER(VEC)[2:] }}_H

/// \file gf/vec{{ SUFFIX }}.h
/// \ingroup group_gf_LinearAlgebra

#include "pxr/pxr.h"
#include "pxr/base/tf/diagnostic.h"
#include "pxr/base/gf/api.h"
#include "pxr/base/gf/limits.h"
#include "pxr/base/gf/traits.h"
{% if IS_FLOATING_POINT(SCL) -%}
#include "pxr/base/gf/math.h"
{% if SCL == 'GfHalf' -%}
#include "pxr/base/gf/half.h"
{% endif %}
{% endif %}

#include <boost/functional/hash.hpp>

#include <cstddef>
{% if IS_FLOATING_POINT(SCL) -%}
#include <cmath>
{% endif %}

#include <iosfwd>

PXR_NAMESPACE_OPEN_SCOPE

class {{ VEC }};

template <>
struct GfIsGfVec<class {{ VEC }}> { static const bool value = true; };

/// \class {{ VEC }}
/// \ingroup group_gf_LinearAlgebra
///
/// Basic type for a vector of {{ DIM }} {{ SCL }} components.
///
/// Represents a vector of {{ DIM }} components of type \c {{ SCL }}.
/// It is intended to be fast and simple.
///
class {{ VEC }}
{
public:
    /// Scalar element type and dimension.
    typedef {{ SCL }} ScalarType;
    static const size_t dimension = {{ DIM }};

    /// Default constructor does no initialization.
    {{ VEC }}() = default;

    /// Initialize all elements to a single value.
    constexpr explicit {{ VEC }}({{ SCL }} value)
        : _data{ {{ LIST("value") }} }
    {
    }

    /// Initialize all elements with explicit arguments.
    constexpr {{ VEC }}({{ LIST(SCL + " s%(i)s") }})
        : _data{ {{ LIST("s%(i)s") }} }
    {
    }

    /// Construct with pointer to values.
    template <class Scl>
    constexpr explicit {{ VEC }}(Scl const *p)
        : _data{ {{ LIST("p[%(i)s]") }} }
    {
    }
{% if IS_FLOATING_POINT(SCL) %}
{% for S in SCALARS if S != SCL %}

    /// {{ "Implicitly convert" if ALLOW_IMPLICIT_CONVERSION(S, SCL) else "Construct" }} from {{ VECNAME(DIM, S) }}.
    {{ '' if ALLOW_IMPLICIT_CONVERSION(S, SCL) else 'explicit ' }}{{ VEC }}(class {{ VECNAME(DIM, S) }} const &other);
{% endfor %}
{% endif %}
 
{% for X in 'XYZW'[:DIM] %}
    /// Create a unit vector along the {{ X }}-axis.
    static {{ VEC }} {{ X }}Axis() {
        {{ VEC }} result(0);
        result[{{ loop.index0 }}] = 1;
        return result;
    }
{% endfor %}

    /// Create a unit vector along the i-th axis, zero-based.  Return the zero
    /// vector if \p i is greater than or equal to {{ DIM }}.
    static {{ VEC }} Axis(size_t i) {
        {{ VEC }} result(0);
        if (i < {{ DIM }})
            result[i] = 1;
        return result;
    }

    /// Set all elements with passed arguments.
    {{ VEC }} &Set({{ LIST(SCL + " s%(i)s") }}) {
        {{ LIST("_data[%(i)s] = s%(i)s;", sep='\n        ') }}
        return *this;
    }

    /// Set all elements with a pointer to data.
    {{ VEC }} &Set({{ SCL }} const *a) {
        return Set({{ LIST("a[%(i)s]") }});
    }

    /// Direct data access.
    {{ SCL }} const *data() const { return _data; }
    {{ SCL }} *data() { return _data; }
    {{ SCL }} const *GetArray() const { return data(); }

    /// Indexing.
    {{ SCL }} const &operator[](size_t i) const { return _data[i]; }
    {{ SCL }} &operator[](size_t i) { return _data[i]; }

    /// Hash.
    friend inline size_t hash_value({{ VEC }} const &vec) {
        size_t h = 0;
        {{ LIST("boost::hash_combine(h, vec[%(i)s]);", sep='\n        ') }}
        return h;
    }

    /// Equality comparison.
    bool operator==({{ VEC }} const &other) const {
        return {{ LIST("_data[%(i)s] == other[%(i)s]",
                       sep=' &&\n               ') }};
    }
    bool operator!=({{ VEC }} const &other) const {
        return !(*this == other);
    }

    // TODO Add inequality for other vec types...
    {% for S in SCALARS if S != SCL -%}
    /// Equality comparison.
    GF_API
    bool operator==(class {{ VECNAME(DIM, S) }} const &other) const;
    {% endfor %}

    /// Create a vec with negated elements.
    {{ VEC }} operator-() const {
        return {{ VEC }}({{ LIST("-_data[%(i)s]") }});
    }

    /// Addition.
    {{ VEC }} &operator+=({{ VEC }} const &other) {
        {{ LIST("_data[%(i)s] += other[%(i)s];", sep='\n        ') }}
        return *this;
    }
    friend {{ VEC }} operator+({{ VEC }} const &l, {{ VEC }} const &r) {
        return {{ VEC }}(l) += r;
    }

    /// Subtraction.
    {{ VEC }} &operator-=({{ VEC }} const &other) {
        {{ LIST("_data[%(i)s] -= other[%(i)s];", sep='\n        ') }}
        return *this;
    }
    friend {{ VEC }} operator-({{ VEC }} const &l, {{ VEC }} const &r) {
        return {{ VEC }}(l) -= r;
    }

    /// Multiplication by scalar.
    {{ VEC }} &operator*=(double s) {
        {{ LIST("_data[%(i)s] *= s;", sep='\n        ') }}
        return *this;
    }
    {{ VEC }} operator*(double s) const {
        return {{ VEC }}(*this) *= s;
    }
    friend {{ VEC }} operator*(double s, {{ VEC }} const &v) {
        return v * s;
    }

    {% if IS_FLOATING_POINT(SCL) %}
    /// Division by scalar.
    // TODO should divide by the scalar type.
    {{ VEC }} &operator/=(double s) {
        // TODO This should not multiply by 1/s, it should do the division.
        // Doing the division is more numerically stable when s is close to
        // zero.
        return *this *= (1.0 / s);
    }
    {{ VEC }} operator/(double s) const {
        return *this * (1.0 / s);
    }
    {% else %}
    /// Division by scalar.
    {{ VEC }} &operator/=({{ SCL }} s) {
        {{ LIST("_data[%(i)s] /= s;", sep='\n        ') }}
        return *this;
    }
    {{ VEC }} operator/({{ SCL }} s) const {
        return {{ VEC }}(*this) /= s;
    }
    {% endif %}

    /// See GfDot().
    {{ SCL }} operator*({{ VEC }} const &v) const {
        return {{ LIST("_data[%(i)s] * v[%(i)s]", sep=" + ") }};
    }

    /// Returns the projection of \p this onto \p v. That is:
    /// \code
    /// v * (*this * v)
    /// \endcode
    {{ VEC }} GetProjection({{ VEC }} const &v) const {
        return v * (*this * v);
    }

    /// Returns the orthogonal complement of \p this->GetProjection(b).
    /// That is:
    /// \code
    ///  *this - this->GetProjection(b)
    /// \endcode
    {{ VEC }} GetComplement({{ VEC }} const &b) const {
        return *this - this->GetProjection(b);
    }

    /// Squared length.
    {{ SCL }} GetLengthSq() const {
        return *this * *this;
    }

{% if IS_FLOATING_POINT(SCL) %}
    /// Length
    {{ SCL }} GetLength() const {
        // TODO should use GfSqrt.
        return sqrt(GetLengthSq());
    }

    /// Normalizes the vector in place to unit length, returning the
    /// length before normalization. If the length of the vector is
    /// smaller than \p eps, then the vector is set to vector/\c eps.
    /// The original length of the vector is returned. See also GfNormalize().
    ///
    /// \todo This was fixed for bug 67777. This is a gcc64 optimizer bug.
    /// By tickling the code, it no longer tries to write into
    /// an illegal memory address (in the code section of memory).
    {{ SCL }} Normalize({{ SCL }} eps = {{ EPS }}) {
        // TODO this seems suspect...  suggest dividing by length so long as
        // length is not zero.
        {{ SCL }} length = GetLength();
        *this /= (length > eps) ? length : eps;
        return length;
    }

    {{ VEC }} GetNormalized({{ SCL }} eps = {{ EPS }}) const {
        {{ VEC }} normalized(*this);
        normalized.Normalize(eps);
        return normalized;
    }

{% if DIM == 3 %}
    /// Orthogonalize and optionally normalize a set of basis vectors. This
    /// uses an iterative method that is very stable even when the vectors are
    /// far from orthogonal (close to colinear).  The number of iterations and
    /// thus the computation time does increase as the vectors become close to
    /// colinear, however. Returns a bool specifying whether the solution
    /// converged after a number of iterations.  If it did not converge, the
    /// returned vectors will be as close as possible to orthogonal within the
    /// iteration limit. Colinear vectors will be unaltered, and the method
    /// will return false.
    GF_API
    static bool OrthogonalizeBasis(
        {{ VEC }} *tx, {{ VEC }} *ty, {{ VEC }} *tz,
        const bool normalize,
        double eps = GF_MIN_ORTHO_TOLERANCE);

    /// Sets \c v1 and \c v2 to unit vectors such that v1, v2 and *this are
    /// mutually orthogonal.  If the length L of *this is smaller than \c eps,
    /// then v1 and v2 will have magnitude L/eps.  As a result, the function
    /// delivers a continuous result as *this shrinks in length.
    GF_API
    void BuildOrthonormalFrame({{ VEC }} *v1, {{ VEC }} *v2,
                    {{ SCL }} eps = {{ EPS }}) const;

{% endif %} {# DIM == 3 #}
{% endif %} {# IS_FLOATING_POINT(SCL) #}

private:
    {{ SCL }} _data[{{ DIM }}];
};

/// Output a {{ VEC }}.
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream& operator<<(std::ostream &, {{ VEC }} const &);

{% if IS_FLOATING_POINT(SCL) %}

PXR_NAMESPACE_CLOSE_SCOPE

{% for S in SCALARS if S != SCL %}
#include "pxr/base/gf/vec{{ DIM }}{{ SCALAR_SUFFIX(S) }}.h"
{% endfor %}

PXR_NAMESPACE_OPEN_SCOPE

{% for S in SCALARS if S != SCL %}
inline
{{ VEC }}::{{ VEC }}(class {{ VECNAME(DIM, S) }} const &other)
{
    {{ LIST("_data[%(i)s] = other[%(i)s];", sep='\n    ') }}
}
{% endfor %}
{% endif %}

/// Returns component-wise multiplication of vectors \p v1 and \p v2.
inline {{ VEC }}
GfCompMult({{ VEC }} const &v1, {{ VEC }} const &v2) {
    return {{ VEC }}(
        {{ LIST("v1[%(i)s] * v2[%(i)s]", sep=",\n        ") }}
        );
}

/// Returns component-wise quotient of vectors \p v1 and \p v2.
inline {{ VEC }}
GfCompDiv({{ VEC }} const &v1, {{ VEC }} const &v2) {
    return {{ VEC }}(
        {{ LIST("v1[%(i)s] / v2[%(i)s]", sep=",\n        ") }}
        );
}

/// Returns the dot (inner) product of two vectors.
inline {{ SCL }}
GfDot({{ VEC }} const &v1, {{ VEC }} const &v2) {
    return v1 * v2;
}

{% if IS_FLOATING_POINT(SCL) %}

/// Returns the geometric length of \c v.
inline {{ SCL }}
GfGetLength({{ VEC }} const &v)
{
    return v.GetLength();
}

/// Normalizes \c *v in place to unit length, returning the length before
/// normalization. If the length of \c *v is smaller than \p eps then \c *v is
/// set to \c *v/eps.  The original length of \c *v is returned.
inline {{ SCL }}
GfNormalize({{ VEC }} *v, {{ SCL }} eps = {{ EPS }})
{
    return v->Normalize(eps);
}

/// Returns a normalized (unit-length) vector with the same direction as \p v.
/// If the length of this vector is smaller than \p eps, the vector divided by
/// \p eps is returned.
inline {{ VEC }}
GfGetNormalized({{ VEC }} const &v, {{ SCL }} eps = {{ EPS }})
{
    return v.GetNormalized(eps);
}

/// Returns the projection of \p a onto \p b. That is:
/// \code
/// b * (a * b)
/// \endcode
inline {{ VEC }}
GfGetProjection({{ VEC }} const &a, {{ VEC }} const &b)
{
    return a.GetProjection(b);
}

/// Returns the orthogonal complement of \p a.GetProjection(b). That is:
/// \code
///  a - a.GetProjection(b)
/// \endcode
inline {{ VEC }}
GfGetComplement({{ VEC }} const &a, {{ VEC }} const &b)
{
    return a.GetComplement(b);
}

/// Tests for equality within a given tolerance, returning \c true if the
/// length of the difference vector is less than or equal to \p tolerance.
inline bool
GfIsClose({{ VEC }} const &v1, {{ VEC }} const &v2, double tolerance)
{
    {{ VEC }} delta = v1 - v2;
    return delta.GetLengthSq() <= tolerance * tolerance;
}

{% if DIM == 3 %}

GF_API bool
GfOrthogonalizeBasis({{ VEC }} *tx, {{ VEC }} *ty, {{ VEC }} *tz,
                     bool normalize, double eps = GF_MIN_ORTHO_TOLERANCE);

GF_API void
GfBuildOrthonormalFrame({{ VEC }} const &v0,
                        {{ VEC }}* v1,
                        {{ VEC }}* v2,
                        {{ SCL }} eps = {{ EPS }});

/// Returns the cross product of \p v1 and \p v2.
inline {{ VEC }}
GfCross({{ VEC }} const &v1, {{ VEC }} const &v2)
{
    return {{ VEC }}(
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]);
}

/// Returns the cross product of \p v1 and \p v2. 
/// \see GfCross()
inline {{ VEC }}
operator^({{ VEC }} const &v1, {{ VEC }} const &v2)
{
    return GfCross(v1, v2);
}

/// Spherical linear interpolation in three dimensions.
GF_API {{ VEC }}
GfSlerp(double alpha, {{ VEC }} const &v0, {{ VEC }} const &v1);

{% endif %} {# DIM == 3 #}

{% endif %} {# IS_FLOATING_POINT(SCL) #}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // GF_{{ UPPER(VEC)[2:] }}_H
