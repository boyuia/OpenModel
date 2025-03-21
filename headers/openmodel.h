#ifndef OPENMODEL_H
#define OPENMODEL_H

#include <iostream>
#include <cmath>
#include <vector> // Useful for storing collections of vectors, e.g., for polygons

// Forward declarations (Good practice to avoid circular dependencies in larger projects)
namespace OpenModel {

    // Base class for both 2D and 3D vectors.  This is an abstract class,
    // meaning you can't create an object of type Vector itself, but you
    // can create objects of derived classes (Vector2D and Vector3D).
    class Vector {
    public:
        virtual ~Vector() = default; // Virtual destructor is crucial for proper inheritance.
                                     // It ensures that the correct destructor is called
                                     // when deleting a pointer to a base class object.

        // Pure virtual functions make this class abstract.  Derived classes *must*
        // provide implementations for these functions.
        virtual float magnitude() const = 0;
        virtual Vector* normalize() = 0; // Returns a new normalized vector.  Using a pointer
                                          // because we're creating a new object.  We'll need to
                                          // manage the memory.  Consider returning std::unique_ptr<Vector>.

        virtual float dot(const Vector& other) const = 0; // Corrected to take a const reference
        virtual Vector* cross(const Vector& other) const = 0; // Only applicable to 3D vectors,
                                                              // so it will be implemented only in Vector3D.
                                                              // Returns a new vector (pointer).

        virtual void print() const = 0; // For simple debugging

        // Overload the + operator for vector addition.  This is common to both 2D and 3D vectors.
        // Returns a *new* Vector object (pointer).  Uses the 'const &' to avoid unnecessary copying.
        virtual Vector* operator+(const Vector& other) const = 0;

        // Overload the - operator for vector subtraction.
        virtual Vector* operator-(const Vector& other) const = 0;

        // Scalar multiplication (scaling a vector).  Defined at this level
        // to avoid duplication in Vector2D and Vector3D.
        virtual Vector* operator*(float scalar) const = 0;

        // Scalar division
        virtual Vector* operator/(float scalar) const = 0;

        // You could also define the following at the base class, but make them
        // virtual, if you want the derived classes to have the option to
        // override them.
        bool operator==(const Vector& other) const {
            // Default implementation: compare magnitudes.  This is often *not*
            // sufficient for vectors!  Derived classes should provide
            // more accurate comparisons.  For example, two 2D vectors (1, 0) and
            // (1.00001, 0) could be considered equal for most practical purposes,
            // even though their magnitudes are slightly different.  So, it's better
            // to do the comparison in the derived class.
            return std::abs(magnitude() - other.magnitude()) < 0.0001f; // Epsilon comparison.
        }

        bool operator!=(const Vector& other) const {
            return !(*this == other); // Reuse the == operator.
        }
    };

    // Class for 2D vectors.  Inherits from the Vector base class.
    class Vector2D : public Vector {
    public:
        float x, y; // Use meaningful names like x and y.

        // Constructors
        Vector2D();
        Vector2D(float x, float y);
        Vector2D(const Vector2D& other); // Copy constructor.  Important for deep copying.

        // Destructor (Rule of 3/5/0: If you need a destructor, you probably need
        // a copy constructor and a copy assignment operator).  In this case,
        // we're using the default destructor, but it's good practice to include it.
        ~Vector2D() override = default;

        // Assignment operator (Part of the Rule of 3/5/0).  Handles copying
        // the state of one object to another.
        Vector2D& operator=(const Vector2D& other);


        // Implementations of the pure virtual functions from the base class.
        float magnitude() const override;
        Vector2D* normalize() override; // Return a Vector2D*
        float dot(const Vector& other) const override;
        Vector* cross(const Vector& other) const override; // Returns nullptr, as cross product is not defined for 2D vectors.
        void print() const override;

        // Override the + operator to return a Vector2D.
        Vector2D* operator+(const Vector& other) const override;
        Vector2D* operator-(const Vector& other) const override;
        Vector2D* operator*(float scalar) const override;
        Vector2D* operator/(float scalar) const override;
        bool operator==(const Vector& other) const override;

    };

    // Class for 3D vectors.  Inherits from the Vector base class.
    class Vector3D : public Vector {
    public:
        float x, y, z;

        // Constructors
        Vector3D();
        Vector3D(float x, float y, float z);
        Vector3D(const Vector3D& other); // Copy constructor
        ~Vector3D() override = default;
        Vector3D& operator=(const Vector3D& other);

        // Implementations of the virtual functions.
        float magnitude() const override;
        Vector3D* normalize() override; // Returns a Vector3D*
        float dot(const Vector& other) const override;
        Vector3D* cross(const Vector& other) const override; // Corrected return type.
        void print() const override;

        // Override the + operator to return a Vector3D
        Vector3D* operator+(const Vector& other) const override;
        Vector3D* operator-(const Vector& other) const override;
        Vector3D* operator*(float scalar) const override;
        Vector3D* operator/(float scalar) const override;
        bool operator==(const Vector& other) const override;
    };

    // Non-member function for the cross product.  This is often preferred
    // for mathematical operations like the cross product.  It can be defined
    // *outside* the class definition.  It's only applicable to 3D vectors.
    Vector3D* cross(const Vector3D& a, const Vector3D& b);

    // Function to calculate the angle between two vectors.  This can be a non-member function.
    float angleBetween(const Vector& a, const Vector& b);

} // namespace OpenModel

#endif // OPENMODEL_H
