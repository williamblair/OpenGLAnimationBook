#ifndef INDEX_BUFFER_H_INCLUDED
#define INDEX_BUFFER_H_INCLUDED

#include <vector>

#include <GL/glew.h>

class IndexBuffer
{
public:

    IndexBuffer();
    ~IndexBuffer();

    void Set(unsigned int* arr, unsigned int len);
    void Set(std::vector<unsigned int>& input);

    unsigned int GetHandle() const { return id;    }
    unsigned int GetCount()  const { return count; }

private:
    // disallow assignment and copy
    IndexBuffer(const IndexBuffer&);
    IndexBuffer& operator=(const IndexBuffer&);
    
    unsigned int id;
    unsigned int count;
};

#endif // INDEX_BUFFER_H_INCLUDED

