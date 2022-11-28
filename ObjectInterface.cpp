#include <string>
using namespace std;

struct opsStruct {
    char op;
    int key;
    string value;

    opsStruct(char op, int key, string value) : op(op), key(key), value(value) {}
    opsStruct(char op, int key) : op(op), key(key) {}
    opsStruct() {}

};


namespace TS {
class ObjectInterface
{
private:
    /* data */
public:
    virtual string runOp(struct opsStruct *op) = 0;
    virtual bool insert(int key, string value) = 0;
    virtual string get(int key) = 0;
    virtual bool remove(int key) = 0;
    virtual void print() = 0;
};
}