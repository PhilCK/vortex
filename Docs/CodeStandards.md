# Code Standards

General coding standards for C++.

The main goal of these coding standards is to reduce bugs, make code better maintainable, and to have a consistent coding style.

It is also to provide a guide to how the methodologies are implemented.

Remember _"Always code as if the person who ends up maintaining your code is a violent psychopath who knows where you live."_ - Jeff Atwood.

## Naming Conventions

### Classes

Classes and Structs should start with a capital letter.

```cpp
class Foo;
struct Bar;
```

___Rational:___ _Standard way of doing things._

### Methods

Member functions are camel case with the first letter being lower case.

```cpp
someClass.someMethod();
```

Where possible use verbs to help identify what a member method does.

```cpp
void 		setPosition(const float x);
float x 	getPosition() const;
bool 		isAlive() const;
```

___Rational:___ _Standard way of doing things._

### Functions

C style functions should start with a capital with each additional word being capitalised.

```cpp
void ThisIsAFuction();
```
___Rational:___ _Standard way of doing things._

### Variables

Variables should follow the following layout.

```cpp
m_memberVar;	// Class member var.
localVar;	// Class/Function local var.
g_globalVar;	// Global var.
anonVar;	// Unnamed namespace var.
```

Where appropriate you can use prefix's and suffix's to indicate extra information about the type.

```cpp
Vec3 m_positionVec;
Foo *m_componentPtr;
bool m_isReady;
```

Member variables should be descriptive. Local variables should be shorter, and as a general rule and the more insignificant the variable the more abbreviated it should be, local vars should be defined as close as possible to the point of usage.

___Rational:___ _Easily identify scope of variables. Standard way of doing things._

### Filenames

Filenames are capitalized at start and every preceding word. `.cpp` and `.hpp` are the extensions used for C++ files and `.c` and `.h` used for C files.

```cpp
// C++
FooBar.hpp
FooBar.cpp
// C
BarFoo.h
BarFoo.c
```
___Rational:___ _Standard way of doing things._

## Const Correctness

Const as much as possible.

### Const Variables

This makes it easier to quickly identify variables that change throughout the program.


```cpp
bool someMethod(const Type & input)
{
	const float x = 10;
	float y = x;

	y += 2;

	return true;
}
```

___Rational:___ _Improve maintainability for next programmer._

### Const Pointers

There is no requirement for const'ing pointers, however it is recommended to const ints.

```cpp
void foo(const int *value)
{
	value++; // This is pointer arithmetic.
}

void foo(const int *const value)
{
	value++; // fails
}
```

___Rational:___ _To reduce accidental errors._

### Const Methods

If a method makes no member changes you must indicate it with the const keyword.

```cpp
class Bar {
public:
	void fooMethod() const;
}; // class
```

___Rational:___ _Facilitates const-correctness._

### Mutable

The mutable keyword is fine as long as it isn't invalidating a logical const.

## Class and Function Structure


### Class Structure

A class should follow the outline, and be formatted with return types and method names aligned.

```cpp
class Foo
{
public:
	explicit 	Foo();
			~Foo();

	void 		setter(const int x);
	int 		getter() const;
private:
	m_memberVar;
}; // class
```

___Rational:___ _Standard way of doing things._

### Constructors

Should always be explicit unless a valid reason for it to be implicit.

```cpp
// Valid implicit
class Color {
public:
	Color(const uint32_t hex);
}; // class

void someFuncThatTakesAColor(const Color color);
someObj.someFuncThatTakesAColor(0x332244); // Hex is common for color so implicit is fine.

// Not valid implicit
class Color {
public:
	Color(const float r, const float g, const float b);
}; // class

void someFuncThatTakesAColor(const Color color);
someObj.someFuncThatTakesAColor((1, 2, 3)); // Cannot identity easily what object this is, could be a vector.
```

___Rational:___ _Maintainability - We want the next coder to be able to understand it._

### Noncopyable

If there is no valid reason for an object to be copied make it non-copyable. If you are inheriting from a noncopyable class use private inheritance.

```cpp
class Foo : private Noncopyable
{
	//...//
}; // class
```


### Virtual Methods

If using C++11 use the `override` keyword, else pre C++11 use a `v` at the start of the method.

```cpp
void vMethod();
void method() override;
```

This helps the programmer quickly identify virtual methods without having to check inherited classes.

___Rational:___ _To allow other programmers to quickly identify if a function is virtual. So we can laugh justifiable at others when they use virtual methods in ctors._

### Static Methods

Static methods are not to be over used. Consider using a function in place for a static method, classes with lots of static functions will be considered bad design.

### Initializer Lists

Initialize _all_ variables in the initialize list, in the _correct_ order. Initailizer lists are to be formatted with the colon or comma preceding the variable.

```cpp
// Header
class Foo {
public:
	explicit Foo();
private:
	int x, y;
	float z;
	bool w;
}; // class

// Source
Foo::Foo()
: x(0)
, y(0)
, z(0.0f)
, w(false)
{}
```

## Pointers & References

Reference are preferred over pointers, unless it makes sense to return a nullptr consider using a reference.

Pointers are to be asserted regularly, if there is a known reason an assert might fail place a comment next to it. If a pointer is guaranteed to return a valid result, consider returning a reference so that caller doesn't have to assert it.

### Raw Pointers

Raw pointers are to be considered for _access_ only to an object and allocations should be done through smart pointers. The following example is fine, as we are not talking about ownership just access.

```cpp
Foo *foo = someObj.getPtr();
```

Raw pointers should be initialized with a `0` pre C++11 and `nullptr` with C++11. **Do not use NULL.**

```cpp
Foo *ptr(0); 		// Pre C++11
Foo *ptr(nullptr);	// C++11
```

### Smart Pointers

With smart pointers consider who owns the object. For example A component is owned by an entity, and an entity is owned by a scene. In this case the ownership is clear and unique pointers are best suited.

When using C++11 use move semantics to transfer ownership.

```cpp
std::unique_ptr<Entity> entity(new Entity);
std::unique_ptr<Component> component(new Component);

entity->addComponent(std::move(component));
```

When ownership is less clear use shared pointers. Shared pointers are not a replacement for raw pointers they address ownership not access.

Use weak pointers only when possible cyclic situations arise with shared pointers, weak pointers are not a replacement for raw pointers they address ownership not access.

### References

References are preferred when possible, even when internally you store a pointer return a reference unless there is good reason to return a pointer, or that it might be null.

```cpp
Foo & getFoo() const { return *m_foo; }
```

## Arrays
Use c-style arrays only when is necessary, otherwise use an array with bounds checking like `std::array` and `std::vector`.

### Array Access 
Prefer using `.at(i)` over `[i]`. To remove the chances of out of bounds access, even when iterating through fixed bounds.

```cpp
for(int i = 0; i < array.size(); ++i)
{
	array.at(i); // rather than array[i]
}
```

___Rational:___ _Maintainability - Future modifications may change the way the loop is iterated._

### Vectors
Please `reserve()` or `resize()` to reduce allocation at run time.

___Rational:___ _Reduce number of allocations and moves._


## Scope

### Namespace
Named namespaces should be formatted like so with `{` inline. Unnamed namespace is at the discretion of the coder.

```cpp
namespace Outter {
namespace Innter {
	
class Foo
{
	// ... //
}; // class

} // namespace
} // namespace

```

### Unnamed Namespace

Unnamed namespaces are encouraged where appropriate, they are considered superior to static/global variables.

```cpp
namespace 
{
	const int foo(1);
	int bar(2);
}

int GetInt() { return foo; }
```

### Local Scope
Consider using local scope to help clarify what variables are being used, and where. This helps identify the significance of a variable. A comment at the top can further help. Even with loops prefer local space (unless dynamic allocations could cause performance issues.)

```cpp
void onUpdate(float dt)
{
	// Calculate Player something
	{
		// ...
	}

	float newSpeed = 0.f;
	{
		// .. calc new speed.
	}

	for(auto i : container)
	{
		std::array<int> {1,2,3,4,5,6,7}; // prefer lower scope.
	}
}

```

___Rational:___ _Maintainability - Reducing scope helps with development later down line, its easier to increase scope than to decrease scope._

### Curly Braces

With the exception of namespaces it is up to the programmer where to place braces.

```cpp
void MyFunc() { /* is fine */ }

void YourFunc() {
	// also fine.
}

void YourOtherFunc()
{
	// also fine.	
}
```

___Rational:___ _Does it really matter? I won't force my religion on you on, and you don't force yours on me - Deal?_

With `if` statements, curly braces are *required* in all situations.

```cpp
if(true) {
	// Braces are a must!
	setFunc();
}
```

___Rational:___ _Maintainability - Reducing errors that may happen later by other programmers._

### Globals

Globals variables are discouraged in all forms, including singletons.

___Rational:___ _Do we need have this talk?_

### Local Variables

Define local variables as close to use as possible.

```cpp
// Bad
int a = 0;
for(//..//)
{
	a = 1;
}

// Good
for(//..//)
{
	int a = 1;
}
```

___Rational:___ _Maintainability - also see scope._

## Forward Declarations

In general put all forward declarations in one file, or split them up per module, refrain from doing them inline unless they are for internal structs etc.

```cpp
// ApplicationFwd.hpp
namespace Outter {
namespace Innter {
	
class Foo;
class Bar;
class Baz;
class Boo;

} // namespace
} // namespace
```

## Operator Overloading

Be wary of operator overloading, while it can be clear to you it might not be clear to somebody else. Overloading is acceptable for math related functions where a clear definition is present.

## Other Stuff

### RAII

When possible avoid init style functions these can cause confusion about how to properly create an object.

### Rule of Three/Five

Rule of three states that if you have to define a `Destructor`, `Copy Constructor` or `Copy Assignment Operator` you should define all three. This is usually because we are dealing with memory.


### Books (Recommended Reading)

- Effective C++
- Effective STL
- The Design of Everyday Things
- Game Coding Complete

