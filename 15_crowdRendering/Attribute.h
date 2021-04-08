#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <vector>

#include <GL/glew.h>

template<typename T>
class Attribute
{
public:

    Attribute();
    ~Attribute();

    void SetAttribPointer(unsigned int slot);

    void Set(T* inputArray, unsigned int length);
    void Set(std::vector<T>& input);

    void BindTo(unsigned int slot);
    void UnbindFrom(unsigned int slot);

    unsigned int Count()     const { return count; }
    unsigned int GetHandle() const { return id; }

protected:

    unsigned int id;
    unsigned int count;

private:

    // prevent copy/assignment
    Attribute(const Attribute& other);
    Attribute& operator=(const Attribute& other);
};

#endif // ATTRIBUTE_H_INCLUDED

