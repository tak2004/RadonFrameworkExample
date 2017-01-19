#pragma inline_depth( 255 )
#pragma inline_recursion( on )
#pragma auto_inline( on )
#define inline __forceinline 
namespace vector {
///////////////////////////////////////////////////////////////////////////
// ARGUMENTS
///////////////////////////////////////////////////////////////////////////

template< class ta_a >
class vecarg
{
    const ta_a& Argv;
public:
    __forceinline vecarg(const ta_a& A): Argv(A)
    {}
    __forceinline const float Evaluate(const int i) const
    {
        return Argv.Evaluate(i);
    }
};

template<>
class vecarg< const float >
{
    const float& Argv;
public:
    __forceinline vecarg(const float& A): Argv(A)
    {}
    __forceinline const float Evaluate(const int i) const
    {
        return Argv;
    }
};

template<>
class vecarg< const int >
{
    const int& Argv;
public:
    __forceinline vecarg(const int& A): Argv(A)
    {}
    __forceinline const float Evaluate(const int i) const
    {
        return (float)Argv;
    }
};

///////////////////////////////////////////////////////////////////////////
// EXPRESSIONS
///////////////////////////////////////////////////////////////////////////

template< class ta_a, class ta_b, class ta_eval >
class vecexp_2
{
    const vecarg<ta_a>   Arg1;
    const vecarg<ta_b>   Arg2;

public:
    __forceinline vecexp_2(const ta_a& A1, const ta_b& A2): Arg1(A1), Arg2(A2)
    {}
    __forceinline const float Evaluate(const int i) const
    {
        return ta_eval::Evaluate(i, Arg1, Arg2);
    }
};

template< class ta_a, class ta_eval >
class vecexp_1
{
    const vecarg<ta_a>   Arg1;

public:
    __forceinline vecexp_1(const ta_a& A1): Arg1(A1)
    {}

    __forceinline const float Evaluate(const int i) const
    {
        return ta_eval::Evaluate(i, Arg1.Evaluate(i));
    }
};

///////////////////////////////////////////////////////////////////////////
// BASE CLASS
///////////////////////////////////////////////////////////////////////////

template< int ta_dimension, class T >
struct base: public T
{
    __forceinline       float&  operator[](const int i)
    {
        return ((float*)this)[i];
    }
    __forceinline const float   Evaluate(const int i) const
    {
        return ((float*)this)[i];
    }

    //////////////////////////////////////////////////////////////////
    // ASSIGMENT
    //////////////////////////////////////////////////////////////////
    template< class ta >
    struct assigment
    {
        template< int I, class R >
        struct recurse
        {
            enum
            {
                COUNTER = I + 1
            };

            static __forceinline void Assign(base<ta_dimension, T>& V, const ta& A)
            {
                V[I] = A.Evaluate(I);
                recurse<COUNTER, int>::Assign(V, A);
            }
        };

        template<> struct recurse<ta_dimension, int>
        {
            static __forceinline void Assign(base<ta_dimension, T>& V, const ta& A)
            {}
        };

        static __forceinline void Assign(base<ta_dimension, T>& V, const ta& A)
        {
            recurse<0, int>::Assign(V, A);
        }
    };

    template< class ta_type > __forceinline
        const base<ta_dimension, T>& operator = (const ta_type& A)
    {
        assigment<ta_type>::Assign(*this, A);
        return *this;
    }

    //////////////////////////////////////////////////////////////////
    // DOT PRODUCT
    //////////////////////////////////////////////////////////////////
    template< class ta, class tb >
    struct dot_prod
    {
        template< int I, class R >
        struct recurse
        {
            enum
            {
                COUNTER = I + 1
            };

            static __forceinline float Dot_Prod(const ta& A, const tb& B)
            {
                return A.Evaluate(I) * B.Evaluate(I) +
                    recurse<COUNTER, int>::Dot_Prod(A, B);
            }
        };

        template<> struct recurse<ta_dimension, int>
        {
            static __forceinline float Dot_Prod(const ta& A, const tb& B)
            {
                return 0;
            }
        };

        static __forceinline float Dot_Prod(const ta& A, const tb& B)
        {
            return recurse<0, int>::Dot_Prod(A, B);
        }
    };

    template< class ta_type > __forceinline
        float Dot(const ta_type& A) const
    {
        return dot_prod<base<ta_dimension, T>, ta_type>::Dot_Prod(*this, A);
    }
};

///////////////////////////////////////////////////////////////////////////
// SUM
///////////////////////////////////////////////////////////////////////////

struct sum
{
    template< class ta_a, class ta_b > __forceinline static
        const float Evaluate(const int i, const ta_a& A, const ta_b& B)
    {
        return A.Evaluate(i) + B.Evaluate(i);
    }
};

template< class ta_c1, class ta_c2 > __forceinline
const vecexp_2< const ta_c1, const ta_c2, sum >
operator + (const ta_c1& Pa, const ta_c2& Pb)
{
    return vecexp_2< const ta_c1, const ta_c2, sum >(Pa, Pb);
}

///////////////////////////////////////////////////////////////////////////
// DATA
///////////////////////////////////////////////////////////////////////////

struct desc_xyz
{
    float X, Y, Z;
};

struct desc_xy
{
    float X, Y;
};

}

struct vector3: public vector::base< 3, vector::desc_xyz >
{
    typedef vector::base< 3, vector::desc_xyz > base;

    __forceinline  vector3(const float x, const float y, const float z)
    {
        X = z; Y = y; Z = z;
    }

    template< class ta_type > __forceinline
        vector3& operator = (const ta_type& A)
    {
        base::operator = (A); return *this;
    }
};