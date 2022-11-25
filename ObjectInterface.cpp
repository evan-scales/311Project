#include <string>
using namespace std;

namespace TS {
class ObjectInterface
{
private:
    /* data */
public:
    virtual bool insert(int key, string value) = 0;
    virtual string get(int key) = 0;
    virtual bool remove(int key) = 0;
    virtual void print() = 0;
};
}